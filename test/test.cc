#include "gtest/gtest.h"
#include "../src/vector.cpp"

TEST(Test, SubTest) {
	vector<int> v;
    EXPECT_EQ(v.empty(), 1);
}

TEST(Test, SubTest1) {
	vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);

	EXPECT_EQ(v.size(), 3);		
}

// all tests coming soon...


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
