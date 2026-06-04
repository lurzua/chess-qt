#ifndef ROOK_H
#define ROOK_H

#include "Piece.hpp"

class Rook : public Piece
{

public:

    Rook(const Position&, const Color&);
    ~Rook() = default;
    QList<Position> legalMoves(ChessMapConst&) const override;
    QList<Position> legalCaptures(ChessMapConst&) const override;
    QList<Position> protectedSquares(ChessMapConst&) const override;
};

#endif // ROOK_H
