#include "test_fen.hpp"
#include "ForsythEdwardsNotation.hpp"

void TestFen::test_default_fen()
{
    const auto default_fen = QString("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    QVERIFY(isValidFEN(default_fen));
}

