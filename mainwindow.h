#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QGraphicsScene;
class QGraphicsView;

class GameController;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = 0);
  virtual ~MainWindow();

private:
  void initScene();
  void initView();

  QGraphicsScene *scene;
  QGraphicsView *view;
  GameController *controller;
};

#endif // MAINWINDOW_H
