#ifndef PREVIOUS_MOVE_RENDERER_H
#define PREVIOUS_MOVE_RENDERER_H

#include "IRenderer.hpp"

class PreviousMoveRenderer : public IRenderer
{
public:

    PreviousMoveRenderer(const QRect&, const QColor&, const QString&);
    ~PreviousMoveRenderer();
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

