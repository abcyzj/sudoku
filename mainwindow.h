#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QGraphicsScene;
class QGraphicsView;
class QToolBar;

class GameController;

class MainWindow : public QMainWindow
{
  Q_OBJECT

  friend class GameController;

public:
  MainWindow(QWidget *parent = 0);
  virtual ~MainWindow();

public slots:
  void openGameFile();
  void selectLevel();
  void switchToInputMode();
  void startSolving();
  void startGenerating();

private:
  void initWidgets();
  void initScene();
  void initView();
  void initToolBar();

  QWidget *gameArea;
  QGraphicsScene *mainScene;
  QGraphicsView *mainView;
  QGraphicsScene *timerScene;
  QGraphicsView *timerView;
  QGraphicsScene *controlScene;
  QGraphicsView *controlView;
  GameController *controller;
  QToolBar *toolBar;
};

#endif // MAINWINDOW_H
