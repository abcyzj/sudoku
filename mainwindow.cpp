#include "mainwindow.h"
#include "gamecontroller.h"
#include "constants.h"
#include "ui_selectleveldialog.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>

#include <QAction>
#include <QToolBar>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent),
    gameArea(new QWidget(this)),
    mainScene(new QGraphicsScene(this)),
    mainView(new QGraphicsView(mainScene, gameArea)),
    timerScene(new QGraphicsScene(this)),
    timerView(new QGraphicsView(timerScene, gameArea)),
    controlScene(new QGraphicsScene(this)),
    controlView(new QGraphicsView(controlScene, gameArea)),
    controller(new GameController(mainScene, timerScene, controlScene, gameArea, this))
{
  initToolBar();
  initWidgets();
  initScene();
  setCentralWidget(gameArea);

  resize(gameArea->width(), toolBar->height() + gameArea->height());

  QTimer::singleShot(0, this, &MainWindow::initView);
}

MainWindow::~MainWindow()
{}

void MainWindow::initToolBar()
{
  QAction *loadAction = new QAction(QIcon(":/icons/loadGame.png"), tr("加载游戏文件"), this);
  loadAction->setShortcut(QKeySequence::Open);
  connect(loadAction, &QAction::triggered, this, &MainWindow::openGameFile);

  QAction *selectAction = new QAction(QIcon(":/icons/selectLevel.png"), tr("选择关卡"), this);
  selectAction->setShortcut(QKeySequence::Save);
  connect(selectAction, &QAction::triggered, this, &MainWindow::selectLevel);

  QAction *inputAction = new QAction(QIcon(":/icons/input.png"), tr("输入数独"), this);
  selectAction->setShortcut(QKeySequence::Paste);
  connect(inputAction, &QAction::triggered, this, &MainWindow::switchToInputMode);

  QAction *submitAction = new QAction(QIcon(":/icons/submit.png"), tr("开始求解"), this);
  connect(submitAction, &QAction::triggered, this, &MainWindow::startSolving);

  QAction *genAction = new QAction(QIcon(":/icons/generate.png"), tr("随机生成数独"), this);
  connect(genAction, &QAction::triggered, this, &MainWindow::startGenerating);

  toolBar = addToolBar(tr("游戏"));
  toolBar->addAction(loadAction);
  toolBar->addAction(selectAction);
  toolBar->addAction(inputAction);
  toolBar->addAction(submitAction);
  toolBar->addAction(genAction);
}

void MainWindow::initScene()
{
  timerScene->setSceneRect(0, 0, CUBE_SIZE * SUDOKU_ORDER, CUBE_SIZE);
  mainScene->setSceneRect(0, 0, SUDOKU_ORDER * CUBE_SIZE, SUDOKU_ORDER * CUBE_SIZE);
  controlScene->setSceneRect(0, 0, CUBE_SIZE * SUDOKU_ORDER, CUBE_SIZE * 1.5);
}

void MainWindow::initView()
{
  timerView->fitInView(timerScene->sceneRect());
  mainView->fitInView(mainScene->sceneRect());
  controlView->fitInView(controlScene->sceneRect());
}

void MainWindow::initWidgets()
{
  timerView->setGeometry(0, 0, CUBE_SIZE * SUDOKU_ORDER, CUBE_SIZE);
  mainView->setGeometry(0, CUBE_SIZE, CUBE_SIZE * SUDOKU_ORDER, CUBE_SIZE * SUDOKU_ORDER);
  controlView->setGeometry(0, CUBE_SIZE * (SUDOKU_ORDER + 1), CUBE_SIZE * SUDOKU_ORDER, CUBE_SIZE * 1.5);
  gameArea->resize(SUDOKU_ORDER * CUBE_SIZE, (SUDOKU_ORDER + 2.5) * CUBE_SIZE);
}

void MainWindow::openGameFile()
{
  QString filePath = QFileDialog::getOpenFileName(this, tr("选择游戏文件"), ".", tr("Sudoku File (*.sudo)"));
  if(!filePath.isEmpty()) {
      controller->startGame(filePath);
    }
}

void MainWindow::selectLevel()
{
  QDialog selectDialog(this);
  Ui::SelectDiaglog ui;
  ui.setupUi(&selectDialog);

  if(selectDialog.exec() == QDialog::Accepted) {
      QString gameFilePath = QString(":/gameFiles/game") + QString::number(ui.levelSelector->value()) + ".sudo";
      controller->startGame(gameFilePath);
    }
}

void MainWindow::switchToInputMode()
{
  controller->inputMode();
}

void MainWindow::startSolving()
{
  controller->solveSudoku();
}

void MainWindow::startGenerating()
{
  controller->generateSudoku();
}
