#include <gtest/gtest.h>
#include "matrix.h"

template<size_t H, size_t W>
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

class TestMatrixConstruction : public MatrixBaseSuite<5, 5> {};

class TestMatrixAccess : public MatrixBaseSuite<5, 5> {};

class TestArithmeticOperators : public MatrixBaseSuite<5, 5> {
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

    EXPECT_TRUE(got == self);
}

TEST_F(TestMatrixConstruction, test_creation_from_vectors) {
    MatrixRow<5> hv1({0, 1, 2, 3, 4});
    MatrixRow<5> hv2({5, 6, 7, 8, 9});
    MatrixRow<5> hv3({10, 11, 12, 13, 14});
    MatrixRow<5> hv4({15, 16, 17, 18, 19});
    MatrixRow<5> hv5({20, 21, 22, 23, 24});
    MatrixRow<5> hvr5({20, 21, 22, 23, 24});

    std::array<MatrixRow<5>, 5> rdm({ hv1, hv2, hv3, hv4, hv5 });

    Matrix<5, 5> got_h(rdm);

    EXPECT_TRUE(got_h == self);

    MatrixCol<5> vv1({0, 5, 10, 15, 20});
    MatrixCol<5> vv2({1, 6, 11, 16, 21});
    MatrixCol<5> vv3({2, 7, 12, 17, 22});
    MatrixCol<5> vv4({3, 8, 13, 18, 23});
    MatrixCol<5> vv5({4, 9, 14, 19, 24});

    std::array<MatrixCol<5>, 5> cdm({ vv1, vv2, vv3, vv4, vv5 });

    Matrix<5, 5> got_v(cdm);

    EXPECT_TRUE(got_v == self);
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

    EXPECT_TRUE((self + other) == expected);
    EXPECT_TRUE((other + self) == expected);
}

TEST_F(TestArithmeticOperators, test_substraction) {
    Matrix<5, 5> expected({
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0
    });

    EXPECT_TRUE((self - self) == expected);
}

TEST_F(TestArithmeticOperators, test_multiplication) {
    Matrix<5, 5> expected({
        6, 6, 6, 6, 6,
        6, 6, 6, 6, 6,
        6, 6, 6, 6, 6,
        6, 6, 6, 6, 6,
        6, 6, 6, 6, 6,
    });

    EXPECT_FALSE(2 * other == expected);
    EXPECT_TRUE(3 * other * 2 == expected);
    EXPECT_TRUE(other * 6 == expected);
    EXPECT_TRUE(2 * 3 * other == expected);

    EXPECT_TRUE(other.mul(expected) == expected);
}

using MatrixBaseSuite_3 = MatrixBaseSuite<3, 3>;
TEST_F(MatrixBaseSuite_3, test_determinant) {
    Matrix<3, 3> mat({
        1, 2, 3,
        -4, 5, 6,
        7, 8, 9
    });

    EXPECT_EQ(mat.det(), -48);
}

using MatrixBaseSuite_3_2 = MatrixBaseSuite<3, 2>;
TEST_F(MatrixBaseSuite_3_2, test_transpose) {
    Matrix<2, 3> mat({
        1, 2, 3,
        4, 5, 6
    });

    Matrix<3, 2> mat_T({
        1, 4,
        2, 5,
        3, 6
    });

    EXPECT_TRUE(mat.transpose() == mat_T);
}

using MatrixBaseSuite_2_2 = MatrixBaseSuite<2, 2>;
TEST_F(MatrixBaseSuite_2_2, test_minor) {
    Matrix<3, 3> mj({
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    });

    Matrix<2, 2> mn({
        1, 3,
        7, 9
    });

    EXPECT_TRUE(mj.minor(1, 1) == mn);
}

TEST_F(MatrixBaseSuite_2_2, test_matrix_multiplication) {
    Matrix<2, 3> m1({
        1, 2, 3,
        4, 5, 6,
    });

    Matrix<3, 2> m2({
        6, 5,
        4, 3,
        2, 1
    });

    Matrix<2, 2> m3({
        20, 14,
        56, 41
    });

    EXPECT_TRUE(m1 * m2 == m3);
}

using MatrixBaseSuite_2 = MatrixBaseSuite<2, 2>;
TEST_F(MatrixBaseSuite_2, test_inverse) {
    Matrix<2, 2> e({
        1, 0,
        0, 1
    });
    Matrix<2, 2> m1({
        1, 2,
        3, 4
    });

    EXPECT_TRUE(m1 * m1.inv() == e);
}

using MatrixBaseSuite_5 = MatrixBaseSuite<5, 5>;
TEST_F(MatrixBaseSuite_5, test_vector_slice) {
    MatrixRow<5> r {1, 2, 3, 4, 5};

    auto print_slice = [](std::vector<float> v){
        std::cout << ">>>>> size of slice is " << v.size() << std::endl;
        for (auto b = v.begin(); b != v.end(); b++) {
            std::cout << *b << ' ';
        }
        std::cout << std::endl;
    };

    std::vector<float> s;

    s = r.slice(0, 5);
    print_slice(s);
    s = r.slice(0, 2);
    print_slice(s);
    s = r.slice(0, 5, 2);
    print_slice(s);
    s = r.slice(4, 0, -1);
    print_slice(s);
    s = r.slice(4, -1, -1);
    print_slice(s);
    s = r.slice(4, 0);
    print_slice(s);
}