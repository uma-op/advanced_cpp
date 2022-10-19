#include <iostream>
#include "matrix.h"

int main() {
    float *a_data = new float[4]{1, 2, 3, 4};
    float *b_data = new float[4]{5, 6, 7, 8};
    Matrix<2, 2> a(a_data, 4);
    Matrix<2, 2> b(b_data, 4);
    Matrix<1, 4> c(a_data, 4);
    Matrix<1, 4> d(b_data, 4);

    Matrix<1, 4> *vs = new Matrix<1, 4>[2]{c, d};

    Matrix<2, 4> e(vs, 2);

    std::cout << e.get(0, 0) << ' ' << e.get(0, 1) << std::endl;
    std::cout << e.get(1, 0) << ' ' << e.get(1, 1) << std::endl;
    std::cout << e.get(2, 0) << ' ' << e.get(2, 1) << std::endl;
    std::cout << e.get(3, 0) << ' ' << e.get(3, 1) << std::endl;
}