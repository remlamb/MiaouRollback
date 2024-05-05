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
	 * @brief Represents the game logic of the application, managing physics simulation,
	 * player interaction, and input handling.
	 *
	 * The GameLogic class initializes and updates the game world, manages player input,
	 * handles collision detection, and controls game state transitions. It also creates
	 * platforms and ropes within the game world and updates their associated colliders.
	 *
	 * Member Variables:
	 * - world_: The physics world for simulating game physics.
	 * - rollback_manager: Manages game state rollback for network synchronization.
	 * - network_logic: Manages networking logic and communication.
	 * - player_manager: Manages player entities and their interactions with the game world.
	 * - inputs: Stores the current frame inputs for all players.
	 * - last_inputs: Stores the inputs from the previous frames for rollback purposes.
	 * - network_events: Queue to store incoming network events.
	 * - colliders_: Vector of collider structures representing physics objects and their colliders.
	 * - client_player_nbr: ID of the local client player.
	 * - current_game_state: Enum representing the current state of the game.
	 *
	 * Member Functions:
	 * - Rollback: Rolls back the game state to match the provided GameLogic instance.
	 * - SetPlayerInput: Sets the input for a specific player.
	 * - ComputeChecksum: Computes a checksum for the current game state.
	 * - RegisterNetworkLogic: Registers the NetworkLogic instance for networking purposes.
	 * - SendFrameConfirmationEvent: Sends frame confirmation events to confirm synchronized frames.
	 * - OnFrameConfirmationReceived: Handles frame confirmation events received from the network.
	 * - Init: Initializes the game environment, including creating platforms and ropes.
	 * - OnInputReceived: Handles input events received from the network.
	 * - Update: Updates the game logic, processes inputs, and advances the game state.
	 * - DeInit: Deinitializes the game environment.
	 * - ManageInput: Manages player inputs and sends input events to the network.
	 * - UpdateGameplay: Updates player actions and game physics.
	 * - CreatePlatform: Creates a platform in the game world.
	 * - CreateRope: Creates a rope in the game world.
	 * - UpdateCollider: Updates the colliders in the game world.
	 */
	class GameLogic {
	public:
		GameLogic(RollbackManager* rollback) { rollback_manager = rollback; }


		Physics::World world_;// The physics world for simulating game physics.
		RollbackManager* rollback_manager;// Manages game state rollback for network synchronization.
		NetworkLogic* network_logic; // Manages networking logic and communication.
		PlayerManager player_manager{ &world_ }; // Manages player entities and their interactions.
		Input::FrameInput inputs; //Stores the current frame inputs for all players.
		std::vector<Input::FrameInput> last_inputs; // Stores inputs from previous frames for rollback.
		std::queue<NetworkEvent> network_events; // Queue to store incoming network events.
		std::vector<game::collider> colliders_; // Vector of collider structures representing physics objects and their colliders.
		static constexpr float fixedUpdateFrenquency = 1 / 50.f; //Frenquency of the world Update fixed for 50hz
		static constexpr int invalid_client_player_nbr = -1; 
		int client_player_nbr = invalid_client_player_nbr; // ID of the local client player.
		static constexpr int master_client_ID = 0;
		GameState current_game_state = GameState::LogMenu; // Enum representing the current state of the game.

		/**
		* @brief Rolls back the game state to match the provided `GameLogic` instance.
		* @param game_logic The `GameLogic` instance to roll back to.
		 * @details This function updates the current game state, physics world, and player entities to match the state of another `GameLogic` instance.
		 */
		void Rollback(const GameLogic& game_logic);
		/**
		 * @brief Sets the input for a specific player.
		 * @param input The input to set for the player.
		 * @param player_id The ID of the player whose input is being set.
		 */
		void SetPlayerInput(const Input::FrameInput& input, int player_id);
		/**
		 * @brief Computes a checksum for the current game state.
		 * @return The computed checksum value.
		 */
		int ComputeChecksum();
		/**
		 * @brief Registers the network logic instance.
		 * @param network The `NetworkLogic` instance to register.
		 */
		void RegisterNetworkLogic(NetworkLogic* network);
		/**
		 * @brief Sends frame confirmation events to the master client.
		 * @param remote_frame_inputs The remote frame inputs to confirm.
		 */
		void SendFrameConfirmationEvent(
			const std::vector<Input::FrameInput>& remote_frame_inputs) noexcept;
		/**
		 * @brief Handles frame confirmation events received from the master client.
		 * @param event_content The content of the received event.
		 */
		void OnFrameConfirmationReceived(
			const ExitGames::Common::Hashtable& event_content);

		/**
		 * @brief Initializes the game environment.
		 */
		void Init() noexcept;

		/**
		 * @brief Handles input events received from the network.
		 * @param content The content of the received event.
		 */
		void OnInputReceived(const ExitGames::Common::Hashtable& content);


		/**
		 * @brief Updates the player gameplay logic based on player inputs.
		 */
		void UpdateGameplay() noexcept;


		/**
		 * @brief Updates the game logic and physics simulation.
		 */
		void Update() noexcept;
		/**
		 * @brief Deinitializes the game environment.
		 */
		void DeInit() noexcept;

		/**
		 * @brief Manages player inputs and sends input events to the network.
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
		static constexpr int platform_collider_id_ = 10; //ID for platform collider
		static constexpr int rope_collider_id_ = 11; //ID for rope collider

		const float border_size_ = 20.0f; //Size for border Collider
		const Math::Vec2F platform_size_{ 220.0f, 40.0f };  //Size for platform Collider
	};
}  // namespace game
