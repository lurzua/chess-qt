#include "KingJaqueRenderer.hpp"
#include <QSvgRenderer>

KingJaqueRenderer::KingJaqueRenderer(const QRect& _rect, const QColor& _color, const QString& _file)
    : m_Rect(_rect)
    , m_Color(_color)
    , m_SvgFile(_file)
{
}

KingJaqueRenderer::~KingJaqueRenderer()
{
}

void KingJaqueRenderer::render(QPainter& _painter)
{
    _painter.fillRect(m_Rect, m_Color);
    _painter.setRenderHint(QPainter::Antialiasing, true);

    QPoint center = m_Rect.center();

    // Ellipse radii (adjust to match your CSS "ellipse")
    qreal rx = m_Rect.width() / 1.75;
    qreal ry = m_Rect.height() / 1.75;

    QRadialGradient grad(center, rx); // base radius

    // Match CSS stops
    grad.setColorAt(0.0,  QColor(255, 0, 0));        // 0%
    grad.setColorAt(0.25, QColor(231, 0, 0));        // 25%
    grad.setColorAt(0.89, QColor(169, 0, 0, 0));     // 89%
    grad.setColorAt(1.0,  QColor(158, 0, 0, 0));     // 100%

    // Make it elliptical via transform
    QTransform t;
    t.translate(center.x(), center.y());
    t.scale(1.0, ry / rx);   // squash vertically
    t.translate(-center.x(), -center.y());
    //grad.setTransform(t);

    _painter.setBrush(grad);
    _painter.setPen(Qt::NoPen);
    _painter.drawRect(m_Rect);

    QSvgRenderer svg(m_SvgFile);
    svg.render(&_painter, m_Rect);
}

void KingJaqueRenderer::resize(const QRect& _rect)
{
    m_Rect = _rect;
}

void KingJaqueRenderer::enter()
{
}

void KingJaqueRenderer::leave()
{
}

