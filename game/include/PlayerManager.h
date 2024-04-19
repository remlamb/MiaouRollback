#pragma once
#include "World.h"
#include <raylib.h>

/**
 * @brief Represents the Player position, and event that must be shared with other client.
 * The collider struct consists of three members:
 * - `position`:  Reference the position of the player at the current frame.
 * - `is_grounded`: Reference if the player is or isn't touching the ground at the current frame.
 * - `velocity`: Reference to the player movement speed in the physical world.
 */
struct Player {
	Math::Vec2F position = { 50, 650 };  // in pixels
	bool is_grounded = false;
	Math::Vec2F velocity = { 0, 0 };
};

/**
 * @brief Manages player entities in the game, including physics interactions, movement, and input handling.
 *
 * The PlayerManager class handles the creation and update of player entities within the game world.
 * It manages player physics bodies, colliders, and interactions with the game environment.
 * This class also implements collision and trigger event handling for player entities.
 */
class PlayerManager : public Physics::ContactListener {
private:
	Physics::World* world_;

	static constexpr float gravity_ = 1000;
	static constexpr float rope_gravity_ = 10;
	static constexpr float jump_velocity_ = -580.0f;
	static constexpr float move_velocity_ = 500.0f;
	static constexpr float collider_radius_ = 28.0f;
	static constexpr float grounded_collider_pos_y_ = -20.0f;
	static constexpr Math::Vec2F grounded_collider_dimension_{ 10.0f, 10.0f };

	static constexpr float acceleration_time_ = 0.04f;
	static constexpr float deceleration_time_ = 0.1f;

	static constexpr int nbr_player_ = 2;


	std::array<Physics::BodyRef, nbr_player_> players_BodyRefs_;
	std::array<Physics::ColliderRef, nbr_player_> players_CollidersRefs_;

	std::array<Physics::BodyRef, nbr_player_> players_grounded_BodyRefs_;
	std::array<Physics::ColliderRef, nbr_player_> players_grounded_CollidersRefs_;
	std::array<int, nbr_player_> trigger_nbrs_;
	int collider_id_ = 0;


public:
	PlayerManager(Physics::World* world_);

	/**
	 * @brief Create player entities within the game world.
	 */
	void SetUp();
	/**
	 * @brief Updates player entities and their interactions.
	 */
	void Update();

	/**
	 * @brief Initiates a jump action for a specified player.
	 * @param playerIdx Index of the player.
	 */
	void Jump(int playerIdx);
	/**
	 * @brief Initiates a movement action for a specified player.
	 * @param rightDirection Flag indicating the movement direction (true for right, false for left).
	 * @param playerIdx Index of the player.
	 */
	void Move(bool rightDirection, int playerIdx);
	/**
	* @brief Initiates a deceleration action for a specified player.
	* @param playerIdx Index of the player.
	*/
	void Decelerate(int playerIdx);

	/**
	* @brief Draws collider form for debug information for player entities.
	*/
	void DrawDebug();


	void OnTriggerEnter(Physics::Collider colliderA,
		Physics::Collider colliderB) noexcept override;
	void OnTriggerExit(Physics::Collider colliderA,
		Physics::Collider colliderB) noexcept override;
	void OnCollisionEnter(Physics::Collider colliderA,
		Physics::Collider colliderB) noexcept override;
	void OnCollisionExit(Physics::Collider colliderA,
		Physics::Collider colliderB) noexcept override;

	std::array<Player, nbr_player_> players;
};
