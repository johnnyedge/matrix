/*
 * #pragma once is non-standard, but it seems to be
 * supported by a wide variety of platforms and compilers
 * and doesn't require worrying about whether the chosen
 * "ifndef" include-guard conflicts with another
 */
#pragma once

#include <vector>
#include <functional>
#include <cstddef>
#include <utility>

#if !defined(__cplusplus)
#error "Unable to determine C++ version in use"
#elif __cplusplus < 201103L
#error "C++11 is required to build this library"
#elif __cplusplus > 201103L
#warning "This library has only been tested with C++11"
#else
/*
 * c++11 is in use, away we go!
 */
#endif

template <typename T>
class matrix
{
public:
    /*!
     * @brief The type of the elements contained in the matrix
     *
     * This type must be an integral type, e.g. char, int, long, etc.
     */
    typedef T element_type;

    /*!
     * @brief An unsigned type used to index elements in the matrix
     */
    typedef std::size_t size_type;

    virtual ~matrix(void);

    /*!
     * @brief Construct an empty matrix
     *
     * An empty matrix is represented as a 0x0 matrix, internally.
     */
    matrix(void);
    /*!
     * @brief Construct a MxN matrix
     *
     * @param[in] rows The number of rows in the matrix
     * @param[in] cols The number of columns in the matrix
     *
     * @note The number of rows and columns may be zero; but if
     * one of them is non-zero, they both must be non-zero.
     */
    matrix(size_type rows, size_type cols);

    /*!
     * @brief Construct a matrix as a copy of an existing matrix
     *
     * @param[in] other The matrix from which to create a copy
     */
    matrix(const matrix<element_type> & other);
    /*!
     * @brief Construct a matrix as a copy of an existing matrix
     *
     * @param[in,out] other The matrix from which to create a copy.
     *                      This object referenced by this parameter is
     *                      valid upon return from this function; however,
     *                      its contents are undefined.
     */
    matrix(matrix<element_type> && other);

    /*!
     * @brief Assign the value of a given matrix to `*this`
     *
     * This function enables the `x = y` syntax, where `x` is `*this`
     * and `y` is the `rhs` parameter.
     *
     * @param[in] rhs The "right-hand side" of the `x = y` operation.
     *
     * @return A reference to `*this`
     */
    matrix<element_type> & operator =(const matrix<element_type> & rhs);
    /*!
     * @brief Assign the value of a given matrix to `*this`
     *
     * @param[in,out] rhs The "right-hand side" of an `x = y` operation.
     *                    The object reference by this parameter is valid
     *                    upon return from this function; however, its
     *                    contents are undefined.
     *
     * @return A reference to `*this`
     */
    matrix<element_type> & operator =(matrix<element_type> && rhs);

    /*!
     * @brief Access an element at a specific row and column of the matrix
     *
     * No bounds checking is performed on the access. For bounds checking,
     * see the at() function.
     *
     * @param[in] row The row in which the desired element is located
     * @param[in] col The column in which the desired element is located
     *
     * @return A reference to the specified element
     */
    element_type & operator ()(size_type row, size_type col);
    /*!
     * @brief Access an element and a specific
     *        row and column of a `const` matrix
     *
     * @see operator()(size_type, size_type)
     */
    const element_type & operator ()(size_type row, size_type col) const;

    /*!
     * @brief Access an element at a specific row and column of the matrix
     *
     * If `row` or `col` is out of range, `std::out_of_range` is thrown.
     *
     * @param[in] row The row in which the desired element is located
     * @param[in] col The column in which the desired element is located
     *
     * @return A reference to the specified element
     */
    element_type & at(size_type row, size_type col);
    /*!
     * @brief Access an element and a specific
     *        row and column of a `const` matrix
     *
     * @see at(size_type, size_type)
     */
    const element_type & at(size_type row, size_type col) const;

    /*!
     * @brief Create a new matrix that is a result of "transposing" `*this`
     *
     * The current matrix is not modified by this operation, and the result
     * of the function is a new, independent matrix. This complexity of
     * this operation is constant.
     *
     * @return The transposition of the current matrix
     *
     * @note An explanation of the matrix transposition operation can be
     *       found at: https://en.wikipedia.org/wiki/Transpose
     */
    matrix<element_type> transpose(void) const;
    /*!
     * @brief Multiply the current matrix by the supplied matrix
     *
     * The two matricies involved must be compatible; that is, the
     * number rows in `*this` must be equal to the number of columns
     * in `rhs`. The resulting matrix will have a number of rows equal
     * to the number of rows in `*this` and a number of columns equal
     * to the number of columns in `rhs`.
     *
     * If the two matricies are not dimensionally compatible for
     * multiplication, std::domain_error will be thrown.
     *
     * The complexity of this operation is `m * n * p` where `m` is the
     * number of rows in `*this`, `n` is the number of columns in `rhs`,
     * and `p` is the number of columns in `*this` (which is also the
     * number of rows in `rhs`).
     *
     * @param[in] rhs The matrix by which to multiply `*this`
     *
     * @return The product of the two matricies
     *
     * @note An explanation of the matrix multiplication operation can be
     *       found at: https://en.wikipedia.org/wiki/Matrix_multiplication
     */
    matrix<element_type> multiply(const matrix<element_type> & rhs) const;
    /*!
     * @brief Multiply the current matrix by a scalar value
     *
     * The complexity of this operation is `m * n` where `m` is the number
     * of rows in `*this`, and `n` is the number of columns in `*this`.
     *
     * @note An explanation of the matrix multiplication operation with a
     *       scalar can be found at:
     *       https://en.wikipedia.org/wiki/Matrix_multiplication
     */
    matrix<element_type> multiply(const element_type & rhs) const;

    /*!
     * @brief Multiply two matricies
     *
     * This function enables matrix multiplication via the `x * y` syntax
     * where `x` and `y` are compatible matricies.
     *
     * @see multiply(const matrix<element_type> &) const
     */
    matrix<element_type> operator *(const matrix<element_type> & rhs) const;
    /*!
     * @brief Multiply two matricies, storing the result in `*this`
     *
     * This function enables matrix multiplication via the `x *= y` syntax
     * where `x` and `y` are compatible matricies.
     *
     * @return A reference to `*this`
     *
     * @see multiply(const matrix<element_type> &) const
     */
    matrix<element_type> & operator *=(const matrix<element_type> & rhs);

    /*!
     * @brief Multiply a matrix by a given scalar value
     *
     * This function enables matrix multiplication via the `x * y` syntax
     * where `x` is a matrix and `y` is a scalar of type element_type
     *
     * @see multiply(const element_type &) const
     */
    matrix<element_type> operator *(const element_type & rhs) const;
    /*!
     * @brief Multiply a matrix by a given scalar value
     *
     * This function enables matrix multiplication via the `x * y` syntax
     * where `x` is a matrix and `y` is a scalar of type element_type
     *
     * @see multiply(const element_type &) const
     */
    matrix<element_type> & operator *=(const element_type & rhs);

    /*!
     * @brief Compare two matricies for equality
     *
     * This function enables the `x == y` syntax where both `x`
     * and `y` are matricies.
     *
     * Two matricies compare as equal when they are dimensionally equivalent,
     * and all elements in each of the corresponding locations are equal.
     *
     * @retval true The matricies are equivalent
     * @retval false The matricies are not equivalent
     */
    bool operator ==(const matrix<element_type> & rhs) const;
    /*!
     * @brief Compare two matricies for inequality
     *
     * @see operator ==(const matrix<element_type> &) const
     */
    bool operator !=(const matrix<element_type> & rhs) const;

    /*!
     * @brief Get the number of rows and columns in a matrix
     *
     * @return A pair, the first element of which is the number of
     *         rows in the matrix and the second of which is the
     *         number of columns
     */
    std::pair<size_type, size_type> size(void) const;
    /*!
     * @brief Determine a matrix is empty
     *
     * A matrix is empty when it contains no rows and no columns.
     *
     * @retval true The matrix is empty
     * @retval false The matrix is not empty
     */
    bool empty(void) const;

    /*!
     * @brief Remove all elements from a matrix
     *
     * After completion of this operation the empty() function
     * will evaluate to `true`
     */
    void clear(void);

    /*!
     * @brief Call a supplied function for each element in a matrix
     *
     * @param[in] each The function, lambda, etc. to call for each
     *                 element in the matrix. The function is supplied
     *                 with the row, column, and value for each element.
     */
    void foreach(const std::function<void(size_type, size_type,
                                          element_type)> & each) const;
    /*!
     * @brief Call a supplied function for each element in a matrix,
     *        storing the result back into that element
     *
     * @param[in] xfrm The function, lambda, etc. to call for each
     *                 element in the matrix. The result of each call
     *                 will be stored into the matrix at the location
     *                 indicated by the parameters to the function
     *
     * @see foreach(const std::function<void(size_type, size_type,
     *                                       element_type)> &) const
     */
    void transform(const std::function<element_type(size_type, size_type,
                                                    element_type)> & xfrm);

private:
    /*!
     * @brief Enumeration denoting whether the matrix is
     *        stored by rows or by columns
     */
    typedef enum
    {
        ROWS,
        COLS,
    } order_type;

    /*!
     * @brief Internal representation of the matrix
     *
     * The matrix is represented internally as a two-dimensional array
     * using a `std::vector` of `std::vector`'s. The "inner" vector, the
     * one actually containing elements, can represent either rows or
     * columns, depending on the specified order. The benefit of this
     * is that the matrix can be transposed simply by changing the order.
     *
     * @see transpose()
     */
    std::vector<std::vector<element_type>> _elements;
    /*!
     * @brief Variable denoting the order in which matrix elements are stored
     */
    order_type _order;

    /*!
     * @brief Call a function for each element in matrix with the ability
     *        to stop the processing before all elements have been visited
     *
     * @param[in] each The function, lambda, etc. to call for each
     *                 element in the matrix. The function is supplied
     *                 with the row, column, and value for each element.
     *                 The function returns `true` to continue to the next
     *                 element and `false` to stop processing.
     *
     * @return A pair denoting the indicies of the last element
     *         visited/processed when the called function returned `false`.
     *         If the called function did not return `false`, then the
     *         returned value will be equivalent to the result of the size()
     *         member function.
     */
    std::pair<size_type, size_type> foreach(
        const std::function<bool(size_type, size_type,
                                 element_type)> & each) const;
};

#include "matrix.tpp"

/*
 * local variables:
 * mode: c++
 * end:
 */
