#include "board.h"
#include <QPainter>
#include <QMessageBox>
#include <QMouseEvent>

#define EPS 7
#define EMPTY_CELL 0
#define POINT_CELL 1

//Конструктор
board::board(QWidget *parent, int board_size, int ai_lvl)
    : QWidget(parent)
{
    Q_UNUSED(ai_lvl);
    //Включаем отслеживание мыши для рисования
    this->setMouseTracking(true);
    //Создаем матрицу и заполняем начальными данными
    board_matrix = new Matrix(board_size,board_size);
    for(int i(board_size/2 - 50); i < board_size/2 + 100; i += 100)
        for(int j(board_size/2 - 50); j < board_size/2 + 100; j += 100)
            addPoint(i,j);
}

//Диструктор
board::~board()
{
    //Удаляем матрицу
    delete board_matrix;
}

//Обновляем поля из матрицы
void board::updateFromMatrix()
{
    //Создаем инструмент рисования
    QPainter pen(this);
    //Отрисовываем в зависимости от объекта
    for(int i(0); i < board_matrix->size().width(); i++)
        for (int j(0); j < board_matrix->size().height(); j++)
        {
          //Отрисовка ростка
          if(*board_matrix->take(i,j) > POINT_CELL)
          {
              pen.setPen(QPen(*board_matrix->take(i,j)%2 == 0 ? Qt::blue : Qt::green, 5, Qt::SolidLine, Qt::RoundCap));
              pen.drawPoint(i,j);
          }
          //Отрисовка точки
          if(*board_matrix->take(i,j) == POINT_CELL)
          {
            pen.setPen(QPen(Qt::red, 10, Qt::SolidLine, Qt::RoundCap));
            pen.drawPoint(i,j);
          }
        }
}

//Метод добавления точки
void board::addPoint(int x, int y, int sprouts)
{
    //Создаем точку
    GamePoint *p = new GamePoint;
    //Хаполняем её данными
    p->point.rx() = x;
    p->point.ry() = y;
    p->sproutsCount = sprouts;
    //Кладем в список
    points.push_back(p);
    //Регестрируем в матрице с небольшими отступами для простоты привязки
    *board_matrix->take(x,y) = POINT_CELL;
    for(int dX(-2); dX < 3; dX++)
        for (int dY(-2);dY < 3; dY++)
        {
                *board_matrix->take(x+dX,y+dY) = POINT_CELL;
        }
}

GamePoint *board::findPoint(int x, int y)
{
    for(auto elm : points)
    {
        if(std::abs(x - elm->point.x()) < EPS && std::abs(y - elm->point.y()) < EPS)
        {
            return elm;
        }
    }
    return nullptr;
}

//Находит точку и проверяет возможность привязки
bool board::isPosibleSprouts(int x, int y)
{
    GamePoint *p = findPoint(x,y);
    return p && p->sproutsCount < 3;
}

bool board::isOverlapYourself(int x, int y)
{
  for(int i(0); i < stkForCancel.size() - EPS; i++)
  {
      if(stkForCancel[i].x() == x && stkForCancel[i].y() == y)
      {
        return true;
      }
  }
  return false;
}

//Метод отмены
void board::cancel()
{
    //Отменяем все элементы попавшие в стек
    for(auto elm : stkForCancel)
    {
        *board_matrix->take(elm.x(),elm.y()) = EMPTY_CELL;
    }
    //Очищаем данные для отмены
    stkForCancel.clear();
    //Снимаем нажатие
    clicked = false;
    lineCount -= 1;
    update();
}

void board::safe()
{
    //Осуществляем добавление очков
    startP->sproutsCount++;
    finishP->sproutsCount++;
}

//Перерисовка объкта
void board::paintEvent(QPaintEvent *)
{
    updateFromMatrix();
}

//Обработка события нажатия кнопки мыши
void board::mousePressEvent(QMouseEvent *event)
{
    bool isPosible = isPosibleSprouts(event->x(),event->y());
    if(!isPosible && !partSetPointFlag)
    {
        cancel();
    }
    else if(!clicked && !partSetPointFlag && isPosible)
    {
        startP = findPoint(event->x(),event->y());
        clicked = true;
        lineCount += 1;
    }
    else if(clicked && !partSetPointFlag && isPosible)
    {
        finishP = findPoint(event->x(),event->y());
        if(finishP == startP && finishP->sproutsCount > 1)
        {
          cancel();
          finishP = nullptr;
          startP = nullptr;
          return;
        }
        partSetPointFlag = true;
        clicked = false;
    }
    else if(!clicked && partSetPointFlag && !isPosible)
    {
        for(int dX(-EPS/2); dX < EPS/2; dX++)
            for (int dY(-EPS/2);dY < EPS/2; dY++)
                if(*board_matrix->take(event->x() + dX,event->y() + dY) == lineCount)
                {
                    addPoint(event->x(),event->y(),2);
                    safe();
                    finishP = nullptr;
                    startP = nullptr;
                    stkForCancel.clear();
                    partSetPointFlag = false;
                    update();
                    return;
                }
        update();
    }
}
//Обработка события движения мыши
void board::mouseMoveEvent(QMouseEvent *event)
{
    //Если мышь не за границами окна
    if(event->x() < board_matrix->size().width()
            && event->y() < board_matrix->size().height()
            && event->x() > 0 && event->y() > 0)
    {
        //Если было нажатие
        if(clicked)
        {
            int tX = event->x();
            int tY = event->y();
            if(*board_matrix->take(tX,tY) == EMPTY_CELL)
            {
                *board_matrix->take(tX,tY) = lineCount;
                QPoint p(tX,tY);
                stkForCancel.push_back(p);
            }
            else if((*board_matrix->take(tX,tY) < lineCount && *board_matrix->take(tX,tY) > POINT_CELL) || isOverlapYourself(tX,tY))
                cancel();
            update();
        }
        else
            return;
    }
}
