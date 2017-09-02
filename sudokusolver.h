#ifndef SUDOKUSOLVER_H
#define SUDOKUSOLVER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>

class SudokuSolver : public QObject
{
  Q_OBJECT

  enum Status {
    NODATA,
    READY
  };

  struct Cube {
    short x;
    short y;
    short value;
    short possibilites;
  };

public:
  SudokuSolver(QObject *parent = nullptr);

public slots:
  void setData(const QString &data);//给定中盘
  QStringList solveSudoku(int num);//给出num个答案,0代表所有答案

signals:
  void solvingFinished(QStringList results);

private:
  void initialSort();
  bool DFS(int depth);
  QString outputCurrentAnswer();

  Cube panel[9][9];//盘面情况，0表示未填
  QVector<Cube*> dfsQueue;
  int rowSet[9];
  int colSet[9];
  int squareSet[9];
  Status status;
  QStringList answers;
  int curAnsNum;
  int targetAnsNum;
};

#endif // SUDOKUSOLVER_H
