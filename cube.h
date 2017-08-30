#ifndef CUBE_H
#define CUBE_H

#include <QGraphicsItem>
#include <QColor>

class Cube : public QGraphicsItem
{
public:
  Cube(qreal x, qreal y, QColor normal, QColor highlight, QGraphicsItem *parent = nullptr);
  virtual ~Cube();

  bool isHighlight();

  QRectF boundingRect() const;
  QPainterPath shape() const;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

public slots:
  void highlight();
  void unhighlight();
  void setText(const QString &text);

protected:
  void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
  void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private:
  bool is_highlight;
  QColor normalColor;
  QColor highlightColor;
  QColor currentColor;
  void changeCorlor(const QColor &color);
};

#endif // CUBE_H
