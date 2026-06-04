#ifndef ENUMS_H
#define ENUMS_H

#include <string_view>

enum class Col   { A, B, C, D, E, F, G, H };
enum class Row   { One, Two, Three, Four, Five, Six, Seven, Eight };
enum class Color { White, Black };
enum class Type  { P, N, B, R, Q, K };

constexpr Color operator!(Color _color)
{
    switch(_color)
    {
    case Color::White:
        return Color::Black;
    case Color::Black:
        return Color::White;
    }
}

constexpr std::string_view to_string(Color _color)
{
    switch (_color)
    {
        case Color::White:
            return "WHITE";
        case Color::Black:
            return "BLACK";
        default:
            return "UNKNOWN_COLOR";
    }
}

constexpr std::string_view to_string(Type _type)
{
    switch (_type)
    {
        case Type::P:
            return "PAWN";
        case Type::N:
            return "KNIGHT";
        case Type::B:
            return "BISHOP";
        case Type::R:
            return "ROOK";
        case Type::Q:
            return "QUEEN";
        case Type::K:
            return "KING";
        default:
            return "UNKNOWN_TYPE";
    }
}

#endif

