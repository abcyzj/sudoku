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
  int getValue();
  int& rx();
  int& ry();

  QRectF boundingRect() const;
  QPainterPath shape() const;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

public slots:
  void highlight();
  void unhighlight();
  void setText(const QString &text = QString());
  void setValue(const int &value);

protected:
  void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
  void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

signals:
  void textChanged(int x, int y, QString text);
  void valueChanged(int x, int y, int value);
  void clicked(int x, int y);

private:
  bool is_highlight;
  int corx;
  int cory;
  QColor normalColor;
  QColor highlightColor;
  QColor currentColor;
  QString text;
  int value;
  QFont font;
  void changeCorlor(const QColor &color);
};

#endif // CUBE_H
