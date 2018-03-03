#include <gtest/gtest.h>

#include "matrix.h"

static const int TEST_CYCLES = 100;

TEST(matrix, basic)
{
    EXPECT_NO_THROW(matrix<int>(0, 0));

    for (int c = 0; c < TEST_CYCLES; c++) {
        const int rows = 1 + rand() % 100;
        const int cols = 1 + rand() % 100;

        matrix<int> m(rows, cols);

        EXPECT_THROW(matrix<int>(0, cols), std::logic_error);
        EXPECT_THROW(matrix<int>(rows, 0), std::logic_error);

        EXPECT_EQ(m.size().first, rows);
        EXPECT_EQ(m.size().second, cols);

        m.transform(
            [cols]
            (const std::size_t row,
             const std::size_t col,
             const int /* ignored */)
            {
                return row * cols + col;
            });

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                EXPECT_EQ(m.at(i, j), i * cols + j);
            }
        }

        EXPECT_THROW(m.at(rows, 0), std::out_of_range);
        EXPECT_THROW(m.at(0, cols), std::out_of_range);
        EXPECT_THROW(m.at(rows, cols), std::out_of_range);

        m.clear();

        EXPECT_TRUE(m.empty());

        EXPECT_EQ(m.size().first, 0);
        EXPECT_EQ(m.size().second, 0);
    }
}

TEST(matrix, equality)
{
    for (int c = 0; c < TEST_CYCLES; c++) {
        const int rows = 1 + rand() % 100;
        const int cols = 1 + rand() % 100;

        matrix<int> m(rows, cols);
        matrix<int> n(rows, cols);
        matrix<int> x(rows, cols);

        int v;

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

        n = m;

        m.foreach(
            [n]
            (const std::size_t row,
             const std::size_t col,
             const int val)
            {
                EXPECT_EQ(n(row, col), val);
            });

        EXPECT_EQ(m, n);
        EXPECT_NE(m, x);
        EXPECT_NE(n, x);

        n(rand() % rows, rand() % cols)++;
        EXPECT_NE(m, n);
    }
}

TEST(matrix, transpose)
{
    for (int c = 0; c < TEST_CYCLES; c++) {
        const int rows = 1 + rand() % 100;
        const int cols = 1 + rand() % 100;

        matrix<int> m(rows, cols);
        matrix<int> n;

        m.transform(
            []
            (const std::size_t /* ignored */,
             const std::size_t /* ignored */,
             const int /* ignored */)
            {
                return rand();
            });

        n = m.transpose();

        ASSERT_EQ(m.size().first, n.size().second);
        ASSERT_EQ(m.size().second, n.size().first);

        m.foreach(
            [n]
            (const std::size_t row,
             const std::size_t col,
             const int val)
            {
                EXPECT_EQ(n(col, row), val);
            });

        n = n.transpose();

        EXPECT_EQ(m, n);
    }
}

TEST(matrix, matrix_multiply)
{
    for (int c = 0; c < TEST_CYCLES; c++) {
        const int rows = 1 + rand() % 20;
        const int cols = 1 + rand() % 20;

        matrix<int> m(rows, cols);
        matrix<int> n, p, v;

        m.transform(
            [cols]
            (const std::size_t row,
             const std::size_t col,
             const int /* ignored */)
            {
                return row * cols + col;
            });

        if (rows != cols) {
            EXPECT_THROW(m * m, std::logic_error);
        } else {
            EXPECT_NO_THROW(m * m);
        }

        n = m.transpose();
        p = m * n;

        ASSERT_EQ(p.size().first, m.size().first);
        ASSERT_EQ(p.size().second, n.size().second);

        v = matrix<int>(p.size().first, p.size().second);

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

        EXPECT_EQ(p, v);
    }
}

TEST(matrix, scalar_multiply)
{
    for (int c = 0; c < TEST_CYCLES; c++) {
        const int rows = 1 + rand() % 100;
        const int cols = 1 + rand() % 100;

        matrix<int> m(rows, cols);
        matrix<int> n;
        int v;

        m.transform(
            [cols]
            (const std::size_t row,
             const std::size_t col,
             const int /* ignored */)
            {
                return row * cols + col;
            });

        v = rand() % 100;
        n = m * v;

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
