#ifndef POSITION_H
#define POSITION_H

#include "Enums.hpp"
#include <functional>
#include <QString>

struct Position
{
    Col col;
    Row row;

    Position(const Col&, const Row&);
    Position(const Col&&, const Row&&);
    bool operator==(const Position&) const;
    bool operator!=(const Position&) const;
    bool operator<=(const Col&) const;
    bool operator<=(const Row&) const;
    std::pair<int, int> toInt() const;
    //std::string toStr() const;
    QString toStr() const;
    friend QDebug operator<<(QDebug, const Position&);
    void nextCol();
    void prevCol();
    void nextRow();
    void prevRow();
};

namespace std {
    template<>
    struct hash<Position> {
        size_t operator()(const Position& p) const noexcept {
            size_t h1 = std::hash<int>{}(static_cast<int>(p.col));
            size_t h2 = std::hash<int>{}(static_cast<int>(p.row));

            // Combine hashes (boost-style)
            return h1 ^ (h2 << 1);
        }
    };
}

constexpr std::array<Col, 8> g_ArrayCol = { Col::A, Col::B, Col::C, Col::D, Col::E, Col::F, Col::G, Col::H };
constexpr std::array<Row, 8> g_ArrayRow = { Row::One, Row::Two, Row::Three, Row::Four, Row::Five, Row::Six, Row::Seven, Row::Eight };

constexpr std::array<std::tuple<Col, Row, Color, Type>, 32> g_StartingPieces =
{
    std::make_tuple( Col::A, Row::Two, Color::White, Type::P ),
    std::make_tuple( Col::B, Row::Two, Color::White, Type::P ),
    std::make_tuple( Col::C, Row::Two, Color::White, Type::P ),
    std::make_tuple( Col::D, Row::Two, Color::White, Type::P ),
    std::make_tuple( Col::E, Row::Two, Color::White, Type::P ),
    std::make_tuple( Col::F, Row::Two, Color::White, Type::P ),
    std::make_tuple( Col::G, Row::Two, Color::White, Type::P ),
    std::make_tuple( Col::H, Row::Two, Color::White, Type::P ),

    std::make_tuple( Col::A, Row::One, Color::White, Type::R ),
    std::make_tuple( Col::B, Row::One, Color::White, Type::N ),
    std::make_tuple( Col::C, Row::One, Color::White, Type::B ),
    std::make_tuple( Col::D, Row::One, Color::White, Type::Q ),
    std::make_tuple( Col::E, Row::One, Color::White, Type::K ),
    std::make_tuple( Col::F, Row::One, Color::White, Type::B ),
    std::make_tuple( Col::G, Row::One, Color::White, Type::N ),
    std::make_tuple( Col::H, Row::One, Color::White, Type::R ),

    std::make_tuple( Col::A, Row::Seven, Color::Black, Type::P ),
    std::make_tuple( Col::B, Row::Seven, Color::Black, Type::P ),
    std::make_tuple( Col::C, Row::Seven, Color::Black, Type::P ),
    std::make_tuple( Col::D, Row::Seven, Color::Black, Type::P ),
    std::make_tuple( Col::E, Row::Seven, Color::Black, Type::P ),
    std::make_tuple( Col::F, Row::Seven, Color::Black, Type::P ),
    std::make_tuple( Col::G, Row::Seven, Color::Black, Type::P ),
    std::make_tuple( Col::H, Row::Seven, Color::Black, Type::P ),

    std::make_tuple( Col::A, Row::Eight, Color::Black, Type::R ),
    std::make_tuple( Col::B, Row::Eight, Color::Black, Type::N ),
    std::make_tuple( Col::C, Row::Eight, Color::Black, Type::B ),
    std::make_tuple( Col::D, Row::Eight, Color::Black, Type::Q ),
    std::make_tuple( Col::E, Row::Eight, Color::Black, Type::K ),
    std::make_tuple( Col::F, Row::Eight, Color::Black, Type::B ),
    std::make_tuple( Col::G, Row::Eight, Color::Black, Type::N ),
    std::make_tuple( Col::H, Row::Eight, Color::Black, Type::R ),
};

#endif // POSITION_H

