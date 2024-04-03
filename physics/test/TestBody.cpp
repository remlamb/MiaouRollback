#include "../physics/include/Body.h"
#include "gtest/gtest.h"
#include <array>

TEST(BodyTest, DefaultConstructor)
{
    Engine::Body body;

    EXPECT_FLOAT_EQ(body.Mass(), 0);
    EXPECT_EQ(body.Velocity(), Math::Vec2F(0, 0));
    EXPECT_EQ(body.Position(), Math::Vec2F(0, 0));
    EXPECT_EQ(body.Force(), Math::Vec2F(0, 0));
}

struct TestRealBody : public ::testing::TestWithParam<float>
{
};

INSTANTIATE_TEST_SUITE_P(BodyTest, TestRealBody, testing::Values(
        -9, -3., -1., 0., 1., 2., 8
));

TEST_P(TestRealBody, Constructor)
{
    auto param = GetParam();
    float mass = param;
    Math::Vec2F velocity = Math::Vec2F(param, param);
    Math::Vec2F position = Math::Vec2F(param, param);
    Engine::Body body(param, velocity, position);

    EXPECT_FLOAT_EQ(body.Mass(), mass);
    EXPECT_EQ(body.Velocity(), velocity);
    EXPECT_EQ(body.Position(), position);
    EXPECT_EQ(body.Force(), Math::Vec2F(0, 0));
}

TEST_P(TestRealBody, AddForce)
{
    auto param = GetParam();
    Engine::Body body(1, Math::Vec2F(10, 10), Math::Vec2F(10, 10));
    Math::Vec2F startBodyForce = body.Force();
    Math::Vec2F force = Math::Vec2F(param, param);
    body.AddForce(force);
    Math::Vec2F finalForce = startBodyForce + force;

    EXPECT_EQ(body.Force(), finalForce);
}


TEST_P(TestRealBody, IsRealBody)
{
    auto param = GetParam();
    Engine::Body body(param, Math::Vec2F(10, 10), Math::Vec2F(10, 10));
    if (param > 0)
    {
        EXPECT_TRUE(body.IsValid());
    }
    else
    {
        EXPECT_FALSE(body.IsValid());
    }
}
