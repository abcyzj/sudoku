#include "recordmaintainer.h"
#include "gamecontroller.h"
#include "cube.h"

RecordMaintainer::RecordMaintainer(GameController *controller, QObject *parent)
  : QObject(parent),
    gameController(controller),
    currentIndex(0),
    status(Recording)
{
  foreach(Cube *cube, controller->m_cubes) {
      connect(cube, &Cube::numberAdded, this, &RecordMaintainer::addAddRecord);
      connect(cube, &Cube::numberRemoved, this, &RecordMaintainer::addRemoveRecord);
    }
}

void RecordMaintainer::addAddRecord(Cube *cube, int set)
{
  if(status != Recording) {
      return;
    }

  if(currentIndex != recordVec.length()) {
      recordVec.erase(recordVec.begin() + currentIndex, recordVec.end());
    }

  recordVec.push_back(Record(cube->rx(), cube->ry(), set));
  currentIndex++;
}

void RecordMaintainer::addRemoveRecord(Cube *cube, int set)
{
  if(status != Recording) {
      return;
    }

  if(currentIndex != recordVec.length()) {
      recordVec.erase(recordVec.begin() + currentIndex, recordVec.end());
    }

  recordVec.push_back(Record(cube->rx(), cube->ry(), -set));
  currentIndex++;
}

void RecordMaintainer::backout()
{
  if(currentIndex == 0) {//不能再退了
      return;
    }

  unperformRecord(recordVec[--currentIndex]);
}

void RecordMaintainer::forward()
{
  if(currentIndex == recordVec.length()) {//不能再前进了
      return;
    }

  performRecord(recordVec[currentIndex++]);
}


void RecordMaintainer::performRecord(const Record &record)
{
  status = Performing;

  if(record.set > 0) {
      for(int i = 1; i <= 9; i++) {
          if(record.set & 1 << i) {
              gameController->cubeAt(record.x, record.y)->addNumber(i);
            }
        }
    }
  else {
      for(int i = 1; i <= 9; i++) {
          if(-record.set & 1 << i)
              gameController->cubeAt(record.x, record.y)->removeNumber(i);
        }
    }

  status = Recording;
}


void RecordMaintainer::unperformRecord(const Record &record)
{
  status = Performing;

  if(record.set > 0) {
      for(int i = 1; i <= 9; i++) {
          if(record.set & 1 << i) {
              gameController->cubeAt(record.x, record.y)->removeNumber(i);
            }
        }
    }
  else {
      for(int i = 1; i <= 9; i++) {
          if(-record.set & 1 << i)
              gameController->cubeAt(record.x, record.y)->addNumber(i);
        }
    }

  status = Recording;
}
