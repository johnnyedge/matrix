#pragma once

#include <type_traits>
#include <utility>

template <typename T>
matrix<T>::~matrix(void)
{
}

template <typename T>
matrix<T>::matrix(void)
    : _elements(), _order(ROWS)
{
}

template <typename T>
matrix<T>::matrix(const size_type rows, const size_type cols)
{
    static_assert(std::is_integral<element_type>::value,
                  "matrix template parameter must be integral type");

    if (rows == 0 || cols == 0) {
        throw std::invalid_argument(
            "matrix must have at least 1 row and 1 column");
    }

    _elements = std::vector<std::vector<element_type>>(
        rows, std::vector<element_type>(cols, 0));
    _order = ROWS;
}

template <typename T>
matrix<T>::matrix(const matrix & other)
    : _elements(other._elements), _order(other._order)
{
}

template <typename T>
matrix<T>::matrix(const matrix && other)
    : _elements(std::move(other._elements)), _order(other._order)
{
}

template <typename T>
matrix<T> & matrix<T>::operator =(const matrix & rhs)
{
    _elements = rhs._elements;
    _order = rhs._order;
    return *this;
}

template <typename T>
matrix<T> & matrix<T>::operator =(matrix && rhs)
{
    _elements = std::move(rhs._elements);
    _order = rhs._order;
    return *this;
}

template <typename T>
T & matrix<T>::at(const size_type row, const size_type col)
{
    const matrix<element_type> & _this = *this;
    return const_cast<element_type &>(_this.at(row, col));
}

template <typename T>
const T & matrix<T>::at(size_type row, size_type col) const
{
    if (_order == COLS) {
        std::swap(row, col);
    }

    return _elements.at(row).at(col);
}

template <typename T>
matrix<T> matrix<T>::transpose(void) const
{
    matrix<element_type> m(*this);
    m._order = (m._order == ROWS) ? COLS : ROWS;
    return m;
}

template <typename T>
matrix<T> matrix<T>::multiply(const matrix<element_type> & rhs) const
{
    matrix<element_type> res(rows(), rhs.columns());

    if (columns() != rhs.rows()) {
        throw std::domain_error(
            "matrix multiplication column/row mismatch");
    }

    for (int i = 0; i < res.rows(); i++) {
        for (int j = 0; j < res.columns(); j++) {
            element_type & sum = res.at(i, j);

            sum = 0;
            for (int k = 0; k < columns(); k++) {
                sum += at(i, k) * rhs.at(k, j);
            }
        }
    }

    return res;
}

template <typename T>
matrix<T> matrix<T>::multiply(const element_type & rhs) const
{
    return *this * rhs;
}

template <typename T>
matrix<T> matrix<T>::operator *(const matrix<element_type> & rhs) const
{
    return multiply(rhs);
}

template <typename T>
matrix<T> & matrix<T>::operator *=(const matrix<element_type> & rhs)
{
    *this = multiply(rhs);
    return *this;
}

template <typename T>
matrix<T> matrix<T>::operator *(const element_type & rhs) const
{
    matrix<element_type> m(*this);
    m *= rhs;
    return m;
}

template <typename T>
matrix<T> & matrix<T>::operator *=(const element_type & rhs)
{
    for (int i = 0; i < rows(); i++) {
        for (int j = 0; j < columns(); j++) {
            at(i, j) *= rhs;
        }
    }

    return *this;
}

template <typename T>
typename matrix<T>::size_type matrix<T>::rows(void) const
{
    return (_order == ROWS) ? _elements.size() : _elements.at(0).size();
}

template <typename T>
typename matrix<T>::size_type matrix<T>::columns(void) const
{
    return (_order == COLS) ? _elements.size() : _elements.at(0).size();
}

/*
 * local variables:
 * mode: c++
 * end:
 */
