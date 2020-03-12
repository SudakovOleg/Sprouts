#include "matrix.h"
#include <vector>

//Конструктор матрицы
Matrix::Matrix(int _m, int _n)
{
  resize(_n,_m);
}

//Метод получения значения по индексам
int *Matrix::take(int i, int j)
{
  //Если индексы в пределах матрицы возвращаем результат
  if((i > -1 && j > -1) && (i < m && j < n))
    return &matrix[i][j];
  //Если нет, возвращаем пустой указатель
  return nullptr;
}
//Метод изменяет размер векторов
//в соответствии с размером матрицы
void Matrix::resize(int _m, int _n)
{
  //Запоминаем размеры
  n = _n;
  m = _m;
  //Изменяем вектора
  matrix.resize(_m);
  for (int i(0); i < _n;i++)
  {
    matrix[i].resize(_n);
  }
}

//Метод возращающий размер матрицы
QSize Matrix::size()
{
  QSize res;
  res.setHeight(m);
  res.setWidth(n);
  return res;
}

//Перегрузка оператора присваивания
Matrix &Matrix::operator=(const Matrix& _original)
{
  n = _original.n;
  m = _original.m;
  resize(m,n);
  matrix = _original.matrix;
  return *this;
}
