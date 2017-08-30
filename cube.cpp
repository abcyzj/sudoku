#include "cube.h"
#include "constants.h"

#include <QPainter>

#include <QDebug>

Cube::Cube(qreal x, qreal y, QColor normal, QColor highlight, QGraphicsItem *parent)
  : QGraphicsItem(parent),
    is_highlight(false),
    normalColor(normal),
    highlightColor(highlight),
    currentColor(normal)
{
  setPos(x, y);
  setAcceptHoverEvents(true);
  setAcceptedMouseButtons(Qt::LeftButton);
}

Cube::~Cube(){}

void Cube::highlight()
{
  if(is_highlight) {
      return;
    }

  changeCorlor(highlightColor);
  is_highlight = true;
  qDebug() << "Highlighted!!";
}

void Cube::unhighlight()
{
  if(!is_highlight) {
      return;
    }

  changeCorlor(normalColor);
  is_highlight = false;
}

void Cube::changeCorlor(const QColor &color)
{
  if(color == currentColor) {
      return;
    }

  currentColor = color;
  update();
}

QRectF Cube::boundingRect() const
{
  return QRectF(-CUBE_SIZE, -CUBE_SIZE,
                CUBE_SIZE * 2, CUBE_SIZE * 2);
}

QPainterPath Cube::shape() const
{
  QPainterPath path;
  path.addRect(-CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0,
               CUBE_SIZE, CUBE_SIZE);
  return path;
}

void Cube::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
  painter->save();

  painter->setPen(Qt::NoPen);
  painter->fillPath(shape(), QBrush(currentColor));

  painter->restore();
}

void Cube::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
  qDebug() << "Enter!!";
  highlight();
}

void Cube::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
  qDebug() << "Leave!!";
  unhighlight();
}
