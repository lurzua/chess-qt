#ifndef POTENTIAL_SQUARE_RENDERER_H
#define POTENTIAL_SQUARE_RENDERER_H

#include "IRenderer.hpp"

class PotentialSquareRenderer : public IRenderer
{
public:

    PotentialSquareRenderer(const QRect&, const QColor&, const QString&);
    ~PotentialSquareRenderer();
    void render(QPainter&) override;
    void resize(const QRect&) override;
    void enter() override;
    void leave() override;

private:

    QRect m_Rect;
    const QColor m_Color;
    const QString m_SvgFile;
    enum class Mouse { Default, Hover };
    Mouse m_Mouse = Mouse::Default;
};

#endif

