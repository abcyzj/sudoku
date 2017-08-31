#ifndef TIMETAG_H
#define TIMETAG_H

#include <QLabel>
#include <QTimer>

class TimeTag : public QLabel
{
  Q_OBJECT

  enum Stautus {
    INITIAL,
    RUNNING,
    STOPPED
  };

public:
  explicit TimeTag(QWidget *parent = nullptr);

signals:

public slots:
  void init();
  void stop();
  void start();
  void toggle();

private slots:
  void updateTime();

private:
  QTimer timer;
  int minutes;
  int seconds;
  Stautus st;
  inline QString fillZero(int num);
};

#endif // TIMETAG_H
