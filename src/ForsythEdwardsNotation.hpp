#ifndef FORSYTH_EDWARDS_NOTATION_H
#define FORSYTH_EDWARDS_NOTATION_H

#include <QString>

// Total FEN Fields: 6
// { Piece Placement }, { Active Color }, { Castling Rights }, { Enpassant Target Square }, { Halfmove Clock }, { Fullmove Clock };

bool isValidFEN(const QString&);
bool is50MoveRuleTriggered(const QString&);

#endif

