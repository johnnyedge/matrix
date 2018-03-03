/*
 * #pragma once is non-standard, but it seems to be
 * supported by a wide variety of platforms and compilers
 * and doesn't require worrying about whether the chosen
 * "ifndef" include-guard conflicts with another
 */
#pragma once

#include <functional>
#include <cstddef>

template <typename T>
class matrix
{
public:
    typedef T element_type;
    typedef std::size_t size_type;

    virtual ~matrix(void);

    matrix(void);
    matrix(size_type rows, size_type cols);

    matrix(const matrix<element_type> & other);
    matrix(const matrix<element_type> && other);

    matrix<element_type> & operator =(const matrix<element_type> & rhs);
    matrix<element_type> & operator =(matrix<element_type> && rhs);

    element_type & operator ()(size_type row, size_type col);
    const element_type & operator ()(size_type row, size_type col) const;

    element_type & at(size_type row, size_type col);
    const element_type & at(size_type row, size_type col) const;

    matrix<element_type> transpose(void) const;
    matrix<element_type> multiply(const matrix<element_type> & rhs) const;
    matrix<element_type> multiply(const element_type & rhs) const;

    matrix<element_type> operator *(const matrix<element_type> & rhs) const;
    matrix<element_type> & operator *=(const matrix<element_type> & rhs);

    matrix<element_type> operator *(const element_type & rhs) const;
    matrix<element_type> & operator *=(const element_type & rhs);

    bool operator ==(const matrix<element_type> & rhs) const;
    bool operator !=(const matrix<element_type> & rhs) const;

    std::pair<size_type, size_type> size(void) const;
    bool empty(void) const;

    void clear(void);

    void foreach(const std::function<void(size_type, size_type,
                                          element_type)> & each) const;
    void transform(const std::function<element_type(size_type, size_type,
                                                    element_type)> & xfrm);

private:
    typedef enum
    {
        ROWS,
        COLS,
    } order_type;

    std::vector<std::vector<element_type>> _elements;
    order_type _order;

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
