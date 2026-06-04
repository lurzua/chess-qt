#include "Knight.hpp"
#include <QDebug>

Knight::Knight(const Position& _pos, const Color& _color)
    : Piece(_pos, _color, Type::N)
{}

QList<Position> Knight::legalMoves(ChessMapConst& _chessmap) const
{
    auto legal = QList<Position>();

    if (m_Pos.col > Col::A && m_Pos.row < Row::Seven)
    {
        auto _12pm_left = m_Pos;
        _12pm_left.nextRow();
        _12pm_left.nextRow();
        _12pm_left.prevCol();
        if (const auto& piece = _chessmap.at(_12pm_left);
            piece == nullptr)
            legal += _12pm_left;
    }

    if (m_Pos.col < Col::H && m_Pos.row < Row::Seven)
    {
        auto _12pm_right = m_Pos;
        _12pm_right.nextRow();
        _12pm_right.nextRow();
        _12pm_right.nextCol();
        if (const auto& piece = _chessmap.at(_12pm_right);
            piece == nullptr)
            legal += _12pm_right;
    }

    if (m_Pos.col < Col::G && m_Pos.row < Row::Eight)
    {
        auto _3pm_up = m_Pos;
        _3pm_up.nextRow();
        _3pm_up.nextCol();
        _3pm_up.nextCol();
        if (const auto& piece = _chessmap.at(_3pm_up);
            piece == nullptr)
            legal += _3pm_up;
    }

    if (m_Pos.col < Col::G && m_Pos.row > Row::One)
    {
        auto _3pm_down = m_Pos;
        _3pm_down.prevRow();
        _3pm_down.nextCol();
        _3pm_down.nextCol();
        if (const auto& piece = _chessmap.at(_3pm_down);
            piece == nullptr)
            legal += _3pm_down;
    }

    if (m_Pos.col < Col::H && m_Pos.row > Row::Two)
    {
        auto _6pm_right = m_Pos;
        _6pm_right.prevRow();
        _6pm_right.prevRow();
        _6pm_right.nextCol();
        if (const auto& piece = _chessmap.at(_6pm_right);
            piece == nullptr)
            legal += _6pm_right;
    }

    if (m_Pos.col > Col::A && m_Pos.row > Row::Two)
    {
        auto _6pm_left = m_Pos;
        _6pm_left.prevRow();
        _6pm_left.prevRow();
        _6pm_left.prevCol();
        if (const auto& piece = _chessmap.at(_6pm_left);
            piece == nullptr)
            legal += _6pm_left;
    }

    if (m_Pos.col > Col::B && m_Pos.row > Row::One)
    {
        auto _9pm_down = m_Pos;
        _9pm_down.prevRow();
        _9pm_down.prevCol();
        _9pm_down.prevCol();
        if (const auto& piece = _chessmap.at(_9pm_down);
            piece == nullptr)
            legal += _9pm_down;
    }

    if (m_Pos.col > Col::B && m_Pos.row < Row::Eight)
    {
        auto _9pm_up = m_Pos;
        _9pm_up.nextRow();
        _9pm_up.prevCol();
        _9pm_up.prevCol();
        if (const auto& piece = _chessmap.at(_9pm_up);
            piece == nullptr)
            legal += _9pm_up;
    }

    return legal;
}

QList<Position> Knight::legalCaptures(ChessMapConst& _chessmap) const
{
    auto legal = QList<Position>();

    if (m_Pos.col > Col::A && m_Pos.row < Row::Seven)
    {
        auto _12pm_left = m_Pos;
        _12pm_left.nextRow();
        _12pm_left.nextRow();
        _12pm_left.prevCol();
        if (const auto& piece = _chessmap.at(_12pm_left))
        {
            if (piece->getColor() != m_Color)
                legal += _12pm_left;
        }
    }

    if (m_Pos.col < Col::H && m_Pos.row < Row::Seven)
    {
        auto _12pm_right = m_Pos;
        _12pm_right.nextRow();
        _12pm_right.nextRow();
        _12pm_right.nextCol();
        if (const auto& piece = _chessmap.at(_12pm_right))
        {
            if (piece->getColor() != m_Color)
                legal += _12pm_right;
        }
    }

    if (m_Pos.col < Col::G && m_Pos.row < Row::Eight)
    {
        auto _3pm_up = m_Pos;
        _3pm_up.nextRow();
        _3pm_up.nextCol();
        _3pm_up.nextCol();
        if (const auto& piece = _chessmap.at(_3pm_up))
        {
            if (piece->getColor() != m_Color)
                legal += _3pm_up;
        }
    }

    if (m_Pos.col < Col::G && m_Pos.row > Row::One)
    {
        auto _3pm_down = m_Pos;
        _3pm_down.prevRow();
        _3pm_down.nextCol();
        _3pm_down.nextCol();
        if (const auto& piece = _chessmap.at(_3pm_down))
        {
            if (piece->getColor() != m_Color)
                legal += _3pm_down;
        }
    }

    if (m_Pos.col < Col::H && m_Pos.row > Row::Two)
    {
        auto _6pm_right = m_Pos;
        _6pm_right.prevRow();
        _6pm_right.prevRow();
        _6pm_right.nextCol();
        if (const auto& piece = _chessmap.at(_6pm_right))
        {
            if (piece->getColor() != m_Color)
                legal += _6pm_right;
        }
    }

    if (m_Pos.col > Col::A && m_Pos.row > Row::Two)
    {
        auto _6pm_left = m_Pos;
        _6pm_left.prevRow();
        _6pm_left.prevRow();
        _6pm_left.prevCol();
        if (const auto& piece = _chessmap.at(_6pm_left))
        {
            if (piece->getColor() != m_Color)
                legal += _6pm_left;
        }
    }

    if (m_Pos.col > Col::B && m_Pos.row > Row::One)
    {
        auto _9pm_down = m_Pos;
        _9pm_down.prevRow();
        _9pm_down.prevCol();
        _9pm_down.prevCol();
        if (const auto& piece = _chessmap.at(_9pm_down))
        {
            if (piece->getColor() != m_Color)
                legal += _9pm_down;
        }
    }

    if (m_Pos.col > Col::B && m_Pos.row < Row::Eight)
    {
        auto _9pm_up = m_Pos;
        _9pm_up.nextRow();
        _9pm_up.prevCol();
        _9pm_up.prevCol();
        if (const auto& piece = _chessmap.at(_9pm_up))
        {
            if (piece->getColor() != m_Color)
                legal += _9pm_up;
        }
    }

    return legal;
}

QList<Position> Knight::protectedSquares(ChessMapConst&) const
{
    auto legal = QList<Position>();

    if (m_Pos.col > Col::A && m_Pos.row < Row::Seven)
    {
        auto _12pm_left = m_Pos;
        _12pm_left.nextRow();
        _12pm_left.nextRow();
        _12pm_left.prevCol();
        legal += _12pm_left;
}

    if (m_Pos.col < Col::H && m_Pos.row < Row::Seven)
    {
        auto _12pm_right = m_Pos;
        _12pm_right.nextRow();
        _12pm_right.nextRow();
        _12pm_right.nextCol();
        legal += _12pm_right;
    }

    if (m_Pos.col < Col::G && m_Pos.row < Row::Eight)
    {
        auto _3pm_up = m_Pos;
        _3pm_up.nextRow();
        _3pm_up.nextCol();
        _3pm_up.nextCol();
        legal += _3pm_up;
    }

    if (m_Pos.col < Col::G && m_Pos.row > Row::One)
    {
        auto _3pm_down = m_Pos;
        _3pm_down.prevRow();
        _3pm_down.nextCol();
        _3pm_down.nextCol();
        legal += _3pm_down;
    }

    if (m_Pos.col < Col::H && m_Pos.row > Row::Two)
    {
        auto _6pm_right = m_Pos;
        _6pm_right.prevRow();
        _6pm_right.prevRow();
        _6pm_right.nextCol();
        legal += _6pm_right;
    }

    if (m_Pos.col > Col::A && m_Pos.row > Row::Two)
    {
        auto _6pm_left = m_Pos;
        _6pm_left.prevRow();
        _6pm_left.prevRow();
        _6pm_left.prevCol();
        legal += _6pm_left;
    }

    if (m_Pos.col > Col::B && m_Pos.row > Row::One)
    {
        auto _9pm_down = m_Pos;
        _9pm_down.prevRow();
        _9pm_down.prevCol();
        _9pm_down.prevCol();
        legal += _9pm_down;
    }

    if (m_Pos.col > Col::B && m_Pos.row < Row::Eight)
    {
        auto _9pm_up = m_Pos;
        _9pm_up.nextRow();
        _9pm_up.prevCol();
        _9pm_up.prevCol();
        legal += _9pm_up;
    }

    return legal;
}

