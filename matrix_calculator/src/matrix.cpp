#pragma once

#include <algorithm>
#include <stdexcept>
#include <iostream>

#include "matrix.h"

template<size_t H, size_t W>
Matrix<H, W>::Matrix() {
    if (W == 0 || H == 0) {
        throw std::runtime_error("Trying to define empty matrix");
    }
}

template<size_t H, size_t W>
Matrix<H, W>::Matrix(const Matrix &other) : Matrix() {
    std::copy(other.data.cbegin(), other.data.cend(), this->data.begin());
}

template<size_t H, size_t W>
Matrix<H, W>::Matrix(const std::array<float, H * W> data) : Matrix() {
    std::copy(data.cbegin(), data.cend(), this->data.begin());
}

template<size_t H, size_t W>
Matrix<H, W>::Matrix(std::array<MatrixRow<W>, H> vs) : Matrix() {
    for (size_t i = 0; i < H; i++) {
        for (size_t j = 0; j < W; j++) {
            this->set(vs[i].get(0, j), i, j);
        }
    }
}

template<size_t H, size_t W>
Matrix<H, W>::Matrix(std::array<MatrixCol<H>, W> vs) : Matrix() {
    for (size_t j = 0; j < W; j++) {
        for (size_t i = 0; i < H; i++) {
            this->set(vs[j].get(i, 0), i, j);
        }
    }
}

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
MatrixRow<N>::MatrixRow(const MatrixRow& other) : Matrix<1, N>::Matrix(other) {}

template<size_t N>
MatrixRow<N>::MatrixRow(const std::array<float, N> data) : Matrix<1, N>::Matrix(data) {}

template<size_t N>
MatrixRow<N>::MatrixRow(const MatrixCol<N>& other) : Matrix<1, N>::Matrix(other.data) {}

template<size_t N>
float& MatrixRow<N>::operator[](size_t i) {
    if (i >= N) {
        throw std::runtime_error("Trying to access to out of bounds");
    }
    return this->data[i];
}

template<size_t N>
float MatrixRow<N>::operator*(MatrixCol<N>& other) {
    float scal_mul = 0;
    for (size_t i = 0; i < N; i++) {
        scal_mul += (*this)[i] * other[i];
    }
    return scal_mul;
}

template<size_t N>
MatrixCol<N>::MatrixCol(const MatrixCol& other) : Matrix<N, 1>::Matrix(other) {}

template<size_t N>
MatrixCol<N>::MatrixCol(const std::array<float, N> data) : Matrix<N, 1>::Matrix(data) {}

template<size_t N>
MatrixCol<N>::MatrixCol(const MatrixRow<N> &other) : Matrix<N, 1>::Matrix(other.data) {}

template<size_t N>
float& MatrixCol<N>::operator[](size_t i){
    if (i >= N) {
        throw std::runtime_error("Trying to access to out of bounds");
    }
    return this->data[i];
}
