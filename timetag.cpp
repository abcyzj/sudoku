#include "timetag.h"

#include <QDebug>

TimeTag::TimeTag(QWidget *parent) : QLabel(parent)
{
  timer.setInterval(1000);
  connect(&timer, &QTimer::timeout, this, &TimeTag::updateTime);
  init();
}

void TimeTag::init()
{
  minutes = seconds = 0;
  timer.stop();
  updateTime();
  st = INITIAL;
}

void TimeTag::start()
{
  timer.start();
  st = RUNNING;
}

void TimeTag::stop()
{
  timer.stop();
  st = STOPPED;
}

void TimeTag::toggle()
{
  if(st == STOPPED || st == INITIAL) {
      start();
    }
  else if(st == RUNNING) {
      stop();
    }
}

void TimeTag::updateTime()
{
  if(seconds >= 60) {
      minutes += 1;
      seconds = 0;
    }

  setText(QString("%1:%2").arg(fillZero(minutes)).arg(fillZero(seconds)));
  seconds += 1;
  update();
}

QString TimeTag::fillZero(int num)
{
  return num < 10 ? "0" + QString::number(num) : QString::number(num);
}
