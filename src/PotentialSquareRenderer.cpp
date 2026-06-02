#include "PotentialSquareRenderer.hpp"
#include <QSvgRenderer>

PotentialSquareRenderer::PotentialSquareRenderer(const QRect& _rect, const QColor& _color, const QString& _file)
    : m_Rect(_rect)
    , m_Color(_color)
    , m_SvgFile(_file)
{
}

PotentialSquareRenderer::~PotentialSquareRenderer()
{
}

void PotentialSquareRenderer::render(QPainter& _painter)
{
    switch (m_Mouse)
    {

        case Mouse::Default:
        {
            if (m_SvgFile.isEmpty())
            {
                const auto green = QColor(20, 85, 30, 128);
                const auto r = m_Rect;
                const auto ratio = 0.25;
                const auto radius = r.width() * ratio;
                const auto center = r.center();
                const auto circle = QRectF(
                    center.x() - (r.width() * (ratio / 2)),
                    center.y() - (r.height() * (ratio / 2)),
                    radius,
                    radius
                    );
                _painter.fillRect(m_Rect, m_Color);
                _painter.setPen(Qt::NoPen);
                _painter.setBrush(green);
                _painter.drawEllipse(circle);
            }
            else 
            {
                const auto green = QColor(20, 85, 30, 128);
                const auto s = m_Rect.height() * 0.20;
                auto leftTop  = QPolygonF();
                auto leftBot  = QPolygonF();
                auto rightTop = QPolygonF();
                auto rightBot = QPolygonF();
                const auto w = m_Rect.width();
                const auto h = m_Rect.height();
                leftTop  << QPointF(0, 0)
                         << QPointF(s, 0)
                         << QPointF(0, s);
                leftBot  << QPointF(0, h)
                         << QPointF(s, h)
                         << QPointF(0, h - s);
                rightTop << QPointF(w, 0)
                         << QPointF(w - s, 0)
                         << QPointF(w, s);
                rightBot << QPointF(w, h)
                         << QPointF(w - s, h)
                         << QPointF(w, h - s);
                _painter.fillRect(m_Rect, m_Color);
                _painter.setRenderHint(QPainter::Antialiasing);
                _painter.setBrush(green);
                _painter.setPen(green);
                _painter.drawPolygon(leftTop);
                _painter.drawPolygon(leftBot);
                _painter.drawPolygon(rightTop);
                _painter.drawPolygon(rightBot);
                QSvgRenderer svg(m_SvgFile);
                svg.render(&_painter, m_Rect);
            }
            
            break;
        }

        case Mouse::Hover:
        {
            if (m_SvgFile.isEmpty())
            {
                const auto green = QColor(20, 85, 30, 128);
                _painter.fillRect(m_Rect, m_Color);
                _painter.fillRect(m_Rect, green);
            }
            else 
            {
                const auto green_highlight = QColor(20, 85, 30, 128);
                QSvgRenderer svg(m_SvgFile);
                _painter.fillRect(m_Rect, m_Color);
                _painter.fillRect(m_Rect, green_highlight);
                _painter.setRenderHint(QPainter::Antialiasing);
                svg.render(&_painter, m_Rect);
            }

            break;
        }

    }
}

void PotentialSquareRenderer::resize(const QRect& _rect)
{
    m_Rect = _rect;
}

void PotentialSquareRenderer::enter()
{
    m_Mouse = Mouse::Hover;
}

void PotentialSquareRenderer::leave()
{
    m_Mouse = Mouse::Default;
}

