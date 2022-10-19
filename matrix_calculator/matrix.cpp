#include <iostream>
#include "matrix.h"

void foo() {
    Matrix<2, 2> a;
    std::cout << a.get(0, 0) << "foo";
}