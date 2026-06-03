#include "Queen.hpp"

Queen::Queen(const Position& _pos, const Color& _color, Logger& _logger)
    : Piece(_pos, _color, Type::Q, _logger)
{}

QList<Position> Queen::legalMoves(ChessMapConst& _chessmap) const
{
    auto legal = QList<Position>();

    legal += bishopMoves(_chessmap);
    legal += rookMoves(_chessmap);

    return legal;
}

QList<Position> Queen::legalCaptures(ChessMapConst& _chessmap) const
{
    auto legal = QList<Position>();

    legal += bishopCaptures(_chessmap);
    legal += rookCaptures(_chessmap);

    return legal;
}

QList<Position> Queen::bishopMoves(ChessMapConst& _chessmap) const
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

QList<Position> Queen::rookMoves(ChessMapConst& _chessmap) const
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

QList<Position> Queen::bishopCaptures(ChessMapConst& _chessmap) const
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

QList<Position> Queen::rookCaptures(ChessMapConst& _chessmap) const
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

QList<Position> Queen::protectedSquares(ChessMapConst& _chessmap) const
{
    auto legal = QList<Position>();

    legal += bishopProtectedSquares(_chessmap);
    legal += rookProtectedSquares(_chessmap);

    return legal;
}

QList<Position> Queen::bishopProtectedSquares(ChessMapConst& _chessmap) const
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

QList<Position> Queen::rookProtectedSquares(ChessMapConst& _chessmap) const
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

