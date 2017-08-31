#ifndef PIXBUTTON_H
#define PIXBUTTON_H

#include <QObject>
#include <QGraphicsPixmapItem>

class PixButton : public QObject, public QGraphicsPixmapItem
{
  Q_OBJECT
public:
  explicit PixButton(QPixmap defaultPic, QPixmap highlightPic, QObject *parent = nullptr);
  virtual ~PixButton();
  void setPic(QPixmap defaultPic, QPixmap highlightPic);

protected:
  void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
  void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  void mousePressEvent(QGraphicsSceneMouseEvent *event);

public slots:

signals:
  void clicked();

private:
  QPixmap defaultPic;
  QPixmap highlightPic;
};

#endif // PIXBUTTON_H
