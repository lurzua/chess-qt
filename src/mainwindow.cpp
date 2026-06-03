#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "BoardView.hpp"
#include "Controller.hpp"
#include "Model.hpp"
#include "MoveListView.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_UI(new Ui::MainWindow)
{
    m_UI->setupUi(this);

    initBoard();
    initMoveList();
    initRootLayout();
    initController();
    initModel();
    initConnections();
}

MainWindow::~MainWindow()
{
    delete m_UI;
    delete m_BoardView;
    delete m_MoveListView;
}

void MainWindow::initBoard()
{
    m_BoardView = new BoardView(m_Logger);
}

void MainWindow::initMoveList()
{
    m_MoveListView = new MoveListView(m_Logger);
}

void MainWindow::initRootLayout()
{
    m_HboxRootLayout = new QHBoxLayout;
    centralWidget()->setLayout(m_HboxRootLayout);
    m_HboxRootLayout->setObjectName("HBoxViews");
    m_HboxRootLayout->addWidget(m_BoardView);
    m_HboxRootLayout->addWidget(m_MoveListView);
    m_HboxRootLayout->setStretch(0, 4);
    m_HboxRootLayout->setStretch(1, 1);
}

void MainWindow::initController()
{
    m_Controller = std::make_unique<Controller>(m_Logger);
}

void MainWindow::initModel()
{
    m_Model = std::make_unique<Model>(m_Logger);
}

void MainWindow::initConnections()
{
    connect(m_BoardView,  &BoardView::clickOnBoard,     m_Controller.get(), &Controller::receiveClickedOnChessboard);

    connect(m_Controller.get(), &Controller::requestChessMap,                      m_Model.get(), &Model::receiveRequestChessMap);
    connect(m_Controller.get(), &Controller::requestOverlayReset,                  m_Model.get(), &Model::receiveRequestOverlayReset);
    connect(m_Controller.get(), &Controller::requestOverlayUpdateWithPieceOptions, m_Model.get(), &Model::receiveRequestOverlayUpdateWithPieceOptions);
    connect(m_Model.get(),      &Model::sendChessMap, m_Controller.get(), &Controller::receiveChessMap);

    connect(m_Controller.get(), &Controller::requestChessMapUpdateNormalMove,      m_Model.get(), &Model::receiveRequestChessMapUpdateNormalMove);
    connect(m_Controller.get(), &Controller::requestChessMapUpdateNormalCapture,   m_Model.get(), &Model::receiveRequestChessMapUpdateNormalCapture);
    connect(m_Controller.get(), &Controller::requestChessMapUpdateKingSideCastle,  m_Model.get(), &Model::receiveRequestChessMapUpdateKingSideCastle);
    connect(m_Controller.get(), &Controller::requestChessMapUpdateQueenSideCastle, m_Model.get(), &Model::receiveRequestChessMapUpdateQueenSideCastle);
    connect(m_Controller.get(), &Controller::requestChessMapUpdateEnpassant,       m_Model.get(), &Model::receiveRequestChessMapUpdateEnpassant);
    connect(m_Controller.get(), &Controller::requestChessMapUpdatePawnPromotion,   m_Model.get(), &Model::receiveRequestChessMapUpdatePawnPromotion);

    connect(m_Controller.get(), &Controller::requestPawnPromoteToQueen,  m_Model.get(), &Model::receiveRequestPawnPromoteToQueen);
    connect(m_Controller.get(), &Controller::requestPawnPromoteToKnight, m_Model.get(), &Model::receiveRequestPawnPromoteToKnight);
    connect(m_Controller.get(), &Controller::requestPawnPromoteToRook,   m_Model.get(), &Model::receiveRequestPawnPromoteToRook);
    connect(m_Controller.get(), &Controller::requestPawnPromoteToBishop, m_Model.get(), &Model::receiveRequestPawnPromoteToBishop);

    connect(m_Model.get(), &Model::requestSetPieceOnChessBoard, m_BoardView, &BoardView::receiveRequestSetPieceOnChessBoard);
    connect(m_Model.get(), &Model::requestRemovePieceFromChessBoard, m_BoardView, &BoardView::receiveRequestRemovePieceFromChessBoard);
    connect(m_Model.get(), &Model::requestSetDefaultSquareRenderer, m_BoardView, &BoardView::receiveRequestSetDefaultSquareRenderer);
    connect(m_Model.get(), &Model::requestSetSelectedPieceRenderer, m_BoardView, &BoardView::receiveRequestSetSelectedPieceRenderer);
    connect(m_Model.get(), &Model::requestSetPotentialMoveRenderer, m_BoardView, &BoardView::receiveRequestSetPotentialMoveRenderer);
    connect(m_Model.get(), &Model::requestSetPreviousMoveRenderer, m_BoardView, &BoardView::receiveRequestSetPreviousMoveRenderer);
    connect(m_Model.get(), &Model::requestSetKingJaqueRenderer, m_BoardView, &BoardView::receiveRequestSetKingJaqueRenderer);
    connect(m_Model.get(), &Model::requestSetPromoteRenderer, m_BoardView, &BoardView::receiveRequestSetPromoteRenderer);

    connect(m_Model.get(), &Model::requestToRecordNormalMove,    m_MoveListView, &MoveListView::receiveRequestToRecordNormalMove);
    connect(m_Model.get(), &Model::requestToRecordNormalCapture, m_MoveListView, &MoveListView::receiveRequestToRecordNormalCapture);
    connect(m_Model.get(), &Model::requestToRecordCastling,      m_MoveListView, &MoveListView::receiveRequestToRecordCastling);
    connect(m_Model.get(), &Model::requestToRecordPawnPromotion, m_MoveListView, &MoveListView::receiveRequestToRecordPawnPromotion);
    connect(m_Model.get(), &Model::requestToRecordCheckMate,     m_MoveListView, &MoveListView::receiveRequestToRecordCheckMate);
    connect(m_Model.get(), &Model::requestToRecordKingJaque,     m_MoveListView, &MoveListView::receiveRequestToRecordKingJaque);
    connect(m_Model.get(), &Model::requestToRecordStaleMate,     m_MoveListView, &MoveListView::receiveRequestToRecordStaleMate);
}

