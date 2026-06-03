#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.hpp"

class Queen : public Piece
{

public:

    Queen(const Position&, const Color&, Logger&);
    ~Queen() = default;
    QList<Position> legalMoves(ChessMapConst&) const override;
    QList<Position> legalCaptures(ChessMapConst&) const override;
    QList<Position> protectedSquares(ChessMapConst&) const override;

private:

    QList<Position> bishopMoves(ChessMapConst&) const;
    QList<Position> rookMoves(ChessMapConst&) const;
    QList<Position> bishopCaptures(ChessMapConst&) const;
    QList<Position> rookCaptures(ChessMapConst&) const;
    QList<Position> bishopProtectedSquares(ChessMapConst&) const;
    QList<Position> rookProtectedSquares(ChessMapConst&) const;

};

#endif // QUEEN_H
