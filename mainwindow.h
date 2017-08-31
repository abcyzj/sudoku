#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QGraphicsScene;
class QGraphicsView;

class GameController;

class MainWindow : public QMainWindow
{
  Q_OBJECT

  friend class GameController;

public:
  MainWindow(QWidget *parent = 0);
  virtual ~MainWindow();

private:
  void initWidgets();
  void initScene();
  void initView();

  QWidget *gameArea;
  QGraphicsScene *mainScene;
  QGraphicsView *mainView;
  QGraphicsScene *timerScene;
  QGraphicsView *timerView;
  QGraphicsScene *controlScene;
  QGraphicsView *controlView;
  GameController *controller;
};

#endif // MAINWINDOW_H
