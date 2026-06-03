#include "Rook.hpp"

Rook::Rook(const Position& _pos, const Color& _color, Logger& _logger)
    : Piece(_pos, _color, Type::R, _logger)
{}

QList<Position> Rook::legalMoves(ChessMapConst& _chessmap) const
{
    auto legal = QList<Position>();

    // Up
    {
        auto up = m_Pos;
        while (up.row < Row::Eight)
        {
            up.nextRow();
            if (const auto& piece = _chessmap.at(up))
                break;
            else
                legal += up;
        }
    }

    // Down
    {
        auto down = m_Pos;
        while (down.row > Row::One)
        {
            down.prevRow();
            if (const auto& piece = _chessmap.at(down))
                break;
            else
                legal += down;
        }
    }

    // Left
    {
        auto left = m_Pos;
        while (left.col > Col::A)
        {
            left.prevCol();
            if (const auto& piece = _chessmap.at(left))
                break;
            else
                legal += left;
        }
    }

    // Right
    {
        auto right = m_Pos;
        while (right.col < Col::H)
        {
            right.nextCol();
            if (const auto& piece = _chessmap.at(right))
                break;
            else
                legal += right;
        }
    }

    return legal;
}

QList<Position> Rook::legalCaptures(ChessMapConst& _chessmap) const
{
    auto legal = QList<Position>();

    // Up
    {
        auto up = m_Pos;
        while (up.row < Row::Eight)
        {
            up.nextRow();
            if (const auto& piece = _chessmap.at(up))
            {
                if (piece->getColor() != m_Color)
                    legal += up;
                break;
            }
        }
    }

    // Down
    {
        auto down = m_Pos;
        while (down.row > Row::One)
        {
            down.prevRow();
            if (const auto& piece = _chessmap.at(down))
            {
                if (piece->getColor() != m_Color)
                    legal += down;
                break;
            }
        }
    }

    // Left
    {
        auto left = m_Pos;
        while (left.col > Col::A)
        {
            left.prevCol();
            if (const auto& piece = _chessmap.at(left))
            {
                if (piece->getColor() != m_Color)
                    legal += left;
                break;
            }
        }
    }

    // Right
    {
        auto right = m_Pos;
        while (right.col < Col::H)
        {
            right.nextCol();
            if (const auto& piece = _chessmap.at(right))
            {
                if (piece->getColor() != m_Color)
                    legal += right;
                break;
            }
        }
    }

    return legal;
}

QList<Position> Rook::protectedSquares(ChessMapConst& _chessmap) const
{
    auto legal = QList<Position>();

    // Up
    {
        auto up = m_Pos;
        while (up.row < Row::Eight)
        {
            up.nextRow();
            legal += up;
            if (const auto& piece = _chessmap.at(up))
            {
                if (piece->getType() == Type::K)
                    continue;
                else
                    break;
            }
        }
    }

    // Down
    {
        auto down = m_Pos;
        while (down.row > Row::One)
        {
            down.prevRow();
            legal += down;
            if (const auto& piece = _chessmap.at(down))
            {
                if (piece->getType() == Type::K)
                    continue;
                else
                    break;
            }
        }
    }

    // Left
    {
        auto left = m_Pos;
        while (left.col > Col::A)
        {
            left.prevCol();
            legal += left;
            if (const auto& piece = _chessmap.at(left))
            {
                if (piece->getType() == Type::K)
                    continue;
                else
                    break;
            }
        }
    }

    // Right
    {
        auto right = m_Pos;
        while (right.col < Col::H)
        {
            right.nextCol();
            legal += right;
            if (const auto& piece = _chessmap.at(right))
            {
                if (piece->getType() == Type::K)
                    continue;
                else
                    break;
            }
        }
    }

    return legal;
}

