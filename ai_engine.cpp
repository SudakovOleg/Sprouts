#include "ai_engine.h"

//Стандартный конструктор
AiEngine::AiEngine()
= default;

//Оценочная функция
//Принимает список точек
//Возвращает оценку поля игры (где чем ближе к 0 тем ближе к победе)
int AiEngine::costFunction(QList<point> points)
{
  //Переменная возвращаемого значения
  int result;
  //Для каждой точки в списке
  for (auto pnt : points)
  {
    //Прибавляем числов возможных ходов на каждый свободный выход для хвоста
    result += (MAX_TIE-pnt.tie_count) * pnt.posible_tie_count;
  }
  //Возвращаем результат
  return result;
}
