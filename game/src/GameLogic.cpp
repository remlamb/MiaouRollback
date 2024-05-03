#include "GameLogic.h"

#include <iostream>

#include "NetworkLogic.h"
#include "Random.h"
#include "RollbackManager.h"

namespace game {
void GameLogic::Rollback(const GameLogic& game_logic) {
  world_ = game_logic.world_;
  world_.contactListener = &player_manager;
  player_manager.players = game_logic.player_manager.players;
}

void GameLogic::SetPlayerInput(const Input::FrameInput& input, int player_id) {
  player_manager.players[player_id].input = input.input;
}

int GameLogic::ComputeChecksum() {
  int checksum = 0;

  int it = 0;
  for (const auto& player : player_manager.players) {
    auto& body = world_.GetBody(player_manager.players_BodyRefs_[it]);
    auto& pos = body.Position();
    const auto* pos_ptr = reinterpret_cast<const int*>(&pos);

    // Add position
    for (size_t i = 0; i < sizeof(Math::Vec2F) / sizeof(int); i++) {
      checksum += pos_ptr[i];
    }

    // Add velocity
    const auto& velocity = body.Velocity();
    const auto* velocity_ptr = reinterpret_cast<const int*>(&velocity);
    for (size_t i = 0; i < sizeof(Math::Vec2F) / sizeof(int); i++) {
      checksum += velocity_ptr[i];
    }

    // Add input.
    checksum += player.input;
    it++;
  }
  return checksum;
}

void GameLogic::RegisterNetworkLogic(NetworkLogic* network) {
  network_logic = network;
}

void GameLogic::SendFrameConfirmationEvent(
    const std::vector<Input::FrameInput>& remote_frame_inputs) noexcept {
#ifdef TRACY_ENABLE
  ZoneScoped;
#endif  // TRACY_ENABLE

  auto frame_to_confirm_it = std::find_if(
      remote_frame_inputs.begin(), remote_frame_inputs.end(),
      [this](const Input::FrameInput& frame_input) {
        return frame_input.frame_nbr == rollback_manager->frame_to_confirm();
      });

  auto end_it = remote_frame_inputs.end();

  const auto current_frame = rollback_manager->current_frame();

  // If the last remote inputs is greater than the current frame. The end
  // iterator must be equal to the frame input of the local current frame.
  if (remote_frame_inputs.back().frame_nbr > current_frame) {
    // Get the iterator of the inputs at the current frame to avoid to confirm
    // a frame greater than the local current frame.
    const auto current_frame_it =
        std::find_if(remote_frame_inputs.begin(), remote_frame_inputs.end(),
                     [current_frame](const Input::FrameInput& frame_input) {
                       return frame_input.frame_nbr == current_frame;
                     });

    end_it = current_frame_it;
  }

  while (frame_to_confirm_it != end_it) {
    const auto frame_to_confirm = *frame_to_confirm_it;

    if (frame_to_confirm.frame_nbr > current_frame) {
      // Tried to confirm a frame greater than the local current frame.
      break;
    }

    const int check_sum = rollback_manager->ConfirmFrame();

    ExitGames::Common::Hashtable event_check_sum;
    event_check_sum.put(static_cast<nByte>(EventKey::kCheckSum), check_sum);
    event_check_sum.put(static_cast<nByte>(EventKey::kPlayerInput),
                        last_inputs.data(), last_inputs.size());

    network_logic->RaiseEvent(true, EventCode::kFrameConfirmation,
                              event_check_sum);

    ++frame_to_confirm_it;
  }
}

void GameLogic::OnFrameConfirmationReceived(
    const ExitGames::Common::Hashtable& event_content) {
  if (client_player_nbr == kMasterClientId) {
    last_inputs.erase(last_inputs.begin());
    return;
  }

  std::vector<Input::FrameInput> frame_inputs{};

  const auto checksum_value =
      event_content.getValue(static_cast<nByte>(EventKey::kCheckSum));
  int checksum = ExitGames::Common::ValueObject<int>(checksum_value).getDataCopy();

  const auto input_value =
      event_content.getValue(static_cast<nByte>(EventKey::kPlayerInput));

  const Input::FrameInput* inputs =
      ExitGames::Common::ValueObject<Input::FrameInput*>(input_value)
          .getDataCopy();

  const int inputs_count =
      *ExitGames::Common::ValueObject<Input::FrameInput*>(input_value)
           .getSizes();

  frame_inputs.reserve(inputs_count);
  for (int i = 0; i < inputs_count; i++) {
    Input::FrameInput frame_input = inputs[i];
    frame_inputs.push_back(frame_input);
  }

  // If we did not receive the inputs before the frame to confirm, add them.
  if (rollback_manager->last_remote_input_frame() <
      frame_inputs.back().frame_nbr) {
    const int other_client_id = client_player_nbr == 0 ? 1 : 0;
    rollback_manager->SetRemotePlayerInput(frame_inputs, other_client_id);
  }

  const int check_sum = rollback_manager->ConfirmFrame();

  if (check_sum != checksum) {
    std::cerr << "Not same checksum for frame: "
              << rollback_manager->frame_to_confirm() << '\n';
    return;
  }

  // Send a frame confirmation event with empty data to the master client
  // just to tell him that we confirmed the frame and that he can erase
  // the input at the confirmed frame in its vector of inputs.
  network_logic->RaiseEvent(true, EventCode::kFrameConfirmation,
                            ExitGames::Common::Hashtable());

  last_inputs.erase(last_inputs.begin());
  ExitGames::Common::MemoryManagement::deallocateArray(inputs);
}

void GameLogic::Init() noexcept {
  timer_.OnStart();
  world_.Init();
  player_manager.SetUp();


  //// Border
  CreatePlatform({0, 0}, {0.0, 0.0}, {game::screen_width, border_size_});
  CreatePlatform({0, 0}, {0.0, 0.0}, {border_size_, game::screen_height});
  CreatePlatform({0, game::screen_height - border_size_}, {0, 0},
                 {game::screen_width, border_size_});
  CreatePlatform({game::screen_width - border_size_, 0}, {0, 0},
                 {border_size_, game::screen_height});

  ////Platform
  CreatePlatform({250 - static_cast<float>(platform_size_.X * 0.5),
                  game::screen_height - 150},
                 {0.0, 0.0}, {platform_size_});
  CreatePlatform(
      {game::screen_width - 250 - static_cast<float>(platform_size_.X * 0.5),
       game::screen_height - 150},
      {0.0, 0.0}, {platform_size_});
  CreatePlatform(
      {static_cast<float>(game::screen_width * 0.5) -
           static_cast<float>(platform_size_.X * 0.5),
       static_cast<float>(platform_size_.X * 0.5) + game::screen_height * 0.5f},
      {0, 0}, {platform_size_});

  //// Rope
  CreateRope({450 - static_cast<float>(20.0f * 0.5), game::screen_height - 650},
             {0.0, 0.0}, {20.0f, 250.0f});
  CreateRope({game::screen_width - 450 - static_cast<float>(20.0f * 0.5),
              game::screen_height - 650},
             {0.0, 0.0}, {20.0f, 250.0f});
}

void GameLogic::OnInputReceived(const ExitGames::Common::Hashtable& content) {
  std::vector<Input::FrameInput> remote_frame_inputs{};

  const auto input_value =
      content.getValue(static_cast<nByte>(EventKey::kPlayerInput));

  const Input::FrameInput* inputs =
      ExitGames::Common::ValueObject<Input::FrameInput*>(input_value)
          .getDataCopy();

  const int inputs_count =
      *ExitGames::Common::ValueObject<Input::FrameInput*>(input_value)
           .getSizes();

  if (inputs_count <= 0)
  {
	  return;
  }

  remote_frame_inputs.reserve(inputs_count);
  for (int i = 0; i < inputs_count; i++) {
    Input::FrameInput frame_input{inputs[i]};
    remote_frame_inputs.push_back(frame_input);
    // std::cout << static_cast<int>(frame_input.input) << " , " <<
    // frame_input.frame_nbr << std::endl;
  }

  if (remote_frame_inputs.back().frame_nbr <
      rollback_manager->last_remote_input_frame()) {
    // received old input, no need to send confirm packet.
    return;
  }

  const int other_client_id = client_player_nbr == 0 ? 1 : 0;
  rollback_manager->SetRemotePlayerInput(remote_frame_inputs, other_client_id);

  if (client_player_nbr == kMasterClientId) {
    SendFrameConfirmationEvent(remote_frame_inputs);
  }

  ExitGames::Common::MemoryManagement::deallocateArray(inputs);
}

void GameLogic::Update() noexcept {
  if (current_game_state != GameState::GameLaunch) {
    return;
  }
  rollback_manager->IncreaseCurrentFrame();

  while (!network_events.empty()) {
    const auto& event = network_events.front();

    switch (event.code) {
      case EventCode::kInput:
        OnInputReceived(event.content);
        break;
      case EventCode::kFrameConfirmation:
        OnFrameConfirmationReceived(event.content);
        break;
      default:
        break;
    }
    network_events.pop();
  }

  // PlayerManager Input
  ManageInput();
  for (int i = 0; i < game::max_player; i++) {
    auto input = rollback_manager->GetLastPlayerConfirmedInput(i);
    SetPlayerInput(input, i);
  }
  UpdateGameplay();
  //
  if (player_manager.players[0].life_point <= 0 ||
      player_manager.players[1].life_point <= 0) {
    current_game_state = GameState::GameVictory;
  }
}

void GameLogic::DeInit() noexcept {
  world_.Clear();

  world_.contactListener = nullptr;
  colliders_.empty();
}

void GameLogic::ManageInput() noexcept {
  inputs.UpdatePlayerInputs();
  inputs.frame_nbr = rollback_manager->current_frame();
  last_inputs.emplace_back(inputs);
  rollback_manager->SetLocalPlayerInput(inputs, client_player_nbr);

  // Send Input
  ExitGames::Common::Hashtable event_data;
  event_data.put(static_cast<nByte>(EventCode::kInput), last_inputs.data(),
                 static_cast<int>(last_inputs.size()));
  network_logic->RaiseEvent(false, EventCode::kInput, event_data);
}

void GameLogic::UpdateGameplay() noexcept {
  world_.Update(fixedUpdateFrenquency);
  int it = 0;
  for (auto& player : player_manager.players) {
    // Projectile
    if (player.attack_timer >= 0.0f) {
      player.attack_timer -= fixedUpdateFrenquency;
    } else {
      player.is_projectile_ready = true;
    }

    // Gravity
    player.is_grounded = player.trigger_nbr > 1;
    // std::cout << "player " << it << " trigger nbrs : " << player.trigger_nbr
    // << std::endl;
    auto& body = world_.GetBody(player_manager.players_BodyRefs_[it]);
    if (!player.is_grounded) {
      body.AddForce({0, PlayerManager::gravity_});
    } else {
      // for the rope
      body.AddForce({0, PlayerManager::rope_gravity_});
    }
    it++;
  }

  for (int i = 0; i < game::max_player; i++) {
    if (player_manager.players[i].input &
        static_cast<std::uint8_t>(Input::kAttack)) {
      player_manager.Attack(i);
    }

    if (player_manager.players[i].input &
        static_cast<std::uint8_t>(Input::kJump)) {
      player_manager.Jump(i);
    }
    if (player_manager.players[i].input &
        static_cast<std::uint8_t>(Input::kLeft)) {
      player_manager.Move(false, i);
    } else if (player_manager.players[i].input &
               static_cast<std::uint8_t>(Input::kRight)) {
      player_manager.Move(true, i);
    } else {
      player_manager.Decelerate(i);
    }
  }

  UpdateCollider();
  player_manager.Update();
}

void GameLogic::CreatePlatform(Math::Vec2F position, Math::Vec2F rectMinBound,
                               Math::Vec2F rectMaxBound) noexcept {
  Physics::BodyRef bodyRef = world_.CreateBody();
  auto& newBody = world_.GetBody(bodyRef);
  newBody.SetMass(10);
  newBody.SetPosition(position);
  newBody.SetVelocity(Math::Vec2F(0, 0));
  newBody.type = Physics::BodyType::STATIC;

  Physics::ColliderRef colliderRef = world_.CreateCollider(bodyRef);
  auto& newCollider = world_.GetCollider(colliderRef);
  newCollider._shape = Math::ShapeType::Rectangle;
  newCollider.isTrigger = false;
  newCollider.restitution = 0.0f;
  newCollider.rectangleShape.SetMinBound(rectMinBound);
  newCollider.rectangleShape.SetMaxBound(rectMaxBound);
  newCollider.ID = platform_collider_id_;
  colliders_.emplace_back(collider{bodyRef, colliderRef});
}

void GameLogic::CreateRope(Math::Vec2F position, Math::Vec2F rectMinBound,
                           Math::Vec2F rectMaxBound) noexcept {
  Physics::BodyRef bodyRef = world_.CreateBody();
  auto& newBody = world_.GetBody(bodyRef);
  newBody.SetMass(1);
  newBody.SetPosition(position);
  newBody.SetVelocity(Math::Vec2F(0, 0));
  newBody.type = Physics::BodyType::STATIC;

  Physics::ColliderRef colliderRef = world_.CreateCollider(bodyRef);
  auto& newCollider = world_.GetCollider(colliderRef);
  newCollider._shape = Math::ShapeType::Rectangle;
  newCollider.isTrigger = true;
  newCollider.restitution = 0.0f;
  newCollider.rectangleShape.SetMinBound(rectMinBound);
  newCollider.rectangleShape.SetMaxBound(rectMaxBound);
  newCollider.ID = rope_collider_id_;
  colliders_.emplace_back(collider{bodyRef, colliderRef});
}

void GameLogic::UpdateCollider() noexcept {
  for (auto& collider : colliders_) {
    auto& physicsCollider = world_.GetCollider(collider.colliderRef);
    auto& physicsBody = world_.GetBody(collider.bodyRef);
    switch (physicsCollider._shape) {
      case Math::ShapeType::Rectangle:
        physicsCollider.rectangleShape = Math::RectangleF(
            physicsBody.Position(),
            physicsBody.Position() + physicsCollider.rectangleShape.MaxBound() -
                physicsCollider.rectangleShape.MinBound());
        break;
      case Math::ShapeType::Circle:
        physicsCollider.circleShape = Math::CircleF(
            physicsBody.Position(), physicsCollider.circleShape.Radius());
        break;
      default:
        break;
    }
  }
}
}  // namespace game
