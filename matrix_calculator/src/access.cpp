#pragma once

#include <stdexcept>

#include "matrix.h"

template<size_t H, size_t W>
void Matrix<H, W>::set(float val, size_t row, size_t col) {
    if (row >= H || col >= W) {
        throw std::runtime_error("Trying to set to out of bounds");
    }
    this->data[row * W + col] = val;
}

template<size_t H, size_t W>
float Matrix<H, W>::get(size_t row, size_t col) const {
    if (row >= H || col >= W) {
        throw std::runtime_error("Trying to get from out of bounds");
    }
    return this->data[row * W + col];
}

template<size_t H, size_t W>
MatrixRow<W> Matrix<H, W>::get_row(size_t row) const {
    if (row >= H) {
        throw std::runtime_error("Trying to get row from out of bounds");
    }

    MatrixRow<W> res;

    for (size_t i = 0; i < W; i++) {
        res[i] = this->get(row, i);
    }

    return res;
}

template<size_t H, size_t W>
std::array<MatrixRow<W>, H> Matrix<H, W>::get_rows() const {
    std::array<MatrixRow<W>, H> res;
    for (size_t i = 0; i < H; i++) {
        res[i] = this->get_row(i);
    }
    return res;
}

template<size_t H, size_t W>
MatrixCol<H> Matrix<H, W>::get_col(size_t col) const {
    if (col >= W) {
        throw std::runtime_error("Trying to get column from out of bounds");
    }

    MatrixCol<H> res;

    for (size_t i = 0; i < H; i++) {
        res[i] = this->get(i, col);
    }

    return res;
}

template<size_t H, size_t W>
std::array<MatrixCol<H>, W> Matrix<H, W>::get_cols() const {
    std::array<MatrixCol<H>, W> res;
    for (size_t i = 0; i < W; i++) {
        res[i] = this->get_col(i);
    }
    return res;
}

template<size_t H, size_t W>
Matrix<H, W> Matrix<H, W>::get_diag() const {
    if (W != H) {
        throw std::runtime_error("Trying to get diagonal of non square matrix");
    }

    Matrix res;

    for (size_t i = 0; i < W; i++) {
        for (size_t j = 0; j < H; j++) {
            if (i == j) {
                res.set(this->get(i, j), i, j);
            } else {
                res.set(0, i, j);
            }
        }
    }

    return res;
}

template<size_t H, size_t W>
Matrix<H, W> Matrix<H, W>::get_sec_diag() const {
    if (W != H) {
        throw std::runtime_error("Trying to get secondary diagonal of non square matrix");
    }

    Matrix res;

    for (size_t i = 0; i < W; i++) {
        for (size_t j = 0; j < H; j++) {
            if (i + j == W - 1) {
                res.set(this->get(i, j), i, j);
            } else {
                res.set(0, i, j);
            }
        }
    }

    return res;
}

template<size_t N>
float& MatrixRow<N>::operator[](size_t i) {
    if (i >= N) {
        throw std::runtime_error("Trying to access to out of bounds");
    }
    return this->data[i];
}

template<size_t N>
float& MatrixCol<N>::operator[](size_t i) {
    if (i >= N) {
        throw std::runtime_error("Trying to access to out of bounds");
    }
    return this->data[i];
}
