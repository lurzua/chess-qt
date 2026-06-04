#include "BoardView.hpp"
#include "SquareView.hpp"
#include <QGridLayout>
#include "Log.hpp"

BoardView::BoardView()
    : QWidget{ nullptr }
{
    init();
    initGridLayout();
    initSquares();
    initConnections();
}

BoardView::~BoardView()
{
    delete m_GridLayout;
}

void BoardView::init()
{
    LOG_TRACE("BoardView Initialize");
    setObjectName("BoardView");
    const auto min = QSize(600, 600);
    const auto max = QSize(1000, 1000);
    const auto policy = QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMinimumSize(min);
    setMaximumSize(max);
    setSizePolicy(policy);
}

void BoardView::initGridLayout()
{
    m_GridLayout = new QGridLayout(this);
    m_GridLayout->setObjectName("GridLayout_BoardView");
    m_GridLayout->setSpacing(0);
    m_GridLayout->setGeometry(geometry());
}

void BoardView::initSquares()
{
    LOG_TRACE("BoardView Initialize SquareView");
    for (const auto& row : g_ArrayRow)
    {
        for (const auto& col : g_ArrayCol)
        {
            const auto pos = Position(col, row);
            const auto [c, r] = pos.toInt();
            const auto color = ((c + r) % 2 != 0) ? QColor("#f0d9b5") : QColor("#b58863");
            auto* square = new SquareView(m_GridLayout->widget(), pos, color);
            const auto& [ grid_row , grid_col ] = convertToGridLayout(pos);
            m_GridLayout->addWidget(square, grid_row, grid_col);
        }
    }

    const auto cols = m_GridLayout->columnCount();
    const auto rows = m_GridLayout->rowCount();
    for (auto r = 0; r < rows; r++)
    {
        for (auto c = 0; c < cols; c++)
        {
            auto* cell = m_GridLayout->itemAtPosition(r, c);
            if (auto* square = dynamic_cast<SquareView*>(cell->widget()))
                m_Squares.insert({ square->getPosition(), square });
        }
    }
}

void BoardView::initConnections()
{
    LOG_TRACE("BoardView Connect Signals/Slots");
    for (const auto& [ pos, square ] : m_Squares)
        connect(square, &SquareView::squareClick, this, &BoardView::receiveClickOnSquare);
}

std::pair<int, int> BoardView::convertToGridLayout(const Position& _pos)
{
    auto rowCol = std::make_pair( 0, 0 );

    switch (_pos.row)
    {
    case Row::One:
        rowCol.first = 7;
        break;
    case Row::Two:
        rowCol.first = 6;
        break;
    case Row::Three:
        rowCol.first = 5;
        break;
    case Row::Four:
        rowCol.first = 4;
        break;
    case Row::Five:
        rowCol.first = 3;
        break;
    case Row::Six:
        rowCol.first = 2;
        break;
    case Row::Seven:
        rowCol.first = 1;
        break;
    case Row::Eight:
        rowCol.first = 0;
        break;
    }

    switch (_pos.col)
    {
    case Col::A:
        rowCol.second = 0;
        break;
    case Col::B:
        rowCol.second = 1;
        break;
    case Col::C:
        rowCol.second = 2;
        break;
    case Col::D:
        rowCol.second = 3;
        break;
    case Col::E:
        rowCol.second = 4;
        break;
    case Col::F:
        rowCol.second = 5;
        break;
    case Col::G:
        rowCol.second = 6;
        break;
    case Col::H:
        rowCol.second = 7;
        break;
    }

    return rowCol;
}

void BoardView::receiveClickOnSquare(const Position& _pos)
{
    emit clickOnBoard(_pos);
}

void BoardView::receiveRequestSetPieceOnChessBoard(const Position& _pos, const Color& _color, const Type& _type)
{
    m_Squares.at(_pos)->setPiece(_color, _type);
}

void BoardView::receiveRequestRemovePieceFromChessBoard(const Position& _pos)
{
    m_Squares.at(_pos)->removePiece();
}

void BoardView::receiveRequestSetDefaultSquareRenderer(const Position& _pos)
{
    m_Squares.at(_pos)->setDefaultSquareRenderer();
}

void BoardView::receiveRequestSetSelectedPieceRenderer(const Position& _pos)
{
    m_Squares.at(_pos)->setSelectedPieceRenderer();
}

void BoardView::receiveRequestSetPotentialMoveRenderer(const Position& _pos)
{
    m_Squares.at(_pos)->setPotentialSquareRenderer();
}

void BoardView::receiveRequestSetPreviousMoveRenderer(const Position& _pos)
{
    m_Squares.at(_pos)->setPreviousMoveRenderer();
}

void BoardView::receiveRequestSetKingJaqueRenderer(const Position& _pos)
{
    m_Squares.at(_pos)->setKingJaqueRenderer();
}

void BoardView::receiveRequestSetPromoteRenderer(const Position& _pawn_pos)
{
    auto curr = _pawn_pos;
    if (_pawn_pos.row == Row::Eight)
    {
        const auto queen = curr;
        curr.prevRow();
        const auto knight = curr;
        curr.prevRow();
        const auto rook = curr;
        curr.prevRow();
        const auto bishop = curr;
        for (auto& [ pos, square ] : m_Squares)
        {
            if (pos == queen)
                square->setPromoteOptionRenderer(Color::White, Type::Q);
            else if (pos == knight)
                square->setPromoteOptionRenderer(Color::White, Type::N);
            else if (pos == rook)
                square->setPromoteOptionRenderer(Color::White, Type::R);
            else if (pos == bishop)
                square->setPromoteOptionRenderer(Color::White, Type::B);
            else 
                square->setPromoteDefaultRenderer();
        }
    }
    else if (_pawn_pos.row == Row::One)
    {
        const auto queen = curr;
        curr.nextRow();
        const auto knight = curr;
        curr.nextRow();
        const auto rook = curr;
        curr.nextRow();
        const auto bishop = curr;
        for (auto& [ pos, square ] : m_Squares)
        {
            if (pos == queen)
                square->setPromoteOptionRenderer(Color::Black, Type::Q);
            else if (pos == knight)
                square->setPromoteOptionRenderer(Color::Black, Type::N);
            else if (pos == rook)
                square->setPromoteOptionRenderer(Color::Black, Type::R);
            else if (pos == bishop)
                square->setPromoteOptionRenderer(Color::Black, Type::B);
            else 
                square->setPromoteDefaultRenderer();
        }
    }
    else
     Q_UNREACHABLE();
}

