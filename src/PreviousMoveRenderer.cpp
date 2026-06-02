#include "PreviousMoveRenderer.hpp"
#include <QSvgRenderer>

PreviousMoveRenderer::PreviousMoveRenderer(const QRect& _rect, const QColor& _color, const QString& _file)
    : m_Rect(_rect)
    , m_Color(_color)
    , m_SvgFile(_file)
{
}

PreviousMoveRenderer::~PreviousMoveRenderer()
{
}

void PreviousMoveRenderer::render(QPainter& _painter)
{
    const auto yellow_highlight = QColor(155, 199, 0, 105);
    _painter.fillRect(m_Rect, m_Color);
    _painter.fillRect(m_Rect, yellow_highlight);
    _painter.setRenderHint(QPainter::Antialiasing);
    
    if (!m_SvgFile.isEmpty())
    {
        QSvgRenderer svg(m_SvgFile);
        svg.render(&_painter, m_Rect);
    }
}

void PreviousMoveRenderer::resize(const QRect& _rect)
{
    m_Rect = _rect;
}

void PreviousMoveRenderer::enter()
{
}

void PreviousMoveRenderer::leave()
{
}

