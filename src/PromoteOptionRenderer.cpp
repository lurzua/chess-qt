#include "PromoteOptionRenderer.hpp"
#include <QSvgRenderer>

PromoteOptionRenderer::PromoteOptionRenderer(const QRect& _rect, const QColor& _color, const QString& _file)
    : m_Rect(_rect)
    , m_Color(_color)
    , m_SvgFile(_file)
{
    if (_file.contains("queen"))
        m_Mouse = Mouse::Hover;
}

PromoteOptionRenderer::~PromoteOptionRenderer()
{
}

void PromoteOptionRenderer::render(QPainter& _painter)
{
    switch (m_Mouse)
    {

        case Mouse::Default:
        {
            _painter.fillRect(m_Rect, QColor(30, 30, 30));
            const auto side = qMin(m_Rect.width(), m_Rect.height());
            QRect circle_rect( (m_Rect.width() - side) / 2, (m_Rect.height() - side) / 2, side, side);
            _painter.setBrush(QColor(120, 120, 120));
            _painter.setPen(Qt::NoPen);
            _painter.drawEllipse(circle_rect);
            QSvgRenderer svg(m_SvgFile);
            svg.render(&_painter, m_Rect);
            break;
        }

        case Mouse::Hover:
        {
            const auto orange_highlight = QColor(252, 107, 3);
            _painter.fillRect(m_Rect, m_Color);
            _painter.fillRect(m_Rect, orange_highlight);
            _painter.setRenderHint(QPainter::Antialiasing);
            QSvgRenderer svg(m_SvgFile);
            svg.render(&_painter, m_Rect);
            break;
        }

    }
}

void PromoteOptionRenderer::resize(const QRect& _rect)
{
    m_Rect = _rect;
}

void PromoteOptionRenderer::enter()
{
    m_Mouse = Mouse::Hover;
}

void PromoteOptionRenderer::leave()
{
    m_Mouse = Mouse::Default;
}

