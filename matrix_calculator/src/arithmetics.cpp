#pragma once

#include <stdexcept>

#include "matrix.h"

template<size_t H, size_t W>
Matrix<H, W> Matrix<H, W>::eval_bin_op(const Matrix& other, float (*op)(float, float)) const {
    Matrix res;

    for (size_t row = 0; row < H; row++) {
        for (size_t col = 0; col < W; col++) {
            res.set(
                op(
                    this->get(row, col),
                    other.get(row, col)),
                row, col);
        }
    }

    return res;
}

template<size_t H, size_t W>
Matrix<H, W> Matrix<H, W>::eval_bin_op(float k, float (*op)(float, float)) const {
    Matrix<W, H> res;

    for (size_t row = 0; row < H; row++) {
        for (size_t col = 0; col < W; col++) {
            res.set(op(k, this->get(row, col)), row, col);
        }
    }

    return res;
}

template<size_t H, size_t W>
Matrix<H, W> Matrix<H, W>::operator+(const Matrix &other) const {
    return this->eval_bin_op(other, [](float a, float b) { return a + b; });
}

template<size_t H, size_t W>
Matrix<H, W> Matrix<H, W>::operator-(const Matrix &other) const {
    return this->eval_bin_op(other, [](float a, float b) { return a - b; });
}

template<size_t H, size_t W>
Matrix<H, W> Matrix<H, W>::mul(const Matrix &other) const {
    return this->eval_bin_op(other, [](float a, float b) { return a * b; });
}

template<size_t H, size_t W>
bool Matrix<H, W>::operator==(const Matrix &other) const {
    auto this_iter = this->data.cbegin();
    auto other_iter = other.data.cbegin();
    while (this_iter != this->data.cend()) {
        if (std::abs(*this_iter - *other_iter) > 1e-7) {
            return false;
        }
        this_iter++;
        other_iter++;
    }
    return true;
}

template<size_t H, size_t W>
template<size_t _W>
Matrix<H, _W> Matrix<H, W>::operator*(const Matrix<W, _W> &other) const {
    Matrix<H, _W> res;

    std::array<MatrixRow<W>, H> self_rows = this->get_rows();
    std::array<MatrixCol<W>, H> other_cols = other.get_cols();

    for (size_t i = 0; i < H; i++) {
        for (size_t j = 0; j < _W; j++) {
            res.set(self_rows[i] * other_cols[j], i, j);
        }
    }

    return res;
}

template<size_t H, size_t W>
Matrix<H, W> Matrix<H, W>::operator+(float k) const {
    return this->eval_bin_op(k, [](float a, float b) { return a + b; });
}

template<size_t H, size_t W>
Matrix<H, W> Matrix<H, W>::operator-(float k) const {
    return this->eval_bin_op(k, [](float a, float b) { return a - b; });
}

template<size_t H, size_t W>
Matrix<H, W> Matrix<H, W>::operator*(float k) const {
    return this->eval_bin_op(k, [](float a, float b) { return a * b; });
}

template<size_t H, size_t W>
Matrix<H, W> Matrix<H, W>::add_row(const MatrixRow<W>& row, size_t row_id) {
    if (row_id > H) {
        throw std::runtime_error("Trying to add row to out of bounds");
    }
    MatrixRow<W> *rows = this->get_rows();
    rows[row_id] = rows[row_id] + row;
    return Matrix(rows, H);
}

template<size_t H, size_t W>
Matrix<H, W> Matrix<H, W>::add_col(const MatrixCol<H>& col, size_t col_id) {
    if (col_id > W) {
        throw std::runtime_error("Trying to add column to out of bounds");
    }
    MatrixCol<H> *cols = this->get_cols();
    cols[col_id] = cols[col_id] + col;
    return Matrix(cols, W);
}

template<size_t W, size_t H>
Matrix<W, H> operator+(float k, const Matrix<W, H>& matrix) {
    return matrix + k;
}

template<size_t W, size_t H>
Matrix<W, H> operator-(float k, const Matrix<W, H>& matrix) {
    return matrix - k;
}

template<size_t W, size_t H>
Matrix<W, H> operator*(float k, const Matrix<W, H>& matrix) {
    return matrix * k;
}

template<size_t N>
float MatrixRow<N>::operator*(MatrixCol<N>& other) {
    float scal_mul = 0;
    for (size_t i = 0; i < N; i++) {
        scal_mul += (*this)[i] * other[i];
    }
    return scal_mul;
}
