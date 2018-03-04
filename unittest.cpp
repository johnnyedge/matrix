#include <gtest/gtest.h>

#include "matrix.h"

static const int TEST_CYCLES = 100;

/*
 * some basic tests:
 * can the matrix be initialized?
 * can the values be read back after being set?
 * do bad initialization parameters cause an exception?
 */
TEST(matrix, basic)
{
    /* it's safe to create an empty matrix */
    EXPECT_NO_THROW(matrix<int>(0, 0));

    for (int c = 0; c < TEST_CYCLES; c++) {
        const int rows = 1 + rand() % 100;
        const int cols = 1 + rand() % 100;

        /* if the matrix has rows or columns, it has to have both */
        EXPECT_THROW(matrix<int>(0, cols), std::logic_error);
        EXPECT_THROW(matrix<int>(rows, 0), std::logic_error);

        matrix<int> m(rows, cols);

        /* test the size() and empty() functions */
        EXPECT_EQ(m.size().first, rows);
        EXPECT_EQ(m.size().second, cols);

        EXPECT_FALSE(m.empty());

        /* initialize m with sequential values */
        m.transform(
            [cols]
            (const std::size_t row,
             const std::size_t col,
             const int /* ignored */)
            {
                return row * cols + col;
            });

        /*
         * verify that the values written can be read back.
         * the manual loops here also verify that the matrix
         * object's internal foreach functionality works.
         */
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                EXPECT_EQ(m.at(i, j), i * cols + j);
            }
        }

        /* test accesses beyond the bounds of the matrix */
        EXPECT_THROW(m.at(rows, 0), std::out_of_range);
        EXPECT_THROW(m.at(0, cols), std::out_of_range);
        EXPECT_THROW(m.at(rows, cols), std::out_of_range);

        /*
         * erase all elements from the matrix and test
         * the empty and size functions
         */

        m.clear();

        EXPECT_TRUE(m.empty());

        EXPECT_EQ(m.size().first, 0);
        EXPECT_EQ(m.size().second, 0);
    }
}

/* test the equality and inequality operators */
TEST(matrix, equality)
{
    for (int c = 0; c < TEST_CYCLES; c++) {
        const int rows = 1 + rand() % 100;
        const int cols = 1 + rand() % 100;

        matrix<int> m(rows, cols);
        matrix<int> n(rows, cols);
        matrix<int> x(rows, cols);

        int v;

        /*
         * initialize m. initialize x to something *other*
         * than something equal to m
         */
        m.transform(
            [&x]
            (const std::size_t row,
             const std::size_t col,
             const int /* ignored */)
            {
                int v = rand();
                x(row, col) = v;
                return v + 1;
            });

        /*
         * assign m to n (tests the assignment operator
         */
        n = m;

        /*
         * manually verify that m and n are equal, using
         * the foreach function (give the foreach function
         * a bit of testing)
         */
        m.foreach(
            [n]
            (const std::size_t row,
             const std::size_t col,
             const int val)
            {
                EXPECT_EQ(n(row, col), val);
            });

        /*
         * now test the equality and inequality operators
         */
        EXPECT_EQ(m, n);
        EXPECT_NE(m, x);
        EXPECT_NE(n, x);

        /*
         * m and n are equal. now change one random
         * location in n, and make sure that the
         * (in)equality operator catches it
         */
        n(rand() % rows, rand() % cols)++;
        EXPECT_NE(m, n);
    }
}

/* test matrix transposition */
TEST(matrix, transpose)
{
    for (int c = 0; c < TEST_CYCLES; c++) {
        const int rows = 1 + rand() % 100;
        const int cols = 1 + rand() % 100;

        matrix<int> m(rows, cols);
        matrix<int> n;

        /* assign random values to m */
        m.transform(
            []
            (const std::size_t /* ignored */,
             const std::size_t /* ignored */,
             const int /* ignored */)
            {
                return rand();
            });

        /* transpose m, put result in n */
        n = m.transpose();

        /*
         * the number of rows of m should be equal to the
         * number of columns in n, and the number of columns
         * in m should be equivalent to the number of rows
         * in n
         */
        ASSERT_EQ(m.size().first, n.size().second);
        ASSERT_EQ(m.size().second, n.size().first);

        /*
         * test that the values were correctly transposed,
         * i.e. that they are in the correct locations
         */
        m.foreach(
            [n]
            (const std::size_t row,
             const std::size_t col,
             const int val)
            {
                EXPECT_EQ(n(col, row), val);
            });

        /*
         * transposing the matrix again should
         * yield the original matrix
         */
        n = n.transpose();

        EXPECT_EQ(m, n);
    }
}

/*
 * test multiplication of two matrices
 */
TEST(matrix, matrix_multiply)
{
    for (int c = 0; c < TEST_CYCLES; c++) {
        /*
         * limit matrix size sizes since multiplication
         * can be a lengthy operation
         */
        const int rows = 1 + rand() % 20;
        const int cols = 1 + rand() % 20;

        matrix<int> m(rows, cols);
        matrix<int> n, p, v;

        /* initialize m */
        m.transform(
            [cols]
            (const std::size_t row,
             const std::size_t col,
             const int /* ignored */)
            {
                return row * cols + col;
            });

        /*
         * test that the check for dimensionally compatible
         * matrices is functioning. if the matrix is square,
         * it is dimensionally compatible with itself.
         */
        if (rows != cols) {
            EXPECT_THROW(m * m, std::logic_error);
        } else {
            EXPECT_NO_THROW(m * m);
        }

        /*
         * the transposition of a matrix is dimensionally
         * compatible with the original matrix for multiplication
         */
        n = m.transpose();
        p = m * n;

        /*
         * check that the size of the product matrix is correct
         */
        ASSERT_EQ(p.size().first, m.size().first);
        ASSERT_EQ(p.size().second, n.size().second);

        /*
         * create a "verification" matrix with the same
         * dimensions as p, the product
         */
        v = matrix<int>(p.size().first, p.size().second);

        /*
         * use the foreach (more testing of that function) function
         * to iterate over the matrices and perform a manual
         * multiplication, storing the result in v.
         *
         * this algorithm is possibly the least efficient way to do
         * matrix multiplication (note that it visits combinations
         * of elements that do not require any computation), and
         * so follows a different algorithm (hopefully!) than the
         * one used by the matrix object. the theory here is that
         * if two different algorithms arrive at the same answer,
         * they're more likely to both be correct than both wrong in
         * the exact same way.
         */
        m.foreach(
            [&v, n, cols]
            (const std::size_t m_row,
             const std::size_t m_col,
             const int m_val)
            {
                n.foreach(
                    [&v, cols, m_row, m_col, m_val]
                    (const std::size_t n_row,
                     const std::size_t n_col,
                     const int n_val)
                    {
                        if (m_col == n_row) {
                            v.at(m_row, n_col) += m_val * n_val;
                        }
                    });
            });

        /* test for equality */
        EXPECT_EQ(p, v);
    }
}

/*
 * test that scalar multiplication works
 */
TEST(matrix, scalar_multiply)
{
    for (int c = 0; c < TEST_CYCLES; c++) {
        const int rows = 1 + rand() % 100;
        const int cols = 1 + rand() % 100;

        matrix<int> m(rows, cols);
        matrix<int> n;
        int v;

        /* initialize m */
        m.transform(
            [cols]
            (const std::size_t row,
             const std::size_t col,
             const int /* ignored */)
            {
                return row * cols + col;
            });

        /* choose a random scalar... */
        v = rand() % 100;
        /* ...and multiply m by it */
        n = m * v;

        /*
         * verify that each element is the product
         * of the original element and v
         */
        m.foreach(
            [n, v]
            (const std::size_t row,
             const std::size_t col,
             const int val)
            {
                EXPECT_EQ(n.at(row, col), v * val);
            });
    }
}

/*
 * according to https://en.wikipedia.org/wiki/Matrix_multiplication#Transpose,
 * the transpose of a product of matrices is the product, in the reverse order,
 * of the transposes of the factors. let's test it out.
 */
TEST(matrix, transpose_multiply)
{
    for (int c = 0; c < TEST_CYCLES; c++) {
        const int rows = 1 + rand() % 100;
        const int cols = 1 + rand() % 100;

        matrix<int> a(rows, cols), b(cols, rows);
        matrix<int> r1, r2;

        /*
         * since the number of rows in a is the same as the number
         * of columns in b, and vice versa, we can simultaneously
         * initialize a and b, by reversing the indicies.
         */
        a.transform(
            [&b]
            (const std::size_t row,
             const std::size_t col,
             const int /* ignored */)
            {
                b(col, row) = rand();
                return rand();
            });

        /* now do the transposes and multiplications */
        EXPECT_EQ((a * b).transpose(), b.transpose() * a.transpose());
    }
}
