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

    Matrix() {
        if (W == 0 || H == 0) throw std::runtime_error("Trying to define empty matrix");
    }

    Matrix(const Matrix &other) : Matrix() {
        std::copy(other.data.cbegin(), other.data.cend(), this->data.begin());
    }

    Matrix(const std::array<float, H * W> data) : Matrix() {
        std::copy(data.cbegin(), data.cend(), this->data.begin());
    }

    Matrix(MatrixRow<W> *vs, size_t n) : Matrix() {
        if (n == 0) throw std::runtime_error("There is no vectors!!!");

        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < W; j++) {
                this->data[i * W + j] = vs[i].get(0, j);
            }
        }
    }

    Matrix(MatrixCol<H> *vs, size_t n) : Matrix() {
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

    MatrixRow<W> get_row(size_t row) const {
        if (row >= H) throw std::runtime_error("Trying to get row from out of bounds");

        std::array<float, W> res;

        for (size_t i = 0; i < W; i++) {
            res[i] = this->data[row * W + i];
        }

        return res;
    }

    MatrixRow<W>* get_rows() const {
        MatrixRow<W> *res = new MatrixRow<W>[H];
        for (size_t i = 0; i < H; i++) {
            res[i] = this->get_row(i);
        }
        return res;
    }

    MatrixCol<H> get_col(size_t col) const {
        if (col >= W) throw std::runtime_error("Trying to get column from out of bounds");

        std::array<float, H> res;

        for (size_t i = 0; i < H; i++) {
            res[i] = this->data[i * W + col];
        }

        return res;
    }

    MatrixCol<H>* get_cols() const {
        MatrixCol<H> *res = new MatrixCol<H>[W];
        for (size_t i = 0; i < W; i++) {
            res[i] = this->get_col(i);
        }
        return res;
    }

    Matrix get_diag() const {
        if (W != H) throw std::runtime_error("Trying to get diagonal of non square matrix");

        Matrix res;

        for (size_t i = 0; i < W; i++) {
            for (size_t j = 0; j < H; j++) {
                if (i == j) res.set(this->get(i, j), i, j);
                else
                    res.set(0, i, j);
            }
        }

        return res;
    }

    Matrix get_sec_diag() const {
        if (W != H) throw std::runtime_error("Trying to get secondary diagonal of non square matrix");

        Matrix res;

        for (size_t i = 0; i < W; i++) {
            for (size_t j = 0; j < H; j++) {
                if (i + j == W - 1) res.set(this->get(i, j), i, j);
                else
                    res.set(0, i, j);
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
    Matrix<H, _W> operator*(const Matrix<W, _W> &other) const {
        Matrix<H, _W> res;

        MatrixRow<W> *self_rows = this->get_rows();
        MatrixCol<W> *other_cols = other.get_cols();

        for (size_t i = 0; i < H; i++) {
            for (size_t j = 0; j < _W; j++) {
                res.set(self_rows[i] * other_cols[j], i, j);
            }
        }

        return res;
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

    Matrix<W, H> transpose() {
        MatrixRow<W> *rows = this->get_rows();
        MatrixCol<W> *cols = new MatrixCol<W>[H];

        for (size_t i = 0; i < H; i++) {
            cols[i] = rows[i];
        }

        return Matrix<W, H>(cols, H);
    }

    Matrix<H - 1, W - 1> minor(size_t row_id, size_t col_id) {
        Matrix<H - 1, W - 1> m;
        for (size_t i = 0; i < H - 1; i++) {
            for (size_t j = 0; j < W - 1; j++) {
                if (i >= row_id && j >= col_id) m.set(this->get(i + 1, j + 1), i, j);
                else if (i >= row_id) m.set(this->get(i + 1, j), i, j);
                else if (j >= col_id) m.set(this->get(i, j + 1), i, j);
                else
                    m.set(this->get(i, j), i, j);
            }
        }
        return m;
    }

    Matrix inv() {
        if (W != H) throw std::runtime_error("Trying to get inverse of non square matrix");

        float d = this->det();
        Matrix res;

        for (size_t i = 0; i < H; i++) {
            for (size_t j = 0; j < W; j++) {
                if ((i + j) % 2 == 0) res.set(this->minor(i, j).det() / d, j, i);
                else
                    res.set(-1 * this->minor(i, j).det() / d, j, i);
            }
        }

        return res;
    }

    float det() {
        if (W != H) throw std::runtime_error("Trying to get determinant of non square matrix");

        int parity = 1;
        auto next_permut = [&](size_t *permut, int n){
            size_t i, swap;
            for (i = 0; i < n; i++) {
                if (i == n - 1) return false;

                if (permut[i] < permut[i + 1]) {
                    int t;

                    for (size_t j = 0; j * 2 < i; j++) {
                        t = permut[j];
                        permut[j] = permut[i - j];
                        permut[i - j] = t;
                        parity *= -1;
                    }

                    for (swap = 0; permut[swap] < permut[i + 1]; swap++) {}

                    t = permut[i + 1];
                    permut[i + 1] = permut[swap - 1];
                    permut[swap - 1] = t;
                    parity *= -1;

                    break;
                }
            }
            return true;
        };

        size_t *p = new size_t[W];
        for (size_t i = 0; i < W; i++) { p[i] = i; }

        float mul = 1;
        float res = 0;

        do {
            mul = 1;
            for (size_t i = 0; i < W; i++) {
                mul *= this->get(i, p[i]);
            }
            res += parity * mul;
        } while (next_permut(p, W));

        return res;
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
    MatrixRow() = default;
    MatrixRow(const MatrixRow& other) : Matrix<1, N>::Matrix(other) {}
    MatrixRow(const std::array<float, N> data) : Matrix<1, N>::Matrix(data) {}
    MatrixRow(const MatrixCol<N>& other) : Matrix<1, N>::Matrix(other.data) {}

    float& operator[](size_t i) {
        if (i >= N) throw std::runtime_error("Trying to access to out of bounds");
        return this->data[i];
    }

    float operator*(MatrixCol<N>& other) {
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
    MatrixCol() = default;
    MatrixCol(const MatrixCol& other) : Matrix<N, 1>::Matrix(other) {}
    MatrixCol(const std::array<float, N> data) : Matrix<N, 1>::Matrix(data) {}
    MatrixCol(const MatrixRow<N>& other) : Matrix<N, 1>::Matrix(other.data) {}

    float& operator[](size_t i){
        if (i >= N) throw std::runtime_error("Trying to access to out of bounds");
        return this->data[i];
    }
};
