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
class RecordMaintainer;

class GameController : public QObject
{
  Q_OBJECT

  enum Staus {
    GAMING,
    INITIAL,
    STOPPED,
    OVER,
    NOGAMEMODE
  };

  friend class RecordMaintainer;

public:
  explicit GameController(QGraphicsScene *mainScene, QGraphicsScene *timerScene, QGraphicsScene *selectorScene, QWidget *gameArea, QObject *parent = nullptr);
  virtual ~GameController();

  Staus status();

//signals:
//  void gameOver();

public slots:
  void initGame();
  void startGame(const QString& gameFile);
  void stopGame();
  void resumeGame();
  void restartGame();
  void finishGame();
  void backStep();
  void forwardStep();
  void deleteSelectedCube();
  void onCubeClicked(Cube *cube);
  void onCubeValueChanged(Cube *cube, int formerValue);
  void onSelectorClicked(Cube *selector);

private:
  void initMainScene();
  void initTimerScene();
  void initSelectorScene();
  void initConnections();

  void highlightByValue(int value);
  void unhighlightByValue(int value);
//  void tackleValueChangedEvent(int x, int y, int value);//处理值变化事件
//  void tackleCubeHighlight(Cube *cube);//处理cube高亮事件

  inline int valueAt(int x, int y);
  inline void setValue(int value, int x, int y);
  Cube *cubeAt(int x, int y);
  Cube *selectorAt(int x);
  QFont getFont(const QString &fontFile);

  bool loadGameFromFile(const QString &file);
  bool parseGameData(const QString &data);

  QWidget *gameArea;
  QGraphicsScene *mainScene;
  QGraphicsScene *timerScene;
  QGraphicsScene *selectorScene;
  QTimer timer;
  QFont font;

  QVector<Cube*> m_cubes;
  QVector<Cube*> m_selectors;
  Staus gameStatus;
  Cube *selectedCube;
  int valueSetNum;
  QString currentGameFile;

  TimeTag *timetag;
  QGraphicsTextItem *m_welcomeText;
  PixButton *resumeButton;
  PixButton *stopButton;
  PixButton *backButton;
  PixButton *forwardButton;
  PixButton *deleteButton;
  PixButton *restartButton;
  RecordMaintainer *recordMaintainer;
};

#endif // GAMECONTROLLER_H
