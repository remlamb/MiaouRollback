#include "SharedPtr.h"
#include "gtest/gtest.h"

struct sharedPtrFixture : public ::testing::TestWithParam<float>
{
};

INSTANTIATE_TEST_SUITE_P(SharedPtr, sharedPtrFixture, testing::Values(
        -9., -3., -1., 0., 1., 2., 8.
));

TEST_P(sharedPtrFixture, Constructor)
{
    auto param = GetParam();
    SharedPtr<float> sPtr(new float(param));

    EXPECT_FLOAT_EQ(*sPtr, param);
}

TEST_P(sharedPtrFixture, Copy)
{
    auto param = GetParam();
    SharedPtr<float> sPtr(new float(param));
    SharedPtr<float> sPtr2(sPtr);

    EXPECT_EQ(*sPtr, param);
    EXPECT_EQ(*sPtr2, *sPtr);
    EXPECT_EQ(sPtr.Count(), 2);
    EXPECT_EQ(sPtr2.Count(), 2);

    {
        SharedPtr<float> sPtr3(sPtr);
        EXPECT_EQ(sPtr2.Count(), 3);
    }

    EXPECT_EQ(sPtr2.Count(), 2);
}