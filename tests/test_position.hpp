#pragma once

#include <QTest>

class TestPosition : public QObject
{
    Q_OBJECT

private slots:

    void test_equality();
    void test_not_equals();
    void test_less_than_col();
    void test_less_than_row();
    void test_next_column();
    void test_prev_column();
    void test_next_row();
    void test_prev_row();
};

