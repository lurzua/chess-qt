#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.hpp"

class Knight : public Piece
{

public:

    Knight(const Position&, const Color&);
    ~Knight() = default;
    QList<Position> legalMoves(ChessMapConst&) const override;
    QList<Position> legalCaptures(ChessMapConst&) const override;
    QList<Position> protectedSquares(ChessMapConst&) const override;
};

#endif // KNIGHT_H
