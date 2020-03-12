#include "board.h"
#include <QPainter>
#include <QMessageBox>

board::board(QWidget *parent, int board_size, Matrix *board)
  : QWidget(parent)
{
  this->setMouseTracking(true);
  copy_board = new Matrix(board_size,board_size);
  copy_board = board;
}

board::~board()
{
  delete copy_board;
}

void board::updateFromMatrix()
{
  QPainter pen(this);
  pen.setPen(Qt::red);
  for(int i(0); i < copy_board->size().width(); i++)
    for (int j(0); j < copy_board->size().height(); j++)
    {
      pen.drawPoint(i,j);
    }
}

void board::updateFromMatrix(Matrix& board)
{
  copy_board = &board;
  update();
}

void board::paintEvent(QPaintEvent *)
{
  updateFromMatrix();
}

void board::mousePressEvent(QMouseEvent *event)
{
  return;
}

void board::mouseMoveEvent(QMouseEvent *event)
{
  return;
}
