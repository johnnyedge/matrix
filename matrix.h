/*
 * #pragma once is non-standard, but it seems to be
 * supported by a wide variety of platforms and compilers
 * and doesn't require worrying about whether the chosen
 * "ifndef" include-guard conflicts with another
 */
#pragma once

template <typename T>
class matrix
{
public:
    typename T element_type;
    typename std::size_t size_type;

    virtual ~matrix(void);

    matrix(size_type rows, size_type cols);

    matrix(const matrix<element_type> & other);
    matrix(const matrix<element_type> && other);

    matrix<element_type> & operator =(const matrix<element_type> & rhs);
    matrix<element_type> & operator =(const matrix<element_type> && rhs);

    matrix<element_type> transpose(void) const;
    matrix<element_type> multiply(const matrix<element_type> & rhs) const;

    matrix<element_type> operator *(const matrix<element_type> & rhs) const;

    matrix(void) = delete;

private:
    typedef enum
    {
        ROWS,
        COLS,
    } order_type;

    std::vector<std::vector<element_type>> elements;
    order_type order;
};

#include "matrix.tpp"

/*
 * local variables:
 * mode: c++
 * end:
 */
