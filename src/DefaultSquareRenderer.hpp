#ifndef DEFAULT_SQUARE_RENDERER_H
#define DEFAULT_SQUARE_RENDERER_H

#include "IRenderer.hpp"

class DefaultSquareRenderer : public IRenderer
{
public:

    DefaultSquareRenderer(const QRect&, const QColor&, const QString&);
    ~DefaultSquareRenderer();
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
 
