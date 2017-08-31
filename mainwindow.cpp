#include "mainwindow.h"
#include "gamecontroller.h"
#include "constants.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent),
    gameArea(this),
    mainScene(new QGraphicsScene(gameArea)),
    mainView(new QGraphicsView(mainScene, gameArea)),
    timerScene(new QGraphicsScene(gameArea)),
    timerView(new QGraphicsView(timerScene, gameArea)),
    controlScene(new QGraphicsScene(gameArea)),
    controlView(new QGraphicsView(controlScene, gameArea)),
    controller(new GameController(mainScene, timerScene, controlScene, gameArea, this))
{
  initWidgets();
  initScene();


  QTimer::singleShot(0, this, &MainWindow::initView);
}

MainWindow::~MainWindow()
{}

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
