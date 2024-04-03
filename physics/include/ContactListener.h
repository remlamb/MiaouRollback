#pragma once

#include "Collider.h"

namespace Engine
{
    /**
     * @class ContactListener
     * @brief Interface for handling collision and trigger events between colliders.
     *
     * The ContactListener class defines an interface for handling collision and trigger events between colliders.
     * It includes abstract methods for when two colliders begin or stop intersecting, with options for trigger or collision states.
     * Implementations of this interface can provide custom logic to respond to these events.
     *
     * The class has the following public abstract methods:
     * - `virtual void OnTriggerEnter(Collider colliderA, Collider colliderB) noexcept = 0`: Called when two colliders begin intersecting, and at least one is in trigger state.
     * - `virtual void OnTriggerExit(Collider colliderA, Collider colliderB) noexcept = 0`: Called when two colliders stop intersecting, and at least one is in trigger state.
     * - `virtual void OnCollisionEnter(Collider colliderA, Collider colliderB) noexcept = 0`: Called when two colliders begin intersecting, and neither is in trigger state.
     * - `virtual void OnCollisionExit(Collider colliderA, Collider colliderB) noexcept = 0`: Called when two colliders stop intersecting, and neither is in trigger state.
     *
     * Implementing this interface allows for the customization of collision and trigger response in a physics simulation.
     */
    class ContactListener
    {
    public:
        /**
        * @brief Abstract method that is called when two colliders begin intersecting and if one of them is in trigger state.
        * @param colliderA The first collider Intersecting.
        * @param colliderB The second collider Intersecting.
        */
        virtual void OnTriggerEnter(Collider colliderA, Collider colliderB) noexcept = 0;

        /**
        * @brief Abstract method that is called when two colliders stop intersecting and if one of them is in trigger state.
        * @param colliderA The first collider Intersecting.
        * @param colliderB The second collider Intersecting.
        */
        virtual void OnTriggerExit(Collider colliderA, Collider colliderB) noexcept = 0;

        /**
        * @brief Abstract method that is called when two colliders begin intersecting and if none of them is in trigger state.
        * @param colliderA The first collider Intersecting.
        * @param colliderB The second collider Intersecting.
        */
        virtual void OnCollisionEnter(Collider colliderA, Collider colliderB) noexcept = 0;

        /**
        * @brief Abstract method that is called when two colliders stop intersecting and if none of them is in trigger state.
        * @param colliderA The first collider Intersecting.
        * @param colliderB The second collider Intersecting.
        */
        virtual void OnCollisionExit(Collider colliderA, Collider colliderB) noexcept = 0;
    };
}