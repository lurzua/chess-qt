#include "Position.hpp"
#include <QDebugStateSaver>

Position::Position(const Col&& _c, const Row&& _r)
{
    col = _c;
    row = _r;
}

Position::Position(const Col& _c, const Row& _r)
{
    col = _c;
    row = _r;
}

bool Position::operator==(const Position& _other) const
{
    return col == _other.col && row == _other.row;
}

bool Position::operator!=(const Position& _other) const
{
    return col != _other.col || row != _other.row;
}

bool Position::operator<=(const Col& _other) const
{
    return col <= _other;
}

bool Position::operator<=(const Row& _other) const
{
    return row <= _other;
}

std::pair<int, int> Position::toInt() const
{
    auto colRow = std::make_pair(0, 0);

    switch (col)
    {
    case Col::A:
        colRow.first = 0;
        break;
    case Col::B:
        colRow.first = 1;
        break;
    case Col::C:
        colRow.first = 2;
        break;
    case Col::D:
        colRow.first = 3;
        break;
    case Col::E:
        colRow.first = 4;
        break;
    case Col::F:
        colRow.first = 5;
        break;
    case Col::G:
        colRow.first = 6;
        break;
    case Col::H:
        colRow.first = 7;
        break;
    }

    switch (row)
    {
    case Row::One:
        colRow.second = 0;
        break;
    case Row::Two:
        colRow.second = 1;
        break;
    case Row::Three:
        colRow.second = 2;
        break;
    case Row::Four:
        colRow.second = 3;
        break;
    case Row::Five:
        colRow.second = 4;
        break;
    case Row::Six:
        colRow.second = 5;
        break;
    case Row::Seven:
        colRow.second = 6;
        break;
    case Row::Eight:
        colRow.second = 7;
        break;
    }

    return colRow;
}

QString Position::toStr() const
{
    QString str;

    switch (col)
    {
    case Col::A:
        str += "A";
        break;
    case Col::B:
        str += "B";
        break;
    case Col::C:
        str += "C";
        break;
    case Col::D:
        str += "D";
        break;
    case Col::E:
        str += "E";
        break;
    case Col::F:
        str += "F";
        break;
    case Col::G:
        str += "G";
        break;
    case Col::H:
        str += "H";
        break;
    }

    switch (row)
    {
    case Row::One:
        str += "1";
        break;
    case Row::Two:
        str += "2";
        break;
    case Row::Three:
        str += "3";
        break;
    case Row::Four:
        str += "4";
        break;
    case Row::Five:
        str += "5";
        break;
    case Row::Six:
        str += "6";
        break;
    case Row::Seven:
        str += "7";
        break;
    case Row::Eight:
        str += "8";
        break;
    }

    return str;
}

QDebug operator<<(QDebug _dbg, const Position& _obj)
{
    QDebugStateSaver saver(_dbg);

    _dbg.nospace() << "Position: " << _obj.toStr();

    return _dbg;
}

void Position::nextCol()
{
    switch (col)
    {
    case Col::A:
        col = Col::B;
        break;
    case Col::B:
        col = Col::C;
        break;
    case Col::C:
        col = Col::D;
        break;
    case Col::D:
        col = Col::E;
        break;
    case Col::E:
        col = Col::F;
        break;
    case Col::F:
        col = Col::G;
        break;
    case Col::G:
        col = Col::H;
        break;
    case Col::H:
        col = Col::H;
        break;
    }
}

void Position::prevCol()
{
    switch (col)
    {
    case Col::A:
        col = Col::A;
        break;
    case Col::B:
        col = Col::A;
        break;
    case Col::C:
        col = Col::B;
        break;
    case Col::D:
        col = Col::C;
        break;
    case Col::E:
        col = Col::D;
        break;
    case Col::F:
        col = Col::E;
        break;
    case Col::G:
        col = Col::F;
        break;
    case Col::H:
        col = Col::G;
        break;
    }
}

void Position::nextRow()
{
    switch (row)
    {
    case Row::One:
        row = Row::Two;
        break;
    case Row::Two:
        row = Row::Three;
        break;
    case Row::Three:
        row = Row::Four;
        break;
    case Row::Four:
        row = Row::Five;
        break;
    case Row::Five:
        row = Row::Six;
        break;
    case Row::Six:
        row = Row::Seven;
        break;
    case Row::Seven:
        row = Row::Eight;
        break;
    case Row::Eight:
        row = Row::Eight;
        break;
    }
}

void Position::prevRow()
{
    switch (row)
    {
    case Row::One:
        row = Row::One;
        break;
    case Row::Two:
        row = Row::One;
        break;
    case Row::Three:
        row = Row::Two;
        break;
    case Row::Four:
        row = Row::Three;
        break;
    case Row::Five:
        row = Row::Four;
        break;
    case Row::Six:
        row = Row::Five;
        break;
    case Row::Seven:
        row = Row::Six;
        break;
    case Row::Eight:
        row = Row::Seven;
        break;
    }
}

