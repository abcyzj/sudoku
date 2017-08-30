#include "cube.h"
#include "constants.h"

#include <QPainter>
#include <QDebug>

Cube::Cube(qreal x, qreal y, QColor normal, QColor highlight, QGraphicsItem *parent)
  : QObject(nullptr),
    QGraphicsItem(parent),
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

  if(!text.isEmpty()) {//draw text
      painter->setFont(font);
      painter->setPen(Qt::black);
      painter->drawText(QRectF(-CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, CUBE_SIZE, CUBE_SIZE),
                        Qt::AlignCenter, text);
    }

  painter->restore();
}

void Cube::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
  highlight();
}

void Cube::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
  unhighlight();
}

void Cube::setText(const QString &text)
{
  font.setStyleStrategy(QFont::ForceOutline);
  Cube::text = text;
  emit textChanged(corx, cory, text);
  update();
}

QFont& Cube::rFont()
{
  return font;
}

int& Cube::rx()
{
  return corx;
}

int& Cube::ry()
{
  return cory;
}
