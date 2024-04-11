#include "Body.h"

void Physics::Body::SetMass(float mass) noexcept
{
    _mass = mass;
}

[[nodiscard]] Math::Vec2F Physics::Body::Velocity() noexcept
{
    return _velocity;
}

void Physics::Body::SetVelocity(Math::Vec2F velocity) noexcept
{
    _velocity = velocity;
}

[[nodiscard]] Math::Vec2F Physics::Body::Position() noexcept
{
    return _position;
}

void Physics::Body::SetPosition(Math::Vec2F position) noexcept
{
    _position = position;
}

[[nodiscard]] Math::Vec2F Physics::Body::Force() noexcept
{
    return _totalForce;
}

void Physics::Body::SetForce(Math::Vec2F force) noexcept
{
    _totalForce = force;
}

void Physics::Body::AddForce(Math::Vec2F force) noexcept
{
    _totalForce += force;
};