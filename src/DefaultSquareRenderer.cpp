#include "DefaultSquareRenderer.hpp"
#include <QSvgRenderer>

DefaultSquareRenderer::DefaultSquareRenderer(const QRect& _rect, const QColor& _color, const QString& _file)
    : m_Rect(_rect)
    , m_Color(_color)
    , m_SvgFile(_file)
{
}

DefaultSquareRenderer::~DefaultSquareRenderer()
{
}

void DefaultSquareRenderer::render(QPainter& _painter)
{
    _painter.fillRect(m_Rect, m_Color);
    _painter.setRenderHint(QPainter::Antialiasing);

    if (!m_SvgFile.isEmpty())
    {
        QSvgRenderer svg(m_SvgFile);
        svg.render(&_painter, m_Rect);
    }
}

void DefaultSquareRenderer::resize(const QRect& _rect)
{
    m_Rect = _rect;
}

void DefaultSquareRenderer::enter()
{
}

void DefaultSquareRenderer::leave()
{
}

