#include "World.h"
#include "gtest/gtest.h"
#include <array>

struct WorldFixture : public ::testing::TestWithParam<float>
{
};

INSTANTIATE_TEST_SUITE_P(world, WorldFixture, testing::Values(
        -9., -3., -1., 0., 1., 2., 8.
));

TEST(World, Init)
{
    Engine::World newWorld;
    newWorld.Init();
    EXPECT_EQ(newWorld.CurrentBodyCount(), newWorld.GetInitSizeForVector());
}

TEST_P(WorldFixture, Update)
{
    auto param = GetParam();
    float deltaTime = 1.;
    Engine::Body body(param, Math::Vec2F(param, param), Math::Vec2F(param, param));
    if (!body.IsValid())
    {
        EXPECT_EQ(body.Position(), Math::Vec2F(param, param));
    }
    else
    {
        Math::Vec2F acceleration = body.Force() / body.Mass();
        Math::Vec2F bodyVelocity = body.Velocity() + acceleration * deltaTime;
        Math::Vec2F bodyPosition = body.Position() + body.Velocity() * deltaTime;

        Math::Vec2F velocity = Math::Vec2F(param, param) + acceleration * deltaTime;
        Math::Vec2F position = Math::Vec2F(param, param) + body.Velocity() * deltaTime;

        EXPECT_EQ(bodyVelocity, velocity);
        EXPECT_EQ(bodyPosition, position);
    }
}

TEST(World, CreateBody)
{
    Engine::World newWorld;
    newWorld.Init();
    for (int i = 0; i < 10; i++)
    {
        Engine::BodyRef bRef = newWorld.CreateBody();
        auto& body = newWorld.GetBody(bRef);
        body.SetMass(1);
        EXPECT_EQ(bRef.index, i);
    }
}

TEST(World, Destroy)
{
    Engine::World newWorld;
    newWorld.Init();
    for (int i = 0; i < 10; i++)
    {
        Engine::BodyRef bRef = newWorld.CreateBody();
        auto& body = newWorld.GetBody(bRef);
        body.SetMass(1);
        newWorld.DestroyBody(bRef);

        EXPECT_EQ(bRef.genIdx, i);
    }
}

TEST(World, CreateCollider)
{
    Engine::World newWorld;
    newWorld.Init();
    for (int i = 0; i < 10; i++)
    {
        Engine::BodyRef bRef = newWorld.CreateBody();
        auto& body = newWorld.GetBody(bRef);
        body.SetMass(1);
        Engine::ColliderRef cRef = newWorld.CreateCollider(bRef);
        auto& collider = newWorld.GetCollider(cRef);
        collider._shape = Math::ShapeType::Circle;
        collider.ID = i + 1;
        EXPECT_EQ(cRef.index, i);
    }
}

TEST(World, IsContact)
{
    Engine::Collider colliderA;
    colliderA._shape = Math::ShapeType::Circle;
    colliderA.circleShape.Center() = Math::Vec2F(0.0f, 0.0f);
    colliderA.circleShape.SetRadius(1.0f);

    Engine::Collider colliderB;
    colliderB._shape = Math::ShapeType::Circle;
    colliderB.circleShape.Center() = Math::Vec2F(2.0f, 0.0f);
    colliderB.circleShape.SetRadius(1.0f);

    Engine::Collider colliderD;
    colliderD._shape = Math::ShapeType::Rectangle;
    colliderD.rectangleShape.Center() = Math::Vec2F(0.0f, 0.0f);
    colliderD.rectangleShape.MinBound() = Math::Vec2F(0.0f, 0.0f);
    colliderD.rectangleShape.MaxBound() = Math::Vec2F(2.0f, 2.0f);

    Engine::World world;
    EXPECT_TRUE(world.IsContact(colliderA, colliderB));
    EXPECT_TRUE(world.IsContact(colliderA, colliderD));
}

