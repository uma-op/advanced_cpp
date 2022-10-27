#pragma once

#include <stdexcept>

#include "matrix.h"

template<size_t H, size_t W>
Matrix<W, H> Matrix<H, W>::transpose() {
    std::array<MatrixRow<W>, H> rows = this->get_rows();
    std::array<MatrixCol<W>, H> cols;

    for (size_t i = 0; i < H; i++) {
        cols[i] = rows[i];
    }

    return Matrix<W, H>(cols);
}

template<size_t H, size_t W>
Matrix<H - 1, W - 1> Matrix<H, W>::minor(size_t row_id, size_t col_id) {
    Matrix<H - 1, W - 1> m;
    for (size_t i = 0; i < H - 1; i++) {
        for (size_t j = 0; j < W - 1; j++) {
            if (i >= row_id && j >= col_id) {
                m.set(this->get(i + 1, j + 1), i, j);
            } else if (i >= row_id) {
                m.set(this->get(i + 1, j), i, j);
            } else if (j >= col_id) {
                m.set(this->get(i, j + 1), i, j);
            } else {
                m.set(this->get(i, j), i, j);
            }
        }
    }
    return m;
}

template<size_t H, size_t W>
Matrix<H, W> Matrix<H, W>::inv() {
    if (W != H) {
        throw std::runtime_error("Trying to get inverse of non square matrix");
    }

    float d = this->det();
    Matrix res;

    for (size_t i = 0; i < H; i++) {
        for (size_t j = 0; j < W; j++) {
            if ((i + j) % 2 == 0) {
                res.set(this->minor(i, j).det() / d, j, i);
            } else {
                res.set(-1 * this->minor(i, j).det() / d, j, i);
            }
        }
    }

    return res;
}

template<size_t H, size_t W>
float Matrix<H, W>::det() {
    if (W != H) {
        throw std::runtime_error("Trying to get determinant of non square matrix");
    }

    int parity = 1;
    auto next_permut = [&](std::array<size_t, W> *permut){
        size_t i, swap;
        for (i = 0; i < W; i++) {
            if (i == W - 1) {
                return false;
            }

            if ((*permut)[i] < (*permut)[i + 1]) {
                int t;

                for (size_t j = 0; j * 2 < i; j++) {
                    t = (*permut)[j];
                    (*permut)[j] = (*permut)[i - j];
                    (*permut)[i - j] = t;
                    parity *= -1;
                }

                for (swap = 0; (*permut)[swap] < (*permut)[i + 1]; swap++) {}

                t = (*permut)[i + 1];
                (*permut)[i + 1] = (*permut)[swap - 1];
                (*permut)[swap - 1] = t;
                parity *= -1;

                break;
            }
        }
        return true;
    };

    std::array<size_t, W> p;
    for (size_t i = 0; i < W; i++) { p[i] = i; }

    float mul = 1;
    float res = 0;

    do {
        mul = 1;
        for (size_t i = 0; i < W; i++) {
            mul *= this->get(i, p[i]);
        }
        res += parity * mul;
    } while (next_permut(&p));

    return res;
}
