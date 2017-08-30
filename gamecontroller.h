#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QTimer>

class QGraphicsScene;

class GameController : public QObject
{
  Q_OBJECT
public:
  explicit GameController(QGraphicsScene *scene, QObject *parent = nullptr);

signals:

public slots:

private:
  void addCubes();

  QGraphicsScene *scene;
  QTimer timer;
};

#endif // GAMECONTROLLER_H
