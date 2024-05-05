#pragma once

#include <Common-cpp/inc/Logger.h>
#include <LoadBalancing-cpp/inc/Client.h>
#include <LoadBalancing-cpp/inc/Listener.h>

#include "GameLogic.h"
#include "event.h"

/**
 * @brief NetworkLogic is a class responsible for managing network operations
 * and interactions.
 *
 * It facilitates communication between the game client and server, handling
 * tasks such as connecting to the server, creating and joining rooms, sending
 * and receiving events, and managing network errors using Photon services.
 */
class NetworkLogic : private ExitGames::LoadBalancing::Listener {
 public:
  bool is_connected = false; /* Indicates whether the client is currently
                                connected to the server. */
  const char* currentLogInfo =
      ""; /* Information about the current network state. */

  /**
   * @brief Constructs a new NetworkLogic object.
   *
   * @param appID The application ID used for connection.
   * @param appVersion The application version used for connection.
   * @param game_logic Pointer to the game logic object.
   */
  NetworkLogic(const ExitGames::Common::JString& appID,
               const ExitGames::Common::JString& appVersion,
               game::GameLogic* game_logic);

  void Connect();    /* Connects the client to the server. */
  void Disconnect(); /* Disconnects the client from the server. */
  void Run(); /* Runs the network loop for handling events and operations. */

  /**
   * @brief Creates a room on the server.
   *
   * @param roomName The name of the room to be created.
   * @param maxPlayers The maximum number of players allowed in the room.
   */
  void CreateRoom(const ExitGames::Common::JString& roomName, nByte maxPlayers);

  /**
   * @brief Joins a random room on the server.
   *
   * @param expectedCustomRoomProperties Expected custom room properties for
   * room matching.
   */
  void JoinRandomRoom(
      ExitGames::Common::Hashtable expectedCustomRoomProperties =
          ExitGames::Common::Hashtable());

  /**
   * @brief Joins a random room if available, otherwise creates a new room.
   */
  void JoinRandomOrCreateRoom() noexcept;

  /**
   * @brief Raises an event to the server.
   *
   * @param reliable Indicates whether the event transmission should be
   * reliable.
   * @param event_code The code associated with the event.
   * @param event_data The data associated with the event.
   */
  void RaiseEvent(bool reliable, EventCode event_code,
                  const ExitGames::Common::Hashtable& event_data) noexcept;

  /**
   * @brief Receives an event from the server.
   *
   * @param player_nr The player number associated with the event.
   * @param event_code The code associated with the received event.
   * @param event_content The content of the received event.
   */
  void ReceiveEvent(int player_nr, EventCode event_code,
                    const ExitGames::Common::Hashtable& event_content) noexcept;

 private:
  ExitGames::LoadBalancing::Client
      mLoadBalancingClient;          /* The LoadBalancing client instance. */
  ExitGames::Common::Logger mLogger; /* Logger instance for debug messages. */
  game::GameLogic* game_logic_;      /* Pointer to the game logic object. */

  // Listener callbacks
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
