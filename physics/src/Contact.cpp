#include "Contact.h"

float Physics::Contact::CalculateSeparateVelocity() const noexcept
{
    const auto relativeVelocity = collidingBodies[0] . body -> Velocity() - collidingBodies[1] . body -> Velocity();
    return relativeVelocity . Dot(contactNormal);
}

void Physics::Contact::ResolveVelocity() const noexcept
{
    const auto separatingVelocity = CalculateSeparateVelocity();
    if (separatingVelocity > 0)
    {
        return;
    }
    //Restitution Implementation
    const auto newSeparateVelocity = -separatingVelocity * restitution;
    const auto deltaVelocity = newSeparateVelocity - separatingVelocity;

    const auto inverseMassBody1 = 1 / collidingBodies[0] . body -> Mass();
    const auto inverseMassBody2 = 1 / collidingBodies[1] . body -> Mass();
    const auto totalInverseMass = inverseMassBody1 + inverseMassBody2;

    const auto impulse = deltaVelocity / totalInverseMass;
    const auto impulsePerIMass = contactNormal * impulse;

    if (collidingBodies[0] . body -> type == BodyType::DYNAMIC)
    {
        collidingBodies[0] . body -> SetVelocity(
                collidingBodies[0] . body -> Velocity() + impulsePerIMass * inverseMassBody1);
    }
    else if (collidingBodies[0] . body -> type == BodyType::STATIC)
    {
        collidingBodies[1] . body -> SetVelocity(
                collidingBodies[1] . body -> Velocity() - impulsePerIMass * inverseMassBody1);
    }

    if (collidingBodies[1] . body -> type == BodyType::DYNAMIC)
    {
        collidingBodies[1] . body -> SetVelocity(
                collidingBodies[1] . body -> Velocity() - impulsePerIMass * inverseMassBody2);
    }
    else if (collidingBodies[1] . body -> type == BodyType::STATIC)
    {
        collidingBodies[0] . body -> SetVelocity(
                collidingBodies[0] . body -> Velocity() + impulsePerIMass * inverseMassBody2);
    }
}

void Physics::Contact::ResolveInterpenetration() const noexcept
{
    if (penetration <= 0)
    {
        return;
    }

    const auto inverseMassBody1 = 1 / collidingBodies[0] . body -> Mass();
    const auto inverseMassBody2 = 1 / collidingBodies[1] . body -> Mass();
    const auto totalInverseMass = inverseMassBody1 + inverseMassBody2;

    if (totalInverseMass <= 0)
    {
        return;
    }

    const auto movePerIMass = contactNormal * (penetration / totalInverseMass);
    if (collidingBodies[0] . body -> type == BodyType::DYNAMIC)
    {
        collidingBodies[0] . body -> SetPosition(
                collidingBodies[0] . body -> Position() + movePerIMass * inverseMassBody1);
    }
    if (collidingBodies[1] . body -> type == BodyType::DYNAMIC)
    {
        collidingBodies[1] . body -> SetPosition(
                collidingBodies[1] . body -> Position() - movePerIMass * inverseMassBody2);
    }
}

void Physics::Contact::Resolve()
{
    const auto delta = collidingBodies[0] . body -> Position() - collidingBodies[1] . body -> Position();
    switch (collidingBodies[0] . collider -> _shape)
    {
        case (Math::ShapeType::Circle):
            switch (collidingBodies[1] . collider -> _shape)
            {
                case (Math::ShapeType::Circle):
                {
                    contactNormal = delta . Normalized();
                    penetration = collidingBodies[0] . collider -> circleShape . Radius() +
                                  collidingBodies[1] . collider -> circleShape . Radius() - delta . Length();
                }
                    break;
                case Math::ShapeType::Rectangle:
                {
                    const auto circleCenter = collidingBodies[0] . collider -> circleShape . Center();
                    const auto rectCenter = collidingBodies[1] . collider -> rectangleShape . Center();
                    const auto rectHalfSize = collidingBodies[1] . collider -> rectangleShape . HalfSize();

                    const auto delta = circleCenter - rectCenter;

                    Math::Vec2F closestPoint;

                    closestPoint . X = Math::Clamp(delta . X, -rectHalfSize . X, rectHalfSize . X);
                    closestPoint . Y = Math::Clamp(delta . Y, -rectHalfSize . Y, rectHalfSize . Y);

                    const auto distance = (closestPoint - delta) . Length();
                    const auto closestPointOnRect = rectCenter + closestPoint;

                    auto circleToRectClosestPoint = (circleCenter - closestPointOnRect);

                    if (circleToRectClosestPoint . Length() <= Math::Epsilon)
                    {
                        circleToRectClosestPoint = Math::Vec2F(0.f, 1.f);
                    }

                    contactNormal = circleToRectClosestPoint . Normalized();
                    penetration = collidingBodies[0] . collider -> circleShape . Radius() - distance;
                }


                default:
                    break;
            }
            break;
        case Math::ShapeType::Rectangle:
            switch (collidingBodies[1] . collider -> _shape)
            {
                case (Math::ShapeType::Circle):
                {
                    std::swap(collidingBodies[0], collidingBodies[1]);
                    Resolve();
                }
                    break;
                case (Math::ShapeType::Rectangle):
                {
                    const Math::Vec2F penetrationVec2F(
                            collidingBodies[0] . collider -> rectangleShape . HalfSize() +
                            collidingBodies[1] . collider -> rectangleShape . HalfSize() -
                            Math::Vec2F(std::abs(delta . X), std::abs(delta . Y))
                    );

                    if (penetrationVec2F . X < penetrationVec2F . Y)
                    {
                        contactNormal = delta . X > 0 ? Math::Vec2F(1.0f, 0.0f) : Math::Vec2F(-1.0f, 0.0f);

                        penetration = penetrationVec2F . Normalized() . X;
                    }
                    else
                    {
                        contactNormal = delta . Y > 0 ? Math::Vec2F(0.f, 1.0f) : Math::Vec2F(0.f, -1.0f);

                        penetration = penetrationVec2F . Normalized() . Y;
                    }
                }
                    break;
                default:
                    break;

            }
            break;
        default:
            break;
    }

    const auto mass1 = collidingBodies[0] . body -> Mass(), mass2 = collidingBodies[1] . body -> Mass();
    const auto rest1 = collidingBodies[0] . collider -> restitution, rest2 = collidingBodies[1] . collider -> restitution;

    restitution = (mass1 * rest1 + mass2 * rest2) / (mass1 + mass2);
    ResolveVelocity();
    ResolveInterpenetration();
}
