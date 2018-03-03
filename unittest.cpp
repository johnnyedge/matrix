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
            m.at(i, j) = i * m.columns() + j;
        }
    }

    for (int i = 0; i < m.rows(); i++) {
        for (int j = 0; j < m.columns(); j++) {
            EXPECT_EQ(m.at(i, j), i * m.columns() + j);
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
            m.at(i, j) = i * m.columns() + j;
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
            m.at(i, j) = i * m.columns() + j;
        }
    }

    EXPECT_THROW(m * m, std::logic_error);
    n = m.transpose();

    p = m * n;

    EXPECT_EQ(p.rows(), m.rows());
    EXPECT_EQ(p.columns(), n.columns());

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

    for (int i = 0; i < m.rows(); i++) {
        for (int j = 0; j < m.columns(); j++) {
            m.at(i, j) = i * m.columns() + j;
        }
    }

    n = m * 3;

    for (int i = 0; i < m.rows(); i++) {
        for (int j = 0; j < m.columns(); j++) {
            EXPECT_EQ(n.at(i, j), 3 * m.at(i, j));
        }
    }
}
