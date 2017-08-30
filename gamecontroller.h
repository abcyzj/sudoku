#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QFont>

class QGraphicsScene;
class QWidget;

class GameController : public QObject
{
  Q_OBJECT
public:
  explicit GameController(QGraphicsScene *scene, QObject *parent = nullptr);

signals:

public slots:

private:
  void addCubes();
  QFont getFont(const QString &fontFile);

  QGraphicsScene *mainScene;
  QWidget *timerScene;
  QWidget *selectorScene;
  QTimer timer;
  QFont font;
};

#endif // GAMECONTROLLER_H
