#ifndef FORSYTH_EDWARDS_NOTATION_H
#define FORSYTH_EDWARDS_NOTATION_H

#include <QString>

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
    static constexpr std::array<X, 8> g_ArrayX = { X::_A, X::_B, X::_C, X::_D, X::_E, X::_F, X::_G, X::_H };
    static constexpr std::array<Y, 8> g_ArrayY = { Y::_1, Y::_2, Y::_3, Y::_4, Y::_5, Y::_6, Y::_7, Y::_8 };
};

namespace std {
    template<>
    struct hash<XY> {
        size_t operator()(const XY& _xy) const noexcept {
            size_t h1 = std::hash<int>{}(static_cast<int>(_xy.m_X));
            size_t h2 = std::hash<int>{}(static_cast<int>(_xy.m_Y));

            // Combine hashes (boost-style)
            return h1 ^ (h2 << 1);
        }
    };
}

// TODO: Modify class into a simple procedural functions.
class ForsythEdwardsNotation 
{

private:

    enum class PieceColor { White, Black };
    enum class PieceType  { Pawn, Knight, Bishop, Rook, Queen, King };

public:

    ForsythEdwardsNotation(const QString&);
    bool isValid() const;
    bool updateFen(const QString&);
    bool is50MoveRule() const;

private:

    void setupChessMap();
    bool isFenLegal() const;
    bool isPiecePlacementSyntaxValid(const QString&) const;
    bool isActiveColorSyntaxValid(const QString&) const;
    bool isCastlingRightsSyntaxValid(const QString&) const;
    bool isEnpassantTargetSquareSyntaxValid(const QString&) const;
    bool isHalfMoveClockSyntaxValid(const QString&) const;
    bool isFullMoveClockSyntaxValid(const QString&) const;

    bool isPawnOnFirstOrLastRank(const QString&) const;
    bool isOneKingOnly(const QString&) const;
    bool isCorrectNumberOfPieces(const QString&) const;
    bool isIncorrectKingInCheck(const QString&) const;

    XY findKing(const PieceColor&) const;
    bool isKingInCheck(const PieceColor&) const;
    uint8_t howManyKingAttackers(const PieceColor&) const;


private:
    
    // Total FEN Fields: 6
    // { Piece Placement }, { Active Color }, { Castling Rights }, { Enpassant Target Square }, { Halfmove Clock }, { Fullmove Clock };
    QString m_FenNotation = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    std::unordered_map<XY, std::optional<std::tuple<PieceColor, PieceType>>> m_BoardMap;
};

#endif

