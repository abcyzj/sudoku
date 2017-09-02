#include "mainwindow.h"
#include <QApplication>

//test
#include "sudokusolver.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  return a.exec();

  //test
//  SudokuSolver solver;
//  QFile file(":/gameFiles/game1.sudo");
//  if(file.open(QFile::ReadOnly)) {
//      QTextStream in(&file);
//      solver.setData(in.readAll());
//      qDebug() << solver.solveSudoku(1);
//    }
}
