#include "Piece.hpp"
#include <QDebugStateSaver>

Piece::Piece(const Position& _pos, const Color& _color, const Type& _type)
    : m_Pos(_pos)
    , m_Color(_color)
    , m_Type(_type)
{
}

Position Piece::getPosition() const
{
    return m_Pos;
}
    
Type Piece::getType() const
{
    return m_Type;
}

Color Piece::getColor() const
{
    return m_Color;
}

uint16_t Piece::getUnicode() const
{
    if (m_Color == Color::White)
    {
        switch (m_Type)
        {
        case Type::P:
            return 0x265F;
        case Type::N:
            return 0x265E;
        case Type::B:
            return 0x265D;
        case Type::R:
            return 0x265C;
        case Type::Q:
            return 0x265B;
        case Type::K:
            return 0x265A;
        }
    }
    else if (m_Color == Color::Black)
    {
        switch (m_Type)
        {
        case Type::P:
            return 0x2659;
        case Type::N:
            return 0x2658;
        case Type::B:
            return 0x2657;
        case Type::R:
            return 0x2656;
        case Type::Q:
            return 0x2655;
        case Type::K:
            return 0x2654;
        }
    }
    else
        Q_UNREACHABLE();
}

void Piece::changePosition(const Position& _p)
{
    m_PreviousMove.first = m_Pos;
    m_PreviousMove.second = _p;
    m_Pos = _p;
    m_PieceMoved = true;
}
    
bool Piece::hasPieceMoved() const
{
    return m_PieceMoved;
}

QString Piece::toStr() const
{
    QString str = (m_Color == Color::White) ? "White " : "Black ";

    switch (m_Type)
    {
    case Type::P:
        str += "Pawn ";
        break;
    case Type::N:
        str += "Knight ";
        break;
    case Type::B:
        str += "Bishop ";
        break;
    case Type::R:
        str += "Rook ";
        break;
    case Type::Q:
        str += "Queen ";
        break;
    case Type::K:
        str += "King ";
        break;
    }

    str += m_Pos.toStr();

    return str;
}

QDebug operator<<(QDebug _dbg, const Piece& _obj)
{
    QDebugStateSaver saver(_dbg);

    _dbg.nospace() << "Piece: " << _obj.toStr();

    return _dbg;
}

bool Piece::isKingInCheck(ChessMapConst& _chessmap, const Color& _king_color) const
{
    auto attacks = QList<Position>();
    const auto& king_pos = findKing(_chessmap, _king_color);
    for (const auto& [ pos, piece ] : _chessmap)
    {
        if (piece != nullptr)
        {
            if (piece->getColor() != _king_color)
                attacks += piece->legalCaptures(_chessmap);
        }
    }

    if (attacks.contains(king_pos))
        return true;
    else
        return false;
}

Position Piece::findKing(ChessMapConst& _chessmap, const Color& _king_color) const
{
    for (const auto& [ pos, piece ] : _chessmap)
    {
        if (piece != nullptr)
        {
            if (piece->getColor() == _king_color && piece->getType() == Type::K)
                return piece->getPosition();
        }
    }

    Q_UNREACHABLE();
}

QList<Position> Piece::identifyKingAttackers(ChessMapConst& _chessmap, const Color& _king_color) const
{
    auto enemies = QList<Position>();
    const auto& king_pos = findKing(_chessmap, _king_color);
    for (const auto& [ pos, piece ] : _chessmap)
    {
        if (piece != nullptr)
        {
            if (piece->getColor() != _king_color)
            {
                const auto& attacks = piece->legalCaptures(_chessmap);
                if (attacks.contains(king_pos ))
                    enemies += piece->getPosition();
            }
        }
    }

    Q_ASSERT(enemies.size() == 0 || enemies.size() == 1 || enemies.size() == 2);
    return enemies;
}

int Piece::howManyKingAttackers(ChessMapConst& _chessmap) const
{
    const auto& king_pos = findKing(_chessmap, m_Color);
    auto num = 0;
    for (const auto& [ key, p ] : _chessmap)
    {
        if (p != nullptr)
        {
            if (p->getColor() != m_Color)
            {
                const auto& attacks = p->legalCaptures(_chessmap);
                if (attacks.contains(king_pos))
                    num++;
            }
        }
    }

    return num;
}

Position Piece::findKingAttacker(ChessMapConst& _chessmap) const
{
    Q_ASSERT(howManyKingAttackers(_chessmap) == 1);
    // Assume there is only one King Attacker
    const auto& king_pos = findKing(_chessmap, m_Color);
    for (const auto& [ key, p ] : _chessmap)
    {
        if (p != nullptr)
        {
            if (p->getColor() != m_Color)
            {
                const auto& attacks = p->legalCaptures(_chessmap);
                if (attacks.contains(king_pos))
                    return p->getPosition();
            }
        }
    }

    Q_UNREACHABLE();
}

bool Piece::isPiecePinned(ChessMapConst& _chessmap) const
{
    // King Should Never Be Pinned
    if (m_Type == Type::K)
        return false;

    const auto& kpos = findKing(_chessmap, m_Color);
    const auto& self = m_Pos;
    auto [ kcol, krow ] = kpos.toInt();
    auto [ scol, srow ] = self.toInt();
    const auto dx = scol - kcol;
    const auto dy = srow - krow;

    // Ensure We Only Check For the 8 Cardinal Directions
    if ((dx != 0) and (dy != 0))
    {
        if (abs(dx) != abs(dy))
            return false;
    }

    // king_pos == self_pos
    if ((dx == 0) and (dy == 0))
        return false;

    // The following if-statements represent the 8 cardinal directions.

    // North
    if (dx == 0 and dy > 0)
    {
        auto curr = kpos;
        auto p = QList<Position>();
        while (curr.row < Row::Eight && p.size() < 2)
        {
            curr.nextRow();
            if (_chessmap.at(curr))
                p.append(curr);
        }
        if (p.size() <= 1)
            return false;
        const auto& enemy = _chessmap.at(p.at(1));
        Q_ASSERT(enemy != nullptr);
        if (enemy->getColor() == m_Color)
            return false;
        const auto& t = enemy->getType();
        if ((t == Type::R) or (t == Type::Q))
            return true;
    }

    // NorthEast
    else if (dx > 0 and dy > 0)
    {
        auto curr = kpos;
        auto p = QList<Position>();
        while (curr.col < Col::H && curr.row < Row::Eight && p.size() < 2)
        {
            curr.nextCol();
            curr.nextRow();
            if (_chessmap.at(curr))
                p.append(curr);
        }
        if (p.size() <= 1)
            return false;
        const auto& enemy = _chessmap.at(p.at(1));
        Q_ASSERT(enemy != nullptr);
        if (enemy->getColor() == m_Color)
            return false;
        const auto& t = enemy->getType();
        if ((t == Type::B) or (t == Type::Q))
            return true;
    }

    // East
    else if (dx > 0 and dy == 0)
    {
        auto curr = kpos;
        auto p = QList<Position>();
        while (curr.col < Col::H && p.size() < 2)
        {
            curr.nextCol();
            if (_chessmap.at(curr))
                p.append(curr);
        }
        if (p.size() <= 1)
            return false;
        const auto& enemy = _chessmap.at(p.at(1));
        Q_ASSERT(enemy != nullptr);
        if (enemy->getColor() == m_Color)
            return false;
        const auto& t = enemy->getType();
        if ((t == Type::R) or (t == Type::Q))
            return true;
    }

    // SouthEast
    else if (dx > 0 and dy < 0)
    {
        auto curr = kpos;
        auto p = QList<Position>();
        while (curr.col < Col::H && curr.row > Row::One && p.size() < 2)
        {
            curr.nextCol();
            curr.prevRow();
            if (_chessmap.at(curr))
                p.append(curr);
        }
        if (p.size() <= 1)
            return false;
        const auto& enemy = _chessmap.at(p.at(1));
        Q_ASSERT(enemy != nullptr);
        if (enemy->getColor() == m_Color)
            return false;
        const auto& t = enemy->getType();
        if ((t == Type::B) or (t == Type::Q))
            return true;
    }

    // South
    else if (dx == 0 and dy < 0)
    {
        auto curr = kpos;
        auto p = QList<Position>();
        while (curr.row > Row::One && p.size() < 2)
        {
            curr.prevRow();
            if (_chessmap.at(curr))
                p.append(curr);
        }
        if (p.size() <= 1)
            return false;
        const auto& enemy = _chessmap.at(p.at(1));
        Q_ASSERT(enemy != nullptr);
        if (enemy->getColor() == m_Color)
            return false;
        const auto& t = enemy->getType();
        if ((t == Type::R) or (t == Type::Q))
            return true;
    }

    // SouthWest
    else if (dx < 0 and dy < 0)
    {
        auto curr = kpos;
        auto p = QList<Position>();
        while (curr.col > Col::A && curr.row > Row::One && p.size() < 2)
        {
            curr.prevCol();
            curr.prevRow();
            if (_chessmap.at(curr))
                p.append(curr);
        }
        if (p.size() <= 1)
            return false;
        const auto& enemy = _chessmap.at(p.at(1));
        Q_ASSERT(enemy != nullptr);
        if (enemy->getColor() == m_Color)
            return false;
        const auto& t = enemy->getType();
        if ((t == Type::B) or (t == Type::Q))
            return true;
    }

    // West
    else if (dx < 0 and dy == 0)
    {
        auto curr = kpos;
        auto p = QList<Position>();
        while (curr.col > Col::A && p.size() < 2)
        {
            curr.prevCol();
            if (_chessmap.at(curr))
                p.append(curr);
        }
        if (p.size() <= 1)
            return false;
        const auto& enemy = _chessmap.at(p.at(1));
        Q_ASSERT(enemy != nullptr);
        if (enemy->getColor() == m_Color)
            return false;
        const auto& t = enemy->getType();
        if ((t == Type::R) or (t == Type::Q))
            return true;
    }

    // NorthWest
    else if (dx < 0 and dy > 0)
    {
        auto curr = kpos;
        auto p = QList<Position>();
        while (curr.col > Col::A && curr.row < Row::Eight && p.size() < 2)
        {
            curr.prevCol();
            curr.nextRow();
            if (_chessmap.at(curr))
                p.append(curr);
        }
        if (p.size() <= 1)
            return false;
        const auto& enemy = _chessmap.at(p.at(1));
        Q_ASSERT(enemy != nullptr);
        if (enemy->getColor() == m_Color)
            return false;
        const auto& t = enemy->getType();
        if ((t == Type::B) or (t == Type::Q))
            return true;
    }
    else
        Q_UNREACHABLE();

    return false;
}

QList<Position> Piece::getValidSquares(ChessMapConst& _chessmap) const
{
    const auto moves = legalMoves(_chessmap);
    const auto caps = legalCaptures(_chessmap);

    if (isKingInCheck(_chessmap, m_Color))
    {
        const auto& num = howManyKingAttackers(_chessmap);
        Q_ASSERT(num == 2 || num == 1);
        if (num == 2)
        {
            // Move king
            if (m_Type != Type::K)
                return {};

            auto m = moves;
            m.append(caps);
            return m;
        }
        if (num == 1)
        {
            // capture, block or move king
            if (isPiecePinned(_chessmap))
            {
                const auto& pinned = getPinnedPieceMoves(_chessmap);
                return pinned;
            }
            else
            {
                if (m_Type == Type::K)
                    return moves + caps;

                const auto& enemy = identifyKingAttackers(_chessmap, m_Color);
                auto c = caps;
                for (auto it = c.begin(); it != c.end();)
                {
                    if (enemy.contains(*it))
                        ++it;
                    else
                        it = c.erase(it);
                }
                auto m = moves;
                const auto& lineOfAttack = getLineOfAttack(_chessmap);
                for (auto it = m.begin(); it != m.end();)
                {
                    if (lineOfAttack.contains(*it))
                        ++it;
                    else
                        it = m.erase(it);
                }
                return  m + c ;
            }
        }
    }
    else
    {
        if (isPiecePinned(_chessmap))
        {
            const auto& pinned = getPinnedPieceMoves(_chessmap);
            return pinned;
        }
        else
        {
            auto m = moves;
            m.append(caps);
            return m;
        }
    }

    Q_UNREACHABLE();

    return {};
}

QList<Position> Piece::getPinnedPieceMoves(ChessMapConst& _chessmap) const
{
    auto validMoves = legalMoves(_chessmap);
    validMoves += legalCaptures(_chessmap);
    if (!isPiecePinned(_chessmap)) 
        return validMoves;

    // King Should Never Be Pinned
    if (m_Type == Type::K)
        return validMoves;

    const auto& kpos = findKing(_chessmap, m_Color);
    const auto& self = m_Pos;
    auto [ kcol, krow ] = kpos.toInt();
    auto [ scol, srow ] = self.toInt();
    const auto dx = scol - kcol;
    const auto dy = srow - krow;
    const auto kingCheck = isKingInCheck(_chessmap, m_Color);
    const auto enemy = identifyKingAttackers(_chessmap, m_Color);

    if (enemy.size() > 1)
        return { };

    // The following if-statements represent the 8 cardinal directions.

    // North
    if (dx == 0 and dy > 0)
    {
        auto curr = kpos;
        auto p = QList<Position>();
        auto s = QList<Position>();
        //while (curr.row < self.row)
        while (p.size() < 2)
        {
            curr.nextRow();
            if (_chessmap.at(curr))
                p.append(curr);
            else 
                s.append(curr);

            if (p.size() == 2)
                break;
        }
        for (auto it = validMoves.begin(); it != validMoves.end();)
        {
            if (p.contains(*it) || s.contains(*it) || p.back() == *it)
                ++it;
            else
                it = validMoves.erase(it);
        }
        if (kingCheck)
        {
            if (!validMoves.contains(enemy.front()))
                return { };
        }
    }

    // NorthEast
    else if (dx > 0 and dy > 0)
    {
        auto curr = kpos;
        auto p = QList<Position>();
        auto s = QList<Position>();
        //while (curr.col < self.col && curr.row < self.row)
        while (p.size() < 2)
        {
            curr.nextCol();
            curr.nextRow();
            if (_chessmap.at(curr))
                p.append(curr);
            else 
                s.append(curr);

            if (p.size() == 2)
                break;
        }
        for (auto it = validMoves.begin(); it != validMoves.end();)
        {
            if (p.contains(*it) || s.contains(*it) || p.back() == *it)
                ++it;
            else
                it = validMoves.erase(it);
        }
        if (kingCheck)
        {
            if (!validMoves.contains(enemy.front()))
                return { };
        }
    }

    // East
    else if (dx > 0 and dy == 0)
    {
        auto curr = kpos;
        auto p = QList<Position>();
        auto s = QList<Position>();
        //while (curr.col < self.col)
        while (p.size() < 2)
        {
            curr.nextCol();
            if (_chessmap.at(curr))
                p.append(curr);
            else 
                s.append(curr);

            if (p.size() == 2)
                break;
        }
        for (auto it = validMoves.begin(); it != validMoves.end();)
        {
            if (p.contains(*it) || s.contains(*it) || p.back() == *it)
                ++it;
            else
                it = validMoves.erase(it);
        }
        if (kingCheck)
        {
            if (!validMoves.contains(enemy.front()))
                return { };
        }
    }

    // SouthEast
    else if (dx > 0 and dy < 0)
    {
        auto curr = kpos;
        auto p = QList<Position>();
        auto s = QList<Position>();
        //while (curr.col < self.col && curr.row > self.row)
        while (p.size() < 2)
        {
            curr.nextCol();
            curr.prevRow();
            if (_chessmap.at(curr))
                p.append(curr);
            else 
                s.append(curr);

            if (p.size() == 2)
                break;
        }
        for (auto it = validMoves.begin(); it != validMoves.end();)
        {
            if (p.contains(*it) || s.contains(*it) || p.back() == *it)
                ++it;
            else
                it = validMoves.erase(it);
        }
        if (kingCheck)
        {
            if (!validMoves.contains(enemy.front()))
                return { };
        }
    }

    // South
    else if (dx == 0 and dy < 0)
    {
        auto curr = kpos;
        auto p = QList<Position>();
        auto s = QList<Position>();
        //while (curr.row > self.row)
        while (p.size() < 2)
        {
            curr.prevRow();
            if (_chessmap.at(curr))
                p.append(curr);
            else 
                s.append(curr);

            if (p.size() == 2)
                break;
        }
        for (auto it = validMoves.begin(); it != validMoves.end();)
        {
            if (p.contains(*it) || s.contains(*it) || p.back() == *it)
                ++it;
            else
                it = validMoves.erase(it);
        }
        if (kingCheck)
        {
            if (!validMoves.contains(enemy.front()))
                return { };
        }
    }

    // SouthWest
    else if (dx < 0 and dy < 0)
    {
        auto curr = kpos;
        auto p = QList<Position>();
        auto s = QList<Position>();
        //while (curr.col > self.col && curr.row > self.row)
        while (p.size() < 2)
        {
            curr.prevCol();
            curr.prevRow();
            if (_chessmap.at(curr))
                p.append(curr);
            else 
                s.append(curr);

            if (p.size() == 2)
                break;
        }
        for (auto it = validMoves.begin(); it != validMoves.end();)
        {
            if (p.contains(*it) || s.contains(*it) || p.back() == *it)
                ++it;
            else
                it = validMoves.erase(it);
        }
        if (kingCheck)
        {
            if (!validMoves.contains(enemy.front()))
                return { };
        }
    }

    // West
    else if (dx < 0 and dy == 0)
    {
        auto curr = kpos;
        auto p = QList<Position>();
        auto s = QList<Position>();
        //while (curr.col > self.col)
        while (p.size() < 2)
        {
            curr.prevCol();
            if (_chessmap.at(curr))
                p.append(curr);
            else
                s.append(curr);

            if (p.size() == 2)
                break;
        }
        for (auto it = validMoves.begin(); it != validMoves.end();)
        {
            if (p.contains(*it) || s.contains(*it) || p.back() == *it)
                ++it;
            else
                it = validMoves.erase(it);
        }
        if (kingCheck)
        {
            if (!validMoves.contains(enemy.front()))
                return { };
        }
    }

    // NorthWest
    else if (dx < 0 and dy > 0)
    {
        auto curr = kpos;
        auto p = QList<Position>();
        auto s = QList<Position>();
        //while (curr.col > self.col && curr.row < self.row)
        while (p.size() < 2)
        {
            curr.prevCol();
            curr.nextRow();
            if (_chessmap.at(curr))
                p.append(curr);
            else 
                s.append(curr);

            if (p.size() == 2)
                break;
        }
        for (auto it = validMoves.begin(); it != validMoves.end();)
        {
            if (p.contains(*it) || s.contains(*it) || p.back() == *it)
                ++it;
            else
                it = validMoves.erase(it);
        }
        if (kingCheck)
        {
            if (!validMoves.contains(enemy.front()))
                return { };
        }
    }
    else
        Q_UNREACHABLE();

    return validMoves;
}

/*
 * 'Line of Attack' are the squares between the king and the attacker that can be blocked
 */
QList<Position> Piece::getLineOfAttack(ChessMapConst& _chessmap) const
{
    if (!isKingInCheck(_chessmap, m_Color))
        return { };

    if (m_Type == Type::K)
        return { };

    auto enemies = identifyKingAttackers(_chessmap, m_Color);
    if (enemies.size() > 1)
        return { };

    const auto& enemy = enemies.front();

    auto lineOfAttack = QList<Position>();
    switch (_chessmap.at(enemy)->getType())
    {

    case Type::P:
    case Type::N:
    case Type::K:
    {
        // Pawns, Nights, Kings do not have a line of attack
        // i.e. you can't block these pieces from attacking the king
        break;
    }

    case Type::B:
    {
        const auto& kingPos = findKing(_chessmap, m_Color);
        const auto& self = enemy;
        auto curr = self;

        if (self.col < kingPos.col && self.row < kingPos.row)
        {
            while (curr.col < kingPos.col && curr.row < kingPos.row)
            {
                curr.nextCol();
                curr.nextRow();
                if (_chessmap.at(curr) == nullptr)
                    lineOfAttack.append(curr);
            }
        }
        else if (self.col < kingPos.col && self.row > kingPos.row)
        {
            while (curr.col < kingPos.col && curr.row > kingPos.row)
            {
                curr.nextCol();
                curr.prevRow();
                if (_chessmap.at(curr) == nullptr)
                    lineOfAttack.append(curr);
            }
        }
        else if (self.col > kingPos.col && self.row < kingPos.row)
        {
            while (curr.col > kingPos.col && curr.row < kingPos.row)
            {
                curr.prevCol();
                curr.nextRow();
                if (_chessmap.at(curr) == nullptr)
                    lineOfAttack.append(curr);
            }
        }
        else if (self.col > kingPos.col && self.row > kingPos.row)
        {
            while (curr.col > kingPos.col && curr.row > kingPos.row)
            {
                curr.prevCol();
                curr.prevRow();
                if (_chessmap.at(curr) == nullptr)
                    lineOfAttack.append(curr);
            }
        }

        break;
    }

    case Type::R:
    {
        const auto& kingPos = findKing(_chessmap, m_Color);
        const auto& self = enemy;
        auto curr = self;

        if (self.col < kingPos.col && self.row == kingPos.row)
        {
            while (curr.col < kingPos.col && curr.row == kingPos.row)
            {
                curr.nextCol();
                if (_chessmap.at(curr) == nullptr)
                    lineOfAttack.append(curr);
            }
        }
        else if (self.col > kingPos.col && self.row == kingPos.row)
        {
            while (curr.col > kingPos.col && curr.row == kingPos.row)
            {
                curr.prevCol();
                if (_chessmap.at(curr) == nullptr)
                    lineOfAttack.append(curr);
            }
        }
        else if (self.col == kingPos.col && self.row < kingPos.row)
        {
            while (curr.col == kingPos.col && curr.row < kingPos.row)
            {
                curr.nextRow();
                if (_chessmap.at(curr) == nullptr)
                    lineOfAttack.append(curr);
            }
        }
        else if (self.col == kingPos.col && self.row > kingPos.row)
        {
            while (curr.col == kingPos.col && curr.row > kingPos.row)
            {
                curr.prevRow();
                if (_chessmap.at(curr) == nullptr)
                    lineOfAttack.append(curr);
            }
        }

        break;
    }

    case Type::Q:
    {
        const auto& kingPos = findKing(_chessmap, m_Color);
        const auto& self = enemy;
        auto curr = self;

        if (self.col < kingPos.col && self.row == kingPos.row)
        {
            while (curr.col < kingPos.col && curr.row == kingPos.row)
            {
                curr.nextCol();
                if (_chessmap.at(curr) == nullptr)
                    lineOfAttack.append(curr);
            }
        }
        else if (self.col > kingPos.col && self.row == kingPos.row)
        {
            while (curr.col > kingPos.col && curr.row == kingPos.row)
            {
                curr.prevCol();
                if (_chessmap.at(curr) == nullptr)
                    lineOfAttack.append(curr);
            }
        }
        else if (self.col == kingPos.col && self.row < kingPos.row)
        {
            while (curr.col == kingPos.col && curr.row < kingPos.row)
            {
                curr.nextRow();
                if (_chessmap.at(curr) == nullptr)
                    lineOfAttack.append(curr);
            }
        }
        else if (self.col == kingPos.col && self.row > kingPos.row)
        {
            while (curr.col == kingPos.col && curr.row > kingPos.row)
            {
                curr.prevRow();
                if (_chessmap.at(curr) == nullptr)
                    lineOfAttack.append(curr);
            }
        }
        else if (self.col < kingPos.col && self.row < kingPos.row)
        {
            while (curr.col < kingPos.col && curr.row < kingPos.row)
            {
                curr.nextCol();
                curr.nextRow();
                if (_chessmap.at(curr) == nullptr)
                    lineOfAttack.append(curr);
            }
        }
        else if (self.col < kingPos.col && self.row > kingPos.row)
        {
            while (curr.col < kingPos.col && curr.row > kingPos.row)
            {
                curr.nextCol();
                curr.prevRow();
                if (_chessmap.at(curr) == nullptr)
                    lineOfAttack.append(curr);
            }
        }
        else if (self.col > kingPos.col && self.row < kingPos.row)
        {
            while (curr.col > kingPos.col && curr.row < kingPos.row)
            {
                curr.prevCol();
                curr.nextRow();
                if (_chessmap.at(curr) == nullptr)
                    lineOfAttack.append(curr);
            }
        }
        else if (self.col > kingPos.col && self.row > kingPos.row)
        {
            while (curr.col > kingPos.col && curr.row > kingPos.row)
            {
                curr.prevCol();
                curr.prevRow();
                if (_chessmap.at(curr) == nullptr)
                    lineOfAttack.append(curr);
            }
        }

        break;
    }

    }

    return lineOfAttack;
}

