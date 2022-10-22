#include <iostream>
#include <array>
#include "matrix.h"

int main() {
    std::array<float, 4> a_data{1, 2, 3, 4};
    std::array<float, 4> b_data{5, 6, 7, 8};
    Matrix<2, 2> a(a_data);
    Matrix<2, 2> b(b_data);
    Matrix<1, 4> c(a_data);
    Matrix<1, 4> d(b_data);

    Matrix<1, 4> vs[]{c, d};

    Matrix<2, 4> e(vs, 2);

    std::cout << e.get(0, 0) << ' ' << e.get(0, 1) << std::endl;
    std::cout << e.get(1, 0) << ' ' << e.get(1, 1) << std::endl;
    std::cout << e.get(2, 0) << ' ' << e.get(2, 1) << std::endl;
    std::cout << e.get(3, 0) << ' ' << e.get(3, 1) << std::endl;
}