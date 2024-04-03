#include "QuadTree.h"
#include "gtest/gtest.h"
#include <array>

static HeapAllocator TestHeapAllocator;

TEST(QuadNode, ConstructorDefault)
{
    Engine::QuadNode node{TestHeapAllocator};
    EXPECT_EQ(node.bounds.MaxBound().X, Math::Vec2F::Zero().X);
    EXPECT_EQ(node.bounds.MaxBound().Y, Math::Vec2F::Zero().Y);

    EXPECT_EQ(node.bounds.MinBound().X, Math::Vec2F::Zero().X);
    EXPECT_EQ(node.bounds.MinBound().Y, Math::Vec2F::Zero().Y);

    EXPECT_EQ(node.children[0], nullptr);
    EXPECT_EQ(node.children[1], nullptr);
    EXPECT_EQ(node.children[2], nullptr);
    EXPECT_EQ(node.children[3], nullptr);
}

TEST(QuadTree, ConstructorDefault)
{
    Engine::QuadTree quadTree{TestHeapAllocator};
    EXPECT_EQ(quadTree.nodeIndex, 1);
    EXPECT_EQ(quadTree.MaxColliderInNode, Engine::QuadTree::MaxColliderInNode);
    EXPECT_EQ(quadTree.MaxDepth, Engine::QuadTree::MaxDepth);
}

TEST(QuadTree, Init)
{
    Engine::QuadTree quadTree{TestHeapAllocator};
    quadTree.Init();

    std::size_t maxChildrenPossible = 0;
    for (int i = 0; i <= quadTree.MaxDepth; i++)
    {
        maxChildrenPossible += Math::Pow(4, i);
    }

    EXPECT_EQ(quadTree.nodes[0].children[0], nullptr);
    EXPECT_EQ(quadTree.nodeIndex, 1);
    EXPECT_EQ(quadTree.nodes[0].bounds.MaxBound(), Math::Vec2F(0.0f, 0.0f));
    EXPECT_EQ(quadTree.nodes[maxChildrenPossible].children[0], nullptr);
}