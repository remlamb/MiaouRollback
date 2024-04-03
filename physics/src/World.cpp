#include "World.h"
#include "../../common/include/Metrics.h"

namespace Engine
{
    void World::Init() noexcept
    {
#ifdef TRACY_ENABLE
        ZoneScoped;
#endif
        Clear();
        _bodies.resize(initSizeForVector);
        _genIndices.resize(initSizeForVector, 0);
        _colliders.resize(initSizeForVector);
        _collidersGenIndices.resize(initSizeForVector, 0);
        tree.Init();
    }

    void World::Clear() noexcept
    {
#ifdef TRACY_ENABLE
        ZoneScoped;
#endif
        _bodies.clear();
        _genIndices.clear();
        _colliders.clear();
        _collidersGenIndices.clear();
        _colliderPairs.clear();
    }

    void World::Update(float deltaTime) noexcept
    {
#ifdef TRACY_ENABLE
        ZoneScoped;
#endif
        for (auto& body: _bodies)
        {
            if (body.IsValid())
            {
                Math::Vec2F acceleration = body.Force() / body.Mass();
                body.SetVelocity(body.Velocity() + acceleration * deltaTime);
                body.SetPosition(body.Position() + body.Velocity() * deltaTime);
                body.SetForce(Math::Vec2F(0., 0.));
            }
        }

        if (contactListener != nullptr)
        {
            ResolveBroadPhase();
        }

        ResolveNarrowPhase();
    }

    BodyRef World::CreateBody() noexcept
    {
        auto it = std::find_if(_bodies.begin(), _bodies.end(), [](Body& body)
        {
            return !body.IsValid();
        });

        if (it != _bodies.end())
        {
            std::size_t index = std::distance(_bodies.begin(), it);
            return BodyRef{index, _genIndices[index]};
        }

        auto indexFirstNewBody = _bodies.size();
        auto newBodiesSize = _bodies.size() * 2;
        _bodies.resize(newBodiesSize, Body());
        _genIndices.resize(newBodiesSize, 0);
        return BodyRef{indexFirstNewBody, _genIndices[indexFirstNewBody]};
    }

    void World::DestroyBody(BodyRef bodyRef) noexcept
    {
        _bodies[bodyRef.index] = Body();
        _genIndices[bodyRef.index]++;
    }

    Body& World::GetBody(BodyRef bodyRef)
    {
        if (_genIndices[bodyRef.index] != bodyRef.genIdx)
        {
            throw std::runtime_error("null");
        }
        return _bodies[bodyRef.index];
    }

    [[nodiscard]] std::size_t World::CurrentBodyCount() const noexcept
    {
        return _bodies.size();
    }

    [[nodiscard]] ColliderRef World::CreateCollider(const BodyRef bodyRef) noexcept
    {
        auto it = std::find_if(_colliders.begin(), _colliders.end(), [](Collider& collider)
        {
            return !collider.IsValid();
        });

        if (it != _colliders.end())
        {
            std::size_t index = std::distance(_colliders.begin(), it);
            const auto colliderRef = ColliderRef{index, _genIndices[index]};
            auto& collider = GetCollider(colliderRef);
            collider.bodyRef = bodyRef;
            return colliderRef;

        }

        else
        {
            std::size_t index = _colliders.size();
            auto newCollidersSize = _colliders.size() * 2;
            _colliders.resize(newCollidersSize);
            _collidersGenIndices.resize(newCollidersSize, 0);

            const auto colliderRef = ColliderRef{index, _genIndices[index]};
            auto& collider = GetCollider(colliderRef);
            collider.bodyRef = bodyRef;
            return colliderRef;
        }

    }


    [[nodiscard]] Collider& World::GetCollider(ColliderRef colliderRef)
    {
        if (_collidersGenIndices[colliderRef.index] != colliderRef.genIdx)
        {
            throw std::runtime_error("null");
        }
        return _colliders[colliderRef.index];
    }

    void World::DestroyCollider(Engine::ColliderRef colliderRef) noexcept
    {
        _colliders[colliderRef.index] = Collider();
        _collidersGenIndices[colliderRef.index]++;
    }

    bool World::IsContact(const Collider& colliderA, const Collider& colliderB) noexcept
    {
#ifdef TRACY_ENABLE
        ZoneScoped;
#endif
        if (colliderA._shape == Math::ShapeType::Circle)
        {
            if (colliderB._shape == Math::ShapeType::Circle)
            {
                return Math::Intersect(colliderA.circleShape, colliderB.circleShape);
            }
            if (colliderB._shape == Math::ShapeType::Rectangle)
            {
                return Math::Intersect(colliderA.circleShape, colliderB.rectangleShape);
            }
        }
        else if (colliderA._shape == Math::ShapeType::Rectangle)
        {
            if (colliderB._shape == Math::ShapeType::Circle)
            {
                return Math::Intersect(colliderA.rectangleShape, colliderB.circleShape);
            }
            if (colliderB._shape == Math::ShapeType::Rectangle)
            {
                return Math::Intersect(colliderA.rectangleShape, colliderB.rectangleShape);
            }
        }
        return false;
    }

    void World::ResolveBroadPhase() noexcept
    {
#ifdef TRACY_ENABLE
        ZoneScoped;
#endif
        tree.Clear();
        for (std::size_t i = 0; i < _colliders.size(); i++)
        {
            if (_colliders[i].IsValid())
            {
                if (_colliders[i]._shape == Math::ShapeType::Rectangle)
                {
                    tree.InsertInRootNode(
                            SimplifedCollider{ColliderRef{i, _collidersGenIndices[i]}, _colliders[i].rectangleShape});
                }
                else if (_colliders[i]._shape == Math::ShapeType::Circle)
                {
                    const auto circleBodyPosition = GetBody(_colliders[i].bodyRef).Position();
                    const auto circleRadius = _colliders[i].circleShape.Radius();
                    const auto circleToAABB = Math::RectangleF(
                            circleBodyPosition - Math::Vec2F(circleRadius, circleRadius),
                            circleBodyPosition + Math::Vec2F(circleRadius, circleRadius));
                    tree.InsertInRootNode(
                            SimplifedCollider{ColliderRef{i, _collidersGenIndices[i]}, circleToAABB});
                }
            }
        }
        tree.SubdivideNodeRecursively(tree.nodes[0], 0);
    }

    void World::ResolveNarrowPhase() noexcept
    {
#ifdef TRACY_ENABLE
        ZoneScoped;
#endif
        tree.FindPossiblePairs(tree.nodes[0]);
        for (auto& pair: tree.nodeColliderPairs)
        {
            auto& colliderA = GetCollider(pair.colliderA);
            auto& colliderB = GetCollider(pair.colliderB);

            const auto pairIterator = _colliderPairs.find(pair);
            if (pairIterator != _colliderPairs.end())
            {
                if (IsContact(colliderA, colliderB))
                {
                    if (!colliderA.isTrigger && !colliderB.isTrigger)
                    {
                        Contact contact;
                        contact.collidingBodies[0] = CollidingBody{&GetBody(colliderA.bodyRef), &colliderA};
                        contact.collidingBodies[1] = CollidingBody{&GetBody(colliderB.bodyRef), &colliderB};
                        contact.Resolve();
                        contactListener->OnCollisionEnter(colliderA, colliderB);
                    }
                }
                else
                {
                    if (colliderA.isTrigger || colliderB.isTrigger)
                    {
                        contactListener->OnTriggerExit(colliderA, colliderB);
                    }

                    else
                    {
                        contactListener->OnCollisionExit(colliderA, colliderB);
                    }
                    _colliderPairs.erase(pairIterator);
                }
            }
            else
            {
                if (IsContact(colliderA, colliderB))
                {
                    if (!colliderA.isTrigger && !colliderB.isTrigger)
                    {
                        Contact contact;
                        contact.collidingBodies[0] = {&GetBody(colliderA.bodyRef), &colliderA};
                        contact.collidingBodies[1] = {&GetBody(colliderB.bodyRef), &colliderB};
                        contact.Resolve();
                        contactListener->OnCollisionEnter(colliderA, colliderB);
                    }
                    else
                    {
                        contactListener->OnTriggerEnter(colliderA, colliderB);
                    }
                    _colliderPairs.insert(pair);
                }
            }
        }
    }

    const std::size_t World::GetInitSizeForVector() noexcept
    {
        return initSizeForVector;
    };
}