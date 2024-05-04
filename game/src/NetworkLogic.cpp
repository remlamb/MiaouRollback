#include "NetworkLogic.h"

#include "RollbackManager.h"

void NetworkLogic::debugReturn(int debugLevel,
                               const ExitGames::Common::JString& string) {
  std::cout << "debug return: debug level: " << debugLevel
            << " string: " << string.UTF8Representation().cstr() << '\n';
}

void NetworkLogic::connectionErrorReturn(int errorCode) {
  std::cout << "error connection\n";
  currentLogInfo = "- error connection";
}

void NetworkLogic::clientErrorReturn(int errorCode) {
  std::cout << "client error code : " << errorCode << '\n';
}

void NetworkLogic::warningReturn(int warningCode) {
  std::cout << "client warning code : " << warningCode << '\n';
}

void NetworkLogic::serverErrorReturn(int errorCode) {
  std::cout << "server error code : " << errorCode << '\n';
}

void NetworkLogic::joinRoomEventAction(
    int playerNr, const ExitGames::Common::JVector<int>& playernrs,
    const ExitGames::LoadBalancing::Player& player) {
  std::cout << "Room state: player nr: " << playerNr
            << " player nrs size: " << playernrs.getSize() << " player userID: "
            << player.getUserID().UTF8Representation().cstr() << '\n';
  currentLogInfo = "- Room Join, currently player ";

  // If currentClientPlayer is not set
  if (game_logic_->client_player_nbr == -1) {
    game_logic_->client_player_nbr = playerNr - 1;
    game_logic_->rollback_manager->confirmed_game_manager_.client_player_nbr =
        playerNr - 1;
  }
  if (playerNr >= game::max_player) {
    game_logic_->current_game_state = game::GameState::GameLaunch;
    game_logic_->rollback_manager->confirmed_game_manager_.current_game_state =
        game::GameState::GameLaunch;
  }
}

void NetworkLogic::leaveRoomEventAction(int playerNr, bool isInactive) {
  std::cout << "player nr: " << playerNr << " is inactive: " << isInactive
            << '\n';
}

void NetworkLogic::customEventAction(
    int playerNr, nByte eventCode,
    const ExitGames::Common::Object& eventContent) {
  if (eventContent.getType() != ExitGames::Common::TypeCode::HASHTABLE) {
    std::cerr << "Unsupported event content type \n";
    return;
  }

  const ExitGames::Common::Hashtable& event_data =
      ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(eventContent)
          .getDataCopy();

  ReceiveEvent(playerNr, static_cast<EventCode>(eventCode), event_data);
}

void NetworkLogic::connectReturn(int errorCode,
                                 const ExitGames::Common::JString& errorString,
                                 const ExitGames::Common::JString& region,
                                 const ExitGames::Common::JString& cluster) {
  std::cout << "connected, error code:" << errorCode << " "
            << "error string: " << errorString.UTF8Representation().cstr()
            << " "
            << "region: " << region.UTF8Representation().cstr() << " "
            << "cluster: " << cluster.UTF8Representation().cstr() << '\n';
  currentLogInfo = "- client connected";
  is_connected = true;
}

void NetworkLogic::disconnectReturn() {
  std::cout << "client disconnected\n";
  currentLogInfo = "- client disconnected";
  is_connected = false;
  game_logic_->current_game_state = game::GameState::LogMenu;
}

void NetworkLogic::leaveRoomReturn(
    int errorCode, const ExitGames::Common::JString& errorString) {
  std::cout << "Leave room return: error code: " << errorCode
            << " error string: " << errorString.UTF8Representation().cstr()
            << '\n';
}

NetworkLogic::NetworkLogic(const ExitGames::Common::JString& appID,
                           const ExitGames::Common::JString& appVersion,
                           game::GameLogic* game_logic)
    : mLoadBalancingClient(*this, appID, appVersion) {
  game_logic_ = game_logic;
}

void NetworkLogic::Connect() {
  std::cout << "hello\n";
  if (!mLoadBalancingClient.connect()) {
    EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"Could not connect.");
    currentLogInfo = "- Could not connect.";
  }
}

void NetworkLogic::Run() { mLoadBalancingClient.service(); }

void NetworkLogic::Disconnect() { mLoadBalancingClient.disconnect(); }

void NetworkLogic::CreateRoom(const ExitGames::Common::JString& roomName,
                              nByte maxPlayers) {
  if (mLoadBalancingClient.opCreateRoom(
          roomName,
          ExitGames::LoadBalancing::RoomOptions().setMaxPlayers(maxPlayers))) {
    std::cout << "Room successfully created with name: "
              << roomName.UTF8Representation().cstr() << '\n';
  }
}

void NetworkLogic::JoinRandomRoom(
    ExitGames::Common::Hashtable expectedCustomRoomProperties) {
  mLoadBalancingClient.opJoinRandomRoom(expectedCustomRoomProperties);
}

void NetworkLogic::JoinRandomOrCreateRoom() noexcept {
  const auto game_id = ExitGames::Common::JString();
  const ExitGames::LoadBalancing::RoomOptions room_options(true, true,
                                                           game::max_player);
  if (!mLoadBalancingClient.opJoinRandomOrCreateRoom(game_id, room_options)) {
    EGLOG(ExitGames::Common::DebugLevel::ERRORS,
          L"Could not join or create room.");
    currentLogInfo = "- Could not join or create room.";
  }
}

void NetworkLogic::RaiseEvent(
    bool reliable, EventCode event_code,
    const ExitGames::Common::Hashtable& event_data) noexcept {
  if (!mLoadBalancingClient.opRaiseEvent(reliable, event_data,
                                         static_cast<nByte>(event_code))) {
    EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"Could not raise event.");
  }
}

void NetworkLogic::ReceiveEvent(
    int player_nr, EventCode event_code,
    const ExitGames::Common::Hashtable& event_content) noexcept {
  NetworkEvent event{event_code, event_content};
  game_logic_->network_events.push(event);
}
