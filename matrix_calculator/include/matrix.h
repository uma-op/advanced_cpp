#pragma once

#include <cstddef>
#include <array>
#include <vector>

template<size_t N>
class MatrixRow;

template<size_t N>
class MatrixCol;

template<size_t H, size_t W>
class Matrix {
 public:
    std::array<float, H * W> data;

    Matrix();
    Matrix(const Matrix &other);
    Matrix(std::initializer_list<float> data);
    Matrix(const std::array<float, H * W> data);
    Matrix(std::array<MatrixRow<W>, H> vs);
    Matrix(std::array<MatrixCol<H>, W> vs);

    void set(float val, size_t row, size_t col);

    float get(size_t row, size_t col) const;
    MatrixRow<W> get_row(size_t row) const;
    std::array<MatrixRow<W>, H> get_rows() const;
    MatrixCol<H> get_col(size_t col) const;
    std::array<MatrixCol<H>, W> get_cols() const;
    Matrix get_diag() const;
    Matrix get_sec_diag() const;

    Matrix eval_bin_op(const Matrix& other, float (*op)(float, float)) const;
    Matrix eval_bin_op(float k, float (*op)(float, float)) const;

    Matrix operator+(const Matrix &other) const;
    Matrix operator-(const Matrix &other) const;
    Matrix mul(const Matrix &other) const;
    bool operator==(const Matrix &other) const;

    template<size_t _W>
    Matrix<H, _W> operator*(const Matrix<W, _W> &other) const;

    Matrix operator+(float k) const;
    Matrix operator-(float k) const;
    Matrix operator*(float k) const;

    Matrix add_row(const MatrixRow<W>& row, size_t row_id);
    Matrix add_col(const MatrixCol<H>& col, size_t col_id);

    Matrix<W, H> transpose();
    Matrix<H - 1, W - 1> minor(size_t row_id, size_t col_id);
    Matrix inv();
    float det();

    std::vector<MatrixRow<W>> rows_slice(size_t begin);
    std::vector<MatrixRow<W>> rows_slice(size_t begin, size_t end);
    std::vector<MatrixRow<W>> rows_slice(size_t begin, size_t end, int step);

    std::vector<MatrixCol<H>> cols_slice(size_t begin);
    std::vector<MatrixCol<H>> cols_slice(size_t begin, size_t end);
    std::vector<MatrixCol<H>> cols_slice(size_t begin, size_t end, int step);
};

template<size_t W, size_t H>
Matrix<W, H> operator+(float k, const Matrix<W, H>& matrix);

template<size_t W, size_t H>
Matrix<W, H> operator-(float k, const Matrix<W, H>& matrix);

template<size_t W, size_t H>
Matrix<W, H> operator*(float k, const Matrix<W, H>& matrix);

template<size_t N>
class MatrixRow : public Matrix<1, N> {
 public:
    MatrixRow() = default;
    MatrixRow(const MatrixRow& other);
    MatrixRow(std::initializer_list<float> data);
    MatrixRow(const std::array<float, N> data);
    MatrixRow(const MatrixCol<N>& other);

    float& operator[](size_t i);

    float operator*(MatrixCol<N>& other);

    std::vector<float> slice(size_t begin);
    std::vector<float> slice(size_t begin, size_t end);
    std::vector<float> slice(size_t begin, size_t end, int step);
};

template<size_t N>
class MatrixCol : public Matrix<N, 1> {
 public:
    MatrixCol() = default;
    MatrixCol(const MatrixCol& other);
    MatrixCol(std::initializer_list<float> data);
    MatrixCol(const std::array<float, N> data);
    MatrixCol(const MatrixRow<N>& other);

    float& operator[](size_t i);

    std::vector<float> slice(size_t begin);
    std::vector<float> slice(size_t begin, size_t end);
    std::vector<float> slice(size_t begin, size_t end, int step);
};

#include "constructors.cpp"
#include "access.cpp"
#include "arithmetics.cpp"
#include "special.cpp"
#include "slice.cpp"
