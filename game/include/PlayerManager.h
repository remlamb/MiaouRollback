#pragma once
#include "Constants.h"
#include "FrameInput.h"
#include "World.h"
#include "raylib_wrapper.h"
using namespace raylib;

/**
 * @brief Represents value shared with other clients of player entity in the game.
 *
 * Member Variables:
 * - is_grounded: Flag indicating whether the player is grounded or not.
 * - is_projectile_ready: Flag indicating whether the player is ready to launch a projectile.
 * - trigger_nbr: Number of trigger events detected by the player.
 * - life_point: Remaining life points of the player.
 * - attack_timer: Time remaining until the player can perform another attack.
 * - input: Input data for the player.
 */
struct Player {
	bool is_grounded = false;
	bool is_projectile_ready = true;
	int trigger_nbr = 0;
	int life_point = 5;
	float attack_timer = 0.0f;
	std::uint8_t input;
};


/**
 * @brief Represents a projectile entity in the game.
 *
 * Member Variables:
 * - projectile_body: Reference to the physics body of the projectile.
 * - projectile_collider: Reference to the collider of the projectile.
 * - current_collider_nbr: Number representing the current collider of the projectile.
 * - nbr_launching_player: Number representing the player who launched the projectile.
 * - isActive: Flag indicating whether the projectile is active or not.
 */
struct Projectile {
	Physics::BodyRef projectile_body;
	Physics::ColliderRef projectile_collider;
	int current_collider_nbr = 0;
	int nbr_launching_player = 0;
	bool isActive = false;
};

/**
 * @brief Represents player entities in the game, including physics interactions, movement, and input handling.
 *
 *
 * The PlayerManager class handles the creation and update of player entities
 * within the game world. It manages player physics bodies, colliders, and
 * interactions with the game environment. This class also implements collision
 * and trigger event handling for player entities. Manage also players projectiles.
 *
 * Member Variables:
 * - world_: The physics world for simulating game physics.
 * - jump_velocity_: The velocity applied when a player jumps.
 * - move_velocity_: The velocity applied when a player moves.
 * - collider_radius_: The radius of the player's collider.
 * - grounded_collider_pos_y_: The Y position offset of the player's grounded collider.
 * - grounded_collider_dimension_: The dimensions of the player's grounded collider.
 * - acceleration_time_: The time it takes for a player to reach full velocity when accelerating.
 * - deceleration_time_: The time it takes for a player to come to a stop when decelerating.
 * - nbr_player_: The number of players.
 * - player1_collider_id_: The collider ID for player 1.
 * - player2_collider_id_: The collider ID for player 2.
 * - player1_groundedcollider_id_: The collider ID for player 1's grounded collider.
 * - player2_groundedcollider_id_: The collider ID for player 2's grounded collider.
 * - player1_spawn_pos_: The spawn position for player 1.
 * - player2_spawn_pos_: The spawn position for player 2.
 * - projectile_radius_: The radius of the projectile collider.
 * - projectile_id_: The ID of the projectile collider.
 * - neutral_projectile_id_: The ID of neutral projectiles collider.
 * - projectile_speed_: The speed of the projectile.
 * - time_between_attack: The time between each attack action.
 * - current_projectile_collider_id_: The ID of the current projectile collider, current projectile collider ID is increased for each collider created so each projectile are unique before being neutral
 * - max_projectile_: The maximum number of projectiles allowed in the game.
 * - gravity_: The gravity force applied to players.
 * - rope_gravity_: The gravity force applied to player on ropes.
 * - players: Array of Player structs representing player entities.
 * - players_BodyRefs_: Array of player body references.
 * - players_CollidersRefs_: Array of player collider references.
 * - players_grounded_BodyRefs_: Array of grounded player body references.
 * - players_grounded_CollidersRefs_: Array of grounded player collider references.
 * - projectiles_: Array of Projectile structs representing projectiles.
 */
class PlayerManager : public Physics::ContactListener {
private:
	Physics::World* world_; /* The physics world for simulating game physics.*/
	static constexpr float jump_velocity_ = -580.0f; /* The velocity applied when a player jumps*/
	static constexpr float move_velocity_ = 500.0f; /* The velocity applied when a player moves*/
	static constexpr float collider_radius_ = 28.0f; /* The radius of the player's collider*/
	static constexpr float grounded_collider_pos_y_ = -20.0f; /* The Y position offset of the player's grounded collider*/
	static constexpr Math::Vec2F grounded_collider_dimension_{ 10.0f, 10.0f }; /*The dimensions of the player's grounded collider*/

	static constexpr float acceleration_time_ = 0.04f; /*The time it takes for a player to reach full velocity when accelerating*/
	static constexpr float deceleration_time_ = 0.1f; /*The time it takes for a player to come to a stop when decelerating*/

	static constexpr int nbr_player_ = game::max_player; /*The number of players*/
	static constexpr int player1_collider_id_ = 1; /*The collider ID for player 1*/
	static constexpr int player2_collider_id_ = 2; /*The collider ID for player 2*/

	static constexpr int player1_groundedcollider_id_ = 3; /* The collider ID for player 1's grounded collider*/
	static constexpr int player2_groundedcollider_id_ = 4; /* The collider ID for player 2's grounded collider*/

	static constexpr Math::Vec2F player1_spawn_pos_ = { 250, 550 }; /*The spawn position for player 1*/
	static constexpr Math::Vec2F player2_spawn_pos_ = { game::screen_width - 250,
													   550 }; /*The spawn position for player 2*/

	static constexpr float projectile_radius_ = 24.0f; /*The radius of the projectile*/
	static constexpr int projectile_id_ = 20; /* The ID of the projectile collider*/
	static constexpr int neutral_projectile_id_ = 18; /*The ID of neutral projectiles collider*/
	static constexpr Math::Vec2F projectile_speed_ = Math::Vec2F(0, 580); /*The speed of the projectile*/
	static constexpr float time_between_attack = 1.8f; /*The time between each attack action*/

	int current_projectile_collider_id_ = projectile_id_; /*The ID of the current projectile collider, current projectile collider ID is increased for each collider created so each projectile are unique before being neutral*/


public:
	static constexpr std::int16_t max_projectile_ = 80; /*The maximum number of projectiles allowed in the game*/
	std::array<Projectile, max_projectile_> projectiles_{}; /*Array of Projectile structs representing projectiles*/
	static constexpr float gravity_ = 1000; /*The gravity force applied to players*/
	static constexpr float rope_gravity_ = 10; /*The gravity force applied to player on ropes*/

	std::array<Player, nbr_player_> players; /*Array of Player structs representing player entities*/
	std::array<Physics::BodyRef, nbr_player_> players_BodyRefs_; /*Array of player body references*/
	std::array<Physics::ColliderRef, nbr_player_> players_CollidersRefs_; /*Array of player collider references*/

	std::array<Physics::BodyRef, nbr_player_> players_grounded_BodyRefs_; /*Array of grounded player body references*/
	std::array<Physics::ColliderRef, nbr_player_> players_grounded_CollidersRefs_; /*Array of grounded player collider references*/

	/**
	 * @brief Constructs a new PlayerManager object.
	 *
	 * @param world_ Pointer to the physics world for simulating game physics.
	 */
	PlayerManager(Physics::World* world_);

	/**
	 * @brief Create player entities within the game world.
	 */
	void SetUp();
	/**
	 * @brief Updates player entities and their interactions within the game world.
	 */
	void Update();
	/**
	 * @brief Resets the state of the player manager, including player entities and projectiles.
	 */
	void ResetState();

	/**
	 * @brief Initiates a jump action for the specified player.
	 *
	 * @param playerIdx Index of the player.
	 */
	void Jump(int playerIdx);
	/**
	 * @brief Initiates a movement action for the specified player.
	 *
	 * @param rightDirection Flag indicating the movement direction (true for right, false for left).
	 * @param playerIdx Index of the player.
	 */
	void Move(bool rightDirection, int playerIdx);
	/**
	 * @brief Initiates a deceleration action for the specified player.
	 *
	 * @param playerIdx Index of the player.
	 */
	void Decelerate(int playerIdx);

	/**
	 * @brief Initiates an attack action for the specified player.
	 *
	 * @param player_idx Index of the player.
	 */
	void Attack(int player_idx);

	/**
	 * @brief Retrieves the position of the specified player.
	 *
	 * @param idx Index of the player.
	 * @return Math::Vec2F Position of the player.
	 */
	Math::Vec2F GetPlayerPosition(int idx) const noexcept;

	/**
	 * @brief Handles trigger event detection when colliders enter.
	 *
	 * @param colliderA Collider of the first object.
	 * @param colliderB Collider of the second object.
	 */
	void OnTriggerEnter(Physics::Collider colliderA,
		Physics::Collider colliderB) noexcept override;

	/**
	 * @brief Handles trigger event detection when colliders exit.
	 *
	 * @param colliderA Collider of the first object.
	 * @param colliderB Collider of the second object.
	 */
	void OnTriggerExit(Physics::Collider colliderA,
		Physics::Collider colliderB) noexcept override;

	/**
	 * @brief Handles collision event detection when colliders enter.
	 *
	 * @param colliderA Collider of the first object.
	 * @param colliderB Collider of the second object.
	 */
	void OnCollisionEnter(Physics::Collider colliderA,
		Physics::Collider colliderB) noexcept override;

	/**
	 * @brief Handles collision event detection when colliders exit.
	 *
	 * @param colliderA Collider of the first object.
	 * @param colliderB Collider of the second object.
	 */
	void OnCollisionExit(Physics::Collider colliderA,
		Physics::Collider colliderB) noexcept override;

	/**
	 * @brief Initializes projectile entities within the game world.
	 */
	void InitProjectiles();
	/**
	 * @brief Creates a new projectile for the specified player.
	 *
	 * @param player_idx Index of the player launching the projectile.
	 */
	void GetNewProjectile(int player_idx);
	/**
	 * @brief Retrieves the position of the specified projectile.
	 *
	 * @param idx Index of the projectile.
	 * @return Math::Vec2F Position of the projectile.
	 */
	Math::Vec2F GetProjectilePosition(int idx) const noexcept;
	/**
	 * @brief Resets the state of all projectiles within the game world.
	 */
	void ResetProjectiles();
	/**
	 * @brief Handles projectile trigger event detection when colliders enter.
	 *
	 * @param colliderA Collider of the first object.
	 * @param colliderB Collider of the second object.
	 * @param projectile Reference to the projectile involved in the trigger event.
	 */
	void ProjectileTriggerDetection(Physics::Collider colliderA,
		Physics::Collider colliderB,
		Projectile& projectile);
};
