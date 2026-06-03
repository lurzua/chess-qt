#ifndef SQUAREVIEW_H
#define SQUAREVIEW_H

#include <QWidget>
#include <QSvgRenderer>
#include "Position.hpp"
#include "Enums.hpp"

class IRenderer;
class DefaultSquareRenderer;
class SelectedPieceRenderer;
class PreviousMoveRenderer;
class PotentialSquareRenderer;
class KingJaqueRenderer;
class PromoteDefaultRenderer;
class PromoteOptionRenderer;
class Logger;

class SquareView : public QWidget
{
    Q_OBJECT

signals:

    void squareClick(const Position&);

protected:

    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void enterEvent(QEnterEvent*) override;
    void leaveEvent(QEvent*) override;
    void resizeEvent(QResizeEvent*) override;

public:

    SquareView(QWidget*, const Position&, const QColor&, Logger&);
    ~SquareView();
    Position getPosition() const;
    void initRenderers();
    void updateSvgFile(const Color&, const Type&);
    void removePiece();
    void setPiece(const Color&, const Type&);
    void setDefaultSquareRenderer();
    void setSelectedPieceRenderer();
    void setPreviousMoveRenderer();
    void setPotentialSquareRenderer();
    void setKingJaqueRenderer();
    void setPromoteDefaultRenderer();
    void setPromoteOptionRenderer(const Color&, const Type&);

private:

    void init();
    void selectPieceSvg(const Color&, const Type&);

    const Position m_Pos;
    const QColor m_Color;
    std::unique_ptr<IRenderer> m_Renderer;
    QString m_SvgFile;
    Logger& m_Logger;
};

#endif // SQUAREVIEW_H

