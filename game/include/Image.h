#pragma once
#include "raylib_wrapper.h"

/**
 * @brief Enumeration representing the pivot point for image drawing.
 */
enum class Pivot { Default, Center };

/**
 * @brief Class representing a custom image with the ability to specify pivot point.
 *
 * Member Variables:
 * - sprite: The texture representing the main image.
 * - originalScale: The original scale of the image.
 * - pivot: The pivot point of the image (Default or Center).
 * - icon: The image loaded as an icon.
 */
class ImageCustom {
public:
	raylib::Texture2D sprite;  /* The texture sprite of the image.*/
	float originalScale; /* The original scale of the image.*/
	Pivot pivot; /* The pivot point of the image.*/

	raylib::Image icon;   /* The window icon image.*/

	/**
	 * @brief Sets up the image with the specified path, scale, and pivot mode.
	 * @param path The path to the image file.
	 * @param scale The scale factor for the image.
	 * @param mode The pivot mode for the image.
	 */
	void Setup(const char* path, float scale, Pivot mode);
	/**
	 * @brief Sets up the icon with the specified path and pivot mode.
	 * @param path The path to the icon image file.
	 * @param mode The pivot mode for the icon.
	 */
	void SetupIcon(const char* path, Pivot mode);
	/**
	 * @brief Tears down the image, releasing associated resources.
	 */
	void TearDown();
	/**
	 * @brief Draws the image at the specified position.
	 * @param position The position to draw the image.
	 */
	void Draw(raylib::Vector2 position) const noexcept;
	/**
	 * @brief Draws the image at the specified position and scale.
	 * @param position The position to draw the image.
	 * @param scale The scale factor for the image.
	 */
	void Draw(raylib::Vector2 position, float scale)const noexcept;
};
