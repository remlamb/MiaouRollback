#pragma once
#include "Shape.h"
#include "Body.h"
#include <unordered_set>

namespace Physics
{
    /**
     * @struct ColliderRef
     * @brief Represents a reference to a collider in a physics simulation.
     *
     * The ColliderRef struct consists of two members:
     * - `index`: An index value identifying the collider.
     * - `genIdx`: A generation index used for tracking generation of the collider.
     *
     * The equality and inequality operators (`==` and `!=`) are overridden to compare two ColliderRef instances for equality based on both index and genIdx.
     */
    struct ColliderRef
    {
        std::size_t index;
        std::size_t genIdx;

        constexpr bool operator==(const ColliderRef& other) const noexcept
        {
            return index == other . index && genIdx == other . genIdx;
        }

        constexpr bool operator!=(const ColliderRef& other) const noexcept
        {
            return index != other . index;
        }

    };


    /**
     * @class Collider
     * @brief Represents a collider in a physics simulation.
     *
     * The Collider class models a colliderObj with properties such as shape, restitution, friction, ID, and reference to a physics body.
     * It includes methods and properties to query and modify these properties, making it a key component in physics simulations.
     *
     * The class has the following public methods and properties:
     * - `Math::ShapeType _shape`: The type of shape associated with the collider.
     * - `Math::CircleF circleShape`: The circle shape of the collider (if applicable).
     * - `Math::RectangleF rectangleShape`: The rectangle shape of the collider (if applicable).
     * - `float restitution`: The restitution (bounciness) of the collider.
     * - `float friction`: The friction of the collider.
     * - `int ID`: The unique identifier of the collider.
     * - `BodyRef bodyRef`: The reference to the physics body associated with the collider.
     * - `bool isTrigger`: A flag indicating if the collider is a trigger (does not participate in physical collisions).
     * - `bool IsValid() const noexcept`: Checks if the collider is valid based on its shape.
     * - `constexpr bool operator==(const Collider& other) const noexcept`: Equality comparison operator based on collider ID.
     * - `constexpr bool operator!=(const Collider& other) const noexcept`: Inequality comparison operator based on collider ID.
     *
     * This class is fundamental for defining collision properties and interactions in a physics simulation.
     */
    class Collider
    {
    public:
        Math::ShapeType _shape = {Math::ShapeType::None};
        Math::CircleF circleShape = {Math::Vec2F(0.f, 0.f), 0};
        Math::RectangleF rectangleShape = {Math::Vec2F(0.f, 0.f), Math::Vec2F(0., 0.)};
        float restitution = 1;
        float friction = 0;
        int ID = 0;
        BodyRef bodyRef{};
        bool isTrigger = false;

        Collider() noexcept = default;

        /**
        * @brief A Collider is Valid if he got a real size, here radius of the circle > 0
        * \n Note : The default Collider constructor set the radius to 0 (not valid).
        **/
        [[nodiscard]] bool IsValid() const noexcept
        {
            return _shape != Math::ShapeType::None;

        }

        /**
        * @brief Operator Equal Compare the ID of two Collider
        **/
        constexpr bool operator==(const Collider& other) const noexcept
        {
            return ID == other . ID;
        }

        /**
        * @brief Operator Unequal Compare the ID of two Collider
        **/
        constexpr bool operator!=(const Collider& other) const noexcept
        {
            return ID != other . ID;
        }
    };

    /**
    * @brief A ColliderPair contain 2 colliderRef
    **/
    struct ColliderPair
    {
        ColliderRef colliderA;
        ColliderRef colliderB;

        bool operator==(const ColliderPair& other) const noexcept
        {
            return colliderA . index == other . colliderA . index && colliderB . index == other . colliderB . index ||
                   colliderA . index == other . colliderB . index && colliderB . index == other . colliderA . index;
        }
    };

    /**
    * @brief Hash function for colliderPair, needed to create an unordered_set of colliderPair
    **/
    struct ColliderPairHash
    {
        std::size_t operator()(const ColliderPair& pair) const
        {
            std::size_t hashA = std::hash<int>{}(static_cast<int>(pair . colliderA . index));
            std::size_t hashB = std::hash<int>{}(static_cast<int>(pair . colliderB . index));
            return hashA + hashB;
        }
    };
}