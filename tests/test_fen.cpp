#include "test_fen.hpp"
#include "ForsythEdwardsNotation.hpp"

void TestFen::test_default_fen()
{
    {
        const auto default_fen = QString("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        QVERIFY(isValidFEN(default_fen));
    }
}

void TestFen::test_castling_rights()
{
    {
        const auto without_castling_rights = QString("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - - 0 1");
        QVERIFY(isValidFEN(without_castling_rights));
    }

    {
        const auto without_castling_rights = QString("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQ - 0 1");
        QVERIFY(isValidFEN(without_castling_rights));
    }

    {
        const auto without_castling_rights = QString("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w kq - 0 1");
        QVERIFY(isValidFEN(without_castling_rights));
    }

    {
        const auto without_castling_rights = QString("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w Kk - 0 1");
        QVERIFY(isValidFEN(without_castling_rights));
    }

    {
        const auto without_castling_rights = QString("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w Qq - 0 1");
        QVERIFY(isValidFEN(without_castling_rights));
    }

    {
        const auto without_castling_rights = QString("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQk - 0 1");
        QVERIFY(isValidFEN(without_castling_rights));
    }

    {
        const auto without_castling_rights = QString("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQq - 0 1");
        QVERIFY(isValidFEN(without_castling_rights));
    }

    {
        const auto without_castling_rights = QString("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w Kkq - 0 1");
        QVERIFY(isValidFEN(without_castling_rights));
    }

    {
        const auto without_castling_rights = QString("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w Qkq - 0 1");
        QVERIFY(isValidFEN(without_castling_rights));
    }

    {
        const auto without_castling_rights = QString("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQKq - 0 1");
        QVERIFY(!isValidFEN(without_castling_rights));
    }
}

void TestFen::test_without_white_king()
{
    {
        const auto without_white_king = QString("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQkBNR w KQkq - 0 1");
        QVERIFY(!isValidFEN(without_white_king));
    }

    {
        const auto without_white_king = QString("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQ1BNR w KQkq - 0 1");
        QVERIFY(!isValidFEN(without_white_king));
    }

    {
        const auto without_white_king = QString("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQQBNR w KQkq - 0 1");
        QVERIFY(!isValidFEN(without_white_king));
    }
}

void TestFen::test_without_black_king()
{
    {
        const auto without_black_king = QString("rnbqKbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
        QVERIFY(!isValidFEN(without_black_king));
    }

    {
        const auto without_black_king = QString("rnbq1bnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
        QVERIFY(!isValidFEN(without_black_king));
    }

    {
        const auto without_black_king = QString("rnbqqbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
        QVERIFY(!isValidFEN(without_black_king));
    }
}

