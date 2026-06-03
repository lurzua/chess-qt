#include "King.hpp"
#include <QDebug>

King::King(const Position& _pos, const Color& _color, Logger& _logger)
    : Piece(_pos, _color, Type::K, _logger)
{}

QList<Position> King::legalMoves(ChessMapConst& _chessmap) const
{
    auto legal = QList<Position>();

    // Up
    {
        auto up = m_Pos;
        if (up.row < Row::Eight)
        {
            up.nextRow();
            if (const auto& piece = _chessmap.at(up); piece == nullptr)
            {
                if (!isEnemyProtected(_chessmap, up))
                    legal += up;
            }
        }
    }

    // Down
    {
        auto down = m_Pos;
        if (down.row > Row::One)
        {
            down.prevRow();
            if (const auto& piece = _chessmap.at(down); piece == nullptr)
            {
                if (!isEnemyProtected(_chessmap, down))
                    legal += down;
            }
        }
    }


    // Left
    {
        auto left = m_Pos;
        if (left.col > Col::A)
        {
            left.prevCol();
            if (const auto& piece = _chessmap.at(left); piece == nullptr)
            {
                if (!isEnemyProtected(_chessmap, left))
                    legal += left;
            }
        }
    }

    // Right
    {
        auto right = m_Pos;
        if (right.col < Col::H)
        {
            right.nextCol();
            if (const auto& piece = _chessmap.at(right); piece == nullptr)
            {
                if (!isEnemyProtected(_chessmap, right))
                    legal += right;
            }
        }
    }

    // Up-Left
    {
        auto upLeft = m_Pos;
        if (upLeft.row < Row::Eight && upLeft.col > Col::A)
        {
            upLeft.nextRow();
            upLeft.prevCol();
            if (const auto& piece = _chessmap.at(upLeft); piece == nullptr)
            {
                if (!isEnemyProtected(_chessmap, upLeft))
                    legal += upLeft;
            }
        }
    }

    // Up-Right
    {
        auto upRight = m_Pos;
        if (upRight.row < Row::Eight && upRight.col < Col::H)
        {
            upRight.nextRow();
            upRight.nextCol();
            if (const auto& piece = _chessmap.at(upRight); piece == nullptr)
            {
                if (!isEnemyProtected(_chessmap, upRight))
                    legal += upRight;
            }
        }
    }

    // Down-Left
    {
        auto downLeft = m_Pos;
        if (downLeft.col > Col::A && downLeft.row > Row::One)
        {
            downLeft.prevCol();
            downLeft.prevRow();
            if (const auto& piece = _chessmap.at(downLeft); piece == nullptr)
            {
                if (!isEnemyProtected(_chessmap, downLeft))
                    legal += downLeft;
            }
        }
    }

    // Down-Right
    {
        auto downRight = m_Pos;
        if (downRight.col < Col::H && downRight.row > Row::One)
        {
            downRight.nextCol();
            downRight.prevRow();
            if (const auto& piece = _chessmap.at(downRight); piece == nullptr)
            {
                if (!isEnemyProtected(_chessmap, downRight))
                    legal += downRight;
            }
        }
    }

    // Castle Kingside
    switch (m_Color)
    {

    case Color::White:
    {
        const auto& e1 = m_Pos;
        if (e1 != Position(Col::E, Row::One))
            break;

        if (m_PieceMoved)
            break;

        if (isKingInCheck(_chessmap, m_Color))
            break;

        const auto& f1 = Position(Col::F, Row::One);
        if (_chessmap.at(f1) != nullptr)
            break;

        const auto& g1 = Position(Col::G, Row::One);
        if (_chessmap.at(g1) != nullptr)
            break;

        if (isEnemyProtected(_chessmap, f1))
            break;

        if (isEnemyProtected(_chessmap, g1))
            break;

        const auto& rook = _chessmap.at(Position(Col::H, Row::One));
        if (rook == nullptr)
            break;

        if (rook->getType() != Type::R)
            break;

        if (rook->getColor() != m_Color)
            break;

        if (rook->hasPieceMoved())
            break;

        legal += g1;

        break;
    }

    case Color::Black:
    {
        const auto& e8 = m_Pos;
        if (e8 != Position(Col::E, Row::Eight))
            break;

        if (m_PieceMoved)
            break;

        if (isKingInCheck(_chessmap, m_Color))
            break;

        const auto& f8 = Position(Col::F, Row::Eight);
        if (_chessmap.at(f8) != nullptr)
            break;

        const auto& g8 = Position(Col::G, Row::Eight);
        if (_chessmap.at(g8) != nullptr)
            break;

        if (isEnemyProtected(_chessmap, f8))
            break;

        if (isEnemyProtected(_chessmap, g8))
            break;

        const auto& rook = _chessmap.at(Position(Col::H, Row::Eight));
        if (rook == nullptr)
            break;

        if (rook->getType() != Type::R)
            break;

        if (rook->getColor() != m_Color)
            break;

        if (rook->hasPieceMoved())
            break;

        legal += g8;

        break;
    }

    }

    // Castle Queenside
    switch (m_Color)
    {

    case Color::White:
    {
        const auto& e1 = m_Pos;
        if (e1 != Position(Col::E, Row::One))
            break;

        if (m_PieceMoved)
            break;

        if (isKingInCheck(_chessmap, m_Color))
            break;

        const auto& d1 = Position(Col::D, Row::One);
        if (_chessmap.at(d1) != nullptr)
            break;

        const auto& c1 = Position(Col::C, Row::One);
        if (_chessmap.at(c1) != nullptr)
            break;

        const auto& b1 = Position(Col::B, Row::One);
        if (_chessmap.at(b1) != nullptr)
            break;

        if (isEnemyProtected(_chessmap, d1))
            break;

        if (isEnemyProtected(_chessmap, c1))
            break;

        const auto& rook = _chessmap.at(Position(Col::A, Row::One));
        if (rook == nullptr)
            break;

        if (rook->getType() != Type::R)
            break;

        if (rook->getColor() != m_Color)
            break;

        if (rook->hasPieceMoved())
            break;

        legal += c1;

        break;
    }

    case Color::Black:
    {
        const auto& e8 = m_Pos;
        if (e8 != Position(Col::E, Row::Eight))
            break;

        if (m_PieceMoved)
            break;

        if (isKingInCheck(_chessmap, m_Color))
            break;

        const auto& d8 = Position(Col::D, Row::Eight);
        if (_chessmap.at(d8) != nullptr)
            break;

        const auto& c8 = Position(Col::C, Row::Eight);
        if (_chessmap.at(c8) != nullptr)
            break;

        const auto& b8 = Position(Col::B, Row::Eight);
        if (_chessmap.at(b8) != nullptr)
            break;

        if (isEnemyProtected(_chessmap, d8))
            break;

        if (isEnemyProtected(_chessmap, c8))
            break;

        const auto& rook = _chessmap.at(Position(Col::A, Row::Eight));
        if (rook == nullptr)
            break;

        if (rook->getType() != Type::R)
            break;

        if (rook->getColor() != m_Color)
            break;

        if (rook->hasPieceMoved())
            break;

        legal += c8;

        break;
    }

    }

    for (auto it = legal.begin(); it != legal.end();)
    {
        if (isEnemyProtected(_chessmap, *it))
            it = legal.erase(it);
        else
            ++it;
    }

    return legal;
}

QList<Position> King::legalCaptures(ChessMapConst& _chessmap) const
{
    auto legal = QList<Position>();

    // Up
    {
        auto up = m_Pos;
        if (up.row < Row::Eight)
        {
            up.nextRow();
            if (const auto& piece = _chessmap.at(up))
            {
                if (piece->getColor() != m_Color && !isEnemyProtected(_chessmap, up))
                    legal += up;
            }
        }
    }

    // Down
    {
        auto down = m_Pos;
        if (down.row > Row::One)
        {
            down.prevRow();
            if (const auto& piece = _chessmap.at(down))
            {
                if (piece->getColor() != m_Color && !isEnemyProtected(_chessmap, down))
                    legal += down;
            }
        }
    }

    // Left
    {
        auto left = m_Pos;
        if (left.col > Col::A)
        {
            left.prevCol();
            if (const auto& piece = _chessmap.at(left))
            {
                if (piece->getColor() != m_Color && !isEnemyProtected(_chessmap, left))
                    legal += left;
            }
        }
    }

    // Right
    {
        auto right = m_Pos;
        if (right.col < Col::H)
        {
            right.nextCol();
            if (const auto& piece = _chessmap.at(right))
            {
                if (piece->getColor() != m_Color && !isEnemyProtected(_chessmap, right))
                    legal += right;
            }
        }
    }

    // Up-Left
    {
        auto upLeft = m_Pos;
        if (upLeft.row < Row::Eight && upLeft.col > Col::A)
        {
            upLeft.nextRow();
            upLeft.prevCol();
            if (const auto& piece = _chessmap.at(upLeft))
            {
                if (piece->getColor() != m_Color && !isEnemyProtected(_chessmap, upLeft))
                    legal += upLeft;
            }
        }
    }

    // Up-Right
    {
        auto upRight = m_Pos;
        if (upRight.row > Row::One && upRight.col < Col::H)
        {
            upRight.nextRow();
            upRight.nextCol();
            if (const auto& piece = _chessmap.at(upRight))
            {
                if (piece->getColor() != m_Color && !isEnemyProtected(_chessmap, upRight))
                    legal += upRight;
            }
        }
    }

    // Down-Left
    {
        auto downLeft = m_Pos;
        if (downLeft.col > Col::A && downLeft.row > Row::One)
        {
            downLeft.prevCol();
            downLeft.prevRow();
            if (const auto& piece = _chessmap.at(downLeft))
            {
                if (piece->getColor() != m_Color && !isEnemyProtected(_chessmap, downLeft))
                    legal += downLeft;
            }
        }
    }

    // Down-Right
    {
        auto downRight = m_Pos;
        if (downRight.col < Col::H && downRight.row > Row::One)
        {
            downRight.nextCol();
            downRight.prevRow();
            if (const auto& piece = _chessmap.at(downRight))
            {
                if (piece->getColor() != m_Color && !isEnemyProtected(_chessmap, downRight))
                    legal += downRight;
            }
        }
    }

    return legal;
}

QList<Position> King::protectedSquares(ChessMapConst& _chessmap) const
{
    auto legal = QList<Position>();

    // Up
    {
        auto up = m_Pos;
        if (up.row < Row::Eight) {
            up.nextRow();
            legal += up;
        }
    }

    // Down
    {
        auto down = m_Pos;
        if (down.row > Row::One) {
            down.prevRow();
            legal += down;
        }
    }

    // Left
    {
        auto left = m_Pos;
        if (left.col > Col::A) {
            left.prevCol();
            legal += left;
        }
    }

    // Right
    {
        auto right = m_Pos;
        if (right.col < Col::H) {
            right.nextCol();
            if (const auto &piece = _chessmap.at(right))
                legal += right;
        }
    }

    // Up-Left
    {
        auto upLeft = m_Pos;
        if (upLeft.row < Row::Eight && upLeft.col > Col::A) {
            upLeft.nextRow();
            upLeft.prevCol();
            legal += upLeft;
        }
    }

    // Up-Right
    {
        auto upRight = m_Pos;
        if (upRight.row > Row::One && upRight.col < Col::H) {
            upRight.nextRow();
            upRight.nextCol();
            legal += upRight;
        }
    }

    // Down-Left
    {
        auto downLeft = m_Pos;
        if (downLeft.col > Col::A && downLeft.row > Row::One) {
            downLeft.prevCol();
            downLeft.prevRow();
            legal += downLeft;
        }
    }

    // Down-Right
    {
        auto downRight = m_Pos;
        if (downRight.col < Col::H && downRight.row > Row::One) {
            downRight.nextCol();
            downRight.prevRow();
            legal += downRight;
        }
    }

    return legal;
}

bool King::isEnemyProtected(ChessMapConst& _chessmap, const Position& _enemy_pos) const
{
    const auto& enemy_color = (Color::White == m_Color) ? Color::Black : Color::White;
    for (const auto& [ m_Pos, piece ] : _chessmap)
    {
        if (piece != nullptr)
        {
            if (piece->getColor() == enemy_color)
            {
                const auto& protected_enemies = piece->protectedSquares(_chessmap);
                if (protected_enemies.contains(_enemy_pos))
                    return true;
            }
        }
    }

    return false;
}

