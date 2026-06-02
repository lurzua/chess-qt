#ifndef PAWN_H
#define PAWN_H

#include "Piece.hpp"

class Pawn : public Piece
{

public:

    Pawn(const Position&, const Color&);
    ~Pawn() = default;
    QList<Position> legalMoves(ChessMapConst&) const override;
    QList<Position> legalCaptures(ChessMapConst&) const override;
    QList<Position> protectedSquares(ChessMapConst&) const override;
};

#endif // PAWN_H
