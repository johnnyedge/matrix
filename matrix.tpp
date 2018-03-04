#pragma once

#include <type_traits>
#include <algorithm>
#include <sstream>
#include <stdexcept>

template <typename T>
matrix<T>::~matrix(void)
{
}

/*
 * default constructor just initialized to empty matrix
 */
template <typename T>
matrix<T>::matrix(void)
    : matrix(0, 0)
{
    static_assert(std::is_unsigned<size_type>::value,
                  "size_type must be an unsigned integral type");

    /*
     * in theory, the matrix could be composed of objects
     * that support arithmetic operators, but this hasn't
     * been tried/tested.
     */
    static_assert(std::is_integral<element_type>::value,
                  "matrix elements must be of integral type");
}

/*
 * create a constructor with specified dimensions
 */
template <typename T>
matrix<T>::matrix(const size_type rows, const size_type cols)
{
    /* if one dimension is non-zero, both have to be */
    if (rows != cols && (!rows || !cols)) {
        throw std::domain_error(
            "non-empty matrix must have non-zero number of rows and columns");
    }

    _elements = std::vector<std::vector<element_type>>(
        rows, std::vector<element_type>(cols, 0));

    _order = ROWS;
}

/* copy constructor */
template <typename T>
matrix<T>::matrix(const matrix & other)
    : _elements(other._elements), _order(other._order)
{
}

/*
 * move constructor depends on implementation
 * of std::vector move constructor
 */
template <typename T>
matrix<T>::matrix(matrix && other)
    : _elements(std::move(other._elements)), _order(other._order)
{
}

/* assignment operator */
template <typename T>
matrix<T> & matrix<T>::operator =(const matrix<T> & rhs)
{
    /* just copy everything over */
    _elements = rhs._elements;
    _order = rhs._order;

    return *this;
}

/* move-assignment */
template <typename T>
matrix<T> & matrix<T>::operator =(matrix<T> && rhs)
{
    /* rely on standard library to do the heavy lifting */
    _elements = std::move(rhs._elements);
    _order = rhs._order;

    return *this;
}

/* non-const, unchecked element access */
template <typename T>
T & matrix<T>::operator ()(const size_type row, const size_type col)
{
    /*
     * get a const reference to this. that will allow us
     * to call the const version of the () operator.
     */
    const matrix<element_type> & _this = *this;

    /* remove the const from the returned reference */
    return const_cast<element_type &>(_this(row, col));
}

/* const, unchecked element access */
template <typename T>
const T & matrix<T>::operator ()(size_type row, size_type col) const
{
    /*
     * if the vector is ordered by columns instead of
     * rows, accessing it properly is just a matter
     * of swapping the row and column indicies
     */
    if (_order == COLS) {
        std::swap(row, col);
    }

    return _elements[row][col];
}

/*
 * non-const, checked element access
 *
 * see also the unchecked version of this function for
 * an explanation of what the code is doing
 */
template <typename T>
T & matrix<T>::at(const size_type row, const size_type col)
{
    const matrix<element_type> & _this = *this;
    return const_cast<element_type &>(_this.at(row, col));
}

/*
 * const, checked element access
 *
 * see also the unchecked version of this function for
 * an explanation of what the code is doing
 */
template <typename T>
const T & matrix<T>::at(size_type row, size_type col) const
{
    if (_order == COLS) {
        std::swap(row, col);
    }

    return _elements.at(row).at(col);
}

/* transposition */
template <typename T>
matrix<T> matrix<T>::transpose(void) const
{
    /*
     * transposition can be achieved simply by changing
     * the way that the matrix is accessed. in transposition,
     * the rows simply become the columns.
     */
    matrix<element_type> m(*this);
    m._order = (m._order == ROWS) ? COLS : ROWS;
    return m;
}

/* multiplication of two matrices */
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

    /*
     * check that the matrices are dimensionally
     * compatible for multiplication
     */
    if (p != rhs.size().first) {
        std::stringstream ss;

        ss << "incompatible dimensions for matrix multiplication: ";
        ss << "(" << m << "x" << p << ")";
        ss << " vs. ";
        ss << "(" << rhs.size().first << "x" << n << ")";

        throw std::domain_error(ss.str());
    }

    /*
     * the algorithm for multiplying the matrices comes from here:
     * https://en.wikipedia.org/wiki/Matrix_multiplication_algorithm
     *
     * it is just the iterative algorithm without any attempt at
     * optimization or consideration of cache behavior.
     */
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

/*
 * matrix multiplication by a scalar
 *
 * it was more efficient to implement the actual multiplication
 * in the *= operator, so this function obtains the product by
 * using one of the c++ operators.
 *
 * this function isn't strictly necessary, but is provided for
 * consistency with the multiply function that operates on
 * two matrices
 */
template <typename T>
matrix<T> matrix<T>::multiply(const element_type & rhs) const
{
    return *this * rhs;
}

/* multiplication operator for two matrices */
template <typename T>
matrix<T> matrix<T>::operator *(const matrix<element_type> & rhs) const
{
    return multiply(rhs);
}

/* multiplication/assignment operator for two matrices */
template <typename T>
matrix<T> & matrix<T>::operator *=(const matrix<element_type> & rhs)
{
    *this = multiply(rhs);
    return *this;
}

/*
 * multiplication operator for a matrix and a scalar
 *
 * this function is implemented in terms of *= operator.
 */
template <typename T>
matrix<T> matrix<T>::operator *(const element_type & rhs) const
{
    matrix<element_type> m(*this);
    m *= rhs;
    return m;
}

/*
 * multiplication/assignment operator for a matrix and a scalar
 *
 * this operation is done in-place, unlike the
 * standard multiplication of two matrices
 */
template <typename T>
matrix<T> & matrix<T>::operator *=(const element_type & rhs)
{
    /*
     * just walk through each element and
     * multiply it by the scalar value
     */
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

/* matrix equality operator */
template <typename T>
bool matrix<T>::operator ==(const matrix<element_type> & rhs) const
{
    const std::function<bool(size_type, size_type, element_type)> each =
        [&rhs]
        (const size_type row,
         const size_type col,
         const element_type val)
        {
            /*
             * for each element provided, verify that
             * the corresponding element in rhs is equal
             */
            return val == rhs(row, col);
        };

    /*
     * verify that the two matrices are the same size
     * and if so, verify each value within
     */
    const std::pair<size_type, size_type> this_size = size();
    return this_size == rhs.size() && this_size == foreach(each);
}

/* matrix inequality operator */
template <typename T>
bool matrix<T>::operator !=(const matrix<element_type> & rhs) const
{
    /* simple negation of the equality operator */
    return !operator ==(rhs);
}

/* determine the size (number of rows and columns) */
template <typename T>
std::pair<typename matrix<T>::size_type, typename matrix<T>::size_type>
matrix<T>::size(void) const
{
    size_type rows, cols;

    /* get the number of rows */
    rows = _elements.size();
    /* there can only be columns if there are rows */
    cols = rows ? _elements[0].size() : 0;

    /*
     * if the matrix is ordered differently,
     * simply swap the rows and columns
     */
    if (_order == COLS) {
        std::swap(rows, cols);
    }

    return std::make_pair(rows, cols);
}

/* determine if the matrix contains any elements */
template <typename T>
bool matrix<T>::empty(void) const
{
    return _elements.empty();
}

/* remove all elements from the matrix */
template <typename T>
void matrix<T>::clear(void)
{
    _elements.clear();
}

/* visit each element in the matrix */
template <typename T>
void matrix<T>::foreach(const std::function<void(size_type, size_type,
                                                 element_type)> & each) const
{
    /*
     * have to specifically create a function variable with a `bool`
     * return type so that the compiler can unambiguously know to
     * call the private version of the function below.
     */
    const std::function<bool(size_type, size_type, element_type)> _each =
        [&each]
        (const size_type row,
         const size_type col,
         const element_type val)
        {
            each(row, col, val);
            return true;
        };

    /* call the private version */
    foreach(_each);
}

/* transform each element in the matrix */
template <typename T>
void matrix<T>::transform(
    const std::function<element_type(size_type, size_type,
                                     element_type)> & xfrm)
{
    /*
     * have to specifically create a function variable like in foreach
     */
    const std::function<void(size_type, size_type, element_type)> each =
        [this, &xfrm]
        (const size_type row,
         const size_type col,
         const element_type val)
        {
            if (xfrm != nullptr) {
                (*this)(row, col) = xfrm(row, col, val);
            }
        };

    /* call the private version */
    foreach(each);
}

/*
 * visit each element in the matrix with the ability to
 * stop processing before all elements have been visited
 */
template <typename T>
std::pair<typename matrix<T>::size_type,
          typename matrix<T>::size_type>
matrix<T>::foreach(const std::function<bool(size_type, size_type,
                                            element_type)> & each) const
{
    const std::pair<size_type, size_type> this_size = size();
    size_type i, j;

    for (i = 0; i < this_size.first; i++) {
        for (j = 0; j < this_size.second; j++) {
            /*
             * processing stops if the supplied
             * function returns false
             */
            if (each != nullptr &&
                !each(i, j, (*this)(i, j))) {
                break;
            }
        }

        /*
         * if the inner loop stopped early,
         * this one stops early, too
         */
        if (j != this_size.second) {
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
