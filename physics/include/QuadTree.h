#pragma once

#include "Shape.h"
#include "Collider.h"
#include "Metrics.h"
#include <memory>
#include <vector>
#include "Allocator.h"
#ifdef TRACY_ENABLE
#include <Tracy.hpp>
#include <TracyC.h>
#endif

namespace Physics
{
    /**
     * @struct SimplifiedCollider
     * @brief Represents a simplified view of a collider with its reference and axis-aligned bounding box (AABB).
     *
     * The SimplifiedCollider struct provides a simplified representation of a colliderObj, including its colliderObj reference
     * and its axis-aligned bounding box (AABB). It is useful for scenarios where a simplified view of a colliderObj's data is needed.
     *
     * The struct has the following members:
     * - `Engine::ColliderRef colliderRef`: The reference to the associated collider.
     * - `Math::RectangleF aabb`: The axis-aligned bounding box (AABB) of the collider.
 */
    struct SimplifedCollider
    {
        Physics::ColliderRef colliderRef;
        Math::RectangleF aabb;
    };

    /**
     * @struct QuadNode
     * @brief Represents a node in a quadtree used for spatial partitioning.
     *
     * The QuadNode struct represents a node in a quadtree, which is a tree data structure used for spatial partitioning.
     * It is commonly used in collision detection and other spatially-related algorithms. The quadtree recursively divides
     * the space into quadrants, allowing for efficient spatial queries.
     *
     * The struct has the following members:
     * - `Math::RectangleF bounds`: The bounding rectangle defining the region covered by the quad node.
     * - `std::array<QuadNode*, 4> children`: An array of pointers to the quad node's four children.
     * - `AllocatedVector<SimplifedCollider> colliders`: A vector of simplified colliders contained within the quad node.
     *
     *
     * This struct facilitates the creation and management of a quadtree for spatial partitioning.
     */
    struct QuadNode
    {
        Math::RectangleF bounds{Math::Vec2F::Zero(), Math::Vec2F::Zero()};
        std::array<QuadNode*, 4> children{nullptr, nullptr, nullptr, nullptr};
        AllocatedVector <SimplifedCollider> colliders{};

        QuadNode() noexcept = default;

        explicit QuadNode(Allocator& allocator) noexcept:
                colliders{StandardAllocator < Collider > {allocator}}
        {};
    };

/**
     * @class QuadTree
     * @brief Represents a quadtree used for spatial partitioning of colliders.
     *
     * The QuadTree class represents a quadtree, a tree data structure used for spatial partitioning in applications
     * such as collision detection. The quadtree divides space into quadrants, allowing for efficient spatial queries.
     *
     * The class has the following members:
     * - `HeapAllocator heapAllocator`: An allocator for managing memory used by the quadtree.
     * - `AllocatedVector<QuadNode> nodes{StandardAllocator<QuadNode>{heapAllocator}}` : represent the nodes created in the quad tree
     * - `int nodeIndex`: index used to navigate through the nodes.
     * - `AllocatedVector <ColliderPair> nodeColliderPairs{StandardAllocator <ColliderPair > {heapAllocator}}`: An allocated vector to store collider pairs within the quadtree.
     * - `static const auto MaxColliderInNode`: A constant defining the maximum number of colliders allowed in a single quadtree node.
     * - `static const auto MaxDepth`: A constant defining the maximum depth of the quadtree.
     *
     * The class provides the following methods:
     * - `void Init()`: pre allocating memory for nodes and collider pairs.
     * - `void Subdivide(QuadNode& node)`: Subdivides the quad node into four children, splitting the space into quadrants.
     * - `void InsertInRootNode(const SimplifiedCollider &simplifiedCollider) noexcept`: Inserts a simplified collider into the root QuadNode of the QuadTree.
     * - `void SubdivideNodeRecursively(QuadNode &node, int depth) noexcept`: Recursively subdivides a QuadNode if it contains more colliders than the maximum allowed or if the depth limit is not reached.
     * - `void FindPossiblePairs(QuadNode &node) noexcept`: Finds possible collider pairs within a QuadNode and its children.
     * - `void FindInChildrenNodePossiblePairs(QuadNode &node, Engine::ColliderRef &colliderRef) noexcept`: Finds possible collider pairs between a specific collider and the colliders within a QuadNode and its children.
     * - `void Clear() noexcept`: Clears the QuadTree, resetting it to an empty state.
     *
     * This class facilitates the creation and management of a quadtree for spatial partitioning of colliders.
     */
    class QuadTree
    {
    public:
        HeapAllocator heapAllocator;
        AllocatedVector <QuadNode> nodes{StandardAllocator < QuadNode > {heapAllocator}};
        int nodeIndex = 1;
        AllocatedVector <ColliderPair> nodeColliderPairs{
                StandardAllocator < ColliderPair > {heapAllocator}};

        static const auto MaxColliderInNode = 4;
        static const auto MaxDepth = 6;

        QuadTree() noexcept = default;

        /**
         * @brief Initializes the QuadTree by preallocating memory for nodes and collider pairs.
         *
         * The method has the following key steps:
         * 1. Calculates the maximum possible number of children nodes based on the maximum depth.
         * 2. Resizes the nodes vector to allocate memory for the calculated maximum number of nodes.
         * 3. Iterates over each node and reserves space for colliders within the node.
         * 4. Reserves space for collider pairs based on the calculated maximum number of children nodes and a Ratio.
         *
         * This initialization ensures that the QuadTree has sufficient memory to accommodate its structure
         * and allows for efficient insertion and retrieval of colliders during collision detection.
         */
        void Init() noexcept;

        /**
         * @brief Subdivides the current Node into four quadNodes.
         */
        void Subdivide(QuadNode& node);

        /**
         * @brief Inserts a simplified collider into the root QuadNode of the QuadTree.
         * \n Note : RootNode dimension are set with the position of the collider inserted.
         * @param simplifedCollider The simplified collider to be inserted.
         */
        void InsertInRootNode(const SimplifedCollider& simplifedCollider) noexcept;

        /**
         * @brief Recursively subdivides a QuadNode if it contains more colliders than the maximum allowed or if the depth limit is not reached.
         * @param node The QuadNode to be subdivided.
         * @param depth The current depth of the recursion.
         */
        void SubdivideNodeRecursively(QuadNode& node, int depth) noexcept;

        /**
         * @brief Finds possible collider pairs within a QuadNode and its children.
         * @param node The QuadNode to search for possible pairs.
         */
        void FindPossiblePairs(QuadNode& node) noexcept;

        /**
         * @brief Finds possible collider pairs between a specific collider and the colliders within a QuadNode and its children.
         *
         * @param node The QuadNode to search for possible pairs.
         * @param colliderRef Reference to the collider to compare with.
         */
        void FindInChildrenNodePossiblePairs(QuadNode& node, Physics::ColliderRef& colliderRef) noexcept;

        /**
         * @brief Clears the QuadTree, resetting it to an empty state.
         */
        void Clear() noexcept;
    };
}
