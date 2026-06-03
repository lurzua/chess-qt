#include "test_fen.hpp"
#include "ForsythEdwardsNotation.hpp"
#include <fstream>
#include <string>

void TestFen::test_default_fen()
{
    {
        const auto default_fen = QString("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        QVERIFY(isValidFEN(default_fen));
    }
}

void TestFen::test_castling_rights()
{
    const auto piece_placement = QString("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    {
        const auto without_castling_rights = piece_placement + QString(" w - - 0 1");
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

    
void TestFen::test_lichess_puzzle_db_fen()
{
    if (std::ifstream puzzle_db_fen("data/lichess-db-sample-fen.csv"); puzzle_db_fen.is_open())
    {
        std::string line;

        while (std::getline(puzzle_db_fen, line))
        {
            size_t comma_pos = line.find(',');
            
            if (comma_pos != std::string::npos)
            {
                const auto fen = line.substr(0, comma_pos);
                //const auto solution_moves = line.substr(comma_pos + 1);
                QVERIFY(isValidFEN(QString::fromStdString(fen)));
            }
        }

        puzzle_db_fen.close();
    }
    else 
        QVERIFY(false);
}

