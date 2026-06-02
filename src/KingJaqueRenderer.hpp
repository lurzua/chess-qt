#ifndef KING_JAQUE_RENDERER_H
#define KING_JAQUE_RENDERER_H

#include "IRenderer.hpp"

class KingJaqueRenderer : public IRenderer
{
public:

    KingJaqueRenderer(const QRect&, const QColor&, const QString&);
    ~KingJaqueRenderer();
    void render(QPainter&) override;
    void resize(const QRect&) override;
    void enter() override;
    void leave() override;

private:

    QRect m_Rect;
    const QColor m_Color;
    const QString m_SvgFile;
};

#endif

