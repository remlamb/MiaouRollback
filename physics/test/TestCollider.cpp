#include "Collider.h"
#include "gtest/gtest.h"

struct ColliderFixture : public ::testing::TestWithParam<std::size_t>
{
};

INSTANTIATE_TEST_SUITE_P(Collider, ColliderFixture, testing::Values(
        -9., -3., -1., 0., 1., 2., 8.
));

struct ColliderFixtureFloat : public ::testing::TestWithParam<float>
{
};

INSTANTIATE_TEST_SUITE_P(Collider, ColliderFixtureFloat, testing::Values(
        -9., -3., -1., 0., 1., 2., 8.
));

TEST_P(ColliderFixture, Equality)
{
    auto param = GetParam();
    Engine::ColliderRef ref1{param, param};
    Engine::ColliderRef ref2{param, param};
    Engine::ColliderRef ref3{param + 1, param + 1};

    EXPECT_EQ(ref1, ref2);
    EXPECT_FALSE(ref1 == ref3);
}

TEST(Collider, ValidityCheck)
{
    Engine::Collider collider;
    EXPECT_FALSE(collider.IsValid());

    collider._shape = Math::ShapeType::Circle;
    collider.circleShape = {Math::Vec2F(0.f, 0.f), 1.f};
    EXPECT_TRUE(collider.IsValid());
}

TEST_P(ColliderFixtureFloat, EqualityOperators)
{
    Engine::Collider collider1;
    Engine::Collider collider2;
    auto param = static_cast<int>(GetParam());
    collider1.ID = param, collider2.ID = param;
    EXPECT_EQ(collider1, collider2);

    collider1.ID += 1;
    collider2.ID += 2;
    EXPECT_FALSE(collider1 == collider2);
}

TEST_P(ColliderFixture, PairEquality)
{
    auto param = GetParam();
    Engine::ColliderRef ref1{param, 0};
    Engine::ColliderRef ref2{param + 1, 0};
    Engine::ColliderRef ref3{param + 10, 0};
    Engine::ColliderPair pair1{ref1, ref2};
    Engine::ColliderPair pair2{ref2, ref3};
    Engine::ColliderPair pair3{ref1, ref2};

    EXPECT_EQ(pair1, pair3);
    EXPECT_FALSE(pair1 == pair2);
}

TEST_P(ColliderFixture, HashFunction)
{
    auto param = GetParam();
    Engine::ColliderRef ref1{param, 0};
    Engine::ColliderRef ref2{param + 1, 0};
    Engine::ColliderPair pair1{ref1, ref2};
    Engine::ColliderPair pair2{ref2, ref1};

    Engine::ColliderPairHash hashFunction;
    std::size_t hash1 = hashFunction(pair1);
    std::size_t hash2 = hashFunction(pair2);

    EXPECT_EQ(hash1, hash2);
}