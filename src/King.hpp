#ifndef KING_H
#define KING_H

#include "Piece.hpp"

class King : public Piece
{

public:

    King(const Position&, const Color&, Logger&);
    ~King() = default;
    QList<Position> legalMoves(ChessMapConst&) const override;
    QList<Position> legalCaptures(ChessMapConst&) const override;
    QList<Position> protectedSquares(ChessMapConst&) const override;
    bool isEnemyProtected(ChessMapConst&, const Position&) const;
};

#endif // KING_H
