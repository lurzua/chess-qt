#pragma once

#include <QTest>

class TestPiece : public QObject
{
    Q_OBJECT

private slots:

    void test_piece_position();
    void test_piece_color();
    void test_piece_type();
    void test_change_position();
};


