#include "gamecontroller.h"
#include "cube.h"
#include "constants.h"

#include <QGraphicsScene>

static const QColor normalColor(Qt::red);
static const QColor hightlightColor(Qt::blue);

GameController::GameController(QGraphicsScene *scene, QObject *parent)
  : QObject(parent),
    scene(scene)
{

  for(int i = 0; i < SUDOKU_ORDER; i++) {
      for(int j = 0; j < SUDOKU_ORDER; j++) {
          qreal x = (i + 0.5) * CUBE_SIZE;
          qreal y = (j + 0.5) * CUBE_SIZE;
          scene->addItem(new Cube(x, y, normalColor, hightlightColor));
        }
    }
}
