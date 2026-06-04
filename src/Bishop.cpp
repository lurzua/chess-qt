#include "Bishop.hpp"

Bishop::Bishop(const Position& _pos, const Color& _color)
    : Piece(_pos, _color, Type::B)
{}

QList<Position> Bishop::legalMoves(ChessMapConst& _chessmap) const
{
    auto legal = QList<Position>();

    // Diagonal Towards H8
    {
        auto h8diag = m_Pos;
        while (h8diag.col < Col::H && h8diag.row < Row::Eight)
        {
            h8diag.nextCol();
            h8diag.nextRow();
            if (const auto& piece = _chessmap.at(h8diag))
                break;
            else
                legal += h8diag;
        }
    }

    // Diagonal Towards H1
    {
        auto h1diag = m_Pos;
        while (h1diag.col < Col::H && h1diag.row > Row::One)
        {
            h1diag.nextCol();
            h1diag.prevRow();
            if (const auto& piece = _chessmap.at(h1diag))
                break;
            else
                legal += h1diag;
        }
    }

    // Diagonal Towards A1
    {
        auto a1diag = m_Pos;
        while (a1diag.col > Col::A && a1diag.row > Row::One)
        {
            a1diag.prevCol();
            a1diag.prevRow();
            if (const auto& piece = _chessmap.at(a1diag))
                break;
            else
                legal += a1diag;
        }
    }

    // Diagonal Towards A8
    {
        auto a8diag = m_Pos;
        while (a8diag.col > Col::A && a8diag.row < Row::Eight)
        {
            a8diag.prevCol();
            a8diag.nextRow();
            if (const auto& piece = _chessmap.at(a8diag))
                break;
            else
                legal += a8diag;
        }
    }

    return legal;
}

QList<Position> Bishop::legalCaptures(ChessMapConst& _chessmap) const
{
    auto legal = QList<Position>();

    // Diagonal Towards H8
    {
        auto h8diag = m_Pos;
        while (h8diag.col < Col::H && h8diag.row < Row::Eight)
        {
            h8diag.nextCol();
            h8diag.nextRow();
            if (const auto& piece = _chessmap.at(h8diag))
            {
                if (piece->getColor() != m_Color)
                    legal += h8diag;
                break;
            }
        }
    }

    // Diagonal Towards H1
    {
        auto h1diag = m_Pos;
        while (h1diag.col < Col::H && h1diag.row > Row::One)
        {
            h1diag.nextCol();
            h1diag.prevRow();
            if (const auto& piece = _chessmap.at(h1diag))
            {
                if (piece->getColor() != m_Color)
                    legal += h1diag;
                break;
            }
        }
    }

    // Diagonal Towards A1
    {
        auto a1diag = m_Pos;
        while (a1diag.col > Col::A && a1diag.row > Row::One)
        {
            a1diag.prevCol();
            a1diag.prevRow();
            if (const auto& piece = _chessmap.at(a1diag))
            {
                if (piece->getColor() != m_Color)
                    legal += a1diag;
                break;
            }
        }
    }

    // Diagonal Towards A8
    {
        auto a8diag = m_Pos;
        while (a8diag.col > Col::A && a8diag.row < Row::Eight)
        {
            a8diag.prevCol();
            a8diag.nextRow();
            if (const auto& piece = _chessmap.at(a8diag))
            {
                if (piece->getColor() != m_Color)
                    legal += a8diag;
                break;
            }
        }
    }

    return legal;
}

QList<Position> Bishop::protectedSquares(ChessMapConst& _chessmap) const
{
    auto legal = QList<Position>();

    // Diagonal Towards H8
    {
        auto h8diag = m_Pos;
        while (h8diag.col < Col::H && h8diag.row < Row::Eight)
        {
            h8diag.nextCol();
            h8diag.nextRow();
            legal += h8diag;
            if (const auto& piece = _chessmap.at(h8diag))
            {
                if (piece->getType() == Type::K)
                    continue;
                else
                    break;
            }
        }
    }

    // Diagonal Towards H1
    {
        auto h1diag = m_Pos;
        while (h1diag.col < Col::H && h1diag.row > Row::One)
        {
            h1diag.nextCol();
            h1diag.prevRow();
            legal += h1diag;
            if (const auto& piece = _chessmap.at(h1diag))
            {
                if (piece->getType() == Type::K)
                    continue;
                else
                    break;
            }
        }
    }

    // Diagonal Towards A1
    {
        auto a1diag = m_Pos;
        while (a1diag.col > Col::A && a1diag.row > Row::One)
        {
            a1diag.prevCol();
            a1diag.prevRow();
            legal += a1diag;
            if (const auto& piece = _chessmap.at(a1diag))
            {
                if (piece->getType() == Type::K)
                    continue;
                else
                    break;
            }
        }
    }

    // Diagonal Towards A8
    {
        auto a8diag = m_Pos;
        while (a8diag.col > Col::A && a8diag.row < Row::Eight)
        {
            a8diag.prevCol();
            a8diag.nextRow();
            legal += a8diag;
            if (const auto& piece = _chessmap.at(a8diag))
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

