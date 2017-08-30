#include "mainwindow.h"
#include "gamecontroller.h"
#include "constants.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent),
    scene(new QGraphicsScene(this)),
    view(new QGraphicsView(scene, this)),
    controller(new GameController(scene, this))
{
  setCentralWidget(view);
  resize(SUDOKU_ORDER * CUBE_SIZE, SUDOKU_ORDER * CUBE_SIZE);
  initScene();

  QTimer::singleShot(0, this, &MainWindow::initView);
}

MainWindow::~MainWindow()
{}

void MainWindow::initScene()
{
  scene->setSceneRect(0, 0, SUDOKU_ORDER * CUBE_SIZE, SUDOKU_ORDER * CUBE_SIZE);
}

void MainWindow::initView()
{
  view->fitInView(scene->sceneRect());
}
