#ifndef PIECE_H
#define PIECE_H

#include "Position.hpp"
#include <QList>

class Logger;

class Piece
{
public:

    using ChessMapConst = const std::unordered_map<Position, const Piece*>;

    Piece(const Position&, const Color&, const Type&, Logger&);
    virtual ~Piece() = default;
    Position getPosition() const;
    Color getColor() const;
    Type getType() const;
    uint16_t getUnicode() const;
    void changePosition(const Position&);
    virtual QList<Position> legalMoves(ChessMapConst&) const = 0;
    virtual QList<Position> legalCaptures(ChessMapConst&) const = 0;
    virtual QList<Position> protectedSquares(ChessMapConst&) const = 0;
    QList<Position> getValidSquares(ChessMapConst&) const;
    QList<Position> getPinnedPieceMoves(ChessMapConst&) const;
    QList<Position> getLineOfAttack(ChessMapConst&) const;
    bool isPiecePinned(ChessMapConst&) const;
    bool isKingInCheck(ChessMapConst&, const Color&) const;
    Position findKing(ChessMapConst&, const Color&) const;
    bool hasPieceMoved() const;

    // debugging purposes
    QString toStr() const;
    friend QDebug operator<<(QDebug, const Piece&);

private:

    QList<Position> identifyKingAttackers(ChessMapConst&, const Color&) const;
    int howManyKingAttackers(ChessMapConst&) const;
    Position findKingAttacker(ChessMapConst&) const;

protected:

    Position m_Pos;
    const Color m_Color;
    Type m_Type;
    inline static std::pair<Position, Position> m_PreviousMove = { Position(Col::A, Row::One), Position(Col::A, Row::One )};
    bool m_PieceMoved = false;
    Logger& m_Logger;
};

#endif

