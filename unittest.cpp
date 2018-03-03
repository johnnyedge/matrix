#include <gtest/gtest.h>

#include "matrix.h"

TEST(matrix, basic)
{
    EXPECT_THROW(matrix<int>(0, 0), std::logic_error);
    EXPECT_THROW(matrix<int>(0, 1), std::logic_error);
    EXPECT_THROW(matrix<int>(1, 0), std::logic_error);

    matrix<int> m(4, 3);

    EXPECT_EQ(m.rows(), 4);
    EXPECT_EQ(m.columns(), 3);

    for (int i = 0; i < m.rows(); i++) {
        for (int j = 0; j < m.columns(); j++) {
            m.at(i, j) = i * m.rows() + j;
        }
    }

    for (int i = 0; i < m.rows(); i++) {
        for (int j = 0; j < m.columns(); j++) {
            EXPECT_EQ(m.at(i, j), i * m.rows() + j);
        }
    }
}

TEST(matrix, transpose)
{
    matrix<int> m(4, 3);
    matrix<int> t;

    EXPECT_EQ(m.rows(), 4);
    EXPECT_EQ(m.columns(), 3);

    for (int i = 0; i < m.rows(); i++) {
        for (int j = 0; j < m.columns(); j++) {
            m.at(i, j) = i * m.rows() + j;
        }
    }

    t = m.transpose();

    EXPECT_EQ(m.rows(), t.columns());
    EXPECT_EQ(m.columns(), t.rows());

    for (int i = 0; i < m.rows(); i++) {
        for (int j = 0; j < m.columns(); j++) {
            EXPECT_EQ(m.at(i, j), t.at(j, i));
        }
    }
}

TEST(matrix, matrix_multiply)
{
    matrix<int> m(4, 3);
    matrix<int> n, p;

    for (int i = 0; i < m.rows(); i++) {
        for (int j = 0; j < m.columns(); j++) {
            m.at(i, j) = i * m.rows() + j;
        }
    }

    n = m.transpose();

    p = m * n;

    EXPECT_EQ(p.rows(), m.rows());
    EXPECT_EQ(p.columns(), n.columns());
}

TEST(matrix, scalar_multiply)
{
    matrix<int> m(4, 3);
    matrix<int> n;

    for (int i = 0; i < m.rows(); i++) {
        for (int j = 0; j < m.columns(); j++) {
            m.at(i, j) = i * m.rows() + j;
        }
    }

    n = m * 3;

    for (int i = 0; i < m.rows(); i++) {
        for (int j = 0; j < m.columns(); j++) {
            EXPECT_EQ(n.at(i, j), 3 * m.at(i, j));
        }
    }
}
