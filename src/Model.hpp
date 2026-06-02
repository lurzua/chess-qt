#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include "Position.hpp"

class Piece;

class Model : public QObject
{
    Q_OBJECT

public:

    using ChessMap = std::unordered_map<Position, Piece*>;
    using ChessMapConst = const std::unordered_map<Position, const Piece*>;

signals:

    void sendChessMap(ChessMapConst&);
    void requestSetPieceOnChessBoard(const Position&, const Color&, const Type&);
    void requestRemovePieceFromChessBoard(const Position&);
    void requestSetDefaultSquareRenderer(const Position&);
    void requestSetSelectedPieceRenderer(const Position&);
    void requestSetPotentialMoveRenderer(const Position&);
    void requestSetPreviousMoveRenderer(const Position&);
    void requestSetKingJaqueRenderer(const Position&);
    void requestSetPromoteRenderer(const Position&);

    void requestToRecordNormalMove(const QString&);
    void requestToRecordNormalCapture(const QString&);
    void requestToRecordCastling(const QString&);
    void requestToRecordPawnPromotion(const QString&, const Color&);
    void requestToRecordCheckMate(const Color&);
    void requestToRecordKingJaque(const Color&);
    void requestToRecordStaleMate(const Color&);

public slots:

    void receiveRequestChessMap();
    void receiveRequestOverlayReset();
    void receiveRequestOverlayUpdateWithPieceOptions(const Position&);
    void receiveRequestChessMapUpdateNormalMove(const Position&, const Position&);
    void receiveRequestChessMapUpdateNormalCapture(const Position&, const Position&);
    void receiveRequestChessMapUpdateQueenSideCastle(const Position&, const Position&);
    void receiveRequestChessMapUpdateKingSideCastle(const Position&, const Position&);
    void receiveRequestChessMapUpdateEnpassant(const Position&, const Position&);
    void receiveRequestChessMapUpdatePawnPromotion(const Position&, const Position&);
    void receiveRequestPawnPromoteToQueen(const Position&);
    void receiveRequestPawnPromoteToKnight(const Position&);
    void receiveRequestPawnPromoteToRook(const Position&);
    void receiveRequestPawnPromoteToBishop(const Position&);

    //void receiveRequestSetModelAccordingToFEN(const QString& fen_string);

public:

    explicit Model(QObject* _parent = nullptr);
    ~Model();

private:

    void initChessMap();
    void initPieces();
    void setPiecesOnChessMap();
    void resetOverlay();
    ChessMapConst getChessMapConst() const;
    void updateChessBoard();
    bool isKingInCheck(const Color&) const;
    bool isCheckMate(const Color&) const;
    bool isStaleMate(const Color&) const;
    QString getAlgebraicNotationNormalMove(const Position&, const Position&);
    QString getAlgebraicNotationNormalCapture(const Position&, const Position&);
    QString getAlgebraicNotationCastling(const Position&);
    QString getAlgebraicNotationPawnPromotion(const Position&);
    //QString convertChessMapToFen() const;
    //bool isThreefoldRepetition() const;
    //How to implement 3-fold repetition?
    //Use FEN strings uniqueness OR use Zobrist Hashing
    //bool is50MoveRule() const;
    //How does the 50 move rule works?
    //If both players have played 50 consecutive moves without capturing or moving a PAWN, then game ends in a draw (ELECTED).

    std::vector<Piece*> m_Pieces;
    ChessMap m_ChessMap;
    std::optional<Position> m_PreviousMoveStart;
    std::optional<Position> m_PreviousMoveEnd;
    //std::unordered_map<QString, uint16_t> m_Fen;
};

#endif

