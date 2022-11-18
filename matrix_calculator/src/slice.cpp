#pragma once

#include <stdexcept>

#include "matrix.h"

template<size_t H, size_t W>
std::vector<MatrixRow<W>> Matrix<H, W>::rows_slice(size_t begin) {
    return this->rows_slice(begin, H, 1);
}

template<size_t H, size_t W>
std::vector<MatrixRow<W>> Matrix<H, W>::rows_slice(size_t begin, size_t end) {
    return this->rows_slice(begin, end, 1);
}

template<size_t H, size_t W>
std::vector<MatrixRow<W>> Matrix<H, W>::rows_slice(size_t begin, size_t end, int step) {
    if (step == 0) {
        throw std::runtime_error("Lenght of step is zero");
    }

    std::vector<MatrixRow<W>> res;
    int sign = step > 0 ? 1 : -1;

    while (static_cast<int>((end - begin) * sign) > 0) {
        res.push_back(this->get_row(begin));
        begin += step;
    }

    return res;
}

template<size_t H, size_t W>
std::vector<MatrixCol<H>> Matrix<H, W>::cols_slice(size_t begin) {
    return this->cols_slice(begin, H, 1);
}

template<size_t H, size_t W>
std::vector<MatrixCol<H>> Matrix<H, W>::cols_slice(size_t begin, size_t end) {
    return this->cols_slice(begin, end, 1);
}

template<size_t H, size_t W>
std::vector<MatrixCol<H>> Matrix<H, W>::cols_slice(size_t begin, size_t end, int step) {
    if (step == 0) {
        throw std::runtime_error("Lenght of step is zero");
    }

    std::vector<MatrixCol<W>> res;
    int sign = step > 0 ? 1 : -1;

    while (static_cast<int>((end - begin) * sign) > 0) {
        res.push_back(this->get_col(begin));
        begin += step;
    }

    return res;
}

template<size_t N>
std::vector<float> MatrixRow<N>::slice(size_t begin) {
    return this->slice(begin, N, 1);
}

template<size_t N>
std::vector<float> MatrixRow<N>::slice(size_t begin, size_t end) {
    return this->slice(begin, end, 1);
}

template<size_t N>
std::vector<float> MatrixRow<N>::slice(size_t begin, size_t end, int step) {
    if (step == 0) {
        throw std::runtime_error("Lenght of step is zero");
    }

    std::vector<float> res;
    int sign = step > 0 ? 1 : -1;

    while (static_cast<int>((end - begin) * sign) > 0) {
        res.push_back((*this)[begin]);
        begin += step;
    }

    return res;
}

template<size_t N>
std::vector<float> MatrixCol<N>::slice(size_t begin) {
    return this->slice(begin, N, 1);
}

template<size_t N>
std::vector<float> MatrixCol<N>::slice(size_t begin, size_t end) {
    return this->slice(begin, end, 1);
}

template<size_t N>
std::vector<float> MatrixCol<N>::slice(size_t begin, size_t end, int step) {
    if (step == 0) {
        throw std::runtime_error("Lenght of step is zero");
    }

    std::vector<float> res;
    int sign = step > 0 ? 1 : -1;

    while (static_cast<int>((end - begin) * sign) > 0) {
        res.push_back((*this)[begin]);
        begin += step;
    }

    return res;
}
