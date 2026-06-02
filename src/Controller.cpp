#include "Controller.hpp"
#include "Piece.hpp"
#include <QEvent>
#include <QDebug>

Controller::Controller(QObject* _parent)
    : QObject{ _parent }
{}

void Controller::receiveChessMap(ChessMapConst& _map)
{
    m_ChessMap = _map;
}

void Controller::nextPlayerTurn()
{
    m_PlayerTurn = !m_PlayerTurn;
}

void Controller::receiveClickedOnChessboard(const Position& _selected_square)
{
    switch (m_PlayerTurnState)
    {

    case PlayerTurnState::SelectPiece:
    {
        processGameStateBegin(_selected_square);
        break;
    }

    case PlayerTurnState::SelectDestination:
    {
        processGameStateEnd(_selected_square);
        break;
    }

    case PlayerTurnState::PawnPromotion:
    {
        processGameStatePawnPromotion(_selected_square);
        break;
    }

    }
}

void Controller::processGameStateBegin(const Position& _selected_piece)
{
    emit requestChessMap();
    emit requestOverlayReset();
    m_CurrentPiece = nullptr;
    const auto* piece = m_ChessMap.at(_selected_piece);
    if (piece == nullptr)
    {
        m_PlayerTurnState = PlayerTurnState::SelectPiece;
        return;
    }

    if (piece->getColor() != m_PlayerTurn)
    {
        m_PlayerTurnState = PlayerTurnState::SelectPiece;
        return;
    }

    m_CurrentPiece = piece;
    m_PlayerTurnState = PlayerTurnState::SelectDestination;
    emit requestOverlayUpdateWithPieceOptions(_selected_piece);
}

void Controller::processGameStateEnd(const Position& _destination_square)
{
    Q_ASSERT(m_CurrentPiece != nullptr);
    emit requestChessMap();
    emit requestOverlayReset();

    const auto& squares = m_CurrentPiece->getValidSquares(m_ChessMap);
    const auto& piece_on_square = m_ChessMap.at(_destination_square);
    if (squares.contains(_destination_square))
    {
        const auto prev_position = m_CurrentPiece->getPosition();
        
        if (piece_on_square) //Pawn Promotion or Normal Capture
        {
            if (isPawnPromotion(prev_position, _destination_square))
            {
                m_PlayerTurnState = PlayerTurnState::PawnPromotion;
                emit requestChessMapUpdatePawnPromotion(prev_position, _destination_square);
            }
            else // Normal Capture
            {
                m_PlayerTurnState = PlayerTurnState::SelectPiece;
                m_CurrentPiece = nullptr;
                nextPlayerTurn();
                emit requestChessMapUpdateNormalCapture(prev_position, _destination_square);
            }
        }
        else
        {
            if (isPawnPromotion(prev_position, _destination_square))
            {
                m_PlayerTurnState = PlayerTurnState::PawnPromotion;
                emit requestChessMapUpdatePawnPromotion(prev_position, _destination_square);
            }
            else if (isKingsideCastle(prev_position, _destination_square))
            {
                m_PlayerTurnState = PlayerTurnState::SelectPiece;
                m_CurrentPiece = nullptr;
                nextPlayerTurn();
                emit requestChessMapUpdateKingSideCastle(prev_position, _destination_square);
            }
            else if (isQueensideCastle(prev_position, _destination_square))
            {
                m_PlayerTurnState = PlayerTurnState::SelectPiece;
                m_CurrentPiece = nullptr;
                nextPlayerTurn();
                emit requestChessMapUpdateQueenSideCastle(prev_position, _destination_square);
            }
            else if (isEnpassant(prev_position, _destination_square))
            {
                m_PlayerTurnState = PlayerTurnState::SelectPiece;
                m_CurrentPiece = nullptr;
                nextPlayerTurn();
                emit requestChessMapUpdateEnpassant(prev_position, _destination_square);
            }
            else // Normal Move
            {
                m_PlayerTurnState = PlayerTurnState::SelectPiece;
                m_CurrentPiece = nullptr;
                nextPlayerTurn();
                emit requestChessMapUpdateNormalMove(prev_position, _destination_square);
            }
        }
    }
    else
    {
        m_PlayerTurnState = PlayerTurnState::SelectPiece;
        m_CurrentPiece = nullptr;
        emit requestOverlayReset();
    }
}

void Controller::processGameStatePawnPromotion(const Position& _selected_promoted_piece)
{
    const auto& pawn_pos = m_CurrentPiece->getPosition();
    auto selection = m_CurrentPiece->getPosition();

    std::array<Type, 4> promote_options = { Type::Q, Type::N, Type::R, Type::B };

    for (const auto& option : promote_options)
    {
        if (_selected_promoted_piece == selection)
        {
            m_CurrentPiece = nullptr;
            m_PlayerTurnState = PlayerTurnState::SelectPiece;
            nextPlayerTurn();
            emit requestChessMap();
            emit requestOverlayReset();

            switch (option)
            {
                case Type::Q:
                    emit requestPawnPromoteToQueen(pawn_pos);
                    break;
                case Type::N:
                    emit requestPawnPromoteToKnight(pawn_pos);
                    break;
                case Type::R:
                    emit requestPawnPromoteToRook(pawn_pos);
                    break;
                case Type::B:
                    emit requestPawnPromoteToBishop(pawn_pos);
                    break;

                // Can't Promote To Pawn or King
                case Type::P:
                case Type::K:
                    Q_UNREACHABLE();
                    break;
            }

            break;
        }

        if (m_PlayerTurn == Color::White)
            selection.prevRow();
        else if (m_PlayerTurn == Color::Black)
            selection.nextRow();
        else
            Q_UNREACHABLE();
    }
}

bool Controller::isPawnPromotion(const Position& _start, const Position& _end)
{
    switch (m_PlayerTurn)
    {

    case Color::White:
    {
        const auto& pawn = m_ChessMap.at(_start);
        if (pawn)
        {
            if (pawn->getType() == Type::P && _end.row == Row::Eight)
                return true;
        }

        break;
    }

    case Color::Black:
    {
        const auto& pawn = m_ChessMap.at(_start);
        if (pawn)
        {
            if (pawn->getType() == Type::P && _end.row == Row::One)
                return true;
        }

        break;
    }

    }

    return false;
}

bool Controller::isKingsideCastle(const Position& _start, const Position& _end)
{
    switch (m_PlayerTurn)
    {

    case Color::White:
    {
        const auto& king = m_ChessMap.at(_start);

        if (king == nullptr)
            return false;

        if (king->getType() != Type::K)
            return false;

        if (king->getPosition() != Position(Col::E, Row::One))
            return false;

        const auto& g1 = m_ChessMap.at(_end);

        if (g1 != nullptr)
            return false;

        if (_end != Position(Col::G, Row::One))
            return false;

        break;
    }

    case Color::Black:
    {
        const auto& king = m_ChessMap.at(_start);

        if (king == nullptr)
            return false;

        if (king->getType() != Type::K)
            return false;

        if (king->getPosition() != Position(Col::E, Row::Eight))
            return false;

        const auto& g8 = m_ChessMap.at(_end);

        if (g8 != nullptr)
            return false;

        if (_end != Position(Col::G, Row::Eight))
            return false;

        break;
    }

    }

    return true;
}

bool Controller::isQueensideCastle(const Position& _start, const Position& _end)
{
    switch (m_PlayerTurn)
    {

    case Color::White:
    {
        const auto& king = m_ChessMap.at(_start);

        if (king == nullptr)
            return false;

        if (king->getType() != Type::K)
            return false;

        if (king->getPosition() != Position(Col::E, Row::One))
            return false;

        const auto& c1 = m_ChessMap.at(_end);

        if (c1 != nullptr)
            return false;

        if (_end != Position(Col::C, Row::One))
            return false;

        break;
    }

    case Color::Black:
    {
        const auto& king = m_ChessMap.at(_start);

        if (king == nullptr)
            return false;

        if (king->getType() != Type::K)
            return false;

        if (king->getPosition() != Position(Col::E, Row::Eight))
            return false;

        const auto& c8 = m_ChessMap.at(_end);

        if (c8 != nullptr)
            return false;

        if (_end != Position(Col::C, Row::Eight))
            return false;

        break;
    }

    }

    return true;
}

bool Controller::isEnpassant(const Position& _start, const Position& _end)
{
    switch (m_PlayerTurn)
    {

    case Color::White:
    {
        const auto& pawn = m_ChessMap.at(_start);

        if (pawn == nullptr)
            return false;

        if (pawn->getType() != Type::P)
            return false;

        const auto& pos = pawn->getPosition();
        if (pos.row == Row::Eight)
            return false;

        const auto& empty_square = m_ChessMap.at(_end);
        if (empty_square != nullptr)
            return false;

        auto oneForward = _start;
        oneForward.nextRow();
        if (oneForward.row != _end.row)
            return false;

        if (_start.col == Col::A)
        {
            auto right = _start;
            right.nextCol();
            if (right.col != _end.col)
                return false;
        }
        else if (_start.col == Col::H)
        {
            auto left = _start;
            left.prevCol();
            if (left.col != _end.col)
                return false;
        }
        else
        {
            auto left = _start;
            left.prevCol();
            auto right = _start;
            right.nextCol();
            if (left.col != _end.col && right.col != _end.col)
                return false;
        }

        break;
    }

    case Color::Black:
    {
        const auto& pawn = m_ChessMap.at(_start);

        if (pawn == nullptr)
            return false;

        if (pawn->getType() != Type::P)
            return false;

        const auto& pos = pawn->getPosition();
        if (pos.row == Row::One)
            return false;

        const auto& empty_square = m_ChessMap.at(_end);
        if (empty_square != nullptr)
            return false;

        auto oneBackward = _start;
        oneBackward.prevRow();
        if (oneBackward.row != _end.row)
            return false;

        if (_start.col == Col::A)
        {
            auto right = _start;
            right.nextCol();
            if (right.col != _end.col)
                return false;
        }
        else if (_start.col == Col::H)
        {
            auto left = _start;
            left.prevCol();
            if (left.col != _end.col)
                return false;
        }
        else
        {
            auto left = _start;
            left.prevCol();
            auto right = _start;
            right.nextCol();
            if (left.col != _end.col && right.col != _end.col)
                return false;
        }

        break;
    }

    }

    return true;
}

