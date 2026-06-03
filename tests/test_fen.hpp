#pragma once

#include <QTest>

class TestFen : public QObject
{
    Q_OBJECT

private slots:

    void test_default_fen();
    void test_castling_rights();
    void test_without_white_king();
    void test_without_black_king();
    void test_lichess_puzzle_db_fen();
};

