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

    bool MatrixEquality(const Matrix<5, 5>& got, const Matrix<5, 5>& expected) {
        auto got_iter = got.data.cbegin();
        auto exp_iter = expected.data.cbegin();
        while (got_iter != got.data.cend()) {
            if (std::abs(*got_iter - *exp_iter) > 1e-7) return false;
            got_iter++;
            exp_iter++;
        }
        return true;
    }

    Matrix<5, 5> self;
};

class TestMatrixConstruction : public MatrixBaseSuite {};

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

    Matrix<5, 5> other;
};

TEST_F(TestMatrixConstruction, test_creation_from_array) {
    Matrix<5, 5> got({
        0,  1,  2,  3,  4,
        5,  6,  7,  8,  9,
        10, 11, 12, 13, 14,
        15, 16, 17, 18, 19,
        20, 21, 22, 23, 24
    });

    EXPECT_TRUE(MatrixEquality(got, self));
}

TEST_F(TestMatrixConstruction, test_creation_from_vectors) {
    MatrixRow<5> hv1({0, 1, 2, 3, 4});
    MatrixRow<5> hv2({5, 6, 7, 8, 9});
    MatrixRow<5> hv3({10, 11, 12, 13, 14});
    MatrixRow<5> hv4({15, 16, 17, 18, 19});
    MatrixRow<5> hv5({20, 21, 22, 23, 24});
    MatrixRow<5> hvr5({20, 21, 22, 23, 24});

    Matrix<5, 5> got_h(new MatrixRow<5>[5]{ hv1, hv2, hv3, hv4, hv5 }, 5);

    EXPECT_TRUE(MatrixEquality(got_h, self));

    MatrixCol<5> vv1({0, 5, 10, 15, 20});
    MatrixCol<5> vv2({1, 6, 11, 16, 21});
    MatrixCol<5> vv3({2, 7, 12, 17, 22});
    MatrixCol<5> vv4({3, 8, 13, 18, 23});
    MatrixCol<5> vv5({4, 9, 14, 19, 24});

    Matrix<5, 5> got_v(new MatrixCol<5>[5]{ vv1, vv2, vv3, vv4, vv5 }, 5);

    EXPECT_TRUE(MatrixEquality(got_v, self));
}

TEST_F(TestMatrixAccess, test_get_single_elem) {
    EXPECT_EQ(self.get(2, 2), 12);
    EXPECT_EQ(self.get(0, 0), 0);
    EXPECT_THROW(self.get(6, 2), std::runtime_error);
    EXPECT_NO_THROW(self.set(100, 0, 0));
    EXPECT_EQ(self.get(0, 0), 100);
    EXPECT_THROW(self.set(0, 6, 2), std::runtime_error);
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

    EXPECT_TRUE(MatrixEquality(other.mul(expected), expected));
}

TEST_F(MatrixBaseSuite, test_determinant) {
    Matrix<3, 3> mat({
        1, 2, 3,
        -4, 5, 6,
        7, 8, 9
    });

    EXPECT_EQ(mat.det(), -48);
}