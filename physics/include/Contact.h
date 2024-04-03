#pragma once
#include "Body.h"
#include "Collider.h"

namespace Engine
{

    /**
     * @struct CollidingBody
     * @brief Represents a pair of bodies and their colliders involved in a collision.
     *
     * The CollidingBody struct contains pointers to a physics Body and Collider involved in a collision.
     * It provides a way to store and reference the bodies participating in a collision.
     */
    struct CollidingBody
    {
        Body* body = nullptr;
        Collider* collider = nullptr;
    };

    /**
     * @class Contact
     * @brief Represents a contact point between two colliding bodies.
     *
     * The Contact class encapsulates information about a contact point between two bodies, including colliding bodies,
     * contact normal, contact position, penetration depth, and restitution.
     * It includes methods to calculate relative velocity, resolve velocity, resolve interpenetration, and resolve the collision.
     *
     * The class has the following public properties and methods:
     * - `std::array<CollidingBody, 2> collidingBodies`: An array of two CollidingBody instances representing the bodies involved in the collision.
     * - `Math::Vec2F contactNormal`: The normal vector at the contact point.
     * - `Math::Vec2F contactPosition`: The position of the contact point.
     * - `float penetration`: The penetration depth indicating how much the bodies overlap.
     * - `float restitution`: The restitution coefficient for the collision.
     * - `float CalculateSeparateVelocity() const noexcept`: Calculates the relative velocity of colliding bodies along the contact normal.
     * - `void ResolveVelocity() const noexcept`: Resolves the velocity of colliding bodies based on their relative velocity and restitution.
     * - `void ResolveInterpenetration() const noexcept`: Resolves interpenetration of colliding bodies by adjusting their positions.
     * - `void Resolve()`: Resolves the collision by determining the contact normal, penetration, and applying velocity and position corrections.
     *
     * This class is crucial for handling collision responses, ensuring realistic physics interactions between bodies.
     */
    class Contact
    {
    public:
        std::array<CollidingBody, 2> collidingBodies{};
        Math::Vec2F contactNormal{};
        Math::Vec2F contactPosition{};
        float penetration = 0.0f;
        float restitution = 0.0f;

        /**
         * @brief Calculates the relative velocity of colliding bodies along the contact normal.
         * @return The separate velocity along the contact normal.
         */
        float CalculateSeparateVelocity() const noexcept;

        /**
         * @brief Resolves the velocity of colliding bodies based on their relative velocity and restitution.
         */
        void ResolveVelocity() const noexcept;

        /**
         * @brief Resolves interpenetration of colliding bodies by adjusting their positions.
         */
        void ResolveInterpenetration() const noexcept;

        /**
         * @brief Resolves the collision by determining the contact normal, penetration, and applying velocity and position corrections.
         */
        void Resolve();
    };
}