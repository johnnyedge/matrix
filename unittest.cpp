#include <gtest/gtest.h>

#include "matrix.h"

TEST(matrix, basic)
{
    EXPECT_THROW(matrix<int>(0, 0), std::logic_error);
    EXPECT_THROW(matrix<int>(0, 1), std::logic_error);
    EXPECT_THROW(matrix<int>(1, 0), std::logic_error);

    for (int c = 0; c < 100; c++) {
        const int rows = 1 + rand() % 100;
        const int cols = 1 + rand() % 100;

        matrix<int> m(rows, cols);

        EXPECT_EQ(m.size().first, rows);
        EXPECT_EQ(m.size().second, cols);

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                m.at(i, j) = i * cols + j;
            }
        }

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
    const int rows = 1 + rand() % 100;
    const int cols = 1 + rand() % 100;

    matrix<int> m(rows, cols);
    matrix<int> n(rows, cols);
    matrix<int> x(rows, cols);

    int v;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            m.at(i, j) = n(i, j) = rand();
        }
    }

    EXPECT_EQ(m, n);
    EXPECT_NE(m, x);
    EXPECT_NE(n, x);

    n(rand() % rows, rand() % cols)++;
    EXPECT_NE(m, n);
}

TEST(matrix, transpose)
{
    matrix<int> m(4, 3);
    matrix<int> t;

    EXPECT_EQ(m.size().first, 4);
    EXPECT_EQ(m.size().second, 3);

    for (int i = 0; i < m.size().first; i++) {
        for (int j = 0; j < m.size().second; j++) {
            m.at(i, j) = i * m.size().second + j;
        }
    }

    t = m.transpose();

    EXPECT_EQ(m.size().first, t.size().second);
    EXPECT_EQ(m.size().second, t.size().first);

    for (int i = 0; i < m.size().first; i++) {
        for (int j = 0; j < m.size().second; j++) {
            EXPECT_EQ(m.at(i, j), t.at(j, i));
        }
    }

    t = t.transpose();

    EXPECT_EQ(m, t);
}

TEST(matrix, matrix_multiply)
{
    matrix<int> m(4, 3);
    matrix<int> n, p;

    for (int i = 0; i < m.size().first; i++) {
        for (int j = 0; j < m.size().second; j++) {
            m.at(i, j) = i * m.size().second + j;
        }
    }

    EXPECT_THROW(m * m, std::logic_error);
    n = m.transpose();

    p = m * n;

    EXPECT_EQ(p.size().first, m.size().first);
    EXPECT_EQ(p.size().second, n.size().second);

    /*
     *  0  1  2     0  3  6  9      5  14  23  32
     *  3  4  5  *  1  4  7 10  =  14  50  86 122
     *  6  7  8     2  5  8 11     23  86 149 212
     *  9 10 11                    32 122 212 302
     */

    EXPECT_EQ(p.at(0, 0),   5);
    EXPECT_EQ(p.at(0, 1),  14);
    EXPECT_EQ(p.at(0, 2),  23);
    EXPECT_EQ(p.at(0, 3),  32);
    EXPECT_EQ(p.at(1, 0),  14);
    EXPECT_EQ(p.at(1, 1),  50);
    EXPECT_EQ(p.at(1, 2),  86);
    EXPECT_EQ(p.at(1, 3), 122);
    EXPECT_EQ(p.at(2, 0),  23);
    EXPECT_EQ(p.at(2, 1),  86);
    EXPECT_EQ(p.at(2, 2), 149);
    EXPECT_EQ(p.at(2, 3), 212);
    EXPECT_EQ(p.at(3, 0),  32);
    EXPECT_EQ(p.at(3, 1), 122);
    EXPECT_EQ(p.at(3, 2), 212);
    EXPECT_EQ(p.at(3, 3), 302);
}

TEST(matrix, scalar_multiply)
{
    matrix<int> m(4, 3);
    matrix<int> n;

    for (int i = 0; i < m.size().first; i++) {
        for (int j = 0; j < m.size().second; j++) {
            m.at(i, j) = i * m.size().second + j;
        }
    }

    n = m * 3;

    for (int i = 0; i < m.size().first; i++) {
        for (int j = 0; j < m.size().second; j++) {
            EXPECT_EQ(n.at(i, j), 3 * m.at(i, j));
        }
    }
}
