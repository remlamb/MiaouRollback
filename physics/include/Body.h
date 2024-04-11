#pragma once

#include "Vec2.h"


namespace Physics
{
    /**
     * @enum BodyType
     * @brief Enumerates the possible types of physics bodies in a simulation.
     * The BodyType enum class defines three distinct types of physics bodies:
     * - DYNAMIC: Represents a dynamic body with mass and subject to external forces.
     * - STATIC: Represents a static body with infinite mass, unaffected by forces.
     * - NONE: Represents an undefined body type.
     * This enum is commonly used in physics simulations to categorize entities based on their physical behavior.
     */
    enum class BodyType
    {
        DYNAMIC,
        STATIC,
        NONE
    };

    /**
     * @struct BodyRef
     * @brief Represents a reference to a physics body in a simulation.
     * The BodyRef struct consists of two members:
     * - `index`: An index value identifying the body.
     * - `genIdx`: A generation index used for tracking changes in the body.
     *
     * The equality operator (`==`) is overridden to compare two BodyRef instances for equality based on both index and genIdx.
     */
    struct BodyRef
    {
        std::size_t index;
        std::size_t genIdx;

        constexpr bool operator==(const BodyRef& other) const
        {
            return (index == other . index) && (genIdx == other . genIdx);
        }
    };


    /**
     * @class Body
     * @brief Represents a physics body in a simulation.
     *
     * The Body class models a physics body with properties such as mass, velocity, position, and applied forces.
     * It includes methods to query and modify these properties.
     *
     * The class has the following public methods:
     * - `float Mass() const noexcept`: Returns the mass of the body.
     * - `void SetMass(float mass) noexcept`: Sets the mass of the body.
     * - `Math::Vec2F Velocity() noexcept`: Returns the velocity of the body.
     * - `void SetVelocity(Math::Vec2F velocity) noexcept`: Sets the velocity of the body.
     * - `Math::Vec2F Position() noexcept`: Returns the position of the body.
     * - `void SetPosition(Math::Vec2F position) noexcept`: Sets the position of the body.
     * - `Math::Vec2F Force() noexcept`: Returns the total force applied to the body.
     * - `void SetForce(Math::Vec2F force) noexcept`: Sets the force applied to the body.
     * - `void AddForce(Math::Vec2F force) noexcept`: Adds a force to the total forces applied to the body.
     * - `bool IsValid() const noexcept`: Checks if the body is valid -> if it has a positive mass.
     */
    class Body
    {
    private:
        float _mass = 0;
        Math::Vec2F _velocity = Math::Vec2F(0, 0);
        Math::Vec2F _position = Math::Vec2F(0, 0);
        Math::Vec2F _totalForce = Math::Vec2F(0,
                                              0); /** @Note total amout of force applied to the body during a frame **/

    public :
        BodyType type = BodyType::DYNAMIC;

        constexpr Body() = default;

        constexpr Body(float mass, Math::Vec2F velocity, Math::Vec2F position)
        {
            _mass = mass;
            _velocity = velocity;
            _position = position;
        }

        /**
        * @brief Return the mass
        */
        [[nodiscard]] constexpr float Mass() const noexcept
        {
            return _mass;
        };

        void SetMass(float mass) noexcept;

        /**
        * @brief Return the velocity
        */
        [[nodiscard]] Math::Vec2F Velocity() noexcept;

        void SetVelocity(Math::Vec2F velocity) noexcept;

        /**
        * @brief Return the Position
        */
        [[nodiscard]] Math::Vec2F Position() noexcept;

        void SetPosition(Math::Vec2F position) noexcept;

        /**
        * @brief Return the addition of all the forces that will be applied
        */
        [[nodiscard]] Math::Vec2F Force() noexcept;

        void SetForce(Math::Vec2F force) noexcept;

        /**
        * @brief Add the given force to the total Forces we need to apply for the body this Frame
        * @param force a Math::Vec2F that represent the force to add to the body.
        */
        void AddForce(Math::Vec2F force) noexcept;

        /**
        * @brief A body is Valid if he got a real mass, mass > 0
        * \n Note : The default Body constructor set mass to 0 (not a valid Body).
        */
        [[nodiscard]] constexpr bool IsValid() const noexcept
        {
            return _mass > 0.;
        };

    };
}