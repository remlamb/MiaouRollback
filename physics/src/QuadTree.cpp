#include "QuadTree.h"

namespace Physics
{
    void QuadTree::Subdivide(QuadNode& node)
    {
#ifdef TRACY_ENABLE
        ZoneScoped;
#endif
        const auto center = node.bounds.Center();
        const auto halfSize = node.bounds.Size() / 2;

        const auto topRightCorner = center + halfSize;
        const auto bottomLeftCorner = center - halfSize;

        const auto rightMiddle = Math::Vec2F(center.X + halfSize.X, center.Y);
        const auto leftMiddle = Math::Vec2F(center.X - halfSize.X, center.Y);

        const auto topMiddle = Math::Vec2F(center.X, center.Y + halfSize.Y);
        const auto bottomMiddle = Math::Vec2F(center.X, center.Y - halfSize.Y);

        node.children[0] = &nodes[nodeIndex];
        node.children[0]->bounds = Math::RectangleF(leftMiddle, topMiddle);
        node.children[1] = &nodes[nodeIndex + 1];
        node.children[1]->bounds = Math::RectangleF(center, topRightCorner);
        node.children[2] = &nodes[nodeIndex + 2];
        node.children[2]->bounds = Math::RectangleF(bottomLeftCorner, center);
        node.children[3] = &nodes[nodeIndex + 3];
        node.children[3]->bounds = Math::RectangleF(bottomMiddle, rightMiddle);

        nodeIndex += 4;
    };

    void QuadTree::InsertInRootNode(const SimplifedCollider& simplifedCollider) noexcept
    {
#ifdef TRACY_ENABLE
        ZoneScoped;
#endif

        auto& node = nodes[0];

        const Math::RectangleF& nodeBounds = node.bounds;
        const Math::RectangleF& colliderBounds = simplifedCollider.aabb;

        if (colliderBounds.MinBound().X < nodeBounds.MinBound().X)
        {
            node.bounds.SetMinBound(Math::Vec2F(colliderBounds.MinBound().X, nodeBounds.MinBound().Y));
        }
        if (colliderBounds.MinBound().Y < nodeBounds.MinBound().Y)
        {
            node.bounds.SetMinBound(Math::Vec2F(nodeBounds.MinBound().X, colliderBounds.MinBound().Y));
        }
        if (colliderBounds.MaxBound().X > nodeBounds.MaxBound().X)
        {
            node.bounds.SetMaxBound(Math::Vec2F(colliderBounds.MaxBound().X, nodeBounds.MaxBound().Y));
        }
        if (colliderBounds.MaxBound().Y > nodeBounds.MaxBound().Y)
        {
            node.bounds.SetMaxBound(Math::Vec2F(nodeBounds.MaxBound().X, colliderBounds.MaxBound().Y));
        }
        node.colliders.push_back(simplifedCollider);
    }

    void QuadTree::SubdivideNodeRecursively(QuadNode& node, int depth) noexcept
    {
#ifdef TRACY_ENABLE
        ZoneScoped;
#endif
        if (node.colliders.size() > MaxColliderInNode && depth != MaxDepth)
        {
            Subdivide(node);
            std::vector<SimplifedCollider> collidersStayInNodes;

            for (const auto& col: node.colliders)
            {
                int childNodePossible = 0;
                QuadNode* childNode = nullptr;
                for (const auto& child: node.children)
                {
                    if (Math::Intersect(child->bounds, col.aabb))
                    {
                        childNode = child;
                        childNodePossible++;
                    }
                }

                if (childNodePossible == 1)
                {
                    childNode->colliders.push_back(col);
                }
                else
                {
                    collidersStayInNodes.push_back(col);
                }
            }

            node.colliders.clear();
            for (auto& colliders: collidersStayInNodes)
            {
                node.colliders.push_back(colliders);
            }

            for (const auto& child: node.children)
            {
                if (child->colliders.size() > MaxColliderInNode)
                {
                    SubdivideNodeRecursively(*child, depth + 1);
                }
            }
        }
    }

    void QuadTree::FindPossiblePairs(QuadNode& node) noexcept
    {
#ifdef TRACY_ENABLE
        ZoneScoped;
#endif
        for (std::size_t i = 0; i < node.colliders.size(); i++)
        {
            auto& colliderA = node.colliders[i];

            for (std::size_t j = i + 1; j < node.colliders.size(); j++)
            {
                auto& colliderB = node.colliders[j];

                nodeColliderPairs.push_back(
                        Physics::ColliderPair{colliderA.colliderRef, colliderB.colliderRef}); // now i et j
            }

            if (node.children[0] != nullptr)
            {
                for (const auto& child: node.children)
                {
                    FindInChildrenNodePossiblePairs(*child, colliderA.colliderRef);
                }
            }
        }

        if (node.children[0] != nullptr)
        {
            for (const auto& child: node.children)
            {
                FindPossiblePairs(*child);
            }
        }
    }

    void QuadTree::FindInChildrenNodePossiblePairs(QuadNode& node, Physics::ColliderRef& colliderRef) noexcept
    {
#ifdef TRACY_ENABLE
        ZoneScoped;
#endif
        for (auto& nodeCollider: node.colliders)
        {
            nodeColliderPairs.push_back(Physics::ColliderPair{colliderRef, nodeCollider.colliderRef});
        }

        if (node.children[0] != nullptr)
        {
            for (const auto& child: node.children)
            {
                FindInChildrenNodePossiblePairs(*child, colliderRef);
            }
        }
    }

    void QuadTree::Clear() noexcept
    {
        nodeColliderPairs.clear();
        for (auto& node: nodes)
        {
            std::fill(node.children.begin(), node.children.end(), nullptr);
            node.colliders.clear();
        }
        nodeIndex = 1;

        nodes[0].bounds.SetMinBound(Math::Vec2F(Metrics::WIDTH, Metrics::HEIGHT));
        nodes[0].bounds.SetMaxBound(Math::Vec2F(0.0f, 0.0f));
    }

    void QuadTree::Init() noexcept
    {
        std::size_t maxChildrenPossible = 0;
        for (int i = 0; i <= MaxDepth; i++)
        {
            maxChildrenPossible += Math::Pow(4, i);
        }
        nodes.resize(maxChildrenPossible, QuadNode({heapAllocator}));

        for (auto& node: nodes)
        {
            node.colliders.reserve(MaxColliderInNode);
        }
        nodeColliderPairs.reserve(maxChildrenPossible * 4);
    }
}