#include "board.h"
#include <QPainter>
#include <QMessageBox>
#include <QMouseEvent>

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
  for(int i(0); i < copy_board->size().width(); i++)
    for (int j(0); j < copy_board->size().height(); j++)
    {
      if(*copy_board->take(i,j) == 1)
      {
        pen.setPen(QPen(Qt::red, 10, Qt::SolidLine, Qt::RoundCap));
        pen.drawPoint(i,j);
      }
      if(*copy_board->take(i,j) > 1)
      {
        pen.setPen(QPen(Qt::red, 5, Qt::SolidLine, Qt::RoundCap));
        pen.drawPoint(i,j);
      }
    }
}

void board::updateFromMatrix(Matrix& board)
{
  copy_board = &board;
  update();
}

void board::cancel()
{
  for(auto elm : cancelList.line)
  {
    *copy_board->take(elm.x(),elm.y()) = 0;
  }
  if(!cancelList.point.isNull())
  {
    *copy_board->take(cancelList.point.x(),cancelList.point.y()) = 0;
  }
  cancelList.clear();
  clicked = false;
}

void board::paintEvent(QPaintEvent *)
{
  updateFromMatrix();
}

void board::mousePressEvent(QMouseEvent *event)
{
  Q_UNUSED(event);
  if(clicked) cancel();
  else
  {
    clicked = true;
    lineCount += 1;
  }
}

void board::mouseMoveEvent(QMouseEvent *event)
{
  if(clicked && event->x() < copy_board->size().width() && event->y() < copy_board->size().height())
  {
    int tX = event->x();
    int tY = event->y();
    if(*copy_board->take(tX,tY) == 0)
    {
      *copy_board->take(tX,tY) = lineCount;
      QPoint p(tX,tY);
      cancelList.line.push_back(p);
    }
    else if(*copy_board->take(tX,tY) == 1)
    {
      clicked = false;
      cancelList.clear();
    }
    else if(*copy_board->take(tX,tY) < lineCount && lineCount > 1)
      cancel();
    update();
  }
  else
    return;
}
