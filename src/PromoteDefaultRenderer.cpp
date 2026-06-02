#include "PromoteDefaultRenderer.hpp"
#include <QSvgRenderer>

PromoteDefaultRenderer::PromoteDefaultRenderer(const QRect& _rect, const QColor& _color, const QString& _file)
    : m_Rect(_rect)
    , m_Color(_color)
    , m_SvgFile(_file)
{
}

PromoteDefaultRenderer::~PromoteDefaultRenderer()
{
}

void PromoteDefaultRenderer::render(QPainter& _painter)
{
    if (!m_SvgFile.isEmpty())
    {
        const auto gray = QColor(0, 0, 0, 200);
        _painter.fillRect(m_Rect, m_Color);
        QSvgRenderer svg(m_SvgFile);
        svg.render(&_painter, m_Rect);
        _painter.fillRect(m_Rect, gray);
    }
    else
    {
        const auto gray = QColor(0, 0, 0, 200);
        _painter.fillRect(m_Rect, m_Color);
        _painter.fillRect(m_Rect, gray);
    }
}

void PromoteDefaultRenderer::resize(const QRect& _rect)
{
    m_Rect = _rect;
}

void PromoteDefaultRenderer::enter()
{
}

void PromoteDefaultRenderer::leave()
{
}

