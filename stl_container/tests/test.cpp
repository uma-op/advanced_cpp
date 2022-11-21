#include <gtest/gtest.h>
#include "stl_set.h"

class BaseSuite : public ::testing::Test {
  protected:
    void SetUp() {};
    void TearDown() {};

  	Set<int> s;
};

TEST_F(BaseSuite, passed) {
    s.insert(10);
    // std::cout << s.t->value << std::endl;
    EXPECT_EQ(s.t->value, 10);
    s.insert(11);
    EXPECT_EQ(s.t->right->value, 11);
    s.insert(7);
    EXPECT_EQ(s.t->left->value, 7);
    s.insert(8);
    EXPECT_EQ(s.t->left->right->value, 8);
    s.insert(9);
    EXPECT_EQ(s.t->left->value, 8);
    EXPECT_EQ(s.t->left->left->value, 7);
    EXPECT_EQ(s.t->left->right->value, 9);

	s.erase(7);
	EXPECT_EQ(s.t->left->left, nullptr);

	s.erase(8);
	EXPECT_EQ(s.t->left->value, 9);

	s.insert(7);

	auto b = s.cbegin();
	auto e = s.cend();

	std::cout << "Backward iteration" << std::endl;

	while (e.value) {
		std::cout << *e << std::endl;
		e--;
	}

	std::cout << "Forward iteration" << std::endl;

	while (b.value) {
		std::cout << *b << std::endl;
		b++;
	}

	EXPECT_FALSE(s.empty());
	EXPECT_EQ(s.size(), 4);
}