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
T & matrix<T>::operator ()(const size_type row, const size_type col)
{
    const matrix<element_type> & _this = *this;
    return const_cast<element_type &>(_this(row, col));
}

template <typename T>
const T & matrix<T>::operator ()(size_type row, size_type col) const
{
    if (_order == COLS) {
        std::swap(row, col);
    }

    return _elements[row][col];
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
    /*
     * m is the number of rows in the result
     * n is the number of columns in the result
     * p is the number of rows in *this and should
     *   be the number of columns in rhs
     */
    const size_type m = size().first;
    const size_type n = rhs.size().second;
    const size_type p = size().second;

    matrix<element_type> res(m, n);

    if (p != rhs.size().first) {
        throw std::domain_error(
            "matrix multiplication column/row mismatch");
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            element_type & sum = res(i, j);

            sum = 0;
            for (int k = 0; k < p; k++) {
                sum += (*this)(i, k) * rhs(k, j);
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
    for (int i = 0; i < size().first; i++) {
        for (int j = 0; j < size().second; j++) {
            (*this)(i, j) *= rhs;
        }
    }

    return *this;
}

template <typename T>
bool matrix<T>::operator ==(const matrix<element_type> & rhs) const
{
    if (size() != rhs.size()) {
        return false;
    }

    for (int i = 0; i < size().first; i++) {
        for (int j = 0; j < size().second; j++) {
            if ((*this)(i, j) != rhs(i, j)) {
                return false;
            }
        }
    }

    return true;
}

template <typename T>
bool matrix<T>::operator !=(const matrix<element_type> & rhs) const
{
    return !operator ==(rhs);
}

template <typename T>
std::pair<typename matrix<T>::size_type, typename matrix<T>::size_type>
matrix<T>::size(void) const
{
    size_type rows, cols;

    rows = _elements.size();
    cols = rows ? _elements.at(0).size() : 0;

    if (_order == COLS) {
        std::swap(rows, cols);
    }

    return std::make_pair(rows, cols);
}

template <typename T>
bool matrix<T>::empty(void) const
{
    return _elements.empty();
}

template <typename T>
void matrix<T>::clear(void)
{
    _elements.clear();
}

/*
 * local variables:
 * mode: c++
 * end:
 */
