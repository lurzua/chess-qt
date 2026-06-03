#include "SquareView.hpp"
#include "DefaultSquareRenderer.hpp"
#include "SelectedPieceRenderer.hpp"
#include "PreviousMoveRenderer.hpp"
#include "PotentialSquareRenderer.hpp"
#include "KingJaqueRenderer.hpp"
#include "PromoteDefaultRenderer.hpp"
#include "PromoteOptionRenderer.hpp"
#include <QPainter>
#include "Logger.hpp"

SquareView::SquareView(QWidget* _parent, const Position& _pos, const QColor& _color, Logger& _logger)
    : QWidget{ _parent }
    , m_Pos(_pos)
    , m_Color(_color)
    , m_Logger(_logger)
{
    init();
    initRenderers();
}

SquareView::~SquareView()
{
}

Position SquareView::getPosition() const
{
    return m_Pos;
}

void SquareView::initRenderers()
{
    for (const auto& [ col, row, color, type ] : g_StartingPieces)
    {
        if (m_Pos.col == col && m_Pos.row == row)
            updateSvgFile(color, type);
    }

    m_Renderer = std::make_unique<DefaultSquareRenderer>(rect(), m_Color, m_SvgFile);
}

void SquareView::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    m_Renderer->render(painter);
}

void SquareView::mousePressEvent(QMouseEvent*)
{
    m_Logger.log("Clicked on " + m_Pos.toStr().toStdString());
    emit squareClick(m_Pos);
}

void SquareView::enterEvent(QEnterEvent*)
{
    m_Logger.log("Entered " + m_Pos.toStr().toStdString());
    m_Renderer->enter();
    update();
}

void SquareView::leaveEvent(QEvent*)
{
    m_Logger.log("Leave " + m_Pos.toStr().toStdString());
    m_Renderer->leave();
    update();
}

void SquareView::resizeEvent(QResizeEvent*)
{
    m_Renderer->resize(rect());
}

void SquareView::init()
{
    const auto min = QSize(50, 50);
    const auto max = QSize(200, 200);
    const auto policy = QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMinimumSize(min);
    setMaximumSize(max);
    setSizePolicy(policy);
    setObjectName("SquareView_" + m_Pos.toStr());
}

void SquareView::updateSvgFile(const Color& _color, const Type& _type)
{
    switch (_type)
    {
    case Type::P:
        m_SvgFile = (_color == Color::White) ? ":/piece/img/white-pawn.svg" : ":/piece/img/black-pawn.svg";
        break;
    case Type::N:
        m_SvgFile = (_color == Color::White) ? ":/piece/img/white-knight.svg" : ":/piece/img/black-knight.svg";
        break;
    case Type::B:
        m_SvgFile = (_color == Color::White) ? ":/piece/img/white-bishop.svg" : ":/piece/img/black-bishop.svg";
        break;
    case Type::R:
        m_SvgFile = (_color == Color::White) ? ":/piece/img/white-rook.svg" : ":/piece/img/black-rook.svg";
        break;
    case Type::Q:
        m_SvgFile = (_color == Color::White) ? ":/piece/img/white-queen.svg" : ":/piece/img/black-queen.svg";
        break;
    case Type::K:
        m_SvgFile = (_color == Color::White) ? ":/piece/img/white-king.svg" : ":/piece/img/black-king.svg";
        break;
    }
}

void SquareView::setDefaultSquareRenderer()
{
    m_Renderer.reset();
    m_Renderer = std::make_unique<DefaultSquareRenderer>(rect(), m_Color, m_SvgFile);
    update();
}

void SquareView::setSelectedPieceRenderer()
{
    m_Renderer.reset();
    m_Renderer = std::make_unique<SelectedPieceRenderer>(rect(), m_Color, m_SvgFile);
    update();
}

void SquareView::setPreviousMoveRenderer()
{
    m_Renderer.reset();
    m_Renderer = std::make_unique<PreviousMoveRenderer>(rect(), m_Color, m_SvgFile);
    update();
}

void SquareView::setPotentialSquareRenderer()
{
    m_Renderer.reset();
    m_Renderer = std::make_unique<PotentialSquareRenderer>(rect(), m_Color, m_SvgFile);
    update();
}

void SquareView::setKingJaqueRenderer()
{
    m_Renderer.reset();
    m_Renderer = std::make_unique<KingJaqueRenderer>(rect(), m_Color, m_SvgFile);
    update();
}

void SquareView::setPromoteDefaultRenderer()
{
    m_Renderer.reset();
    m_Renderer = std::make_unique<PromoteDefaultRenderer>(rect(), m_Color, m_SvgFile);
    update();
}

void SquareView::setPromoteOptionRenderer(const Color& _team, const Type& _type)
{
    updateSvgFile(_team, _type);
    m_Renderer.reset();
    m_Renderer = std::make_unique<PromoteOptionRenderer>(rect(), m_Color, m_SvgFile);
    update();
}

void SquareView::removePiece()
{
    m_Logger.log("Remove Piece On " + m_Pos.toStr().toStdString());
    m_SvgFile = QString("");
    setDefaultSquareRenderer();
    update();
}

void SquareView::setPiece(const Color& _color, const Type& _type)
{
    m_Logger.log("Set Piece On " + m_Pos.toStr().toStdString());
    updateSvgFile(_color, _type);
    setDefaultSquareRenderer();
    update();
}

