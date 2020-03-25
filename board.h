#include <utility>

#ifndef BOARD_H
#define BOARD_H

#include "matrix.h"
#include <QWidget>
#include <QMap>
#include <QStack>

//Структура игровой точки
struct GamePoint
{
  QPoint point;
  int sproutsCount;
};

struct PosibleMove
{
  Matrix m;
  QList<GamePoint> points;
};

//Класс игрового поля
class board : public QWidget
{
  Q_OBJECT

public:
  board(QWidget *parent = nullptr, int board_size = 0, int ai_lvl = 0);
  ~board();
private:
  void updateFromMatrix();
  void addPoint(int x, int y, int sprouts = 0);
  bool checkWin(Matrix *m);
  bool canCreatePath(int x, int y, Matrix &m, GamePoint *FinishPoint, int death = 0);
  GamePoint *findPoint(int x, int y);
  //void deletPoint(int x, int y); //Пока не нужна
  bool isPosibleSprouts(int x, int y);
  bool isOverlapYourself(int x, int y);
  void cancel();
  void safe();
  void initCosts(std::vector<int>& costs, int x, int y, Matrix &m);

  int MINIMAX(Matrix &m, QList<GamePoint> points, int line, int deph, int alpha, int beta);
  QList<PosibleMove> generatePosibleMoves(Matrix &m, QList<GamePoint> points);
protected:
  void paintEvent(QPaintEvent *);
  void mousePressEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);

private:
  int lineCount = 1;
  Matrix *board_matrix;
  QStack<QPoint> stkForCancel;
  bool clicked = false;
  bool partSetPointFlag = false;
  QList<GamePoint*> points;
  GamePoint *startP, *finishP;
  int aiLevel;
};

#endif // BOARD_H
