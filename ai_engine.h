#ifndef AI_ENGINE_H
#define AI_ENGINE_H

#include <QList>
//Максимальное число хвостов для одной точки
#define MAX_TIE 3
//Структура точки
struct point
{
  //Координаты
  int x;
  int y;
  //Число хвостов
  int tie_count;
  //Возможные ходы
  //(Вычисляются из количества свободных точек в области данной)
  int posible_tie_count;
};

class AiEngine
{
public:
  AiEngine();
private:
  int costFunction(QList<point> points);
};

#endif // AI_ENGINE_H
