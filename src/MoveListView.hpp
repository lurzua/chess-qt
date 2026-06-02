#ifndef MOVE_LIST_VIEW_H
#define MOVE_LIST_VIEW_H

#include <QListWidget>
#include <QTableWidget>
#include "Enums.hpp"

class MoveListView : public QTableWidget
{
    Q_OBJECT

public:

    explicit MoveListView(QWidget *parent = nullptr);
    ~MoveListView();

private:

    void init();
    void initGeometry();
    void initRowsAndColumns();
    QTableWidgetItem* makeItem(const QString&, const int&);

public slots:

    void receiveRequestToRecordNormalMove(const QString&);
    void receiveRequestToRecordNormalCapture(const QString&);
    void receiveRequestToRecordCastling(const QString&);
    void receiveRequestToRecordPawnPromotion(const QString&, const Color&);
    void receiveRequestToRecordCheckMate(const Color&);
    void receiveRequestToRecordKingJaque(const Color&);
    void receiveRequestToRecordStaleMate(const Color&);

private:
    
    QTableWidgetItem* last_item_hovered = nullptr;
};

#endif

