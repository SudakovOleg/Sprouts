#ifndef BOARD_H
#define BOARD_H

#include "matrix.h"
#include "ai_engine.h"
#include <QWidget>
#include <QMap>
#include <QStack>

//Структура игровой точки
struct GamePoint
{
  QPoint point;
  int sproutsCount;
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
  GamePoint *findPoint(int x, int y);
  //void deletPoint(int x, int y); //Пока не нужна
  bool isPosibleSprouts(int x, int y);
  bool isOverlapYourself(int x, int y);
  void cancel();
  void safe();
protected:
  void paintEvent(QPaintEvent *);
  void mousePressEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);

private:
  Matrix *board_matrix;
  QStack<QPoint> stkForCancel;
  bool clicked = false;
  bool partSetPointFlag = false;
  int lineCount = 1;
  AiEngine bot;
  QList<GamePoint*> points;
  GamePoint *startP, *finishP;
};

#endif // BOARD_H
