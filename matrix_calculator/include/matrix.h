#pragma once

#include <cstddef>
#include <algorithm>
#include <array>

template<size_t W, size_t H>
class Matrix {
 public:
    std::array<float, W * H> data;

    Matrix() = default;

    Matrix(const Matrix &other) {
        std::copy(other.data.cbegin(), other.data.cend(), this->data.begin());
    }

    Matrix(const std::array<float, W * H> data) {
        std::copy(data.cbegin(), data.cend(), this->data.begin());
    }

    Matrix(Matrix<W, 1> *vs, size_t n) {
        if (n == 0) throw std::runtime_error("There is no vectors!!!");

        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < W; j++) {
                this->data[i * W + j] = vs[i].get(0, j);
            }
        }
    }

    Matrix(Matrix<1, H> *vs, size_t n) {
        if (n == 0)
            throw std::runtime_error("There is no vectors!!!");

        for (size_t j = 0; j < n; j++) {
            for (size_t i = 0; i < H; i++) {
                this->data[i * n + j] = vs[j].get(i, 0);
            }
        }
    }

    void set(float val, size_t row, size_t col) {
        if (row >= H || col >= W)
            throw std::runtime_error("Trying to set to out of bounds");
        this->data[row * W + col] = val;
    }

    float get(size_t row, size_t col) const {
        if (row >= H || col >= W)
            throw std::runtime_error("Trying to get from out of bounds");
        return this->data[row * W + col];
    }

    float* get_row(size_t row) {
        if (row >= H)
            throw std::runtime_error("Trying to get row from out of bounds");
        float *res = new float[W];

        for (size_t i = 0; i < W; i++) {
            res[i] = this->data[row * W + i];
        }

        return res;
    }

    float* get_col(size_t col) {
        if (col >= W)
            throw std::runtime_error("Trying to get column from out of bounds");
        float *res = new float[H];

        for (size_t i = 0; i < H; i++) {
            res[i] = this->data[i * W + col];
        }

        return res;
    }

    float* get_diag() {
        if (W != H)
            throw std::runtime_error("Trying to get diagonal of non square matrix");

        float *res = new float[W];
        for (size_t i = 0; i < W; i++) {
            res[i] = this->data[i * W + i];
        }

        return res;
    }

    float* get_sec_diag() {
        if (W != H)
            throw std::runtime_error("Trying to get secondary diagonal of non square matrix");

        float *res = new float[W];
        for (size_t i = 0; i < W; i++) {
            res[i] = this->data[i * W + W - 1 - i];
        }

        return res;
    }

    Matrix eval_bin_op(const Matrix& other, float (*op)(float, float)) const {
        Matrix *res = new Matrix<W, H>;

        for (size_t row = 0; row < H; row++) {
            for (size_t col = 0; col < W; col++) {
                res->set(
                    op(
                        this->get(row, col),
                        other.get(row, col)),
                    row, col);
            }
        }

        return *res;
    }

    Matrix eval_bin_op(float k, float (*op)(float, float)) const {
        Matrix *res = new Matrix<W, H>;

        for (size_t row = 0; row < H; row++) {
            for (size_t col = 0; col < W; col++) {
                res->set(op(k, this->get(row, col)), row, col);
            }
        }

        return *res;
    }

    Matrix operator+(const Matrix &other) const {
        return this->eval_bin_op(other, [](float a, float b) {
            return a + b;
        });
    }

    Matrix operator-(const Matrix &other) const {
        return this->eval_bin_op(other, [](float a, float b) {
            return a - b;
        });
    }

    Matrix operator*(const Matrix &other) const {
        return this->eval_bin_op(other, [](float a, float b) {
            return a * b;
        });
    }

    Matrix operator*(float k) const {
        return this->eval_bin_op(k, [](float a, float b) {
            return a * b;
        });
    }
};

template<size_t W, size_t H>
Matrix<W, H> operator*(float k, const Matrix<W, H>& matrix) {
    return matrix * k;
}
