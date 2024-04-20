#pragma once

#include "event.h"

#include <Common-cpp/inc/Logger.h>
#include <LoadBalancing-cpp/inc/Client.h>
#include <LoadBalancing-cpp/inc/Listener.h>

#include "GameLogic.h"


class NetworkLogic : private ExitGames::LoadBalancing::Listener {
 public:
  bool is_connected = false;
  NetworkLogic(const ExitGames::Common::JString& appID,
               const ExitGames::Common::JString& appVersion, game::GameLogic* game_logic);
  void Connect();
  void Disconnect();
  void Run();

  void CreateRoom(const ExitGames::Common::JString& roomName, nByte maxPlayers);

  void JoinRandomRoom(
      ExitGames::Common::Hashtable expectedCustomRoomProperties =
          ExitGames::Common::Hashtable());

  void JoinRandomOrCreateRoom() noexcept;

    void RaiseEvent(
      bool reliable, EventCode event_code,
      const ExitGames::Common::Hashtable& event_data) noexcept;

    void ReceiveEvent(
        int player_nr, EventCode event_code,
        const ExitGames::Common::Hashtable& event_content) noexcept;

 private:
  ExitGames::LoadBalancing::Client mLoadBalancingClient;
  ExitGames::Common::Logger mLogger;  // accessed by EGLOG()
  game::GameLogic* game_logic_;

  void debugReturn(int debugLevel,
                   const ExitGames::Common::JString& string) override;
  void connectionErrorReturn(int errorCode) override;
  void clientErrorReturn(int errorCode) override;
  void warningReturn(int warningCode) override;
  void serverErrorReturn(int errorCode) override;
  void joinRoomEventAction(
      int playerNr, const ExitGames::Common::JVector<int>& playernrs,
      const ExitGames::LoadBalancing::Player& player) override;
  void leaveRoomEventAction(int playerNr, bool isInactive) override;
  void customEventAction(
      int playerNr, nByte eventCode,
      const ExitGames::Common::Object& eventContent) override;
  void connectReturn(int errorCode,
                     const ExitGames::Common::JString& errorString,
                     const ExitGames::Common::JString& region,
                     const ExitGames::Common::JString& cluster) override;

  void disconnectReturn() override;

  void leaveRoomReturn(int errorCode,
                       const ExitGames::Common::JString& errorString) override;

};
