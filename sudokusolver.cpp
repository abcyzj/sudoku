#include "sudokusolver.h"

#include <algorithm>

#include <QDebug>

SudokuSolver::SudokuSolver(QObject *parent)
  : QObject(parent),
    status(NODATA)
{}

void SudokuSolver::setData(const QString &data)
{
  QStringList list = data.split(',');
  Q_ASSERT(list.length() == 82);

  for(int i = 0; i < 81; i++) {
      Cube &currentCube = panel[i % 9][i / 9];
      currentCube.x = i % 9;
      currentCube.y = i / 9;
      bool ok;
      int value = list[i].toInt(&ok);
      if(ok && value) {
          currentCube.value = value;
        }
      else {
          currentCube.value = 0;
          dfsQueue.push_back(&currentCube);
        }}
  status = READY;

  QStringList results = solveSudoku(1);
  emit solvingFinished(results);
}

QStringList SudokuSolver::solveSudoku(int num)
{
  if(status != READY) {
      return QStringList();
    }
  answers.clear();
  initialSort();
  curAnsNum = 0;
  targetAnsNum = num;
  DFS(0);
  return answers;
}

void SudokuSolver::initialSort()
{
  for(int i = 0; i < 9; i++) {
      rowSet[i] = 0;
      for(int j = 0; j < 9; j++) {
          if(panel[j][i].value) {
              rowSet[i] |= (1 << panel[j][i].value);
            }
        }
    }

  for(int i = 0; i < 9; i++) {
      colSet[i] = 0;
      for(int j = 0; j < 9; j++) {
          if(panel[i][j].value) {
              colSet[i] |= (1 << panel[i][j].value);
            }
        }
    }

  for(int squareIndex = 0; squareIndex < 9; squareIndex++) {
      squareSet[squareIndex] = 0;
      int squareX = squareIndex % 3;
      int squareY = squareIndex / 3;
      int minX = squareX * 3;
      int minY = squareY * 3;
      for(int i = minX; i < minX + 3; i++) {
          for(int j = minY; j < minY + 3; j++) {
              if(panel[i][j].value) {
                  squareSet[squareIndex] |= (1 << panel[i][j].value);
                }
            }
        }
    }

  foreach(Cube *cube, dfsQueue) {
      cube->possibilites = 9;
      int currentWholeSet = (rowSet[cube->y] | colSet[cube->x] | squareSet[cube->x + cube->y * 3]);
      for(int i = 1; i <= 9; i++) {
          if((1 << i) & currentWholeSet) {
              cube->possibilites -= 1;
            }
        }
    }

//  for(int i = 0; i < dfsQueue.length(); i++) {
//      for(int j = dfsQueue.length() - 1; j > i ; j--) {//取出最小的一个
//          if(dfsQueue[j]->possibilites < dfsQueue[j - 1]) {
//              Cube *temp = dfsQueue[j];
//              dfsQueue[j] = dfsQueue[j - 1];
//              dfsQueue[j - 1] = temp;
//            }
//        }

//      for(int k = i; k < dfsQueue.length(); k++) {
//          dfsQueue[k]->possibilites , curAnsNum, targetAnsNum-= 1;
//        }
//    }

  std::sort(dfsQueue.begin(), dfsQueue.end(), [](Cube *cube1, Cube *cube2) -> bool {
    return cube1->possibilites < cube2->possibilites;
  });
}

bool SudokuSolver::DFS(int depth)
{
  //qDebug() << outputCurrentAnswer();
  if(depth == dfsQueue.length()) {
      answers.append(outputCurrentAnswer());
      curAnsNum++;
      if(curAnsNum == targetAnsNum) {
          return false;
        }
      else {
          return true;
        }
    }

  Cube *currentCube = dfsQueue[depth];
  int cubeX = dfsQueue[depth]->x, cubeY = dfsQueue[depth]->y;
  int squareIndex = (cubeX / 3) + (cubeY / 3) * 3;
  int wholeSet = (colSet[cubeX] | rowSet[cubeY] | squareSet[squareIndex]);
  for(int i = 1; i <= 9; i++) {
      if(!(wholeSet & (1 << i) ) ) {
          currentCube->value = i;
          colSet[cubeX] |= (1 << i);
          rowSet[cubeY] |= (1 << i);
          squareSet[squareIndex] |= (1 << i);
          if(!DFS(depth + 1)) {
              return false;
            }
          currentCube->value = 0;
          colSet[cubeX] &= ~(1 << i);
          rowSet[cubeY] &= ~(1 << i);
          squareSet[squareIndex] &= ~(1 << i);
        }
    }
  return true;
}

QString SudokuSolver::outputCurrentAnswer()
{
  QString answer;
  for(int i = 0; i < 9; i++) {
      for(int j = 0; j < 9; j++) {
          answer.append(QString::number(panel[i][j].value) + ',');
        }
    }
 return answer;
}
