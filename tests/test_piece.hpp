#pragma once

#include <QTest>
#include <Logger.hpp>

class TestPiece : public QObject
{
    Q_OBJECT

private slots:

    void test_piece_position();
    void test_piece_color();
    void test_piece_type();
    void test_change_position();

private:

    Logger m_Logger = Logger("test.log");
};

