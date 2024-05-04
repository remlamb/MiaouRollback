#pragma once
#include <queue>
#include <vector>

#include "FrameInput.h"
#include "PlayerManager.h"
#include "Timer.h"
#include "World.h"
#include "event.h"

class RollbackManager;
class NetworkLogic;
namespace game {
enum class GameState { LogMenu, GameLaunch, GameVictory };

/**
 * @brief Represents a link between a physics object and its collider.
 * The collider struct consists of two members:
 * - `bodyRef`:  Reference to the physics body.
 * - `colliderRef`: Reference to the physics collider.
 *
 */
struct collider {
  Physics::BodyRef bodyRef;
  Physics::ColliderRef colliderRef;
};

/**
 * @brief This class manages the game logic, including physics simulation,
 * player management, and input handling. The `GameLogic` class initializes and
 * updates the game world, manages player input, and handles collision detection
 * and response. It creates platforms and ropes within the game world and
 * updates their associated colliders accordingly.
 *
 * The class contains a physics world, player manager, and inputs manager as
 * member variables. It also maintains constants for screen width and height, as
 * well as parameters for platform_ and rope creation.
 */
class GameLogic {
 public:
  // event etat spawning, etat de gameplay -> qui bouge les joueurs avec un
  // timer qui freeze un certain temps, (pas de input)
  GameLogic(RollbackManager* rollback) { rollback_manager = rollback; }
  static constexpr int kMasterClientId = 0;
  Physics::World world_;
  RollbackManager* rollback_manager;
  NetworkLogic* network_logic;
  void Rollback(const GameLogic& game_logic);
  void SetPlayerInput(const Input::FrameInput& input, int player_id);
  int ComputeChecksum();
  void RegisterNetworkLogic(NetworkLogic* network);
  void GameLogic::OnFrameConfirmationReceived(
      const ExitGames::Common::Hashtable& event_content);

  PlayerManager player_manager{&world_};

  Input::FrameInput inputs;
  std::vector<Input::FrameInput> last_inputs;
  std::queue<NetworkEvent> network_events;

  static constexpr int invalid_client_player_nbr = -1;
  int client_player_nbr = invalid_client_player_nbr;

  GameState current_game_state = GameState::LogMenu;
  static constexpr float fixedUpdateFrenquency = 1 / 50.f;

  std::vector<game::collider> colliders_;

  /**
   * @brief Init the game environment.
   */
  void Init() noexcept;

  void OnInputReceived(const ExitGames::Common::Hashtable& content);
  void UpdateGameplay() noexcept;
  void SendFrameConfirmationEvent(
      const std::vector<Input::FrameInput>& remote_frame_inputs) noexcept;

  /**
   * @brief Updates the game logic, colliders, inputs and apply inputs for
   * players.
   */
  void Update() noexcept;
  /**
   * @brief Deinit the game environment.
   */
  void DeInit() noexcept;

  /**
   * @brief Manages player inputs, link between the player and game inputs.
   */
  void ManageInput() noexcept;

  /**
   * @brief Creates a platform in the game world.
   * @param position The position of the platform.
   * @param rectMinBound The minimum bounds of the platform's rectangle.
   * @param rectMaxBound The maximum bounds of the platform's rectangle.
   */
  void CreatePlatform(Math::Vec2F position, Math::Vec2F rectMinBound,
                      Math::Vec2F rectMaxBound) noexcept;
  /**
   * @brief Creates a rope in the game world.
   * @param position The position of the rope.
   * @param rectMinBound The minimum bounds of the rope's rectangle.
   * @param rectMaxBound The maximum bounds of the rope's rectangle.
   */
  void GameLogic::CreateRope(Math::Vec2F position, Math::Vec2F rectMinBound,
                             Math::Vec2F rectMaxBound) noexcept;

  /**
   * @brief Updates the colliders in the game world.
   */
  void UpdateCollider() noexcept;

 private:
  static constexpr int platform_collider_id_ = 10;
  static constexpr int rope_collider_id_ = 11;

  const float border_size_ = 20.0f;
  const Math::Vec2F platform_size_{220.0f, 40.0f};
  Physics::Timer timer_;
};
}  // namespace game
