#include "gamecontroller.h"
#include "cube.h"
#include "pixbutton.h"
#include "timetag.h"
#include "recordmaintainer.h"
#include "sudokusolver.h"
#include "constants.h"

#include "mainwindow.h"

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QFontDatabase>
#include <QFile>
#include <QTextStream>
#include <QThread>

#include <QMessageBox>
#include <QFileDialog>

#include <QDebug>

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
  initConnections();

  recordMaintainer = new RecordMaintainer(this, this);//init recordMaintainer

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
          Cube *newCube = new Cube(x, y, i, j, normalColor, hightlightColor);
          newCube->rFont() = font;
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
  //init buttons
  QPixmap stopPic = QPixmap(":/icons/stop.png").scaled(CUBE_SIZE, CUBE_SIZE,
                                                       Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
  QPixmap resumePic = QPixmap(":/icons/resume.png").scaled(CUBE_SIZE, CUBE_SIZE,
                                                           Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
  QPixmap backPic = QPixmap(":/icons/back.png").scaled(CUBE_SIZE, CUBE_SIZE,
                                                         Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
  QPixmap forwardPic = QPixmap(":/icons/forward.png").scaled(CUBE_SIZE, CUBE_SIZE,
                                                            Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
  QPixmap deletePic = QPixmap(":/icons/delete.png").scaled(CUBE_SIZE, CUBE_SIZE,
                                                                           Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
  QPixmap restartPic = QPixmap(":/icons/restart.png").scaled(CUBE_SIZE, CUBE_SIZE,
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
  PixButton *restart = new PixButton(restartPic, restartPic);
  restart->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
  restart->setPos(CUBE_SIZE * 2, 0);
  timerScene->addItem(restart);
  restartButton = restart;
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
  PixButton *deleteBtn = new PixButton(deletePic, deletePic);
  deleteBtn->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
  deleteBtn->setPos(CUBE_SIZE * 5, 0);
  timerScene->addItem(deleteBtn);
  deleteButton = deleteBtn;

  //init timetag
  TimeTag *timetag = new TimeTag();
  GameController::timetag = timetag;
  timetag->setFont(getFont(FONT_FILE));
  timetag->setGeometry((SUDOKU_ORDER - 3) * CUBE_SIZE, 0, 3 * CUBE_SIZE, CUBE_SIZE);
  timerScene->addWidget(timetag);
  timetag->init();
}

void GameController::initSelectorScene()
{
  for(int i = 0; i < SUDOKU_ORDER; i++) {
      qreal x = (i + 0.5) * CUBE_SIZE;
      Cube *newCube = new Cube(x, CUBE_SIZE * 0.75, i, 0, normalColor, hightlightColor);
      newCube->rFont() = font;
      newCube->setValue(i + 1);
      m_selectors.push_back(newCube);
      selectorScene->addItem(newCube);
    }
}

void GameController::initConnections()
{
  connect(resumeButton, &PixButton::clicked, this, &GameController::resumeGame);
  connect(stopButton, &PixButton::clicked, this, &GameController::stopGame);
  connect(restartButton, &PixButton::clicked, this, &GameController::restartGame);
  connect(backButton, &PixButton::clicked, this, &GameController::backStep);
  connect(forwardButton, &PixButton::clicked, this, &GameController::forwardStep);
  connect(deleteButton, &PixButton::clicked, this, &GameController::deleteSelectedCube);
  foreach(Cube *cube, m_cubes) {
      connect(cube, &Cube::clicked, this, &GameController::onCubeClicked);
      connect(cube, &Cube::valueChanged, this, &GameController::onCubeValueChanged);
    }
  foreach(Cube *selector, m_selectors) {
      connect(selector, &Cube::clicked, this, &GameController::onSelectorClicked);
    }

  //TODO
}

void GameController::initGame()
{
  if(gameStatus == INITIAL) {
      return;
    }

  timetag->init();
  mainScene->addItem(m_welcomeText);
  foreach (Cube *cube, m_cubes) {
      cube->setEnabled(false);
      cube->init();
    }
  foreach(Cube *selector, m_selectors) {
      selector->setEnabled(false);
      selector->unhighlight();
    }

  selectedCube = nullptr;
  gameStatus = INITIAL;
}

void GameController::startGame(const QString &gameFile)
{
  initGame();
  if(mainScene->items().contains(m_welcomeText))
    mainScene->removeItem(m_welcomeText);

  foreach(Cube *cube, m_cubes) {
      cube->setEnabled(true);
    }

  if(loadGameFromFile(gameFile)) {
      currentGameFile = gameFile;
      timetag->init();
      timetag->start();
      gameStatus = GAMING;
    }
}

void GameController::stopGame()
{
  if(gameStatus != GAMING) {
      return;
    }

  timetag->stop();
  foreach(Cube *cube, m_cubes) {
      cube->setEnabled(false);
    }

  gameStatus = STOPPED;
}

void GameController::resumeGame()
{
  if(gameStatus != STOPPED) {
      return;
    }

  foreach(Cube *cube, m_cubes) {
      cube->setEnabled(true);
    }
  timetag->start();
  gameStatus = GAMING;
}

void GameController::restartGame(){
  if(gameStatus != GAMING && gameStatus != OVER) {
      return;
    }

  startGame(currentGameFile);
}

void GameController::finishGame()
{
  if(gameStatus != GAMING) {
      return;
    }

  timetag->stop();

  foreach(Cube *cube, m_cubes) {
      cube->setEnabled(false);
    }
  foreach(Cube *selector, m_selectors) {
      selector->setEnabled(false);
    }

  gameStatus = OVER;
  QMessageBox::information(gameArea, tr("你完成了这个数独"),
                           tr("你在") + timetag->text() + "内完成了这个数独", QMessageBox::Ok);
}

void GameController::inputMode()
{
  if(gameStatus == GAMING || gameStatus == STOPPED) {
      if(QMessageBox::information(gameArea, tr("您有尚未结束的游戏"), tr("您的游戏尚未完成，确定要放弃？"),
                                  QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Cancel) {
          return;
        }
    }

  initGame();
  if(mainScene->items().contains(m_welcomeText))
    mainScene->removeItem(m_welcomeText);

  foreach (Cube *cube, m_cubes) {
      cube->setEnabled(true);
      cube->init();
    }

  if(QMessageBox::information(gameArea, tr("要从文件中读取数独吗"), tr("单击确定将从文件中读取数独"),
                              QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok) {
      QString filePath = QFileDialog::getOpenFileName(gameArea, tr("选择游戏文件"), ".", tr("Sudoku File (*.sudo)"));
      loadGameFromFile(filePath);
    }

  foreach(Cube *selector, m_selectors) {
      selector->setEnabled(true);
      selector->unhighlight();
    }
  gameStatus = INPUT;
}

void GameController::solveSudoku()
{
  if(gameStatus != INPUT) {
      QMessageBox::information(gameArea, tr("请先输入数独"), tr("您尚未给出要解答的题目，请点击输入按钮"));
      return;
    }

  QMessageBox::information(gameArea, tr("提示"), tr("即将开始求解"));

  QThread *solvingThread = new QThread(this);
  SudokuSolver *solver = new SudokuSolver();

  solver->moveToThread(solvingThread);
  connect(this, &GameController::startSolving, solver, &SudokuSolver::setData);
  connect(solver, &SudokuSolver::solvingFinished, [=](QStringList results) {
    solvingThread->quit();
    displayMode(results.first());
  });
  solvingThread->start();
  emit startSolving(outputCurrenPanel());

  foreach(Cube *cube, m_cubes) {
      cube->setEnabled(false);
    }

  gameStatus = SOLVING;
}

void GameController::displayMode(const QString &content)
{
  if(gameStatus != SOLVING) {
      return;
    }

  parseGameData(content);
  foreach(Cube *cube, m_cubes) {
      cube->setEnabled(false);
    }
  foreach(Cube *selector, m_selectors) {
      selector->setEnabled(false);
    }

  gameStatus = DISPLAY;
}

void GameController::backStep()
{
  recordMaintainer->backout();
}

void GameController::forwardStep()
{
  recordMaintainer->forward();
}

void GameController::deleteSelectedCube()
{
  if((gameStatus != GAMING || selectedCube->getIsBound()) && gameStatus != INPUT) {
      return;
    }

  if(selectedCube) {
      selectedCube->clearNumbers();
    }
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

bool GameController::loadGameFromFile(const QString &file)
{
  QFile gameData(file);

  if(gameData.open(QFile::ReadOnly)) {
      QTextStream in(&gameData);
      QString info = in.readAll();
      return parseGameData(info);

    } else {
      QMessageBox::information(gameArea, tr("读取错误"), tr("无法打开游戏文件，请检查"));
      return false;
    }
}

bool GameController::parseGameData(const QString &data)
{
  QStringList list = data.split(',', QString::KeepEmptyParts);
  if(list.length() != 82) {
      QMessageBox::information(gameArea, tr("解析错误"), tr("文件中包含的方格数目有误"));
      return false;
    }

  //parsing
  valueSetNum = 0;
  for(int i = 0; i < 81; i++) {
      bool ok;
      int value = list[i].toInt(&ok);
      if(ok) {//转换成功
          cubeAt(i % 9, i / 9)->bindValue(value);
        }
    }
  return true;
}

GameController::Status GameController::status()
{
  return gameStatus;
}

void GameController::onCubeClicked(Cube *cube)
{
  foreach(Cube *selector, m_selectors) {//所有选择器失效
      selector->setEnabled(false);
    }

  if(selectedCube == cube) { //一样的，要闹
      unhighlightByValue(selectedCube->getValue());
      cube->unhighlightFixed();
      selectedCube = nullptr;
      return;
    }

  //不一样时，处理之前遗留痕迹
  if(selectedCube) {//之前选中的失去焦点
      unhighlightByValue(selectedCube->getValue());
      selectedCube->unhighlightFixed();
      }

  selectedCube = cube;

  highlightByValue(cube->getValue());
  cube->highlightFixed();

  if(cube->getIsBound()) {//已经绑定了值，别闹了
      return;
    }

  //determine which selectors to enable
  int x = cube->rx(), y = cube->ry();
  bool check[10];
  for(int i = 1; i <= SUDOKU_ORDER; i++) {
      check[i] = true;
    }
  //determine row
  for(int i = 0; i < SUDOKU_ORDER; i++) {
      check[cubeAt(i, y)->getValue()] = false;
    }

  //determine col
  for(int i = 0; i < SUDOKU_ORDER; i++) {
      check[cubeAt(x, i)->getValue()] = false;
    }

  //determine square
  int squareX = x / 3, squareY = y /3;
  for(int i = squareX * 3; i < squareX * 3 + 3; i++) {
      for(int j = squareY * 3; j < squareY * 3 + 3; j++) {
          check[cubeAt(i, j)->getValue()] = false;
        }
    }

  //enable selectors
  for(int i = 1; i <= SUDOKU_ORDER; i++) {
      if(check[i]) {
          m_selectors[i - 1]->setEnabled(true);
        }
    }
}

void GameController::onCubeValueChanged(Cube *cube, int formerValue) {
  if(selectedCube == cube) {//如果当前选中的方块值有变化，则刷新高亮显示
      unhighlightByValue(formerValue);
      highlightByValue(cube->getValue());
    }

  if(cube->getValue()) {//如果新增值
      valueSetNum++;
    }
  else {
      valueSetNum--;
    }

  if(valueSetNum == SUDOKU_ORDER * SUDOKU_ORDER) {
      finishGame();
    }
}

void GameController::onSelectorClicked(Cube *selector)
{
  selectedCube->addNumber(selector->getValue());
}

//void GameController::tackleCubeHighlight(Cube *cube)
//{
//}

void GameController::highlightByValue(int value)
{
  if(gameStatus != GAMING || value == 0) {//只在游戏进行时生效，当value为0时同样不做操作
      return;
    }

  for(int i = 0; i < SUDOKU_ORDER * SUDOKU_ORDER; i++) {
      if(m_cubes[i]->getValue() == value) {
          m_cubes[i]->highlightFixed();
        }
    }
}

void GameController::unhighlightByValue(int value)
{
  if(gameStatus != GAMING || value == 0) {
      return;
    }

  for(int i = 0; i < SUDOKU_ORDER * SUDOKU_ORDER; i++) {
      if(m_cubes[i]->getValue() == value) {
          m_cubes[i]->unhighlightFixed();
        }
    }
}

QString GameController::outputCurrenPanel()
{
  QString output;
  for(int i = 0; i < SUDOKU_ORDER * SUDOKU_ORDER; i++) {
      output.append(QString::number(m_cubes[i]->getValue()) + ',');
    }
  qDebug() << output;
  return output;
}
