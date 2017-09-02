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
class SudokuSolver;

class GameController : public QObject
{
  Q_OBJECT

  enum Status {
    GAMING,
    INITIAL,
    STOPPED,
    OVER,
    INPUT,
    DISPLAY,
    SOLVING,
    GENERATING
  };

  friend class RecordMaintainer;

public:
  explicit GameController(QGraphicsScene *mainScene, QGraphicsScene *timerScene, QGraphicsScene *selectorScene, QWidget *gameArea, QObject *parent = nullptr);
  virtual ~GameController();

  Status status();
  QString outputCurrenPanel();

//signals:
//  void gameOver();

public slots:
  void initGame();
  void startGame(const QString& gameFile);
  void stopGame();
  void resumeGame();
  void restartGame();
  void finishGame();
  void inputMode();
  void solveSudoku();
  void displayMode(const QString &content);
  void generateSudoku();
  void backStep();
  void forwardStep();
  void deleteSelectedCube();
  void onCubeClicked(Cube *cube);
  void onCubeValueChanged(Cube *cube, int formerValue);
  void onSelectorClicked(Cube *selector);

signals:
  void startSolving(QString data);

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
  Status gameStatus;
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
