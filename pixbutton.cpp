#include "pixbutton.h"

#include <QDebug>

PixButton::PixButton(QPixmap defaultPic, QPixmap highlightPic, QObject *parent)
  : QObject(parent),
    QGraphicsPixmapItem(nullptr),
    defaultPic(defaultPic),
    highlightPic(highlightPic)
{
  setAcceptedMouseButtons(Qt::LeftButton);
  setAcceptHoverEvents(true);
  setPixmap(defaultPic);
}


PixButton::~PixButton(){}

void PixButton::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
  Q_UNUSED(event)
  setPixmap(highlightPic);
  update();
}

void PixButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
  Q_UNUSED(event)
  setPixmap(defaultPic);
  update();
}

void PixButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  Q_UNUSED(event)
}

void PixButton::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  Q_UNUSED(event)
  qDebug() << "clicked";
  emit clicked();
}

void PixButton::setPic(QPixmap defaultPic, QPixmap highlightPic)
{
  PixButton::defaultPic = defaultPic;
  PixButton::highlightPic = highlightPic;
}
