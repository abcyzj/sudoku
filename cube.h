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
  Cube(qreal x, qreal y, int logX, int logY, QColor normal, QColor highlight, QGraphicsItem *parent = nullptr);
  virtual ~Cube();

  QFont& rFont();
  int getValue();
  int& rx();//坐标系中的位置
  int& ry();
  bool getIsBound();
  bool isFocused();

  QRectF boundingRect() const;
  QPainterPath shape() const;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

public slots:
  void highlight();
  void unhighlight();
  void toggleHighlightFixed();
  void highlightFixed();
  void unhighlightFixed();
  //void setText(const QString &text = QString());
  void setValue(const int &value); //设置value，numberSelected将会被置为唯一选中
  void bindValue(const int &value); //设置value，不可再更改
  void addNumber(const int &number); //若只有一个选中，将设为value
  void removeNumber(const int &number);
  void clearNumbers();
  void init();//初始化

signals:
  //void textChanged(int x, int y, QString text);
  void valueChanged(Cube *self, int former);
  void numberAdded(Cube *self, int numberSet);//numberSet是位图
  void numberRemoved(Cube *self, int numberSet);
  void clicked(Cube *self);

protected:
  void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
  void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
  bool gotFocus;
  int corx;
  int cory;
  QColor normalColor;
  QColor highlightColor;
  QColor currentColor;
//  QString text;
  int value; //方格存储的值，只能有一个，0代表无
  bool isBound;//是否已经绑定
  bool numberSelected[9]; //当前选中的数字，将会显示出来
  QFont font;
  void changeCorlor(const QColor &color);
};

#endif // CUBE_H
