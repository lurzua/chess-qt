#include "test_position.hpp"
#include "Position.hpp"

void TestPosition::test_equality()
{
    for (const auto& col : g_ArrayCol)
    {
        for (const auto& row : g_ArrayRow)
        {
            const auto p = Position(col, row);
            const auto q = Position(col, row);
            QCOMPARE_EQ(p, q);
        }
    }
}

void TestPosition::test_not_equals()
{
    const auto col_a = Col::A;
    const auto row_1 = Row::One;
    const auto col_b = Col::B;

    auto p1 = Position(col_a, row_1);
    auto q1 = Position(col_b, row_1);
    while (p1.col != Col::H)
    {
        QCOMPARE_NE(p1, q1);
        p1.nextCol();
        q1.nextCol();
    }

    const auto row_2 = Row::Two;
    auto p2 = Position(col_a, row_1);
    auto q2 = Position(col_a, row_2);
    while (p2.row != Row::Eight)
    {
        QCOMPARE_NE(p2, q2);
        p2.nextRow();
        q2.nextRow();
    }
}

void TestPosition::test_less_than_col()
{
    for (size_t i = 0, j = i + 1; j < g_ArrayCol.size(); i++, j++)
    {
        auto col_a = g_ArrayCol.at(i);
        auto col_b = g_ArrayCol.at(j);
        QCOMPARE_LT(col_a, col_b);
    }
}

void TestPosition::test_less_than_row()
{
    for (size_t i = 0, j = i + 1; j < g_ArrayRow.size(); i++, j++)
    {
        auto row_1 = g_ArrayRow.at(i);
        auto row_2 = g_ArrayRow.at(j);
        QCOMPARE_LT(row_1, row_2);
    }
}

void TestPosition::test_next_column()
{
    auto p = Position(Col::A, Row::One);

    for (const auto& c : g_ArrayCol)
    {
        QVERIFY(p == Position(c, Row::One));
        p.nextCol();
    }
}

void TestPosition::test_prev_column()
{
    auto p = Position(Col::H, Row::One);

    for (int i = g_ArrayCol.size() - 1; i >= 0; i--)
    {
        QVERIFY(p == Position(g_ArrayCol.at(i), Row::One));
        p.prevCol();
    }
}

void TestPosition::test_next_row()
{
    auto p = Position(Col::A, Row::One);

    for (const auto& r : g_ArrayRow)
    {
        QVERIFY(p == Position(Col::A, r));
        p.nextRow();
    }
}

void TestPosition::test_prev_row()
{
    auto p = Position(Col::A, Row::Eight);

    for (int i = g_ArrayRow.size() - 1; i >= 0; i--)
    {
        QVERIFY(p == Position(Col::A, g_ArrayRow.at(i)));
        p.prevRow();
    }
}

