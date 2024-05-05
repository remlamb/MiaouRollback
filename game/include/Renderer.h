#pragma once
#include "GameLogic.h"
#include "Image.h"


/**
 * @brief Manages rendering operations for the game.
 *
 * This class handles rendering tasks such as drawing player entities, projectiles,
 * platforms, background, UI elements, and debug collider shapes. It provides methods
 * to initialize and deinitialize the renderer, as well as to draw the game scene with
 * optional collider visibility for debugging purposes.
 */
class Renderer {
public:
	/**
	 * @brief Constructs a new Renderer object.
	 *
	 * @param gl Pointer to the GameLogic object.
	 * @param network_logic Pointer to the NetworkLogic object.
	 */
	Renderer(game::GameLogic* gl, NetworkLogic* network_logic)
		: game_logic_(gl), network_logic_(network_logic) {}

	/**
	 * @brief Initializes the renderer.
	 */
	void Init() noexcept;

	/**
	 * @brief Draws the game scene.
	 *
	 * @param isColliderVisible Flag indicating whether collider shapes should be visible for debugging.
	 */
	void Draw(bool isColliderVisible) noexcept;

	/**
	 * @brief Deinitializes the renderer.
	 */
	void Deinit() noexcept;

private:
	/**
	 * @brief Renders collider shapes in the game window for debugging purposes.
	 */
	void DrawColliderShape() noexcept;

	/**
	 * @brief Draws collider shapes for player entities.
	 */
	void DrawPlayerColliderShape() noexcept;

	/**
	 * @brief Draws platforms in the game scene.
	 */
	void DrawPlatforms() noexcept;

	/**
	 * @brief Draws ropes in the game scene.
	 */
	void DrawRopes() noexcept;

	/**
	 * @brief Draws player entities in the game scene.
	 */
	void DrawPlayer() noexcept;

	/**
	 * @brief Draws projectiles in the game scene.
	 */
	void DrawProjectiles() noexcept;

	/**
	 * @brief Draws the background of the game scene.
	 */
	void DrawBackground() noexcept;

	/**
	 * @brief Draws limit borders in the game scene.
	 */
	void DrawLimit() noexcept;

	/**
	 * @brief Draws UI elements in the game scene.
	 */
	void DrawUI() noexcept;

	game::GameLogic* game_logic_ = nullptr; /* Pointer to the GameLogic object.*/
	NetworkLogic* network_logic_ = nullptr; /* Pointer to the NetworkLogic object.*/

	// Images for rendering
	ImageCustom player_;
	ImageCustom player2_;
	ImageCustom player_weapon_;
	ImageCustom background_;
	ImageCustom platform_;
	ImageCustom rope_;
	ImageCustom main_menu_bg_;
	ImageCustom border_bottom_;
	ImageCustom border_left_;
	ImageCustom border_right_;
	ImageCustom border_top_;
	ImageCustom icon_;
	ImageCustom winner_layer_p1;
	ImageCustom winner_layer_p2;
	ImageCustom player1_life_point_;
	ImageCustom player2_life_point_;

	raylib::Vector2 center_pos_ = { game::screen_width * 0.5f,
								   game::screen_height * 0.5f };  /* Represent the center of the window*/
};
