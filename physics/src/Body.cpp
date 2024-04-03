#include "Body.h"

void Engine::Body::SetMass(float mass) noexcept
{
    _mass = mass;
}

[[nodiscard]] Math::Vec2F Engine::Body::Velocity() noexcept
{
    return _velocity;
}

void Engine::Body::SetVelocity(Math::Vec2F velocity) noexcept
{
    _velocity = velocity;
}

[[nodiscard]] Math::Vec2F Engine::Body::Position() noexcept
{
    return _position;
}

void Engine::Body::SetPosition(Math::Vec2F position) noexcept
{
    _position = position;
}

[[nodiscard]] Math::Vec2F Engine::Body::Force() noexcept
{
    return _totalForce;
}

void Engine::Body::SetForce(Math::Vec2F force) noexcept
{
    _totalForce = force;
}

void Engine::Body::AddForce(Math::Vec2F force) noexcept
{
    _totalForce += force;
};