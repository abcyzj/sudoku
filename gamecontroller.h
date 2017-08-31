#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QFont>
#include <QVector>

class QGraphicsScene;
class QGraphicsItem;
class QWidget;

class Cube;
class TimeTag;
class QGraphicsTextItem;
class PixButton;

class GameController : public QObject
{
  Q_OBJECT
public:
  explicit GameController(QGraphicsScene *mainScene, QGraphicsScene *timerScene, QGraphicsScene *selectorScene, QWidget *gameArea, QObject *parent = nullptr);
  virtual ~GameController();

signals:

public slots:
  void initGame();
  void startGame(const QString& gameFile);
  void stopGame();

private:
  void initMainScene();
  void initTimerScene();
  void initSelectorScene();
  void initConnections();
  inline int valueAt(int x, int y);
  inline void setValue(int value, int x, int y);
  Cube *cubeAt(int x, int y);
  Cube *selectorAt(int x);
  QFont getFont(const QString &fontFile);

  void loadGameFromFile(const QString &file);

  QWidget *gameArea;
  QGraphicsScene *mainScene;
  QGraphicsScene *timerScene;
  QGraphicsScene *selectorScene;
  QTimer timer;
  QFont font;

  QVector<Cube*> m_cubes;
  QVector<Cube*> m_selectors;
  TimeTag *timetag;
  QGraphicsTextItem *m_welcomeText;
  PixButton *resumeButton;
  PixButton *stopButton;
  PixButton *backButton;
  PixButton *forwardButton;
};

#endif // GAMECONTROLLER_H
