#ifndef BOARD_H
#define BOARD_H

#include "matrix.h"
#include <QWidget>

class board : public QWidget
{
  Q_OBJECT

public:
  board(QWidget *parent = nullptr, int board_size = 0, Matrix *board = nullptr);
  ~board();
private:
  void updateFromMatrix();
  void updateFromMatrix(Matrix& board);
protected:
  void paintEvent(QPaintEvent *);
  void mousePressEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);

private:
  Matrix *copy_board;
  bool clicked = false;
};

#endif // BOARD_H
