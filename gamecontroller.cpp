#include "gamecontroller.h"
#include "cube.h"
#include "constants.h"

#include <QGraphicsScene>
#include <QFontDatabase>

static const QColor normalColor(Qt::red);
static const QColor hightlightColor(Qt::blue);

GameController::GameController(QGraphicsScene *scene, QObject *parent)
  : QObject(parent),
    mainScene(scene),
    font(getFont(FONT_FILE))
{
  for(int i = 0; i < SUDOKU_ORDER; i++) {
      for(int j = 0; j < SUDOKU_ORDER; j++) {
          qreal x = (i + 0.5) * CUBE_SIZE;
          qreal y = (j + 0.5) * CUBE_SIZE;
          Cube *newCube = new Cube(x, y, normalColor, hightlightColor);
          newCube->rFont() = font;
          newCube->setText(QString::number(j + 1));
          scene->addItem(newCube);
        }
    }
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
