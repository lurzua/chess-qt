#ifndef PROMOTE_OPTION_RENDERER_H
#define PROMOTE_OPTION_RENDERER_H

#include "IRenderer.hpp"

class PromoteOptionRenderer : public IRenderer
{
public:

    PromoteOptionRenderer(const QRect&, const QColor&, const QString&);
    ~PromoteOptionRenderer();
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

