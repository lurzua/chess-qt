#include "ForsythEdwardsNotation.hpp"
#include <QRegularExpression>

namespace 
{
    struct XY
    {
        enum class X { _A, _B, _C, _D, _E, _F, _G, _H };
        enum class Y { _1, _2, _3, _4, _5, _6, _7, _8 };
        X m_X;
        Y m_Y;
        XY() = delete;
        XY(const X& _x, const Y& _y) : m_X(_x), m_Y(_y) { };
        XY(const X&& _x, const Y&& _y) : m_X(_x), m_Y(_y) { };
        bool operator==(const XY&) const;
        bool operator!=(const XY&) const;
        bool operator<=(const X&) const;
        bool operator<=(const Y&) const;
        void nextX();
        void nextY();
        void prevX();
        void prevY();
        QString toStr() const;
        static constexpr std::array<X, 8> g_ArrayX = { X::_A, X::_B, X::_C, X::_D, X::_E, X::_F, X::_G, X::_H };
        static constexpr std::array<Y, 8> g_ArrayY = { Y::_1, Y::_2, Y::_3, Y::_4, Y::_5, Y::_6, Y::_7, Y::_8 };
    };

    enum class PieceColor { White, Black };
    enum class PieceType  { Pawn, Knight, Bishop, Rook, Queen, King };

    //static const auto g_DefaultFen = QString("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    bool isPiecePlacementSyntaxValid(const QString&);
    bool isActiveColorSyntaxValid(const QString&);
    bool isCastlingRightsSyntaxValid(const QString&);
    bool isEnpassantTargetSquareSyntaxValid(const QString&);
    bool isHalfMoveClockSyntaxValid(const QString&);
    bool isFullMoveClockSyntaxValid(const QString&);

    bool isFenLegal(const QString&);
    bool isPawnOnFirstOrLastRank(const QString&);
    bool isOneKingOnly(const QString&);
    bool isCorrectNumberOfPieces(const QString&);
    bool isIncorrectKingInCheck(const QString&);

    XY findKing(const PieceColor&);
    bool isKingInCheck(const PieceColor&);
    uint8_t howManyKingAttackers(const PieceColor&);
}

namespace std {
    template<>
    struct hash<::XY> {
        size_t operator()(const ::XY& _xy) const noexcept {
            size_t h1 = hash<int>{}(static_cast<int>(_xy.m_X));
            size_t h2 = hash<int>{}(static_cast<int>(_xy.m_Y));

            // Combine hashes (boost-style)
            return h1 ^ (h2 << 1);
        }
    };
}

static ::std::unordered_map<XY, ::std::optional<::std::tuple<PieceColor, PieceType>>> g_BoardMap;

/*
 * Returns true iff the following conditions are met:
 *      1. There MAY be a maximum of 16 white pieces and 16 black pieces for a total of 32 pieces.
 *      2. The king (non active color) MAY NOT be in check if it is not its turn.
 *      3. The king (active color) MAY NOT be attacked by more than 2 enemy pieces.
 *      4. There MAY be only one 'K' and one 'k'.
 *      5. Pawns MAY NOT be in the 8th and 1st ranks.
 */
bool isValidFEN(const QString& _fen)
{
    // Check 1 whitespace character between each field for a total of 5 whitespace characters and 6 fields
    static const QRegularExpression re(R"(^\S+(\s\S+){5}$)");

    if (!re.match(_fen).hasMatch())
        return false;


    const auto fen_fields = _fen.split(' ');
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

    if (isPawnOnFirstOrLastRank(piece_placement))
        return false;
    if (!isOneKingOnly(piece_placement))
        return false;
    if (!isCorrectNumberOfPieces(piece_placement))
        return false;


    g_BoardMap.clear();
    // parse and define g_BoardMap;
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
                const auto num_empty_squares = square.digitValue();
                for (auto i = 0; i < num_empty_squares; i++)
                {
                    g_BoardMap.insert({ current_xy, std::nullopt });
                    current_xy.nextX();
                }
            }
            else if (square.isLetter())
            {
                const auto piece_color = (square.isUpper()) ? PieceColor::White : PieceColor::Black;
                if (square.toLatin1() == 'P' or square.toLatin1() == 'p')
                    g_BoardMap.insert({ current_xy, std::make_tuple(piece_color, PieceType::Pawn) });
                else if (square.toLatin1() == 'N' or square.toLatin1() == 'n')
                    g_BoardMap.insert({ current_xy, std::make_tuple(piece_color, PieceType::Knight) });
                else if (square.toLatin1() == 'B' or square.toLatin1() == 'b')
                    g_BoardMap.insert({ current_xy, std::make_tuple(piece_color, PieceType::Bishop) });
                else if (square.toLatin1() == 'R' or square.toLatin1() == 'r')
                    g_BoardMap.insert({ current_xy, std::make_tuple(piece_color, PieceType::Rook) });
                else if (square.toLatin1() == 'Q' or square.toLatin1() == 'q')
                    g_BoardMap.insert({ current_xy, std::make_tuple(piece_color, PieceType::Queen) });
                else if (square.toLatin1() == 'K' or square.toLatin1() == 'k')
                    g_BoardMap.insert({ current_xy, std::make_tuple(piece_color, PieceType::King) });
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

    Q_ASSERT(g_BoardMap.size() == 64);

    if (isIncorrectKingInCheck(active_color))
    {
        // Wrong King is in check
        g_BoardMap.clear();
        return false;
    }
    if (howManyKingAttackers((active_color.toLatin1() == 'w') ? PieceColor::White : PieceColor::Black) > 2)
    {
        // Too many king attackers
        g_BoardMap.clear();
        return false;
    }

    return true;
}

bool is50MoveRuleTriggered(const QString& _fen)
{
    if (!isValidFEN(_fen))
        return false;

    // Check 1 whitespace character between each field for a total of 5 whitespace characters and 6 fields
    static const QRegularExpression re(R"(^\S+(\s\S+){5}$)");

    if (re.match(_fen).hasMatch())
        return false;

    const auto fen_fields = _fen.split(' ');
    if (fen_fields.size() != 6)
        return false;

    Q_ASSERT(fen_fields.size() == 6);
    const auto half_move_clock         = fen_fields.at(4);

    if (const auto num_half_moves = ::std::stoi(half_move_clock.toStdString()); num_half_moves >= 100)
        return true;
    else 
        return false;
}

namespace 
{
/*
 * Returns true iff the following conditions are met:
 *      1. Contains exactly 7 forward slashes '/'.
 *      2. Contains integers 1 through 8 inclusive.
 *      3. Contains letters P, N, B, R, Q, K and their lowercase counterparts as well as the aforementioned characters.
 *      4. Each row is separated by one forward slash.
 *      5. The number of squares + pieces in each row must add up to 8.
 */
bool isPiecePlacementSyntaxValid(const QString& _piece_placement)
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
bool isActiveColorSyntaxValid(const QString& _active_color)
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
bool isCastlingRightsSyntaxValid(const QString& _castling_rights)
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
bool isEnpassantTargetSquareSyntaxValid(const QString& _enpassant_target_square)
{
    static const QRegularExpression re(R"(^(?:-|[a-h][36])$)");

    return re.match(_enpassant_target_square).hasMatch();
}

/*
 * Returns true iff the following conditions are met:
 *      1. String must contain a positive integer greater than or equal to 0.
 *      2. The positive integer must not exceed 100. (100 half moves is 50 moves without pushing a pawn or capturing a piece).
 */
bool isHalfMoveClockSyntaxValid(const QString& _half_move_clock)
{
    static const QRegularExpression re(R"(^(100|[1-9]?[0-9])$)");

    return re.match(_half_move_clock).hasMatch();
}

/*
 * Returns true iff the following conditions are met:
 *      1. String must contain a positive integer greater than or equal to 1.
 *      2. The positive integer must not exceed 500. (This maximum limit is twice longer than the longest recorded game).
 */
bool isFullMoveClockSyntaxValid(const QString& _full_move_clock)
{
    static const QRegularExpression re(R"(^(500|[1-9][0-9]{0,1}|[1-4][0-9]{2})$)");

    return re.match(_full_move_clock).hasMatch();
}

bool isPawnOnFirstOrLastRank(const QString& _piece_placement)
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

bool isOneKingOnly(const QString& _piece_placement)
{
    const auto rows = _piece_placement.split('/');
    auto wkings = 0;
    auto bkings = 0;
    for (const auto& r : rows)
    {
        if (r.contains('K'))
            wkings++;
        if (r.contains('k'))
            bkings++;
    }

    if (wkings > 1)
        return false;

    if (bkings > 1)
        return false;

    return wkings == bkings == 1;
}

bool isCorrectNumberOfPieces(const QString& _piece_placement)
{
    const auto rows = _piece_placement.split('/');
    auto wpieces = 0;
    auto bpieces = 0;
    for (const auto& r : rows)
    {
        wpieces += r.count('P');
        wpieces += r.count('N');
        wpieces += r.count('B');
        wpieces += r.count('R');
        wpieces += r.count('Q');
        wpieces += r.count('K');

        bpieces += r.count('p');
        bpieces += r.count('n');
        bpieces += r.count('b');
        bpieces += r.count('r');
        bpieces += r.count('q');
        bpieces += r.count('k');
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

bool isIncorrectKingInCheck(const QString& _w_or_b)
{
    // If it's white's turn to play, black king CANNOT be in check.
    if (_w_or_b.toLatin1() == 'w')
        return isKingInCheck(PieceColor::Black);
    else if (_w_or_b.toLatin1() == 'b')
        return isKingInCheck(PieceColor::White);
    else 
        Q_UNREACHABLE();
}

XY findKing(const PieceColor& _kcolor)
{
    for (const auto& [ xy, square ] : g_BoardMap)
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

bool isKingInCheck(const PieceColor& _kcolor)
{
    return howManyKingAttackers(_kcolor) > 0;
}

uint8_t howManyKingAttackers(const PieceColor& _kcolor)
{
    using X = XY::X;
    using Y = XY::Y;
    const auto king_xy = findKing(_kcolor);
    auto num_attackers = 0;

    { // Check for Rook & Queen Attackers to the left of king
        for (auto current_xy_left = king_xy; current_xy_left.m_X > X::_A;)
        {
            current_xy_left.prevX();
            if (const auto& square = g_BoardMap.at(current_xy_left); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor != _kcolor)
                {
                    if (ptype == PieceType::Rook or ptype == PieceType::Queen)
                        num_attackers++;
                }

                break;
            }
        }
    }

    { // Check for Rook & Queen Attackers to the right of king 
        for (auto current_xy_right = king_xy; current_xy_right.m_X < X::_H;)
        {
            current_xy_right.nextX();
            if (const auto& square = g_BoardMap.at(current_xy_right); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor != _kcolor)
                {
                    if (ptype == PieceType::Rook or ptype == PieceType::Queen)
                        num_attackers++;
                }

                break;
            }
        }
    }

    { // Check for Rook & Queen Attackers to the north of king 
        for (auto current_xy_north = king_xy; current_xy_north.m_Y < Y::_8;)
        {
            current_xy_north.nextY();
            if (const auto& square = g_BoardMap.at(current_xy_north); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor != _kcolor)
                {
                    if (ptype == PieceType::Rook or ptype == PieceType::Queen)
                        num_attackers++;
                }

                break;
            }
        }
    }

    { // Check for Rook & Queen Attackers to the south of king
        for (auto current_xy_south = king_xy; current_xy_south.m_Y > Y::_1;)
        {
            current_xy_south.prevY();
            if (const auto& square = g_BoardMap.at(current_xy_south); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor != _kcolor)
                {
                    if (ptype == PieceType::Rook or ptype == PieceType::Queen)
                        num_attackers++;
                }

                break;
            }
        }
    }

    { // Check for Bishop & Queen Attackers to the North-West of king 
        for (auto current_xy_nw = king_xy; current_xy_nw.m_X > X::_A && current_xy_nw.m_Y < Y::_8;)
        {
            current_xy_nw.prevX();
            current_xy_nw.nextY();
            if (const auto& square = g_BoardMap.at(current_xy_nw); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor != _kcolor)
                {
                    if (ptype == PieceType::Bishop or ptype == PieceType::Queen)
                        num_attackers++;
                }

                break;
            }
        }
    }

    { // Check for Bishop & Queen Attackers to the North-East of king 
        for (auto current_xy_ne = king_xy; current_xy_ne.m_X < X::_H && current_xy_ne.m_Y < Y::_8;)
        {
            current_xy_ne.nextX();
            current_xy_ne.nextY();
            if (const auto& square = g_BoardMap.at(current_xy_ne); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor != _kcolor)
                {
                    if (ptype == PieceType::Bishop or ptype == PieceType::Queen)
                        num_attackers++;
                }

                break;
            }
        }
    }

    { // Check for Bishop & Queen Attackers to the South-East of king 
        for (auto current_xy_se = king_xy; current_xy_se.m_X < X::_H && current_xy_se.m_Y > Y::_1;)
        {
            current_xy_se.nextX();
            current_xy_se.prevY();
            if (const auto& square = g_BoardMap.at(current_xy_se); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor != _kcolor)
                {
                    if (ptype == PieceType::Bishop or ptype == PieceType::Queen)
                        num_attackers++;
                }

                break;
            }
        }
    }

    { // Check for Bishop & Queen Attackers to the South-West of king 
        for (auto current_xy_sw = king_xy; current_xy_sw.m_X > X::_A && current_xy_sw.m_Y > Y::_1;)
        {
            current_xy_sw.prevX();
            current_xy_sw.prevY();
            if (const auto& square = g_BoardMap.at(current_xy_sw); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor != _kcolor)
                {
                    if (ptype == PieceType::Bishop or ptype == PieceType::Queen)
                        num_attackers++;
                }

                break;
            }
        }
    }

    { // Check for Knight Attackers to the North of king
        if (auto current_xy_north_left = king_xy; current_xy_north_left.m_X > X::_A && current_xy_north_left.m_Y < Y::_7)
        {
            current_xy_north_left.prevX();
            current_xy_north_left.nextY();
            current_xy_north_left.nextY();
            if (const auto& square = g_BoardMap.at(current_xy_north_left); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor != _kcolor)
                {
                    if (ptype == PieceType::Knight)
                        num_attackers++;
                }
            }
        }
        
        if (auto current_xy_north_right = king_xy; current_xy_north_right.m_X < X::_H && current_xy_north_right.m_Y < Y::_7)
        {
            current_xy_north_right.nextX();
            current_xy_north_right.nextY();
            current_xy_north_right.nextY();
            if (const auto& square = g_BoardMap.at(current_xy_north_right); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor != _kcolor)
                {
                    if (ptype == PieceType::Knight)
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
            if (const auto& square = g_BoardMap.at(current_xy_south_left); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor != _kcolor)
                {
                    if (ptype == PieceType::Knight)
                        num_attackers++;
                }
            }
        }
 
        if (auto current_xy_south_right = king_xy; current_xy_south_right.m_X < X::_H && current_xy_south_right.m_Y > Y::_2)
        {
            current_xy_south_right.nextX();
            current_xy_south_right.prevY();
            current_xy_south_right.prevY();
            if (const auto& square = g_BoardMap.at(current_xy_south_right); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor != _kcolor)
                {
                    if (ptype == PieceType::Knight)
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
            if (const auto& square = g_BoardMap.at(current_xy_west_up); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor != _kcolor)
                {
                    if (ptype == PieceType::Knight)
                        num_attackers++;
                }
            }
        }
 
        if (auto current_xy_west_down = king_xy; current_xy_west_down.m_X > X::_B && current_xy_west_down.m_Y > Y::_1)
        {
            current_xy_west_down.prevX();
            current_xy_west_down.prevX();
            current_xy_west_down.prevY();
            if (const auto& square = g_BoardMap.at(current_xy_west_down); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor != _kcolor)
                {
                    if (ptype == PieceType::Knight)
                        num_attackers++;
                }
            }
        }
    }

    { // Check for Knight Attackers to the East of king 
        if (auto current_xy_east_up = king_xy; current_xy_east_up.m_X < X::_G && current_xy_east_up.m_Y < Y::_8)
        {
            current_xy_east_up.nextX();
            current_xy_east_up.nextX();
            current_xy_east_up.nextY();
            if (const auto& square = g_BoardMap.at(current_xy_east_up); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor != _kcolor)
                {
                    if (ptype == PieceType::Knight)
                        num_attackers++;
                }
            }
        }
 
        if (auto current_xy_east_down = king_xy; current_xy_east_down.m_X < X::_G && current_xy_east_down.m_Y > Y::_1)
        {
            current_xy_east_down.nextX();
            current_xy_east_down.nextX();
            current_xy_east_down.prevY();
            if (const auto& square = g_BoardMap.at(current_xy_east_down); square.has_value())
            {
                const auto& [ pcolor, ptype ] = square.value();
                if (pcolor != _kcolor)
                {
                    if (ptype == PieceType::Knight)
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
                if (const auto& square = g_BoardMap.at(current_xy_left); square.has_value())
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
                if (const auto& square = g_BoardMap.at(current_xy_right); square.has_value())
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
                if (const auto& square = g_BoardMap.at(current_xy_left); square.has_value())
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
                if (const auto& square = g_BoardMap.at(current_xy_right); square.has_value())
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

}

namespace 
{

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

QString XY::toStr() const
{
    QString str;

    switch (m_X)
    {
    case X::_A:
        str += "A";
        break;
    case X::_B:
        str += "B";
        break;
    case X::_C:
        str += "C";
        break;
    case X::_D:
        str += "D";
        break;
    case X::_E:
        str += "E";
        break;
    case X::_F:
        str += "F";
        break;
    case X::_G:
        str += "G";
        break;
    case X::_H:
        str += "H";
        break;
    }

    switch (m_Y)
    {
    case Y::_1:
        str += "1";
        break;
    case Y::_2:
        str += "2";
        break;
    case Y::_3:
        str += "3";
        break;
    case Y::_4:
        str += "4";
        break;
    case Y::_5:
        str += "5";
        break;
    case Y::_6:
        str += "6";
        break;
    case Y::_7:
        str += "7";
        break;
    case Y::_8:
        str += "8";
        break;
    }

    return str;
}

}

