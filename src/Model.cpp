#include <QDebug>
#include "Model.hpp"
#include "King.hpp"
#include "Queen.hpp"
#include "Rook.hpp"
#include "Bishop.hpp"
#include "Knight.hpp"
#include "Pawn.hpp"
#include "Logger.hpp"

Model::Model(Logger& _logger)
    : QObject{ nullptr }
    , m_Logger(_logger)
{
    initChessMap();
    initPieces();
    setPiecesOnChessMap();
}

Model::~Model()
{
}

void Model::initChessMap()
{
    for (const auto& r : g_ArrayRow)
    {
        for (const auto& c : g_ArrayCol)
            m_ChessMap.insert({ Position(c, r), nullptr });
    }
}

void Model::initPieces()
{
    for (const auto& [ col, row, color, type ] : g_StartingPieces)
    {
        const auto pos = Position(col, row);
        switch (type)
        {

        case Type::P:
            m_Pieces.push_back(new Pawn(pos, color, m_Logger));
            break;
        case Type::N:
            m_Pieces.push_back(new Knight(pos, color, m_Logger));
            break;
        case Type::B:
            m_Pieces.push_back(new Bishop(pos, color, m_Logger));
            break;
        case Type::R:
            m_Pieces.push_back(new Rook(pos, color, m_Logger));
            break;
        case Type::Q:
            m_Pieces.push_back(new Queen(pos, color, m_Logger));
            break;
        case Type::K:
            m_Pieces.push_back(new King(pos, color, m_Logger));
            break;

        }
    }
}

void Model::setPiecesOnChessMap()
{
    for (const auto& p : m_Pieces)
        m_ChessMap.at(p->getPosition()) = p;
}

Model::ChessMapConst Model::getChessMapConst() const
{
    std::unordered_map<Position, const Piece*> chessmap_const;

    for (const auto& [ pos, piece ] : m_ChessMap)
        chessmap_const.insert({ pos, piece });

    return chessmap_const;
}

void Model::receiveRequestChessMap()
{
    emit sendChessMap(getChessMapConst());
}

void Model::receiveRequestOverlayReset()
{
    updateChessBoard();
}

void Model::receiveRequestOverlayUpdateWithPieceOptions(const Position& _selected_piece)
{
    const auto chessmap_const = getChessMapConst();
    emit requestSetSelectedPieceRenderer(_selected_piece);
    if (const auto piece = chessmap_const.at(_selected_piece))
    {
        const auto potential_squares = piece->getValidSquares(chessmap_const);
        for (const auto& square : potential_squares)
            emit requestSetPotentialMoveRenderer(square);
    }
    else 
        Q_UNREACHABLE();
}

void Model::receiveRequestChessMapUpdateNormalMove(const Position& _start, const Position& _end)
{
    m_PreviousMoveStart = _start;
    m_PreviousMoveEnd = _end;
    auto* pieceToMove = m_ChessMap.at(_start);
    std::swap(m_ChessMap.at(pieceToMove->getPosition()), m_ChessMap.at(_end));
    pieceToMove->changePosition(_end);
    updateChessBoard();
    emit requestToRecordNormalMove(getAlgebraicNotationNormalMove(_start, _end));
    const auto player_color = !pieceToMove->getColor();
    if (isCheckMate(player_color))
        emit requestToRecordCheckMate(player_color);
    else if (isKingInCheck(player_color))
        emit requestToRecordKingJaque(player_color);
    else if (isStaleMate(player_color))
        ;
}

void Model::receiveRequestChessMapUpdateNormalCapture(const Position& _start, const Position& _end)
{
    m_PreviousMoveStart = _start;
    m_PreviousMoveEnd = _end;
    m_Pieces.erase(std::remove_if(m_Pieces.begin(), m_Pieces.end(),
        [&](const Piece* p) {
            return p->getPosition() == _end;
        }), m_Pieces.end());
    auto* pieceToMove = m_ChessMap.at(_start);
    m_ChessMap.at(_end) = nullptr;
    std::swap(m_ChessMap.at(pieceToMove->getPosition()), m_ChessMap.at(_end));
    pieceToMove->changePosition(_end);
    updateChessBoard();
    emit requestToRecordNormalCapture(getAlgebraicNotationNormalCapture(_start, _end));
    const auto player_color = !pieceToMove->getColor();
    if (isCheckMate(player_color))
        emit requestToRecordCheckMate(player_color);
    else if (isKingInCheck(player_color))
        emit requestToRecordKingJaque(player_color);
    else if (isStaleMate(player_color))
        ;
}

void Model::receiveRequestChessMapUpdateQueenSideCastle(const Position& _start, const Position& _end)
{
    m_PreviousMoveStart = _start;
    m_PreviousMoveEnd = _end;
    const auto& R  = (_start.row == Row::One) ? Row::One : Row::Eight;
    const auto& eR = Position(Col::E, R);
    const auto& dR = Position(Col::D, R);
    const auto& cR = Position(Col::C, R);
    const auto& aR = Position(Col::A, R);
    auto* king = m_ChessMap.at(eR);
    auto* rook = m_ChessMap.at(aR);
    std::swap(m_ChessMap.at(aR), m_ChessMap.at(dR));
    std::swap(m_ChessMap.at(eR), m_ChessMap.at(cR));
    king->changePosition(cR);
    rook->changePosition(dR);
    updateChessBoard();
    emit requestToRecordCastling(getAlgebraicNotationCastling(_end));
    const auto player_color = !king->getColor();
    if (isCheckMate(player_color))
        emit requestToRecordCheckMate(player_color);
    else if (isKingInCheck(player_color))
        emit requestToRecordKingJaque(player_color);
    else if (isStaleMate(player_color))
        ;
}

void Model::receiveRequestChessMapUpdateKingSideCastle(const Position& _start, const Position& _end)
{
    m_PreviousMoveStart = _start;
    m_PreviousMoveEnd = _end;
    const auto& R  = (_start.row == Row::One) ? Row::One : Row::Eight;
    const auto& eR = Position(Col::E, R);
    const auto& fR = Position(Col::F, R);
    const auto& gR = Position(Col::G, R);
    const auto& hR = Position(Col::H, R);
    auto* king = m_ChessMap.at(eR);
    auto* rook = m_ChessMap.at(hR);
    std::swap(m_ChessMap.at(hR), m_ChessMap.at(fR));
    std::swap(m_ChessMap.at(eR), m_ChessMap.at(gR));
    king->changePosition(gR);
    rook->changePosition(fR);
    updateChessBoard();
    emit requestToRecordCastling(getAlgebraicNotationCastling(_end));
    const auto player_color = !king->getColor();
    if (isCheckMate(player_color))
        emit requestToRecordCheckMate(player_color);
    else if (isKingInCheck(player_color))
        emit requestToRecordKingJaque(player_color);
    else if (isStaleMate(player_color))
        ;
}

void Model::receiveRequestChessMapUpdateEnpassant(const Position& _start, const Position& _end)
{
    m_PreviousMoveStart = _start;
    m_PreviousMoveEnd = _end;
    auto enemy = _end;
    if (_start.row == Row::Five)
        enemy.prevRow();
    else if (_start.row == Row::Four)
        enemy.nextRow();
    else
        Q_UNREACHABLE();

    m_Pieces.erase(std::remove_if(m_Pieces.begin(), m_Pieces.end(),
        [&](const Piece* p) {
            return p->getPosition() == enemy;
        }), m_Pieces.end());
    auto* pawn = m_ChessMap.at(_start);
    m_ChessMap.at(enemy) = nullptr;
    std::swap(m_ChessMap.at(pawn->getPosition()), m_ChessMap.at(_end));
    pawn->changePosition(_end);
    updateChessBoard();
    emit requestToRecordNormalCapture(getAlgebraicNotationNormalCapture(_start, _end));
    const auto player_color = !pawn->getColor();
    if (isCheckMate(player_color))
        emit requestToRecordCheckMate(player_color);
    else if (isKingInCheck(player_color))
        emit requestToRecordKingJaque(player_color);
    else if (isStaleMate(player_color))
        ;
}

void Model::receiveRequestChessMapUpdatePawnPromotion(const Position& _start, const Position& _end)
{
    m_PreviousMoveStart = _start;
    m_PreviousMoveEnd = _end;
    if (const auto& piece_on_square = m_ChessMap.at(_end))
    {
        // Pawn Captures Into Last Rank
        m_Pieces.erase(std::remove_if(m_Pieces.begin(), m_Pieces.end(),
            [&](const Piece* p) {
                return p->getPosition() == _end;
            }), m_Pieces.end());
        auto* pieceToMove = m_ChessMap.at(_start);
        m_ChessMap.at(_end) = nullptr;
        std::swap(m_ChessMap.at(pieceToMove->getPosition()), m_ChessMap.at(_end));
        pieceToMove->changePosition(_end);
        updateChessBoard();
        emit requestSetPromoteRenderer(_end);
        emit requestToRecordNormalCapture(getAlgebraicNotationNormalCapture(_start, _end));
    }
    else 
    {
        // Pawn Moves Into Last Rank
        auto* pieceToMove = m_ChessMap.at(_start);
        std::swap(m_ChessMap.at(pieceToMove->getPosition()), m_ChessMap.at(_end));
        pieceToMove->changePosition(_end);
        updateChessBoard();
        emit requestSetPromoteRenderer(_end);
        emit requestToRecordNormalMove(getAlgebraicNotationNormalMove(_start, _end));
    }
}

void Model::receiveRequestPawnPromoteToQueen(const Position& _pawn_pos)
{
    const auto pawn_color = m_ChessMap.at(_pawn_pos)->getColor();
    m_Pieces.erase(std::remove_if(m_Pieces.begin(), m_Pieces.end(),
        [&](const Piece* p) {
            return p->getPosition() == _pawn_pos;
        }), m_Pieces.end());
    m_ChessMap.at(_pawn_pos) = nullptr;
    m_Pieces.push_back(new Queen(_pawn_pos, pawn_color, m_Logger));
    m_ChessMap.at(m_Pieces.back()->getPosition()) = m_Pieces.back();
    updateChessBoard();
    emit requestToRecordPawnPromotion(getAlgebraicNotationPawnPromotion(_pawn_pos), pawn_color);
    const auto player_color = !pawn_color;
    if (isCheckMate(player_color))
        emit requestToRecordCheckMate(player_color);
    else if (isKingInCheck(player_color))
        emit requestToRecordKingJaque(player_color);
    else if (isStaleMate(player_color))
        ;
}

void Model::receiveRequestPawnPromoteToKnight(const Position& _pawn_pos)
{
    const auto pawn_color = m_ChessMap.at(_pawn_pos)->getColor();
    m_Pieces.erase(std::remove_if(m_Pieces.begin(), m_Pieces.end(),
        [&](const Piece* p) {
            return p->getPosition() == _pawn_pos;
        }), m_Pieces.end());
    m_ChessMap.at(_pawn_pos) = nullptr;
    m_Pieces.push_back(new Knight(_pawn_pos, pawn_color, m_Logger));
    m_ChessMap.at(m_Pieces.back()->getPosition()) = m_Pieces.back();
    updateChessBoard();
    emit requestToRecordPawnPromotion(getAlgebraicNotationPawnPromotion(_pawn_pos), pawn_color);
    const auto player_color = !pawn_color;
    if (isCheckMate(player_color))
        emit requestToRecordCheckMate(player_color);
    else if (isKingInCheck(player_color))
        emit requestToRecordKingJaque(player_color);
    else if (isStaleMate(player_color))
        ;
}

void Model::receiveRequestPawnPromoteToRook(const Position& _pawn_pos)
{
    const auto pawn_color = m_ChessMap.at(_pawn_pos)->getColor();
    m_Pieces.erase(std::remove_if(m_Pieces.begin(), m_Pieces.end(),
        [&](const Piece* p) {
            return p->getPosition() == _pawn_pos;
        }), m_Pieces.end());
    m_ChessMap.at(_pawn_pos) = nullptr;
    m_Pieces.push_back(new Rook(_pawn_pos, pawn_color, m_Logger));
    m_ChessMap.at(m_Pieces.back()->getPosition()) = m_Pieces.back();
    updateChessBoard();
    emit requestToRecordPawnPromotion(getAlgebraicNotationPawnPromotion(_pawn_pos), pawn_color);
    const auto player_color = !pawn_color;
    if (isCheckMate(player_color))
        emit requestToRecordCheckMate(player_color);
    else if (isKingInCheck(player_color))
        emit requestToRecordKingJaque(player_color);
    else if (isStaleMate(player_color))
        ;
}

void Model::receiveRequestPawnPromoteToBishop(const Position& _pawn_pos)
{
    const auto pawn_color = m_ChessMap.at(_pawn_pos)->getColor();
    m_Pieces.erase(std::remove_if(m_Pieces.begin(), m_Pieces.end(),
        [&](const Piece* p) {
            return p->getPosition() == _pawn_pos;
        }), m_Pieces.end());
    m_ChessMap.at(_pawn_pos) = nullptr;
    m_Pieces.push_back(new Bishop(_pawn_pos, pawn_color, m_Logger));
    m_ChessMap.at(m_Pieces.back()->getPosition()) = m_Pieces.back();
    updateChessBoard();
    emit requestToRecordPawnPromotion(getAlgebraicNotationPawnPromotion(_pawn_pos), pawn_color);
    const auto player_color = !pawn_color;
    if (isCheckMate(player_color))
        emit requestToRecordCheckMate(player_color);
    else if (isKingInCheck(player_color))
        emit requestToRecordKingJaque(player_color);
    else if (isStaleMate(player_color))
        ;
}

void Model::updateChessBoard()
{
    const auto chessmap_const = getChessMapConst();
    for (const auto& [ pos, piece ] : chessmap_const)
    {
        if (piece)
            emit requestSetPieceOnChessBoard(pos, piece->getColor(), piece->getType());
        else 
            emit requestRemovePieceFromChessBoard(pos);
    }

    for (const auto& [ pos, piece ] : chessmap_const)
        emit requestSetDefaultSquareRenderer(pos);

    if (m_PreviousMoveStart.has_value())
        emit requestSetPreviousMoveRenderer(*m_PreviousMoveStart);
    if (m_PreviousMoveEnd.has_value())
        emit requestSetPreviousMoveRenderer(*m_PreviousMoveEnd);

    const auto& arbitrary_piece = m_Pieces.front();
    if (arbitrary_piece->isKingInCheck(getChessMapConst(), Color::White))
        emit requestSetKingJaqueRenderer(arbitrary_piece->findKing(getChessMapConst(), Color::White));
    if (arbitrary_piece->isKingInCheck(getChessMapConst(), Color::Black))
        emit requestSetKingJaqueRenderer(arbitrary_piece->findKing(getChessMapConst(), Color::Black));
}

bool Model::isCheckMate(const Color& _kcolor) const
{
    if (!isKingInCheck(_kcolor))
        return false;

    const auto valid_moves = [ this, _kcolor ] () -> bool {
        for (const auto& [ pos, piece ] : m_ChessMap)
        {
            if (piece)
            {
                if (piece->getColor() == _kcolor)
                {
                    if (!piece->getValidSquares(getChessMapConst()).isEmpty())
                        return true;
                }
            }
        }

        return false;
    }();

    return !valid_moves;
}

bool Model::isKingInCheck(const Color& _kcolor) const
{
    const auto enemy_color = !_kcolor;

    const auto kpos = [ this, _kcolor ] () -> Position {
        for (const auto& [ pos, piece ] : m_ChessMap)
        {
            if (piece)
            {
                if (piece->getColor() == _kcolor and piece->getType() == Type::K)
                    return pos;
            }
        }

        Q_UNREACHABLE();
    }();

    const auto enemy_attacks = [ this, enemy_color ] () -> QList<Position> {
        auto list_of_attacks = QList<Position>();
        for (const auto& [ pos, piece ] : m_ChessMap)
        {
            if (piece)
            {
                if (piece->getColor() == enemy_color)
                    list_of_attacks += piece->protectedSquares(getChessMapConst());
            }
        }

        return list_of_attacks;
    }();

    return enemy_attacks.contains(kpos);
}

bool Model::isStaleMate(const Color& _player_color) const
{
    if (isKingInCheck(_player_color))
        return false;
    if (isCheckMate(_player_color))
        return false;

    const auto has_valid_moves = [ this, _player_color ] () -> bool {
        for (const auto& [ pos, piece ] : m_ChessMap)
        {
            if (piece)
            {
                if (piece->getColor() == _player_color)
                {
                    if (!piece->protectedSquares(getChessMapConst()).isEmpty())
                        return true;
                }
            }
        }

        return false;
    }();

    return !has_valid_moves;
}

QString Model::getAlgebraicNotationNormalMove(const Position& _previous_square, const Position& _current_square)
{
    QString algebraic;

    const auto* piece = m_ChessMap.at(_current_square);

    if (!piece)
        Q_UNREACHABLE();

    algebraic += QChar(piece->getUnicode());

    if (piece->getType() == Type::P)
        algebraic.removeFirst();

    std::unordered_map<Type, int> num_types = {
        { Type::P , 0 },
        { Type::N , 0 },
        { Type::B , 0 },
        { Type::R , 0 },
        { Type::Q , 0 },
        { Type::K , 0 },
    };

    for (const auto [ square , p ] : m_ChessMap)
    {
        if (!p)
            continue;
        if (p->getColor() != piece->getColor())
            continue;

        num_types.at(p->getType()) += 1;
    }

    if (num_types.at(piece->getType()) > 1 &&
        piece->getType() != Type::P &&
        piece->getType() != Type::K)
    {
        // TODO: What about when you have more than 2 of the same type.
        const auto* other_piece = [ this, &piece, _previous_square ] () -> const Piece* {
            const Piece* other_p = nullptr;
            for (const auto& [ square , p ] : m_ChessMap)
            {
                if (!p)
                    continue;
                if (p->getColor() != piece->getColor())
                    continue;
                if (p->getPosition() == _previous_square)
                    continue;

                if (p->getType() == piece->getType())
                    other_p = p;
            }

            return other_p;
        }();

        Q_ASSERT(other_piece != nullptr);

        const auto other_piece_valid_moves = other_piece->protectedSquares(getChessMapConst());
        const auto differing_row_or_col = [ &other_piece, &_previous_square ] () -> QString {
            const auto a = other_piece->getPosition().toStr();
            const auto b = _previous_square.toStr();
            if (a.front() != b.front())
                return b.front();
            else if (a.back() != b.back())
                return b.back();
            else
                Q_UNREACHABLE();
        }();

        if (other_piece_valid_moves.contains(_current_square))
            algebraic += differing_row_or_col;
    }

    algebraic += _current_square.toStr();

    return algebraic;
}

QString Model::getAlgebraicNotationNormalCapture(const Position& _previous_square, const Position& _current_square)
{
    QString algebraic;

    const auto* piece = m_ChessMap.at(_current_square);

    algebraic += QChar(piece->getUnicode());

    if (piece->getType() == Type::P)
        algebraic.front() = _previous_square.toStr().front();

    std::unordered_map<Type, int> num_types = {
        { Type::P , 0 },
        { Type::N , 0 },
        { Type::B , 0 },
        { Type::R , 0 },
        { Type::Q , 0 },
        { Type::K , 0 },
    };

    for (const auto [ square , p ] : m_ChessMap)
    {
        if (!p)
            continue;
        if (p->getColor() != piece->getColor())
            continue;

        num_types.at(p->getType()) += 1;
    }

    if (num_types.at(piece->getType()) > 1 &&
        piece->getType() != Type::P &&
        piece->getType() != Type::K)
    {
        const auto* other_piece = [ this, &piece, _previous_square ] () -> const Piece* {
            const Piece* other_p = nullptr;
            for (const auto& [ square , p ] : m_ChessMap)
            {
                if (!p)
                    continue;
                if (p->getColor() != piece->getColor())
                    continue;
                if (p->getPosition() == _previous_square)
                    continue;

                if (p->getType() == piece->getType())
                    other_p = p;
            }

            return other_p;
        }();

        Q_ASSERT(other_piece != nullptr);

        const auto other_piece_valid_moves = other_piece->protectedSquares(getChessMapConst());
        const auto differing_row_or_col = [ &other_piece, _previous_square ] () -> QString {
            const auto a = other_piece->getPosition().toStr();
            const auto b = _previous_square.toStr();
            if (a.front() != b.front())
                return b.front();
            else if (a.back() != b.back())
                return b.back();
            else
                Q_UNREACHABLE();
        }();

        if (other_piece_valid_moves.contains(_current_square))
            algebraic += differing_row_or_col;
    }

    algebraic += QString("x%1").arg(_current_square.toStr());

    return algebraic;
}

QString Model::getAlgebraicNotationCastling(const Position& _queen_or_king_side)
{
    QString algebraic;

    if (_queen_or_king_side.col == Col::G)
        algebraic += "O-O";
    else if (_queen_or_king_side.col == Col::C)
        algebraic += "O-O-O";
    else 
        Q_UNREACHABLE();


    return algebraic;
}

QString Model::getAlgebraicNotationPawnPromotion(const Position& _promoted_piece)
{
    QString algebraic;

    const auto* piece = m_ChessMap.at(_promoted_piece);

    algebraic += QChar(piece->getUnicode());

    return algebraic;
}

