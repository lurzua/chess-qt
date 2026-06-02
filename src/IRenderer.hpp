#ifndef IRENDERER_H
#define IRENDERER_H

#include <QPainter>
#include <QEnterEvent>
#include <QEvent>

class IRenderer
{
public:

    virtual ~IRenderer() = default;
    virtual void render(QPainter&) = 0;
    virtual void resize(const QRect&) = 0;
    virtual void enter() = 0;
    virtual void leave() = 0;
};

#endif

