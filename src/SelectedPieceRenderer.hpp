#ifndef SELECTED_PIECE_RENDERER_H
#define SELECTED_PIECE_RENDERER_H

#include "IRenderer.hpp"

class SelectedPieceRenderer : public IRenderer
{
public:

    SelectedPieceRenderer(const QRect&, const QColor&, const QString&);
    ~SelectedPieceRenderer();
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

