#include "gtest/gtest.h"
#include "../src/vector.cpp"

TEST(VectorTest, DefaultConstructor) {
    vector<int> v;
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
    EXPECT_TRUE(v.empty());
}

TEST(VectorTest, SizeConstructor) {
    vector<int> v(5);
    EXPECT_EQ(v.size(), 5);
    EXPECT_GE(v.capacity(), 5);
}

TEST(VectorTest, InitializerListConstructor) {
    vector<int> v = {1, 2, 3};
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST(VectorTest, CopyConstructor) {
    vector<int> v1 = {1, 2, 3};
    vector<int> v2 = v1;
    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v2[0], 1);
    EXPECT_EQ(v2[1], 2);
    EXPECT_EQ(v2[2], 3);
}

TEST(VectorTest, MoveConstructor) {
    vector<int> v1 = {1, 2, 3};
    vector<int> v2 = std::move(v1);
    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v2[0], 1);
    EXPECT_EQ(v2[1], 2);
    EXPECT_EQ(v2[2], 3);
    EXPECT_EQ(v1.size(), 0);
}

TEST(VectorTest, PushBack) {
    vector<int> v;
    v.push_back(42);
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 42);
}

TEST(VectorTest, PopBack) {
    vector<int> v = {1, 2, 3};
    v.pop_back();
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v.back(), 2);
}

TEST(VectorTest, ElementAccess) {
    vector<int> v = {1, 2, 3};
    EXPECT_EQ(v.front(), 1);
    EXPECT_EQ(v.back(), 3);
    EXPECT_NO_THROW(v.at(1));
    EXPECT_THROW(v.at(5), std::out_of_range);
}

TEST(VectorTest, CapacityManagement) {
    vector<int> v(2);
    v.reserve(10);
    EXPECT_GE(v.capacity(), 10);
    v.shrink_to_fit();
    EXPECT_EQ(v.capacity(), 2);
}

TEST(VectorTest, InsertAndErase) {
    vector<int> v = {1, 3};
    v.insert(v.cbegin() + 1, 2);
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[1], 2);
    v.erase(v.cbegin() + 1);
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[1], 3);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}






