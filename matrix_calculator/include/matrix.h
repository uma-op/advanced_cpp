#pragma once

// #include <cassert>
#include <cstddef>
// #include <cstring>
#include <algorithm>
#include <array>

template<size_t W, size_t H>
class Matrix {
public:
    std::array<float, W * H> data;
    // Matrix() = default;
    Matrix(const Matrix &other) {
        std::copy(other.data.cbegin(), other.data.cend(), this->data.begin());
    }

    Matrix(const std::array<float, W* H> data) {
        std::copy(data.cbegin(), data.cend(), this->data.begin());
    }

    Matrix(Matrix<W, 1> *vs, size_t n) {
        // assert(n != 0);

        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < W; j++) {
                this->data[i * W + j] = vs[i].get(0, j);
            }
        }
    }

    Matrix(Matrix<1, H> *vs, size_t n) {
        // assert(n != 0);

        for (size_t j = 0; j < n; j++) {
            for (size_t i = 0; i < H; i++) {
                this->data[i * n + j] = vs[j].get(i, 0);
            }
        }
    }

    // ~Matrix() = default;
    
    // std::array<float, W>

    void set(float val, size_t row, size_t col) {
        // assert(row < H);
        // assert(col < W);
        this->data[row * W + col] = val;
    }

    float get(size_t row, size_t col) const {
        // assert(row < H);
        // assert(col < W);
        return this->data[row * W + col];
    }

    float* get_row(size_t row) {
        // assert(row < H);
        float *res = new float[W];

        for (size_t i = 0; i < W; i++) {
            res[i] = this->data[row * W + i];
        }

        return res;
    }

    float* get_col(size_t col) {
        // assert(col < W);

        float *res = new float[H];

        for (size_t i = 0; i < H; i++) {
            res[i] = this->data[i * W + col];
        }

        return res;
    }

    float* get_diag() {
        // assert(W == H);

        float *res = new float[W];
        
        for (size_t i = 0; i < W; i++) {
            res[i] = this->data[i * W + i];
        }

        return res;
    }

    float* get_sec_diag() {
        // assert(W == H);

        float *res = new float[W];
        for (size_t i = 0; i < W; i++) {
            res[i] = this->data[i * W + W - 1 - i];
        }

        return res;
    }

    Matrix eval_bin_op(const Matrix &other, float (*op)(float, float)) {
        Matrix *res = new Matrix<W, H>();

        for (size_t row = 0; row < H; row++) {
            for (size_t col = 0; col < W; col++) {
                res->set(op(this->get(row, col), other.get(row, col)), row, col);
            }
        }

        return *res;
    }

    Matrix operator+(const Matrix &other) {
        return this->eval_bin_op(other, [](float a, float b){return a + b;});
    }

    Matrix operator-(const Matrix &other) {
        return this->eval_bin_op(other, [](float a, float b){return a - b;});
    }

    Matrix operator*(const Matrix &other) {
        return this->eval_bin_op(other, [](float a, float b){return a * b;});
    }
};
