#include "NetworkLogic.h"

void NetworkLogic::debugReturn(int debugLevel,
                               const ExitGames::Common::JString& string) {
  std::cout << "debug return: debug level: " << debugLevel
            << " string: " << string.UTF8Representation().cstr() << '\n';
}

void NetworkLogic::connectionErrorReturn(int errorCode) {
  std::cout << "error connection\n";
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

	//If currentClientPlayer is not set
  if (game_logic_->currentClientPlayer == -1)
  {
    game_logic_->currentClientPlayer = playerNr - 1;
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
}

void NetworkLogic::disconnectReturn() { std::cout << "client disconnected\n"; }

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
  if (!mLoadBalancingClient.connect())
    EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"Could not connect.");
  else {
    is_connected = true;
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
  const ExitGames::LoadBalancing::RoomOptions room_options(true, true, 2);
  if (!mLoadBalancingClient.opJoinRandomOrCreateRoom(game_id, room_options))
    EGLOG(ExitGames::Common::DebugLevel::ERRORS,
          L"Could not join or create room.");
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
  // logging the string representation of the eventContent can be really useful
  // for debugging, but use with care: for big events this might get expensive
  EGLOG(ExitGames::Common::DebugLevel::ALL,
        L"an event of type %d from player Nr %d with the following content has "
        L"just arrived: %ls",
        static_cast<nByte>(event_code), player_nr,
        event_content.toString(true).cstr());

  std::cout << "event content: "
            << event_content.toString().UTF8Representation().cstr() << '\n';

  switch (event_code) {
    case EventCode::kInput: {
      break;
    }
    default: {
      // have a look at demo_typeSupport inside the C++ client SDKs for
      // example code on how to send and receive more fancy data types
    } break;
  }
}