#include "test_piece.hpp"
#include "Pawn.hpp"
#include "Bishop.hpp"
#include <memory>

void TestPiece::test_piece_position()
{
    const auto c1 = Col::A;
    const auto r1 = Row::One;
    const auto c2 = Col::H;
    const auto r2 = Row::Eight;
    const auto A1 = Position(c1, r1);
    const auto H8 = Position(c2, r2);
    auto piece_1 = std::make_unique<Pawn>(A1, Color::White, m_Logger);
    auto piece_2 = std::make_unique<Pawn>(H8, Color::Black, m_Logger);
    QCOMPARE_EQ(piece_1->getPosition(), A1);
    QCOMPARE_EQ(piece_2->getPosition(), H8);
    QCOMPARE_NE(piece_1->getPosition(), piece_2->getPosition());
}

void TestPiece::test_piece_color()
{
    const auto color_white = Color::White;
    const auto color_black = Color::Black;
    auto piece_1 = std::make_unique<Pawn>(Position(Col::A, Row::One), color_white, m_Logger);
    auto piece_2 = std::make_unique<Pawn>(Position(Col::A, Row::One), color_black, m_Logger);
    QCOMPARE_EQ(piece_1->getColor(), Color::White);
    QCOMPARE_EQ(piece_2->getColor(), Color::Black);
    QCOMPARE_NE(piece_1->getColor(), piece_2->getColor());
}

void TestPiece::test_piece_type()
{
    auto piece_1 = std::make_unique<Pawn>(Position(Col::A, Row::One), Color::White, m_Logger);
    auto piece_2 = std::make_unique<Bishop>(Position(Col::A, Row::One), Color::Black, m_Logger);
    QCOMPARE_EQ(piece_1->getType(), Type::P);
    QCOMPARE_EQ(piece_2->getType(), Type::B);
    QCOMPARE_NE(piece_1->getType(), piece_2->getType());
}

void TestPiece::test_change_position()
{
    const auto pos_begin = Position(Col::C, Row::Seven);
    auto piece_1 = std::make_unique<Pawn>(pos_begin, Color::White, m_Logger);
    const auto pos_end = Position(Col::G, Row::Four);
    QCOMPARE_EQ(pos_begin, piece_1->getPosition());
    QCOMPARE_NE(pos_end,   piece_1->getPosition());
    piece_1->changePosition(pos_end);
    QCOMPARE_EQ(pos_end,   piece_1->getPosition());
    QCOMPARE_NE(pos_begin, piece_1->getPosition());
}

