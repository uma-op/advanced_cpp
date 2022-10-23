#pragma once

#include <cstddef>
#include <algorithm>
#include <array>

template<size_t H, size_t W>
class Matrix;

template<size_t N>
class MatrixRow;

template<size_t N>
class MatrixCol;

template<size_t H, size_t W>
class Matrix {
 public:
    std::array<float, H * W> data;

    Matrix() = default;

    Matrix(const Matrix &other) {
        std::copy(other.data.cbegin(), other.data.cend(), this->data.begin());
    }

    Matrix(const std::array<float, H * W> data) {
        std::copy(data.cbegin(), data.cend(), this->data.begin());
    }

    Matrix(MatrixRow<W> *vs, size_t n) {
        if (n == 0) throw std::runtime_error("There is no vectors!!!");

        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < W; j++) {
                this->data[i * W + j] = vs[i].get(0, j);
            }
        }
    }

    Matrix(MatrixCol<H> *vs, size_t n) {
        if (n == 0) throw std::runtime_error("There is no vectors!!!");

        for (size_t j = 0; j < n; j++) {
            for (size_t i = 0; i < H; i++) {
                this->data[i * n + j] = vs[j].get(i, 0);
            }
        }
    }

    void set(float val, size_t row, size_t col) {
        if (row >= H || col >= W) throw std::runtime_error("Trying to set to out of bounds");
        this->data[row * W + col] = val;
    }

    float get(size_t row, size_t col) const {
        if (row >= H || col >= W) throw std::runtime_error("Trying to get from out of bounds");
        return this->data[row * W + col];
    }

    MatrixRow<W> get_row(size_t row) {
        if (row >= H) throw std::runtime_error("Trying to get row from out of bounds");

        std::array<float, W> *res;

        for (size_t i = 0; i < W; i++) {
            res[i] = this->data[row * W + i];
        }

        return res;
    }

    MatrixRow<W>* get_rows() {
        MatrixRow<W> *res = new MatrixRow<W>[H];
        for (size_t i = 0; i < H; i++) {
            res[i] = this->get_row(i);
        }
        return res;
    }

    MatrixCol<H> get_col(size_t col) {
        if (col >= W) throw std::runtime_error("Trying to get column from out of bounds");

        std::array<float, H> *res;

        for (size_t i = 0; i < H; i++) {
            res[i] = this->data[i * W + col];
        }

        return res;
    }

    MatrixCol<H>* get_cols() {
        MatrixCol<H> *res = new MatrixCol<H>[W];
        for (size_t i = 0; i < W; i++) {
            res[i] = this->get_col(i);
        }
        return res;
    }

    Matrix get_diag() {
        if (W != H) throw std::runtime_error("Trying to get diagonal of non square matrix");

        Matrix res;

        for (size_t i = 0; i < W; i++) {
            for (size_t j = 0; j < H; j++) {
                if (i == j) res.set(this->get(i, j), i, j);
                else res.set(0, i, j);
            }
        }

        return res;
    }

    Matrix get_sec_diag() {
        if (W != H) throw std::runtime_error("Trying to get secondary diagonal of non square matrix");

        Matrix res;

        for (size_t i = 0; i < W; i++) {
            for (size_t j = 0; j < H; j++) {
                if (i + j == W - 1) res.set(this->get(i, j), i, j);
                else res.set(0, i, j);
            }
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
        return this->eval_bin_op(other, [](float a, float b) { return a + b; });
    }

    Matrix operator-(const Matrix &other) const {
        return this->eval_bin_op(other, [](float a, float b) { return a - b; });
    }

    Matrix mul(const Matrix &other) const {
        return this->eval_bin_op(other, [](float a, float b) { return a * b; });
    }

    template<size_t _W>
    Matrix operator*(const Matrix<W, _W> &other) const {
        Matrix<H, _W> *res;

        MatrixRow<W> *self_rows = this->get_rows();
        MatrixCol<W> *other_cols = other.get.cols();

        for (size_t i = 0; i < H; i++) {
            for (size_t j = 0; j < _W; j++) {
                res.set(self_rows[i] * other_cols[j], i, j);
            }
        }

        return *res;
    }

    Matrix operator+(float k) const {
        return this->eval_bin_op(k, [](float a, float b) { return a + b; });
    }

    Matrix operator-(float k) const {
        return this->eval_bin_op(k, [](float a, float b) { return a - b; });
    }

    Matrix operator*(float k) const {
        return this->eval_bin_op(k, [](float a, float b) { return a * b; });
    }

    Matrix add_row(const MatrixRow<W>& row, size_t row_id) {
        if (row_id > H) throw std::runtime_error("Trying to add row to out of bounds");
        MatrixRow<W> *rows = this->get_rows();
        rows[row_id] = rows[row_id] + row;
        return Matrix(rows, H);
    }

    Matrix add_col(const MatrixCol<H>& col, size_t col_id) {
        if (col_id > W) throw std::runtime_error("Trying to add column to out of bounds");
        MatrixCol<H> *cols = this->get_cols();
        cols[col_id] = cols[col_id] + col;
        return Matrix(cols, W);
    }
};

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
class MatrixRow : public Matrix<1, N> {
 public:
    MatrixRow(const MatrixRow& other) : Matrix<1, N>::Matrix(other) {}
    MatrixRow(const std::array<float, N> data) : Matrix<1, N>::Matrix(data) {}
    MatrixRow(const MatrixCol<N>& other) : Matrix<1, N>::Matrix(new MatrixCol<N>[1]{other}) {}

    float& operator[](size_t i) {
        if (i >= N) throw std::runtime_error("Trying to access to out of bounds");
        return this->data[i];
    }

    float operator*(const MatrixCol<N>& other) {
        float scal_mul = 0;
        for (size_t i = 0; i < N; i++) {
            scal_mul += (*this)[i] * other[i];
        }
        return scal_mul;
    }
};

template<size_t N>
class MatrixCol : public Matrix<N, 1> {
 public:
    MatrixCol(const MatrixCol& other) : Matrix<N, 1>::Matrix(other) {}
    MatrixCol(const std::array<float, N> data) : Matrix<N, 1>::Matrix(data) {}
    MatrixCol(const MatrixRow<N>& other) : Matrix<N, 1>::Matrix(new MatrixRow<N>[1]{other}) {}

    float& operator[](size_t i){
        if (i >= N) throw std::runtime_error("Trying to access to out of bounds");
        return this->data[i];
    }
};
