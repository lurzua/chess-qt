#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSvgWidget>
#include <QHBoxLayout>

class BoardView;
class Controller;
class Model;
//class TimerView;
class MoveListView;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

    void initBoard();
    void initMoveList();
    void initRootLayout();
    void initController();
    void initModel();
    void initConnections();

    Ui::MainWindow* m_UI;
    BoardView* m_BoardView;
    MoveListView* m_MoveListView;
    QHBoxLayout* m_HboxRootLayout;
    std::unique_ptr<Controller> m_Controller;
    std::unique_ptr<Model> m_Model;
};

#endif

