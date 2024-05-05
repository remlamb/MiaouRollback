#pragma once
#include <Common-cpp/inc/Containers/Hashtable.h>
#include <Common-cpp/inc/defines.h>

/**
 * @brief The EventCode enum represents different types of events that can occur
 * in the application, such as input events or frame confirmation events.
 */
enum class EventCode : nByte {
  kInput = 0,         // Input event code.
  kFrameConfirmation  // Frame confirmation event code.
};

/**
 * @brief The EventKey enum represents different keys used in serializing and
 * deserializing hashtable events, such as keys for player input, frame numbers,
 * delays, or checksums.
 */
enum class EventKey : nByte {
  kPlayerInput = 0,  // Key for player input data.
  kFrameNbr,         // Key for frame number data.
  kDelay,            // Key for delay data.
  kCheckSum          // Key for checksum data.
};

/**
 * @brief NetworkEvent represents an event sent over the network.
 * The NetworkEvent struct encapsulates an event code and a hashtable containing
 * event content.
 */
struct NetworkEvent {
  EventCode code{};  // Event code indicating the type of event.
  ExitGames::Common::Hashtable content{};  // Hashtable containing event data.
};