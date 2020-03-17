#ifndef BOARD_H
#define BOARD_H

#include "matrix.h"
#include <QWidget>
#include <QMap>
#include <QStack>

struct forCancel
{
  QStack<QPoint> line;
  QPoint point;
  void clear()
  {
    line.clear();
    point.rx() = 0;
    point.ry() = 0;
  }
};

class board : public QWidget
{
  Q_OBJECT

public:
  board(QWidget *parent = nullptr, int board_size = 0, Matrix *board = nullptr);
  ~board();
private:
  void updateFromMatrix();
  void updateFromMatrix(Matrix& board);
  void cancel();
protected:
  void paintEvent(QPaintEvent *);
  void mousePressEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);

private:
  Matrix *copy_board;
  forCancel cancelList;
  bool clicked = false;
  int lineCount = 1;
};

#endif // BOARD_H
