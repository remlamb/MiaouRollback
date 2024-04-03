#include "UniquePtr.h"
#include "gtest/gtest.h"

struct uniquePtrFixture : public ::testing::TestWithParam<float>
{
};

INSTANTIATE_TEST_SUITE_P(UniquePtr, uniquePtrFixture, testing::Values(
        -9., -3., -1., 0., 1., 2., 8.
));

TEST_P(uniquePtrFixture, Constructor)
{
    auto param = GetParam();

    UniquePtr<float> uPtr(new float(param));

    EXPECT_FLOAT_EQ(*uPtr, param);
}

TEST_P(uniquePtrFixture, MoveConstructor)
{
    auto param = GetParam();
    UniquePtr<float> uPtr(new float(param));
    UniquePtr<float> uPtr2(std::move(uPtr));

    EXPECT_EQ(*uPtr2, param);
    EXPECT_EQ(uPtr.operator->(), nullptr);
}