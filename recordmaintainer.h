#ifndef RECORDMAINTAINER_H
#define RECORDMAINTAINER_H

#include <QObject>
#include <QVector>

class GameController;
class Cube;

class RecordMaintainer : public QObject
{
  Q_OBJECT

  struct Record {
    int x;
    int y;
    int set;

    Record() : x(0), y(0), set(0) {}
    Record(int x, int y, int set) : x(x), y(y), set(set) {}
  };

  enum Status {
    Performing,
    Recording
  };

public:
  explicit RecordMaintainer(GameController *controller, QObject *parent = nullptr);

  void backout();
  void forward();

public slots:
  void addRemoveRecord(Cube *cube, int set);
  void addAddRecord(Cube *cube, int set);

private:
  void performRecord(const Record &record);//正做
  void unperformRecord(const Record &record);//反做

  GameController *gameController;

  QVector<Record> recordVec;
  int currentIndex;
  Status status;
};

#endif // RECORDMAINTAINER_H
