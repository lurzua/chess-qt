#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.hpp"

class Bishop : public Piece
{

public:

    Bishop(const Position&, const Color&);
    ~Bishop() = default;
    QList<Position> legalMoves(ChessMapConst&) const override;
    QList<Position> legalCaptures(ChessMapConst&) const override;
    QList<Position> protectedSquares(ChessMapConst&) const override;

};

#endif // BISHOP_H
