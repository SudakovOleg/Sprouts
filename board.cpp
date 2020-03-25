#include <utility>

#include "board.h"
#include <QPainter>
#include <QMessageBox>
#include <QMouseEvent>
#include <QDebug>

#define EPS 7
#define EMPTY_CELL 0
#define POINT_CELL 1

//Конструктор
board::board(QWidget *parent, int board_size, int ai_lvl)
  : QWidget(parent)
{
  aiLevel = ai_lvl;
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
  GamePoint p;
  //Хаполняем её данными
  p.point.rx() = x;
  p.point.ry() = y;
  p.sproutsCount = sprouts;
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

//Метод проверки победы
bool board::checkWin(Matrix *m, QList<GamePoint> _points)
{
  //Создаем временную матрицу
  Matrix tmpM = *m;
  //Находим две доступные для хода точки
  for(auto elm : _points)
  {
    if(elm.sproutsCount < 3)
    {
      for(auto finElm : _points)
      {
        if(&elm != &finElm && finElm.sproutsCount < 3)
        {
          //Ищем неприсоединеный край точки старта
          for(int dX(-3); dX < 4; dX++)
            for (int dY(-3);dY < 4; dY++)
            {
              //Если нашли пробуем строить путь между точками
              if(*board_matrix->take(elm.point.x() + dX,elm.point.y() + dY) == EMPTY_CELL)
              {
                if(canCreatePath(elm.point.x() + dX,elm.point.y() + dY,lineCount + 1, tmpM,&finElm,points))
                  return false;
              }
            }
        }
      }
    }
  }
  return true;
}

//Метод построения пути
bool board::canCreatePath(int x, int y, int line,Matrix &m, GamePoint *FinishPoint, const QList<GamePoint>& _points, int death)
{
  //Если мы у точки
  GamePoint gp = findPoint(x,y,_points);
  auto *p = &gp;
  if(p && p->point == FinishPoint->point)
    return true;

  //Если глубина больше 2*размер поля
  if(death > 400)
    return false;

  //Инициализируем оценки на основе расстояний до точки по обеим осям
  std::vector<int> costs(6);
  costs[0] = FinishPoint->point.x() - x;
  costs[1] = FinishPoint->point.y() - y;
  costs[2] = -costs[1]; //Вверх
  costs[3] = costs[1]; //Вниз
  costs[4] = costs[0]; //Впарво
  costs[5] = -costs[0]; //Влево

  //Проводим окончательный расчет оценок смотря на припятствия
  initCosts(costs, x, y, m);

  //Ищем максимальную оценку
  int max = -100;
  int index = 0;
  for (int i(2); i < 6; i++)
  {
    if(costs[i] > max)
    {
      max = costs[i];
      index = i;
    }
  }
  //Делаем соответствующий ход
  switch (index)
  {
    case 2: //Вверх
      {
        if(y - 1 > 0 && *m.take(x,y - 1) == 0)
        {
          *m.take(x, y - 1) = line;
          if(canCreatePath(x,y - 1,line,m,FinishPoint, _points, death + 1))
          {
            return true;
          }
          *m.take(x, y - 1) = 0;
          break;
        }
      }
    case 3: //Вниз
      {
        if(y + 1 < m.size().height() && *m.take(x,y + 1) == 0)
        {
          *m.take(x, y + 1) = line;
          if(canCreatePath(x,y + 1,line,m,FinishPoint, _points,death + 1))
          {
            return true;
          }
          *m.take(x, y + 1) = 0;
          break;
        }
      }
    case 4:  //Вправо
      {
        if(x + 1 < m.size().width() && *m.take(x + 1,y) == 0)
        {
          *m.take(x + 1,y) = line;
          if(canCreatePath(x + 1,y,line,m,FinishPoint,_points, death + 1))
          {
            return true;
          }
          *m.take(x + 1,y) = 0;
          break;
        }
      }
    case 5: //Влево
      {
        if(x - 1 > 0 && *m.take(x - 1,y) == 0)
        {
          *m.take(x - 1, y) = line;
          if(canCreatePath(x - 1,y,line,m,FinishPoint,_points, death + 1))
          {
            return true;
          }
          *m.take(x - 1, y - 1) = 0;
          break;
        }
      }
  }
  //Иначе путь не найден
  return false;
}

//Метод нахождения точки по координатам
GamePoint board::findPoint(int x, int y, QList<GamePoint> _points)
{
  for(auto elm : _points)
  {
    if(std::abs(x - elm.point.x()) < EPS && std::abs(y - elm.point.y()) < EPS)
    {
      return elm;
    }
  }
  return {};
}

//Находит точку и проверяет возможность привязки
bool board::isPosibleSprouts(int x, int y)
{
  GamePoint gp = findPoint(x,y,points);
  return gp.point.rx() > 0 && gp.sproutsCount < 3;
}

//Метод проверки пересечения линий самой себя
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
  if(clicked)
  {
    //Снимаем нажатие
    clicked = false;
    lineCount -= 1;
  }
  update();
}

//Метод сохранения изменений
void board::safe()
{
  //Осуществляем добавление очков
  startP->sproutsCount++;
  finishP->sproutsCount++;
}

//Метод подсчета оценок по четырем направлениям опираясь на припятствия
void board::initCosts(std::vector<int> &costs, int x, int y, Matrix &m)
{
  int i = 0;
  while (*m.take(x,y - i) == 0 && y - i > 0) //Вверх
  {
    costs[2]++;
    i++;
  }
  i = 0;
  while (*m.take(x,y + i) == 0 && y + i < m.size().height() - 1) //Вниз
  {
    costs[3]++;
    i++;
  }
  i = 0;
  while (*m.take(x + i,y) == 0 && x + i < m.size().width() - 1) //Вправо
  {
    costs[4]++;
    i++;
  }
  i = 0;
  while (*m.take(x - i,y) == 0 && x - i > 0) //Влево
  {
    costs[5]++;
    i++;
  }
  i = 0;
}

void board::findPath(int x, int y, int line, int deph, Matrix &m, QList<GamePoint> &_points, QPoint finish)
{
  if((std::abs(finish.x() - x) > 2*EPS && std::abs(finish.y() - y) > 2*EPS) || deph > 2*EPS)
  {
    if(rand()%100 < 30 || deph+1 > 2*EPS)
    {
      GamePoint p;
      p.point.rx() = x;
      p.point.ry() = y;
      p.sproutsCount = 2;
      //Кладем в список
      _points.push_back(p);
      //Регестрируем в матрице с небольшими отступами для простоты привязки
      *m.take(x,y) = POINT_CELL;
      for(int dX(-2); dX < 3; dX++)
        for (int dY(-2);dY < 3; dY++)
        {
          *m.take(x+dX,y+dY) = POINT_CELL;
        }
      return;
    }
  }
  if(*m.take(x+1,y) == line)
    return findPath(x+1,y,line,deph+1,m,_points,finish);
  if(*m.take(x-1,y) == line)
    return findPath(x-1,y,line,deph+1,m,_points,finish);
  if(*m.take(x,y+1) == line)
    return findPath(x,y+1,line,deph+1,m,_points,finish);
  if(*m.take(x,y-1) == line)
    return findPath(x,y-1,line,deph+1,m,_points,finish);
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
  //Если нажато на пустом месте при рисовании ростка или просто так
  if(!isPosible && !partSetPointFlag)
  {
    cancel();
  }
  //Если нажато на точку для начала рисования ростка
  else if(!clicked && !partSetPointFlag && isPosible)
  {
    GamePoint sp = findPoint(event->x(),event->y(),points);
    startP = &sp;
    clicked = true;
    lineCount += 1;
  }
  //Если нажато на точку при рисований ростка
  else if(clicked && !partSetPointFlag && isPosible)
  {
    GamePoint fp = findPoint(event->x(),event->y(),points);
    finishP = &fp;
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
  //Если нажато на поле при добавлений точки
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
          if(checkWin(board_matrix,points)) QMessageBox::information(this,"Win", "Win");
          MINIMAX(*board_matrix,points,++lineCount,0,-10,10);
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
      //Если по данным координатам клетка пуста
      if(*board_matrix->take(tX,tY) == EMPTY_CELL)
      {
        *board_matrix->take(tX,tY) = lineCount;
        QPoint p(tX,tY);
        stkForCancel.push_back(p);
      }
      //Если по данным координатам что то пересекается и это не точка, или пристствует самопересечение
      else if((*board_matrix->take(tX,tY) < lineCount && *board_matrix->take(tX,tY) > POINT_CELL) || isOverlapYourself(tX,tY))
        cancel();
      update();
    }
    else
      return;
  }
}

QVector<PosibleMove> board::generatePosibleMoves(Matrix &m, QList<GamePoint> &_points, int line)
{
  QVector<PosibleMove> moves;
  for(auto &elm : _points)
  {
    if(elm.sproutsCount < 3)
    {
      for(auto &finElm : _points)
      {
        if(&elm != &finElm && finElm.sproutsCount < 3)
        {
          bool breakeFlag = false;
          //Ищем неприсоединеный край точки старта
          for(int dX(-3); dX < 4; dX++)
          {
            for (int dY(-3);dY < 4; dY++)
            {
              Matrix tmpM = m;
              //Если нашли пробуем строить путь между точками
              if(*tmpM.take(elm.point.x() + dX,elm.point.y() + dY) == EMPTY_CELL)
              {
                if(canCreatePath(elm.point.x() + dX,elm.point.y() + dY,line, tmpM, &finElm, _points))
                {
                  breakeFlag = true;

                  elm.sproutsCount++;
                  finElm.sproutsCount++;

                  PosibleMove move;
                  move.m = tmpM;
                  for(auto p : _points)
                  {
                    GamePoint tmp;
                    tmp.point = p.point;
                    tmp.sproutsCount = p.sproutsCount;
                    move.points.push_back(tmp);
                  }

                  //Ставим новую точку
                  findPath(elm.point.x() + dX,elm.point.y() + dY,line,0,move.m,move.points,finElm.point);
                  //Кладем в вектор
                  moves.push_back(move);

                  elm.sproutsCount--;
                  finElm.sproutsCount--;
                  break;
                }
              }
            }
            if(breakeFlag)
              break;
          }
        }
      }
    }
  }
  return moves;
}

int board::MINIMAX(Matrix &m, QList<GamePoint> _points, int line, int deph, int alpha, int beta)
{
  //Если терминальная вершина
  if(deph > aiLevel || checkWin(&m,_points))
    return deph%2 == 0 ? 1 : -1;

  //Генерация возможных ходов
  QVector<PosibleMove> posibleMoves(generatePosibleMoves(m,_points, line));
  //Инициализация переменной для рекорда
  int score;
  deph%2 == 0 ? score = alpha : score = beta;
  PosibleMove *bestmove = &posibleMoves.first();
  //Произведение хода и отправка алгоритма в рекурсию
  for(auto &move : posibleMoves)
  {
    //Рекурсионо определяем оценку хода
    int new_score = MINIMAX(move.m, move.points, line + 1, deph + 1, alpha, beta);

    //Если макс
    if(new_score > score && deph%2 == 0)
    {
      score = new_score;
      bestmove = &move;
    }
    //Если мин
    else if(new_score <= score && deph%2 == 1)
    {
      score = new_score;
      bestmove = &move;
    }

    //Альфа бета отсечение
    if(deph%2 == 0)
      alpha = qMax(alpha,score);
    else
      beta = qMax(beta,score);

    //Условие прирывания рекурсии
    if(beta < alpha)
      break;
  }

  //Если глубина нулевая копирование матрицы с новым ходом
  if(deph == 0)
  {
    m = bestmove->m;
    //Конвертирование и запоминание списка
    points = bestmove->points;
  }
  //Возврат рекорда
  return score;
}
