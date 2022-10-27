#pragma once

#include <algorithm>
#include <stdexcept>

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
Matrix<H, W>::Matrix(std::initializer_list<float> data) : Matrix() {
    if (data.size() != 0 && data.size() != H * W) {
        throw std::runtime_error("Trying to implement with wrong amount of parameters");
    }

    std::copy(data.begin(), data.end(), this->data.begin());
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

template<size_t N>
MatrixRow<N>::MatrixRow(const MatrixRow& other) : Matrix<1, N>::Matrix(other) {}

template<size_t N>
MatrixRow<N>::MatrixRow(std::initializer_list<float> data) : Matrix<1, N>::Matrix(data) {}

template<size_t N>
MatrixRow<N>::MatrixRow(const std::array<float, N> data) : Matrix<1, N>::Matrix(data) {}

template<size_t N>
MatrixRow<N>::MatrixRow(const MatrixCol<N>& other) : Matrix<1, N>::Matrix(other.data) {}

template<size_t N>
MatrixCol<N>::MatrixCol(const MatrixCol& other) : Matrix<N, 1>::Matrix(other) {}

template<size_t N>
MatrixCol<N>::MatrixCol(std::initializer_list<float> data) : Matrix<N, 1>::Matrix(data) {}

template<size_t N>
MatrixCol<N>::MatrixCol(const std::array<float, N> data) : Matrix<N, 1>::Matrix(data) {}

template<size_t N>
MatrixCol<N>::MatrixCol(const MatrixRow<N> &other) : Matrix<N, 1>::Matrix(other.data) {}
