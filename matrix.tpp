#pragma once

#include <type_traits>
#include <algorithm>
#include <sstream>
#include <stdexcept>

template <typename T>
matrix<T>::~matrix(void)
{
}

template <typename T>
matrix<T>::matrix(void)
    : matrix(0, 0)
{
    static_assert(std::is_unsigned<size_type>::value,
                  "size_type must be an unsigned integral type");

    static_assert(std::is_integral<element_type>::value,
                  "matrix elements must be of integral type");
}

template <typename T>
matrix<T>::matrix(const size_type rows, const size_type cols)
{
    if (rows != cols && (!rows || !cols)) {
        throw std::domain_error(
            "non-empty matrix must have non-zero number of rows and columns");
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
matrix<T>::matrix(matrix && other)
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
     *   match the number of columns in rhs
     */
    const size_type m = size().first;
    const size_type n = rhs.size().second;
    const size_type p = size().second;

    matrix<element_type> res(m, n);

    if (p != rhs.size().first) {
        std::stringstream ss;

        ss << "incompatible dimensions for matrix multiplication: ";
        ss << "(" << m << "x" << p << ")";
        ss << " vs. ";
        ss << "(" << rhs.size().first << "x" << n << ")";

        throw std::domain_error(ss.str());
    }

    for (size_type i = 0; i < m; i++) {
        for (size_type j = 0; j < n; j++) {
            element_type & sum = res(i, j);

            sum = 0;
            for (size_type k = 0; k < p; k++) {
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
    transform(
        [&rhs]
        (const size_type /* ignored */,
         const size_type /* ignored */,
         const element_type val)
        {
            return val * rhs;
        });

    return *this;
}

template <typename T>
bool matrix<T>::operator ==(const matrix<element_type> & rhs) const
{
    const std::function<bool(size_type, size_type,
                             element_type)> each =
        [&rhs]
        (const size_type row,
         const size_type col,
         const element_type val)
        {
            return val == rhs(row, col);
        };

    const std::pair<size_type, size_type> this_size = size();

    return this_size == rhs.size() && this_size == foreach(each);
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
    cols = rows ? _elements[0].size() : 0;

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

template <typename T>
void matrix<T>::foreach(const std::function<void(size_type, size_type,
                                                 element_type)> & each) const
{
    const std::function<bool(size_type, size_type,
                             element_type)> _each =
        [&each]
        (const size_type row,
         const size_type col,
         const element_type val)
        {
            each(row, col, val);
            return true;
        };

    foreach(_each);
}

template <typename T>
void matrix<T>::transform(
    const std::function<element_type(size_type, size_type,
                                     element_type)> & xfrm)
{
    const std::function<void(size_type, size_type,
                             element_type)> each =
        [this, &xfrm]
        (const size_type row,
         const size_type col,
         const element_type val)
        {
            (*this)(row, col) = xfrm(row, col, val);
        };

    foreach(each);
}

template <typename T>
std::pair<typename matrix<T>::size_type,
          typename matrix<T>::size_type>
matrix<T>::foreach(const std::function<bool(size_type, size_type,
                                            element_type)> & each) const
{
    size_type i, j;

    for (i = 0; i < size().first; i++) {
        for (j = 0; j < size().second; j++) {
            if (each != nullptr &&
                !each(i, j, (*this)(i, j))) {
                break;
            }
        }

        if (j != size().second) {
            break;
        }
    }

    return std::make_pair(i, j);
}

/*
 * local variables:
 * mode: c++
 * end:
 */
