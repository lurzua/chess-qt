#include "SelectedPieceRenderer.hpp"
#include <QSvgRenderer>

SelectedPieceRenderer::SelectedPieceRenderer(const QRect& _rect, const QColor& _color, const QString& _file)
    : m_Rect(_rect)
    , m_Color(_color)
    , m_SvgFile(_file)
{
}

SelectedPieceRenderer::~SelectedPieceRenderer()
{
}

void SelectedPieceRenderer::render(QPainter& _painter)
{
    const auto green_highlight = QColor(20, 85, 30, 128);
    QSvgRenderer svg(m_SvgFile);
    _painter.fillRect(m_Rect, m_Color);
    _painter.fillRect(m_Rect, green_highlight);
    _painter.setRenderHint(QPainter::Antialiasing);
    svg.render(&_painter, m_Rect);
}

void SelectedPieceRenderer::resize(const QRect& _rect)
{
    m_Rect = _rect;
}

void SelectedPieceRenderer::enter()
{
}

void SelectedPieceRenderer::leave()
{
}

