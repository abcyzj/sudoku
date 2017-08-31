#include "gamecontroller.h"
#include "cube.h"
#include "pixbutton.h"
#include "timetag.h"
#include "constants.h"

#include "mainwindow.h"

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QFontDatabase>
#include <QFile>
#include <QTextStream>

#include <QMessageBox>

static const QColor normalColor(Qt::red);
static const QColor hightlightColor(Qt::blue);

GameController::GameController(QGraphicsScene *scene, QGraphicsScene *timerScene, QGraphicsScene *selectorScene, QWidget *gameArea, QObject *parent)
  : QObject(parent),
    gameArea(gameArea),
    mainScene(scene),
    timerScene(timerScene),
    selectorScene(selectorScene),
    font(getFont(FONT_FILE))
{
  initMainScene();
  initTimerScene();
  initSelectorScene();

  initGame();
}

GameController::~GameController()
{
}

QFont GameController::getFont(const QString &fontFile)
{
  int fontId = QFontDatabase::addApplicationFont(fontFile);
  QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);

  QFont newFont;
  newFont.setFamily(fontFamilies.at(0));
  newFont.setPixelSize(FONT_SIZE);

  return newFont;
}

void GameController::initMainScene()
{
  for(int i = 0; i < SUDOKU_ORDER; i++) {
      for(int j = 0; j < SUDOKU_ORDER; j++) {
          qreal x = (i + 0.5) * CUBE_SIZE;
          qreal y = (j + 0.5) * CUBE_SIZE;
          Cube *newCube = new Cube(x, y, normalColor, hightlightColor);
          newCube->rFont() = font;
          newCube->setText();
          m_cubes.push_back(newCube);
          mainScene->addItem(newCube);
        }
    }

  QGraphicsTextItem *welcomeText = new QGraphicsTextItem(QString("SUDOKU"));
  welcomeText->setFont(getFont(FONT_FILE));
  welcomeText->setDefaultTextColor(QColor(Qt::black));
  welcomeText->setPos(CUBE_SIZE * 2, CUBE_SIZE * 3);
  m_welcomeText = welcomeText;
}

void GameController::initTimerScene()
{
  //init start and stop button
  QPixmap stopPic = QPixmap(":/icons/stop.png").scaled(CUBE_SIZE, CUBE_SIZE,
                                                       Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
  QPixmap resumePic = QPixmap(":/icons/resume.png").scaled(CUBE_SIZE, CUBE_SIZE,
                                                           Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
  QPixmap backPic = QPixmap(":/icons/back.png").scaled(CUBE_SIZE, CUBE_SIZE,
                                                         Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
  QPixmap forwardPic = QPixmap(":/icons/forward.png").scaled(CUBE_SIZE, CUBE_SIZE,
                                                            Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
  PixButton *stop = new PixButton(stopPic, stopPic);
  stop->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
  stop->setPos(0, 0);
  timerScene->addItem(stop);
  stopButton = stop;
  PixButton *resume = new PixButton(resumePic, resumePic);
  resume->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
  resume->setPos(CUBE_SIZE, 0);
  timerScene->addItem(resume);
  resumeButton = resume;
  PixButton *back = new PixButton(backPic, backPic);
  back->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
  back->setPos(CUBE_SIZE * 3, 0);
  timerScene->addItem(back);
  backButton = back;
  PixButton *forward = new PixButton(forwardPic, forwardPic);
  forward->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
  forward->setPos(CUBE_SIZE * 4, 0);
  timerScene->addItem(forward);
  forwardButton = forward;

  //init timetag
  TimeTag *timetag = new TimeTag();
  GameController::timetag = timetag;
  timetag->setFont(getFont(FONT_FILE));
  timetag->setGeometry((SUDOKU_ORDER - 3) * CUBE_SIZE, 0, 3 * CUBE_SIZE, CUBE_SIZE);
  timerScene->addWidget(timetag);
  timetag->init();
  connect(stop, &PixButton::clicked, timetag, &TimeTag::stop);
  connect(resume, &PixButton::clicked, timetag, &TimeTag::start);
}

void GameController::initSelectorScene()
{
  for(int i = 0; i < SUDOKU_ORDER; i++) {
      qreal x = (i + 0.5) * CUBE_SIZE;
      Cube *newCube = new Cube(x, CUBE_SIZE * 0.75, normalColor, hightlightColor);
      newCube->rFont() = font;
      newCube->setValue(i + 1);
      m_selectors.push_back(newCube);
      selectorScene->addItem(newCube);
    }
}

void GameController::initConnections()
{
  //TODO
}

void GameController::initGame()
{
  timetag->init();
  mainScene->addItem(m_welcomeText);
  foreach (Cube *cube, m_cubes) {
      cube->setEnabled(false);
    }
}

void GameController::startGame(const QString &gameFile)
{
  mainScene->removeItem(m_welcomeText);
  foreach(Cube *cube, m_cubes) {
      cube->setEnabled(true);
    }

  loadGameFromFile(gameFile);
  timetag->start();
}

void GameController::stopGame()
{
  timetag->stop();
}

int GameController::valueAt(int x, int y)
{
  return m_cubes[x * SUDOKU_ORDER + y]->getValue();
}

void GameController::setValue(int value, int x, int y)
{
  m_cubes[x * SUDOKU_ORDER + y]->setValue(value);
}

Cube* GameController::cubeAt(int x, int y)
{
  return m_cubes[x * SUDOKU_ORDER + y];
}

Cube* GameController::selectorAt(int x)
{
  return m_selectors[x];
}

void GameController::loadGameFromFile(const QString &file)
{
  QFile gameData(file);

  if(gameData.open(QFile::ReadOnly)) {

    } else {
      QMessageBox::information(gameArea, tr("读取错误"), tr("无法打开游戏文件，请检查"));
    }
}
