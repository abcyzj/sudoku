#include "cube.h"
#include "constants.h"

#include <cstring>

#include <QPainter>
#include <QDebug>

const Qt::GlobalColor CUBE_FONT_COLOR = Qt::black;
const Qt::GlobalColor CUBE_BORDER_COLOR = Qt::black;

Cube::Cube(qreal geoX, qreal geoY, int logX, int logY, QColor normal, QColor highlight, QGraphicsItem *parent)
  : QObject(nullptr),
    QGraphicsItem(parent),
    gotFocus(false),
    corx(logX),
    cory(logY),
    normalColor(normal),
    highlightColor(highlight),
    currentColor(normal),
    value(0),
    isBound(false)
{
  memset(numberSelected, 0, sizeof(numberSelected));
  setPos(geoX, geoY);
  setAcceptHoverEvents(true);
  setAcceptedMouseButtons(Qt::LeftButton);
}

Cube::~Cube(){}

void Cube::highlight()
{
  if(gotFocus) {
      return;
    }

  changeCorlor(highlightColor);
}

void Cube::unhighlight()
{
  if(gotFocus) {
      return;
    }

  changeCorlor(normalColor);
}

void Cube::highlightFixed()
{
  if(gotFocus) {
      return;
    }

  changeCorlor(highlightColor);
  setAcceptHoverEvents(false);
  gotFocus = true;
}

void Cube::unhighlightFixed()
{
  if(!gotFocus) {
      return;
    }

  changeCorlor(normalColor);
  setAcceptHoverEvents(true);
  gotFocus = false;
}

void Cube::toggleHighlightFixed()
{
  if(gotFocus) {
      changeCorlor(normalColor);
      setAcceptHoverEvents(true);
      gotFocus = false;
    }
  else {
      changeCorlor(highlightColor);
      setAcceptHoverEvents(false);
      gotFocus = true;
    }
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

  painter->setPen(CUBE_BORDER_COLOR);
  painter->fillPath(shape(), QBrush(currentColor));

  if(value) {//draw value
      painter->setFont(font);
      painter->setPen(CUBE_FONT_COLOR);
      painter->drawText(QRectF(-CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, CUBE_SIZE, CUBE_SIZE),
                        Qt::AlignCenter, QString::number(value));
    }
  else {//draw numbers
      QFont littleFont = font;
      littleFont.setPixelSize(FONT_SIZE / 3.0);
      painter->setFont(littleFont);
      painter->setPen(CUBE_FONT_COLOR);
      const qreal LITTLE_CUBE_SIZE = CUBE_SIZE / 3.0;
      for(int i = 0; i < 3; i++) {
          for(int j = 0; j < 3; j++) {
              if(numberSelected[i * 3 + j]) {
                  painter->drawText(QRectF((j - 1.5) * LITTLE_CUBE_SIZE, (i - 1.5) * LITTLE_CUBE_SIZE,
                                           LITTLE_CUBE_SIZE, LITTLE_CUBE_SIZE),
                                    Qt::AlignCenter, QString::number(i * 3 + j + 1));
                }
            }
        }

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

void Cube::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  Q_UNUSED(event)
  emit clicked(this);
}

//void Cube::setText(const QString &text)
//{
//  font.setStyleStrategy(QFont::ForceOutline);
//  Cube::text = text;
//  value = 0;
//  update();
//  emit textChanged(corx, cory, text);
//}

void Cube::setValue(const int &value)
{
  if(isBound) {
      return;
    }

  bool changed = false;
  if(Cube::value != value) {
      changed = true;
    }

  int former = Cube::value;
  Cube::value = value;
  memset(numberSelected, 0, sizeof(numberSelected));
  numberSelected[value - 1] = true;
  update();
  if(changed) {
      emit valueChanged(this, former);
    }
}

void Cube::bindValue(const int &value)
{
  if(isBound) {
      return;
    }

  setValue(value);
  isBound = true;
}

void Cube::addNumber(const int &number)
{
  if(isBound) {
      return;
    }

  if(numberSelected[number - 1]) {
      return;
    }

  int count = 0;//计算当前有几个数字被选中
  for(int i = 0; i < 9; i++) {
      if(numberSelected[i]) {
          count++;
        }
    }

  if(count == 0) {
      setValue(number);
      update();
    }
  else {//已经不止一个数字，将value清零
      int formerValue = value;
      value = 0;
      numberSelected[number - 1] = true; //标记
      update();
      emit valueChanged(this, formerValue);
    }
  emit numberAdded(this, 1 << number);
}

void Cube::removeNumber(const int &number)
{
  if(!numberSelected[number - 1]) {//没选就别闹了
      return;
    }

  if(value == number) {//独此一家
      numberSelected[number - 1] = false;
      value = 0;
      emit valueChanged(this, number);
    }
  else {
      int count = 0;
      int theOther = 0;
      for(int i = 0; i < SUDOKU_ORDER; i++) {
          if(numberSelected[i]) {
              count++;
              if(i + 1 != number) {
                  theOther = i + 1;
                }
            }
        }
      if(count == 2) {//有新的value
          setValue(theOther);
        }
      else {
          numberSelected[number - 1] = false;
        }
    }

  emit numberRemoved(this, 1 << number);
  update();
}

void Cube::clearNumbers()
{
  int set = 0;
  for(int i = 0; i < SUDOKU_ORDER; i++) {
      if(numberSelected[i]) {
        set |= 1 << (i + 1);
        }
    }
  emit numberRemoved(this, set);

  int formerValue = value;
  value = 0;
  memset(numberSelected, 0, sizeof(numberSelected));
  update();
  if(formerValue) {
      emit valueChanged(this, formerValue);
    }
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

int Cube::getValue()
{
  return value;
}

bool Cube::getIsBound()
{
  return isBound;
}

bool Cube::isFocused()
{
  return gotFocus;
}

void Cube::init()
{
  if(gotFocus) {
      toggleHighlightFixed();
    }
  gotFocus = false;
  isBound = false;
  clearNumbers();
}
