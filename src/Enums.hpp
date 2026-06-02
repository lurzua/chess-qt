#ifndef ENUMS_H
#define ENUMS_H

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

#endif // ENUMS_H
