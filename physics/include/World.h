#pragma once

#include "QuadTree.h"
#include "Body.h"
#include "Collider.h"
#include "ContactListener.h"
#include "Contact.h"
#ifdef TRACY_ENABLE
#include <Tracy.hpp>
#include <TracyC.h>
#endif

#include <cstdlib>
#include <vector>

namespace Physics
{
    /**
     * @class World
     * @brief Represents the simulation world containing bodies, colliders, and managing collision detection.
     *
     * The World class represents the simulation world, containing bodies, colliders, and managing the physics simulation,
     * including collision detection and resolution. It uses a QuadTree for spatial partitioning to optimize collision detection.
     *
     * The class has the following private members:
     * - `std::vector<Body> _bodies`: Vector storing the bodies in the world.
     * - `std::vector<std::size_t> _genIndices`: Vector storing the generation indices of bodies.
     * - `std::vector<Collider> _colliders`: Vector storing the colliders in the world.
     * - `std::vector<std::size_t> _collidersGenIndices`: Vector storing the generation indices of colliders.
     * - `std::unordered_set<ColliderPair, ColliderPairHash> _colliderPairs`: Unordered set storing collider pairs.
     * - `static constexpr std::size_t initSizeForVector = 500`: Constant defining the initial size for vectors.
     *
     * The class also has the following public members:
     * - `ContactListener* contactListener`: Pointer to a contact listener for handling collision events.
     * - `QuadTree tree`: QuadTree for spatial partitioning.
     *
     * The class provides the following methods:
     * - `void Init() noexcept`: Initializes the World vector size bodies, colliders, and related data structures.
     * - `void Clear() noexcept`: Clear the World vector bodies, colliders, and related data structures.
     * - `void Update(float deltaTime) noexcept`: Updates the state of the World, including body physics and collision resolution.
     * - `BodyRef CreateBody() noexcept`: Creates a new body in the World and returns its reference.
     * - `void DestroyBody(BodyRef bodyRef) noexcept`: Destroys the specified body in the World.
     * - `Body& GetBody(BodyRef bodyRef)`: Retrieves the reference to a specific body in the World.
     * - `std::size_t CurrentBodyCount() const noexcept`: Returns the current number of bodies in the World.
     * - `ColliderRef CreateCollider(BodyRef bodyRef) noexcept`: Creates a new collider associated with a given body and returns its reference.
     * - `Collider& GetCollider(ColliderRef colliderRef)`: Retrieves the reference to a specific collider in the World.
     * - `void DestroyCollider(ColliderRef colliderRef) noexcept`: Destroys the specified collider in the World.
     * - `static bool IsContact(const Engine::Collider& colliderA, const Engine::Collider& colliderB) noexcept`: Checks if there is a contact/overlap between two colliders.
     * - `void ResolveBroadPhase() noexcept`: Resolves broad-phase collision detection using a QuadTree.
     * - `void ResolveNarrowPhase() noexcept`: Resolves narrow-phase collision detection and applies it if necessary using a QuadTree.
     *
     * This class encapsulates the functionality of a physics simulation world with collision detection and resolution.
     */
    class World
    {
    private :
        std::vector<Body> _bodies;
        std::vector<std::size_t> _genIndices;

        std::vector<Collider> _colliders;
        std::vector<std::size_t> _collidersGenIndices;

        HeapAllocator heapAlloc;
        std::unordered_set<ColliderPair, ColliderPairHash, std::equal_to<ColliderPair>, StandardAllocator<ColliderPair>> _colliderPairs{
                heapAlloc
        };

        static constexpr std::size_t initSizeForVector = 500;


    public :
        ContactListener* contactListener = nullptr;
        QuadTree tree;

        World() noexcept = default;

        /**
         * @brief Initializes the World by resizing bodies, colliders, and related data structures.
         */
        void Init() noexcept;

        /**
         * @brief Clear the World by clearing bodies, colliders, and related data structures.
         */
        void Clear() noexcept;

        /**
         * @brief Updates the state of the World, including body physics and collision resolution.
         *
         * @param deltaTime The time elapsed since the last update.
         */
        void Update(float deltaTime) noexcept;

        /**
        * @brief Return the first BodyRef in the vector bodies that is unvalid.
         * If there is no unvalid BodyRef resize the bodies by 2time his current size and return the first new one.
        */
        /**
         * @brief Creates a new body in the World and returns its reference.
         * \n Note : If there is no BodyRef to return it will resize the bodies vector by 2time his current size and return the first new Bodyref.
         * @return Reference to the newly created body.
         */
        [[nodiscard]] BodyRef CreateBody() noexcept;

        /**
         * @brief Destroys the specified body in the World.
         *
         * @param bodyRef Reference to the body to be destroyed.
         */
        void DestroyBody(BodyRef bodyRef) noexcept;

        /**
         * @brief Retrieves the reference to a specific body in the World.
         *
         * @param bodyRef Reference(BodyRef) to the body to be retrieved.
         * @return The specified body.
         */
        [[nodiscard]] Body& GetBody(BodyRef bodyRef);

        /**
         * @return The vector of body size.
         */
        [[nodiscard]] std::size_t CurrentBodyCount() const noexcept;

        /**
         * @brief Creates a new collider associated with a given body and returns its reference.
         * @param bodyRef Reference(BodyRef) to the associated body.
         * @return Reference(ColliderRef) to the newly created collider.
         */
        [[nodiscard]] ColliderRef CreateCollider(BodyRef bodyRef) noexcept;

        /**
         * @brief Retrieves the reference to a specific collider in the World.
         *
         * @param colliderRef Reference(ColliderRef) to the collider to be retrieved.
         * @return The specified collider.
         */
        [[nodiscard]] Collider& GetCollider(ColliderRef colliderRef);

        /**
         * @brief Destroys the specified collider in the World.
         *
         * @param colliderRef Reference to the collider to be destroyed.
         */
        void DestroyCollider(ColliderRef colliderRef) noexcept;

        /**
         * @brief Checks if there is a contact/Overlaps between two colliders.
         *
         * @param colliderA The first collider.
         * @param colliderB The second collider.
         * @return True if there is a contact, false otherwise.
         */
        static bool IsContact(const Physics::Collider& colliderA, const Physics::Collider& colliderB) noexcept;

        /**
         * @brief Resolves broad-phase collision detection and only detection using a QuadTree.
         */
        void ResolveBroadPhase() noexcept;

        /**
         * @brief Resolves narrow-phase collision detection and Apply it if necessary using a QuadTree.
         */
        void ResolveNarrowPhase() noexcept;

        const std::size_t GetInitSizeForVector() noexcept;
    };
}