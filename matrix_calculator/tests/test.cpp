#include <gtest/gtest.h>
#include "matrix.h"

class MatrixBaseSuite : public ::testing::Test {
 protected:
    void SetUp() {
        for (size_t i = 0; i < 5; i++) {
            for (size_t j = 0; j < 5; j++) {
                self.set(i * 5 + j, i, j);
            }
        }
    }

    void TearDown() {}

    Matrix<5, 5> self;
};

class TestMatrixAccess : public MatrixBaseSuite {};

class TestArithmeticOperators : public MatrixBaseSuite {
 protected:
    void SetUp() {
        MatrixBaseSuite::SetUp();
        for (size_t i = 0; i < 5; i++) {
            for (size_t j = 0; j < 5; j++) {
                other.set(1, i, j);
            }
        }
    }

    bool MatrixEquality(const Matrix<5, 5>& got, const Matrix<5, 5>& expected) {
        auto got_iter = got.data.cbegin();
        auto exp_iter = expected.data.cbegin();
        while (got_iter != got.data.cend()) {
            if (*got_iter != *exp_iter) return false;
            got_iter++;
            exp_iter++;
        }
        return true;
    }

    Matrix<5, 5> other;
};

TEST_F(TestMatrixAccess, test_get_single_elem) {
    EXPECT_EQ(self.get(2, 2), 12);
    EXPECT_EQ(self.get(0, 0), 0);
    EXPECT_THROW(self.get(6, 2), std::runtime_error);
}

TEST_F(TestMatrixAccess, DISABLED_test_get_col) {}
TEST_F(TestMatrixAccess, DISABLED_test_get_row) {}

TEST_F(TestArithmeticOperators, test_addition) {
    Matrix<5, 5> expected({
        1,  2,  3,  4,  5,
        6,  7,  8,  9,  10,
        11, 12, 13, 14, 15,
        16, 17, 18, 19, 20,
        21, 22, 23, 24, 25
    });

    EXPECT_TRUE(MatrixEquality(self + other, expected));
    EXPECT_TRUE(MatrixEquality(other + self, expected));
}

TEST_F(TestArithmeticOperators, test_substraction) {
    Matrix<5, 5> expected({
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0
    });

    EXPECT_TRUE(MatrixEquality(self - self, expected));
}

TEST_F(TestArithmeticOperators, test_multiplication) {
    Matrix<5, 5> expected({
        6, 6, 6, 6, 6,
        6, 6, 6, 6, 6,
        6, 6, 6, 6, 6,
        6, 6, 6, 6, 6,
        6, 6, 6, 6, 6,
    });

    EXPECT_FALSE(MatrixEquality(2 * other, expected));
    EXPECT_TRUE(MatrixEquality(3 * other * 2, expected));
    EXPECT_TRUE(MatrixEquality(other * 6, expected));
    EXPECT_TRUE(MatrixEquality(2 * 3 * other, expected));
}
