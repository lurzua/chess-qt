#include "ForsythEdwardsNotation.hpp"
#include <QRegularExpression>

static const auto g_DefaultFen = QString("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

ForsythEdwardsNotation::ForsythEdwardsNotation(const QString& _fen)
    : m_FenNotation(_fen)
{
    setupChessMap();
}

/*
 * Returns true iff the following conditions are met:
 *      1. There MAY be a maximum of 16 white pieces and 16 black pieces for a total of 32 pieces.
 *      2. The king (non active color) MAY NOT be in check if it is not its turn.
 *      3. The king (active color) MAY NOT be attacked by more than 2 enemy pieces.
 *      4. There MAY be only one 'K' and one 'k'.
 *      5. Pawns MAY NOT be in the 8th and 1st ranks.
 */
void ForsythEdwardsNotation::setupChessMap()
{
    // Check 1 whitespace character between each field for a total of 5 whitespace characters and 6 fields
    static const QRegularExpression re(R"(^\S+(\s\S+){5}$)");

    if (re.match(m_FenNotation).hasMatch())
        return ;

    const auto fen_fields = m_FenNotation.split(' ');
    if (fen_fields.size() != 6)
        return ;

    Q_ASSERT(fen_fields.size() == 6);
    const auto piece_placement         = fen_fields.at(0);
    const auto active_color            = fen_fields.at(1);
    const auto castling_rights         = fen_fields.at(2);
    const auto enpassant_target_square = fen_fields.at(3);
    const auto half_move_clock         = fen_fields.at(4);
    const auto full_move_clock         = fen_fields.at(5);

    if (!isPiecePlacementSyntaxValid(piece_placement))
        return ;
    if (!isActiveColorSyntaxValid(active_color))
        return ;
    if (!isFullMoveClockSyntaxValid(full_move_clock))
        return ;
    if (!isCastlingRightsSyntaxValid(castling_rights))
        return ;
    if (!isEnpassantTargetSquareSyntaxValid(enpassant_target_square))
        return ;
    if (!isHalfMoveClockSyntaxValid(half_move_clock))
        return ;
    if (isPawnOnFirstOrLastRank(piece_placement))
        return ;
    if (!isOneKingOnly(piece_placement))
        return ;
    if (!isCorrectNumberOfPieces(piece_placement))
        return ;

    // parse and define m_BoardMap;
    using X = XY::X;
    using Y = XY::Y;
    const auto starting_xy = XY(X::_A, Y::_8);
    const auto rows = piece_placement.split('/');
    auto current_xy = starting_xy;
    for (const auto& row : rows)
    {
        for (const auto& square : row)
        {
            if (square.isDigit())
            {
                const auto num_empty_squares = square.toLatin1() - '0';
                for (auto i = 0; i < num_empty_squares; i++)
                {
                    m_BoardMap.insert({ current_xy, std::nullopt });
                }
            }
            else if (square.isLetter())
            {
                const auto piece_color = (square.isUpper()) ? PieceColor::White : PieceColor::Black;
                if (square.toLatin1() == 'P' or square.toLatin1() == 'p')
                    m_BoardMap.insert({ current_xy, std::make_tuple(piece_color, PieceType::Pawn) });
                else if (square.toLatin1() == 'N' or square.toLatin1() == 'n')
                    m_BoardMap.insert({ current_xy, std::make_tuple(piece_color, PieceType::Knight) });
                else if (square.toLatin1() == 'B' or square.toLatin1() == 'b')
                    m_BoardMap.insert({ current_xy, std::make_tuple(piece_color, PieceType::Bishop) });
                else if (square.toLatin1() == 'R' or square.toLatin1() == 'r')
                    m_BoardMap.insert({ current_xy, std::make_tuple(piece_color, PieceType::Rook) });
                else if (square.toLatin1() == 'Q' or square.toLatin1() == 'q')
                    m_BoardMap.insert({ current_xy, std::make_tuple(piece_color, PieceType::Queen) });
                else if (square.toLatin1() == 'K' or square.toLatin1() == 'k')
                    m_BoardMap.insert({ current_xy, std::make_tuple(piece_color, PieceType::King) });
                else 
                    Q_UNREACHABLE();

                current_xy.nextX();
            }
            else 
                Q_UNREACHABLE();
        }

        current_xy.m_X = X::_A;
        current_xy.prevY();
    }

    if (isIncorrectKingInCheck(active_color))
        m_BoardMap.clear();
    if (howManyKingAttackers((active_color.toLatin1() == 'w') ? PieceColor::White : PieceColor::Black) > 2)
        m_BoardMap.clear();
}

bool ForsythEdwardsNotation::isValid() const
{
    return isFenLegal();
}

bool ForsythEdwardsNotation::updateFen(const QString& _fen)
{
    m_BoardMap.clear();
    m_FenNotation = _fen;
    return false;
}

/*
 * Returns true iff the following conditions are met:
 *      1. Contains exactly 7 forward slashes '/'.
 *      2. Contains integers 1 through 8 inclusive.
 *      3. Contains letters P, N, B, R, Q, K and their lowercase counterparts as well as the aforementioned characters.
 *      4. Each row is separated by one forward slash.
 *      5. The number of squares + pieces in each row must add up to 8.
 */
bool ForsythEdwardsNotation::isPiecePlacementSyntaxValid(const QString& _piece_placement) const
{
    static const QRegularExpression re(R"(^[PNBRQKpnbrqk/1-8]+$)");

    if (!re.match(_piece_placement).hasMatch())
        return false;
    
    if (_piece_placement.count('/') != 7)
        return false;

    auto count = 0;
    for (const auto& current_char : _piece_placement)
    {
        if (current_char.isDigit())
            count += (current_char.toLatin1() - '0');
        else if (current_char.isLetter())
            count++;
        else if (current_char.toLatin1() == '/')
        {
            if (count != 8)
                return false;
            else
                count = 0;
        }
    }
    // Check last row in string (or first rank in board)
    if (count != 8)
        return false;

    return true;
}

/*
 * ActiveColor is either White ('w') or Black ('b').
 */
bool ForsythEdwardsNotation::isActiveColorSyntaxValid(const QString& _active_color) const
{
    static const auto w_char = 'w';
    static const auto b_char = 'b';

    if (_active_color.size() > 1)
        return false;

    const auto w_or_b = _active_color.front();
    if (w_or_b.toLatin1() != w_char && w_or_b.toLatin1() != b_char)
        return false;

    return true;
}

/*
 * Returns true iff the following conditions are met:
 *      1. String must be between 1 and 4 (inclusive) characters long.
 *      2. String must contain any of the following characters: K, Q, k, q.
 *      3. A dash '-' is allowed iff it's the only character in the string.
 *      4. Each character may appear only once.
 *      5. Characters must be in the following order: KQkq.
 */
bool ForsythEdwardsNotation::isCastlingRightsSyntaxValid(const QString& _castling_rights) const
{
    static const QRegularExpression re(R"(^(?:-|(?=.+)K?Q?k?q?)$)");

    return re.match(_castling_rights).hasMatch();
}

/*
 * Returns true iff the following conditions are met:
 *      1. String must be between 1 and 2 (inclusive) characters long.
 *      2. If string is 1 char long, then it must be the dash character '-'.
 *      3. If string is 2 char long, then it must be a valid enpassant square destination (i.e. A3, B3, C3, or, A6, B6, C6 etc).
 */
bool ForsythEdwardsNotation::isEnpassantTargetSquareSyntaxValid(const QString& _enpassant_target_square) const
{
    static const QRegularExpression re(R"(^(?:-|[a-h][36])$)");

    return re.match(_enpassant_target_square).hasMatch();
}

/*
 * Returns true iff the following conditions are met:
 *      1. String must contain a positive integer greater than or equal to 0.
 *      2. The positive integer must not exceed 100. (100 half moves is 50 moves without pushing a pawn or capturing a piece).
 */
bool ForsythEdwardsNotation::isHalfMoveClockSyntaxValid(const QString& _half_move_clock) const
{
    static const QRegularExpression re(R"(^(100|[1-9]?[0-9])$)");

    return re.match(_half_move_clock).hasMatch();
}

/*
 * Returns true iff the following conditions are met:
 *      1. String must contain a positive integer greater than or equal to 1.
 *      2. The positive integer must not exceed 500. (This maximum limit is twice longer than the longest recorded game).
 */
bool ForsythEdwardsNotation::isFullMoveClockSyntaxValid(const QString& _full_move_clock) const
{
    static const QRegularExpression re(R"(^(500|[1-9][0-9]{0,1}|[1-4][0-9]{2})$)");

    return re.match(_full_move_clock).hasMatch();
}

/*
 * Returns true iff the following conditions are met:
 *      1. There MAY be a maximum of 16 white pieces and 16 black pieces for a total of 32 pieces.
 *      2. The king MAY NOT be in check if it is not its turn.
 *      3. The king MAY NOT be attacked by more than 2 enemy pieces.
 *      4. There MAY be only one 'K' and one 'k'.
 *      5. Pawns MAY NOT be in the 8th and 1st ranks.
 */
bool ForsythEdwardsNotation::isFenLegal() const
{
    // Check 1 whitespace character between each field for a total of 5 whitespace characters and 6 fields
    static const QRegularExpression re(R"(^\S+(\s\S+){5}$)");

    if (re.match(m_FenNotation).hasMatch())
        return false;

    const auto fen_fields = m_FenNotation.split(' ');
    if (fen_fields.size() != 6)
        return false;

    Q_ASSERT(fen_fields.size() == 6);
    const auto piece_placement         = fen_fields.at(0);
    const auto active_color            = fen_fields.at(1);
    const auto castling_rights         = fen_fields.at(2);
    const auto enpassant_target_square = fen_fields.at(3);
    const auto half_move_clock         = fen_fields.at(4);
    const auto full_move_clock         = fen_fields.at(5);

    if (!isPiecePlacementSyntaxValid(piece_placement))
        return false;
    if (!isActiveColorSyntaxValid(active_color))
        return false;
    if (!isFullMoveClockSyntaxValid(full_move_clock))
        return false;
    if (!isCastlingRightsSyntaxValid(castling_rights))
        return false;
    if (!isEnpassantTargetSquareSyntaxValid(enpassant_target_square))
        return false;
    if (!isHalfMoveClockSyntaxValid(half_move_clock))
        return false;

    // Check Board Legality.
    if (isPawnOnFirstOrLastRank(piece_placement))
        return false;
    if (!isOneKingOnly(piece_placement))
        return false;
    if (!isCorrectNumberOfPieces(piece_placement))
        return false;

    return true;
}

bool ForsythEdwardsNotation::isPawnOnFirstOrLastRank(const QString& _piece_placement) const
{
    const auto rows = _piece_placement.split('/');
    const auto first_row = rows.front();
    for (const auto& current_col : first_row)
    {
        if (current_col.toLatin1() == 'P' or current_col.toLatin1() == 'p')
            return true;
    }

    const auto last_row = rows.back();
    for (const auto& current_col : last_row)
    {
        if (current_col.toLatin1() == 'P' or current_col.toLatin1() == 'p')
            return true;
    }

    return false;
}

bool ForsythEdwardsNotation::isOneKingOnly(const QString& _piece_placement) const
{
    const auto rows = _piece_placement.split('/');
    auto wkings = 0;
    auto bkings = 0;
    for (const auto& r : rows)
    {
        if (r.contains('K'))
            wkings++;
        else if (r.contains('k'))
            bkings++;
    }

    if (wkings > 1)
        return false;

    if (bkings > 1)
        return false;

    return wkings == bkings == 1;
}

bool ForsythEdwardsNotation::isCorrectNumberOfPieces(const QString& _piece_placement) const
{
    const auto rows = _piece_placement.split('/');
    auto wpieces = 0;
    auto bpieces = 0;
    for (const auto& r : rows)
    {
        if (r.contains('P') or r.contains('N') or r.contains('B') or r.contains('R') or r.contains('Q') or r.contains('K'))
            wpieces++;
        else if (r.contains('p') or r.contains('n') or r.contains('b') or r.contains('r') or r.contains('q') or r.contains('k'))
            bpieces++;
    }

    if (wpieces > 16)
        return false;
    if (wpieces < 1)
        return false;
    if (bpieces > 16)
        return false;
    if (bpieces < 1)
        return false;

    return true;
}

bool ForsythEdwardsNotation::isIncorrectKingInCheck(const QString& _w_or_b) const
{
    // If it's white's turn to play, black king CANNOT be in check.
    if (_w_or_b.toLatin1() == 'w')
        return isKingInCheck(PieceColor::Black);
    else if (_w_or_b.toLatin1() == 'b')
        return isKingInCheck(PieceColor::White);
    else 
        Q_UNREACHABLE();
}

bool XY::operator==(const XY& _xy) const
{
    return m_X == _xy.m_X and m_Y == _xy.m_Y;
}

bool XY::operator!=(const XY& _xy) const
{
    return m_X != _xy.m_X or m_Y != _xy.m_Y;
}

bool XY::operator<=(const X& _x) const
{
    return m_X <= _x;
}

bool XY::operator<=(const Y& _y) const
{
    return m_Y <= _y;
}

void XY::nextX()
{
    switch (m_X)
    {
    case X::_A:
        m_X = X::_B;
        break;
    case X::_B:
        m_X = X::_C;
        break;
    case X::_C:
        m_X = X::_D;
        break;
    case X::_D:
        m_X = X::_E;
        break;
    case X::_E:
        m_X = X::_F;
        break;
    case X::_F:
        m_X = X::_G;
        break;
    case X::_G:
        m_X = X::_H;
        break;
    case X::_H:
        m_X = X::_H;
        break;
    }
}

void XY::nextY()
{
    switch (m_Y)
    {
    case Y::_1:
        m_Y = Y::_2;
        break;
    case Y::_2:
        m_Y = Y::_3;
        break;
    case Y::_3:
        m_Y = Y::_4;
        break;
    case Y::_4:
        m_Y = Y::_5;
        break;
    case Y::_5:
        m_Y = Y::_6;
        break;
    case Y::_6:
        m_Y = Y::_7;
        break;
    case Y::_7:
        m_Y = Y::_8;
        break;
    case Y::_8:
        m_Y = Y::_8;
        break;
    }
}

void XY::prevX()
{
    switch (m_X)
    {
    case X::_A:
        m_X = X::_A;
        break;
    case X::_B:
        m_X = X::_A;
        break;
    case X::_C:
        m_X = X::_B;
        break;
    case X::_D:
        m_X = X::_C;
        break;
    case X::_E:
        m_X = X::_D;
        break;
    case X::_F:
        m_X = X::_E;
        break;
    case X::_G:
        m_X = X::_F;
        break;
    case X::_H:
        m_X = X::_G;
        break;
    }
}

void XY::prevY()
{
    switch (m_Y)
    {
    case Y::_1:
        m_Y = Y::_1;
        break;
    case Y::_2:
        m_Y = Y::_1;
        break;
    case Y::_3:
        m_Y = Y::_2;
        break;
    case Y::_4:
        m_Y = Y::_3;
        break;
    case Y::_5:
        m_Y = Y::_4;
        break;
    case Y::_6:
        m_Y = Y::_5;
        break;
    case Y::_7:
        m_Y = Y::_6;
        break;
    case Y::_8:
        m_Y = Y::_7;
        break;
    }
}

XY ForsythEdwardsNotation::findKing(const PieceColor& _kcolor) const
{
    for (const auto& [ xy, square ] : m_BoardMap)
    {
        if (square.has_value())
        {
            const auto& [ pcolor, ptype ] = square.value();
            if (pcolor == _kcolor and ptype == PieceType::King)
                return xy;
        }
    }

    Q_UNREACHABLE();
}

bool ForsythEdwardsNotation::isKingInCheck(const PieceColor& _kcolor) const
{
    return howManyKingAttackers(_kcolor) > 0;
}

uint8_t ForsythEdwardsNotation::howManyKingAttackers(const PieceColor& _kcolor) const
{
    using X = XY::X;
    using Y = XY::Y;
    const auto king_xy = findKing(_kcolor);
    auto num_attackers = 0;

    { // Check for Rook & Queen Attackers to the left of king
        for (auto current_xy_left = king_xy; current_xy_left.m_X > X::_A;)
        {
            current_xy_left.prevX();
            if (const auto& square = m_BoardMap.at(current_xy_left); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor == _kcolor)
                    break;
                if (ptype == PieceType::Rook or ptype == PieceType::Queen)
                    num_attackers++;
            }
        }
    }

    { // Check for Rook & Queen Attackers to the right of king 
        for (auto current_xy_right = king_xy; current_xy_right.m_X < X::_H;)
        {
            current_xy_right.nextX();
            if (const auto& square = m_BoardMap.at(current_xy_right); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor == _kcolor)
                    break;
                if (ptype == PieceType::Rook or ptype == PieceType::Queen)
                    num_attackers++;
            }
        }
    }

    { // Check for Rook & Queen Attackers to the north of king 
        for (auto current_xy_north = king_xy; current_xy_north.m_Y < Y::_8;)
        {
            current_xy_north.nextY();
            if (const auto& square = m_BoardMap.at(current_xy_north); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor == _kcolor)
                    break;
                if (ptype == PieceType::Rook or ptype == PieceType::Queen)
                    num_attackers++;
            }
        }
    }

    { // Check for Rook & Queen Attackers to the south of king
        for (auto current_xy_south = king_xy; current_xy_south.m_Y > Y::_1;)
        {
            current_xy_south.prevY();
            if (const auto& square = m_BoardMap.at(current_xy_south); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor == _kcolor)
                    break;
                if (ptype == PieceType::Rook or ptype == PieceType::Queen)
                    num_attackers++;
            }
        }
    }

    { // Check for Bishop & Queen Attackers to the North-West of king 
        for (auto current_xy_nw = king_xy; current_xy_nw.m_X > X::_A && current_xy_nw.m_Y < Y::_8;)
        {
            current_xy_nw.prevX();
            current_xy_nw.nextY();
            if (const auto& square = m_BoardMap.at(current_xy_nw); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor == _kcolor)
                    break;
                if (ptype == PieceType::Bishop or ptype == PieceType::Queen)
                    num_attackers++;
            }
        }
    }

    { // Check for Bishop & Queen Attackers to the North-East of king 
        for (auto current_xy_ne = king_xy; current_xy_ne.m_X < X::_H && current_xy_ne.m_Y < Y::_8;)
        {
            current_xy_ne.nextX();
            current_xy_ne.nextY();
            if (const auto& square = m_BoardMap.at(current_xy_ne); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor == _kcolor)
                    break;
                if (ptype == PieceType::Bishop or ptype == PieceType::Queen)
                    num_attackers++;
            }
        }
    }

    { // Check for Bishop & Queen Attackers to the South-East of king 
        for (auto current_xy_se = king_xy; current_xy_se.m_X < X::_H && current_xy_se.m_Y > Y::_1;)
        {
            current_xy_se.nextX();
            current_xy_se.prevY();
            if (const auto& square = m_BoardMap.at(current_xy_se); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor == _kcolor)
                    break;
                if (ptype == PieceType::Bishop or ptype == PieceType::Queen)
                    num_attackers++;
            }
        }
    }

    { // Check for Bishop & Queen Attackers to the South-West of king 
        for (auto current_xy_sw = king_xy; current_xy_sw.m_X > X::_A && current_xy_sw.m_Y > Y::_1;)
        {
            current_xy_sw.prevX();
            current_xy_sw.prevY();
            if (const auto& square = m_BoardMap.at(current_xy_sw); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor == _kcolor)
                    break;
                if (ptype == PieceType::Bishop or ptype == PieceType::Queen)
                    num_attackers++;
            }
        }
    }

    { // Check for Knight Attackers to the North of king
        if (auto current_xy_north_left = king_xy; current_xy_north_left.m_X > X::_A && current_xy_north_left.m_Y < Y::_7)
        {
            current_xy_north_left.prevX();
            current_xy_north_left.nextY();
            current_xy_north_left.nextY();
            if (const auto& square = m_BoardMap.at(current_xy_north_left); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor != _kcolor)
                {
                    if (ptype == PieceType::Bishop or ptype == PieceType::Queen)
                        num_attackers++;
                }
            }
        }
        
        if (auto current_xy_north_right = king_xy; current_xy_north_right.m_X < X::_H && current_xy_north_right.m_Y < Y::_7)
        {
            current_xy_north_right.nextX();
            current_xy_north_right.nextY();
            current_xy_north_right.nextY();
            if (const auto& square = m_BoardMap.at(current_xy_north_right); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor != _kcolor)
                {
                    if (ptype == PieceType::Bishop or ptype == PieceType::Queen)
                        num_attackers++;
                }
            }
        }
    }

    { // Check for Knight Attackers to the South of king 
        if (auto current_xy_south_left = king_xy; current_xy_south_left.m_X > X::_A && current_xy_south_left.m_Y > Y::_2)
        {
            current_xy_south_left.prevX();
            current_xy_south_left.prevY();
            current_xy_south_left.prevY();
            if (const auto& square = m_BoardMap.at(current_xy_south_left); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor != _kcolor)
                {
                    if (ptype == PieceType::Bishop or ptype == PieceType::Queen)
                        num_attackers++;
                }
            }
        }
 
        if (auto current_xy_south_right = king_xy; current_xy_south_right.m_X < X::_H && current_xy_south_right.m_Y > Y::_2)
        {
            current_xy_south_right.nextX();
            current_xy_south_right.prevY();
            current_xy_south_right.prevY();
            if (const auto& square = m_BoardMap.at(current_xy_south_right); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor != _kcolor)
                {
                    if (ptype == PieceType::Bishop or ptype == PieceType::Queen)
                        num_attackers++;
                }
            }
        }
    }

    { // Check for Knight Attackers to the West of king 
        if (auto current_xy_west_up = king_xy; current_xy_west_up.m_X > X::_B && current_xy_west_up.m_Y < Y::_8)
        {
            current_xy_west_up.prevX();
            current_xy_west_up.prevX();
            current_xy_west_up.nextY();
            if (const auto& square = m_BoardMap.at(current_xy_west_up); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor != _kcolor)
                {
                    if (ptype == PieceType::Bishop or ptype == PieceType::Queen)
                        num_attackers++;
                }
            }
        }
 
        if (auto current_xy_west_down = king_xy; current_xy_west_down.m_X > X::_B && current_xy_west_down.m_Y > Y::_1)
        {
            current_xy_west_down.prevX();
            current_xy_west_down.prevX();
            current_xy_west_down.prevY();
            if (const auto& square = m_BoardMap.at(current_xy_west_down); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor != _kcolor)
                {
                    if (ptype == PieceType::Bishop or ptype == PieceType::Queen)
                        num_attackers++;
                }
            }
        }
    }

    { // Check for Knight Attackers to the East of king 
        if (auto current_xy_east_up = king_xy; current_xy_east_up.m_X > X::_B && current_xy_east_up.m_Y < Y::_8)
        {
            current_xy_east_up.nextX();
            current_xy_east_up.nextX();
            current_xy_east_up.nextY();
            if (const auto& square = m_BoardMap.at(current_xy_east_up); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor != _kcolor)
                {
                    if (ptype == PieceType::Bishop or ptype == PieceType::Queen)
                        num_attackers++;
                }
            }
        }
 
        if (auto current_xy_east_down = king_xy; current_xy_east_down.m_X > X::_B && current_xy_east_down.m_Y > Y::_1)
        {
            current_xy_east_down.nextX();
            current_xy_east_down.nextX();
            current_xy_east_down.prevY();
            if (const auto& square = m_BoardMap.at(current_xy_east_down); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor != _kcolor)
                {
                    if (ptype == PieceType::Bishop or ptype == PieceType::Queen)
                        num_attackers++;
                }
            }
        }
    }

    { // Check for Pawn Attackers to the king
        if (_kcolor == PieceColor::White)
        {
            if (auto current_xy_left = king_xy; current_xy_left.m_X > X::_A && current_xy_left.m_Y < Y::_8)
            {
                current_xy_left.prevX();
                current_xy_left.nextY();
                if (const auto& square = m_BoardMap.at(current_xy_left); square.has_value())
                {
                    const auto& [ pcolor, ptype ] = square.value();
                    if (pcolor == PieceColor::Black and ptype == PieceType::Pawn)
                        num_attackers++;
                }
            }

            if (auto current_xy_right = king_xy; current_xy_right.m_X < X::_H && current_xy_right.m_Y < Y::_8)
            {
                current_xy_right.nextX();
                current_xy_right.nextY();
                if (const auto& square = m_BoardMap.at(current_xy_right); square.has_value())
                {
                    const auto& [ pcolor, ptype ] = square.value();
                    if (pcolor == PieceColor::Black and ptype == PieceType::Pawn)
                        num_attackers++;
                }
            }
        }
        else if (_kcolor == PieceColor::Black)
        {
            if (auto current_xy_left = king_xy; current_xy_left.m_X > X::_A && current_xy_left.m_Y > Y::_1)
            {
                current_xy_left.prevX();
                current_xy_left.prevY();
                if (const auto& square = m_BoardMap.at(current_xy_left); square.has_value())
                {
                    const auto& [ pcolor, ptype ] = square.value();
                    if (pcolor == PieceColor::White and ptype == PieceType::Pawn)
                        num_attackers++;
                }
            }

            if (auto current_xy_right = king_xy; current_xy_right.m_X < X::_H && current_xy_right.m_Y > Y::_1)
            {
                current_xy_right.nextX();
                current_xy_right.prevY();
                if (const auto& square = m_BoardMap.at(current_xy_right); square.has_value())
                {
                    const auto& [ pcolor, ptype ] = square.value();
                    if (pcolor == PieceColor::White and ptype == PieceType::Pawn)
                        num_attackers++;
                }
            }
        }
    }

    return num_attackers;
}

