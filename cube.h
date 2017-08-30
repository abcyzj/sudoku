#ifndef CUBE_H
#define CUBE_H

#include <QGraphicsItem>
#include <QColor>
#include <QFont>
#include <QObject>

class Cube : public QObject, public QGraphicsItem
{
  Q_OBJECT
  Q_INTERFACES(QGraphicsItem)
public:
  Cube(qreal x, qreal y, QColor normal, QColor highlight, QGraphicsItem *parent = nullptr);
  virtual ~Cube();

  bool isHighlight();
  QFont& rFont();
  int& rx();
  int& ry();

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

signals:
  void textChanged(int x, int y, QString text);

private:
  bool is_highlight;
  int corx;
  int cory;
  QColor normalColor;
  QColor highlightColor;
  QColor currentColor;
  QString text;
  QFont font;
  void changeCorlor(const QColor &color);
};

#endif // CUBE_H
