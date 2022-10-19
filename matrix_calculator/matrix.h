#pragma once

#include <cassert>
#include <cstddef>

template<size_t W, size_t H>
class Matrix {
private:
    float *data;
public:
    Matrix() {
        this->data = new float[W * H];
    }

    // Matrix(const Matrix &other);
    // Matrix(Matrix *other);
    // ~Matrix();

    void set(float val, size_t row, size_t col) {
        assert(row < H);
        assert(col < W);
        this->data[row * W + col] = val;
    }

    float get(size_t row, size_t col) const {
        assert(row < H);
        assert(col < W);
        return this->data[row * W + col];
    }

    float* get_row(size_t row) {
        assert(row < H);
        float *res = new float[W];

        for (size_t i = 0; i < W; i++) {
            res[i] = this->data[row * W + i];
        }

        return res;
    }

    float* get_col(size_t col) {
        assert(col < W);

        float *res = new float[H];

        for (size_t i = 0; i < H; i++) {
            res[i] = this->data[i * W + col];
        }

        return res;
    }

    // float* get_diag(size_t row, size_t col);

    Matrix& eval_bin_op(const Matrix &other, float (*op)(float, float)) {
        Matrix *res = new Matrix<W, H>();

        for (size_t row = 0; row < H; row++) {
            for (size_t col = 0; col < W; col++) {
                res->set(op(this->get(row, col), other.get(row, col)), row, col);
            }
        }

        return *res;
    }
};
// template<size_t W, size_t H>
// float *Matrix<W, H>::get_diag(size_t row, size_t col) {
//     assert(row < H);
//     assert(col < W);

//     float *res = new float[];
// }