#include "MoveListView.hpp"
#include <QHeaderView>
#include <QFontDatabase>
#include <QLabel>
#include "Logger.hpp"

MoveListView::MoveListView(Logger& _logger)
    : QTableWidget { nullptr }
    , m_Logger(_logger)
{
    init();
    initRowsAndColumns();
    initGeometry();
}

MoveListView::~MoveListView()
{
}

void MoveListView::init()
{
    setObjectName("MoveListView");
    horizontalHeader()->hide();
    verticalHeader()->hide();
    setSelectionMode(QAbstractItemView::NoSelection);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setShowGrid(false);
    setMouseTracking(true);
    setStyleSheet(R"(
    QTableWidget {
        background-color: #262421;
    }
    )");
}

void MoveListView::initGeometry()
{
    const auto min = QSize(300, 130);
    const auto max = QSize(500, 130);
    const auto policy = QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMinimumSize(min);
    setMaximumSize(max);
    setSizePolicy(policy);
}

void MoveListView::initRowsAndColumns()
{
    const auto num_rows = 1;
    const auto num_columns = 3;
    const auto col_num = 0;
    const auto col_white = 1;
    const auto col_black = 2;
    setRowCount(num_rows);
    setColumnCount(num_columns);
    horizontalHeader()->setSectionResizeMode(col_num, QHeaderView::Fixed);
    horizontalHeader()->setSectionResizeMode(col_white, QHeaderView::Stretch);
    horizontalHeader()->setSectionResizeMode(col_black, QHeaderView::Stretch);
    setColumnWidth(0, 60);
}

QTableWidgetItem* MoveListView::makeItem(const QString& _text, const int& _column)
{
    auto *item = new QTableWidgetItem(_text);

    if (_column == 0)
    {
        item->setBackground(QColor("#33312e"));
        item->setTextAlignment(Qt::AlignCenter);
        item->setForeground(QBrush("#bababa"));
    }
    else
    {
        item->setBackground(QColor("#262421"));
        item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        item->setForeground(QBrush("#bababa"));
    }

    return item;
}

void MoveListView::receiveRequestToRecordNormalMove(const QString& _algebraic_notation)
{
    const auto num_col = 0;
    const auto white_col = 1;
    const auto black_col = 2;
    const auto curr_row = rowCount() - 1;

    auto* white_item = item(curr_row, white_col);
    auto piece_notation = QString("");
    auto move_notation = _algebraic_notation;
    if (_algebraic_notation.front().unicode() > 127)
    {
        piece_notation += _algebraic_notation.front();
        move_notation.removeFirst();
    }
    QLabel *label = new QLabel(QString("<span style='font-size:24pt;'>%1</span>%2").arg(piece_notation).arg(move_notation.toLower()));

    if (!white_item)
    {
        setItem(curr_row, num_col, makeItem(QString::number(rowCount()), num_col));
        setItem(curr_row, white_col, makeItem("", white_col));
        setCellWidget(curr_row, white_col, label);
    }
    else 
    {
        auto* black_item = item(curr_row, black_col);
        if (!black_item)
        {
            setItem(curr_row, black_col, makeItem("", white_col));
            setRowCount(rowCount() + 1);
            setCellWidget(curr_row, black_col, label);
        }
    }

    scrollToBottom();
}

void MoveListView::receiveRequestToRecordNormalCapture(const QString& _capture_notation)
{
    const auto num_col = 0;
    const auto white_col = 1;
    const auto black_col = 2;
    const auto curr_row = rowCount() - 1;

    auto* white_item = item(curr_row, white_col);
    auto piece_notation = QString("");
    auto move_notation = _capture_notation;
    if (_capture_notation.front().unicode() > 127)
    {
        piece_notation += _capture_notation.front();
        move_notation.removeFirst();
    }
    QLabel *label = new QLabel(QString("<span style='font-size:24pt;'>%1</span>%2").arg(piece_notation).arg(move_notation.toLower()));

    if (!white_item)
    {
        setItem(curr_row, num_col, makeItem(QString::number(rowCount()), num_col));
        setItem(curr_row, white_col, makeItem("", white_col));
        setCellWidget(curr_row, white_col, label);
    }
    else 
    {
        auto* black_item = item(curr_row, black_col);
        if (!black_item)
        {
            setItem(curr_row, black_col, makeItem("", white_col));
            setRowCount(rowCount() + 1);
            setCellWidget(curr_row, black_col, label);
        }
    }

    scrollToBottom();
}

void MoveListView::receiveRequestToRecordCastling(const QString& _castle_notation)
{
    const auto num_col = 0;
    const auto white_col = 1;
    const auto black_col = 2;
    const auto curr_row = rowCount() - 1;

    auto* white_item = item(curr_row, white_col);
    auto piece_notation = QString("");
    auto move_notation = _castle_notation;
    if (_castle_notation.front().unicode() > 127)
    {
        piece_notation += _castle_notation.front();
        move_notation.removeFirst();
    }
    QLabel *label = new QLabel(QString("<span style='font-size:24pt;'>%1</span>%2").arg(piece_notation).arg(move_notation.toLower()));

    if (!white_item)
    {
        setItem(curr_row, num_col, makeItem(QString::number(rowCount()), num_col));
        setItem(curr_row, white_col, makeItem("", white_col));
        setCellWidget(curr_row, white_col, label);
    }
    else 
    {
        auto* black_item = item(curr_row, black_col);
        if (!black_item)
        {
            setItem(curr_row, black_col, makeItem("", white_col));
            setRowCount(rowCount() + 1);
            setCellWidget(curr_row, black_col, label);
        }
    }

    scrollToBottom();
}

void MoveListView::receiveRequestToRecordPawnPromotion(const QString& _promoted_type, const Color& _player_color)
{
    const auto white_col = 1;
    const auto black_col = 2;
    const auto curr_row = (_player_color == Color::White) ? rowCount() - 1 : rowCount();
    const auto curr_col = (_player_color == Color::White) ? white_col : black_col;

    if (curr_row < 1)
        Q_UNREACHABLE();

    if (QLabel *label = qobject_cast<QLabel*>(cellWidget(curr_row, curr_col)))
        label->setText(label->text() + "=" + _promoted_type);
    else 
        Q_UNREACHABLE();
}

void MoveListView::receiveRequestToRecordCheckMate(const Color& _kcolor)
{
    const auto white_col = 1;
    const auto black_col = 2;
    const auto curr_row = (_kcolor == Color::White) ? rowCount() - 2 : rowCount() - 1;
    const auto curr_col = (_kcolor == Color::White) ? black_col : white_col;

    if (curr_row < 1)
        Q_UNREACHABLE();

    if (QLabel *label = qobject_cast<QLabel*>(cellWidget(curr_row, curr_col)))
        label->setText(label->text() + "#");
    else 
        Q_UNREACHABLE();
}

void MoveListView::receiveRequestToRecordKingJaque(const Color& _kcolor)
{
    const auto white_col = 1;
    const auto black_col = 2;
    const auto curr_row = (_kcolor == Color::White) ? rowCount() - 2 : rowCount() - 1;
    const auto curr_col = (_kcolor == Color::White) ? black_col : white_col;

    if (curr_row < 1)
        Q_UNREACHABLE();

    if (QLabel *label = qobject_cast<QLabel*>(cellWidget(curr_row, curr_col)))
        label->setText(label->text() + "+");
    else 
        Q_UNREACHABLE();
}

void MoveListView::receiveRequestToRecordStaleMate(const Color& )
{
}

