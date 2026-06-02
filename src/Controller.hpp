#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "Position.hpp"

class Piece;

using ChessMapConst = const std::unordered_map<Position, const Piece*>;

class Controller : public QObject
{
    Q_OBJECT

signals:

    void requestChessMap();
    void requestOverlayReset();
    void requestOverlayUpdateWithPieceOptions(const Position&);
    void requestChessMapUpdateNormalMove(const Position&, const Position&);
    void requestChessMapUpdateNormalCapture(const Position&, const Position&);
    void requestChessMapUpdateQueenSideCastle(const Position&, const Position&);
    void requestChessMapUpdateKingSideCastle(const Position&, const Position&);
    void requestChessMapUpdateEnpassant(const Position&, const Position&);
    void requestChessMapUpdatePawnPromotion(const Position&, const Position&);
    void requestPawnPromoteToQueen(const Position&);
    void requestPawnPromoteToKnight(const Position&);
    void requestPawnPromoteToRook(const Position&);
    void requestPawnPromoteToBishop(const Position&);


public slots:

    void receiveClickedOnChessboard(const Position&);
    void receiveChessMap(ChessMapConst&);

public:

    explicit Controller(QObject *parent = nullptr);

private:

    void nextPlayerTurn();
    void processGameStateBegin(const Position&);
    void processGameStateEnd(const Position&);
    void processGameStatePawnPromotion(const Position&);
    bool isPawnPromotion(const Position&, const Position&);
    bool isKingsideCastle(const Position&, const Position&);
    bool isQueensideCastle(const Position&, const Position&);
    bool isEnpassant(const Position&, const Position&);

private:

    enum class PlayerTurnState { SelectPiece, SelectDestination, PawnPromotion };
    PlayerTurnState  m_PlayerTurnState = PlayerTurnState::SelectPiece;
    Color m_PlayerTurn = Color::White;
    const Piece* m_CurrentPiece = nullptr;
    std::unordered_map<Position, const Piece*> m_ChessMap;
};

#endif

