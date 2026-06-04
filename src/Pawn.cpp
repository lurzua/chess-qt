#include "Pawn.hpp"

Pawn::Pawn(const Position& _pos, const Color& _color)
    : Piece(_pos, _color, Type::P)
{
}

QList<Position> Pawn::legalMoves(ChessMapConst& _chessmap) const
{
    auto legal = QList<Position>();
    switch (m_Color)
    {

    case Color::White:
    {
        if (m_Pos.row == Row::Eight)
            return legal;

        auto forward = Position(m_Pos.col, m_Pos.row);
        forward.nextRow();
        const auto forwardSquare = _chessmap.at(forward);
        if (forwardSquare)
            return legal;

        legal += forward;

        if (m_Pos.row != Row::Two)
            return legal;

        auto twoForward = m_Pos;
        twoForward.nextRow();
        twoForward.nextRow();
        const auto twoForwardSquare = _chessmap.at(twoForward);
        if (twoForwardSquare)
            return legal;

        legal += twoForward;

        break;
    }

    case Color::Black:
    {
        if (m_Pos.row == Row::One)
            return legal;

        auto forward = Position(m_Pos.col, m_Pos.row);
        forward.prevRow();
        const auto forwardSquare = _chessmap.at(forward);
        if (forwardSquare)
            return legal;

        legal += forward;

        if (m_Pos.row != Row::Seven)
            return legal;

        auto twoForward = m_Pos;
        twoForward.prevRow();
        twoForward.prevRow();
        const auto twoForwardSquare = _chessmap.at(twoForward);
        if (twoForwardSquare)
            return legal;

        legal += twoForward;

        break;
    }

    }

    return legal;
}

QList<Position> Pawn::legalCaptures(ChessMapConst& _chessmap) const
{
    auto legal = QList<Position>();
    switch (m_Color)
    {

    case Color::White:
    {
        if (m_Pos.row == Row::Eight)
            return legal;

        if (m_Pos.col != Col::A)
        {
            auto forwardLeft = Position(m_Pos.col, m_Pos.row);
            forwardLeft.nextRow();
            forwardLeft.prevCol();
            if (const auto& piece = _chessmap.at(forwardLeft))
            {
                if (piece->getColor() == Color::Black)
                    legal += forwardLeft;
            }
        }

        if (m_Pos.col != Col::H)
        {
            auto forwardRight = Position(m_Pos.col, m_Pos.row);
            forwardRight.nextRow();
            forwardRight.nextCol();
            if (const auto& piece = _chessmap.at(forwardRight))
            {
                if (piece->getColor() == Color::Black)
                    legal += forwardRight;
            }
        }

        /*
         * En-passant Rule
         */

        if (m_Pos.row == Row::Five)
        {
            if (m_Pos.col != Col::A)
            {
                auto forwardLeft = Position(m_Pos.col, m_Pos.row);
                forwardLeft.nextRow();
                forwardLeft.prevCol();
                if (const auto& square = _chessmap.at(forwardLeft); square == nullptr)
                {
                    auto enemy = m_Pos;
                    enemy.prevCol();
                    const auto& pawn = _chessmap.at(enemy);
                    if (pawn)
                    {
                        if (pawn->getType() == Type::P && pawn->getColor() != m_Color)
                        {
                            if (m_PreviousMove.first.col == enemy.col && m_PreviousMove.first.row == Row::Seven && m_PreviousMove.second == enemy)
                                legal += forwardLeft;
                        }
                    }
                }
            }

            if (m_Pos.col != Col::H)
            {
                auto forwardRight = Position(m_Pos.col, m_Pos.row);
                forwardRight.nextRow();
                forwardRight.nextCol();
                if (const auto& square = _chessmap.at(forwardRight); square == nullptr)
                {
                    auto enemy = m_Pos;
                    enemy.nextCol();
                    const auto& pawn = _chessmap.at(enemy);
                    if (pawn)
                    {
                        if (pawn->getType() == Type::P && pawn->getColor() != m_Color)
                        {
                            if (m_PreviousMove.first.col == enemy.col && m_PreviousMove.first.row == Row::Seven && m_PreviousMove.second == enemy)
                                legal += forwardRight;
                        }
                    }
                }
            }
        }

        break;
    }

    case Color::Black:
    {
        if (m_Pos.row == Row::One)
            return legal;

        if (m_Pos.col != Col::A)
        {
            auto forwardLeft = Position(m_Pos.col, m_Pos.row);
            forwardLeft.prevRow();
            forwardLeft.prevCol();
            if (const auto& piece = _chessmap.at(forwardLeft))
            {
                if (piece->getColor() == Color::White)
                    legal += forwardLeft;
            }
        }

        if (m_Pos.col != Col::H)
        {
            auto forwardRight = Position(m_Pos.col, m_Pos.row);
            forwardRight.prevRow();
            forwardRight.nextCol();
            if (const auto& piece = _chessmap.at(forwardRight))
            {
                if (piece->getColor() == Color::White)
                    legal += forwardRight;
            }
        }

        /*
         * En-passant Rule
         */

        if (m_Pos.row == Row::Four)
        {
            if (m_Pos.col != Col::A)
            {
                auto forwardLeft = Position(m_Pos.col, m_Pos.row);
                forwardLeft.prevRow();
                forwardLeft.prevCol();
                if (const auto& square = _chessmap.at(forwardLeft); square == nullptr)
                {
                    auto enemy = m_Pos;
                    enemy.prevCol();
                    const auto& pawn = _chessmap.at(enemy);
                    if (pawn)
                    {
                        if (pawn->getType() == Type::P && pawn->getColor() != m_Color)
                        {
                            if (m_PreviousMove.first.col == enemy.col && m_PreviousMove.first.row == Row::Two && m_PreviousMove.second == enemy)
                                legal += forwardLeft;
                        }
                    }
                }
            }

            if (m_Pos.col != Col::H)
            {
                auto forwardRight = Position(m_Pos.col, m_Pos.row);
                forwardRight.prevRow();
                forwardRight.nextCol();
                if (const auto& square = _chessmap.at(forwardRight); square == nullptr)
                {
                    auto enemy = m_Pos;
                    enemy.nextCol();
                    const auto& pawn = _chessmap.at(enemy);
                    if (pawn)
                    {
                        if (pawn->getType() == Type::P && pawn->getColor() != m_Color)
                        {
                            if (m_PreviousMove.first.col == enemy.col && m_PreviousMove.first.row == Row::Two && m_PreviousMove.second == enemy)
                                legal += forwardRight;
                        }
                    }
                }
            }
        }

        break;
    }

    }

    return legal;
}

QList<Position> Pawn::protectedSquares(ChessMapConst&) const
{
    auto legal = QList<Position>();

    switch (m_Color)
    {

    case Color::White:
    {
        if (m_Pos.row == Row::Eight)
            return legal;

        if (m_Pos.col != Col::A)
        {
            auto forwardLeft = Position(m_Pos.col, m_Pos.row);
            forwardLeft.nextRow();
            forwardLeft.prevCol();
            legal += forwardLeft;
        }

        if (m_Pos.col != Col::H)
        {
            auto forwardRight = Position(m_Pos.col, m_Pos.row);
            forwardRight.nextRow();
            forwardRight.nextCol();
            legal += forwardRight;
        }

        break;
    }

    case Color::Black:
    {
        if (m_Pos.row == Row::One)
            return legal;

        if (m_Pos.col != Col::A)
        {
            auto forwardLeft = Position(m_Pos.col, m_Pos.row);
            forwardLeft.prevRow();
            forwardLeft.prevCol();
            legal += forwardLeft;
    }

        if (m_Pos.col != Col::H)
        {
            auto forwardRight = Position(m_Pos.col, m_Pos.row);
            forwardRight.prevRow();
            forwardRight.nextCol();
            legal += forwardRight;
    }

        break;
    }

    }

    return legal;
}

