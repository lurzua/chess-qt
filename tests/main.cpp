#include <QtTest/QtTest>

#include "test_position.hpp"
#include "test_piece.hpp"
#include "test_fen.hpp"

int main(int argc, char *argv[])
{
    int status = 0;

    {
        TestPosition tc;
        status |= QTest::qExec(&tc, argc, argv);
    }

    {
        TestPiece tc;
        status |= QTest::qExec(&tc, argc, argv);
    }

    {
        TestFen tc;
        status |= QTest::qExec(&tc, argc, argv);
    }

    return status;
}

