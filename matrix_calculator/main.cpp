#include <iostream>
#include "matrix.h"

void foo();

int main() {
    foo();
    Matrix<2, 2> a;
    a.set(1, 0, 0);
    a.set(2, 0, 1);
    a.set(3, 1, 0);
    a.set(4, 1, 1);

    Matrix<2, 2> b;
    b.set(5, 0, 0);
    b.set(6, 0, 1);
    b.set(7, 1, 0);
    b.set(8, 1, 1);

    Matrix<2, 2> c = a.eval_bin_op(b, [](float x, float y){return x * y;});

    std::cout << c.get(0, 0) << ' ' << c.get(0, 1) << std::endl;
    std::cout << c.get(1, 0) << ' ' << c.get(1, 1) << std::endl;
}