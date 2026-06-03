#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include <QWidget>
#include <QGridLayout>
#include "Position.hpp"

class SquareView;
class Piece;
class Logger;

class BoardView : public QWidget
{
    Q_OBJECT

signals:

    void clickOnBoard(const Position&);

public slots:

    void receiveClickOnSquare(const Position&);
    void receiveRequestSetPieceOnChessBoard(const Position&, const Color&, const Type&);
    void receiveRequestRemovePieceFromChessBoard(const Position&);
    void receiveRequestSetDefaultSquareRenderer(const Position&);
    void receiveRequestSetSelectedPieceRenderer(const Position&);
    void receiveRequestSetPotentialMoveRenderer(const Position&);
    void receiveRequestSetPreviousMoveRenderer(const Position&);
    void receiveRequestSetKingJaqueRenderer(const Position&);
    void receiveRequestSetPromoteRenderer(const Position&);

public:

    BoardView(Logger&);
    ~BoardView();

private:

    void init();
    void initGridLayout();
    void initSquares();
    void initPieces();
    void initConnections();
    std::pair<int, int> convertToGridLayout(const Position&);

    std::unordered_map<Position, SquareView* const> m_Squares;
    QGridLayout* m_GridLayout;
    Logger& m_Logger;
};

#endif

