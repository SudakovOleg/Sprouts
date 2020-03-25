#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <QSize>

class Matrix
{
public:
  Matrix() = default;
  Matrix(int _m, int _n);
  int *take(int i, int j);
  void resize(int _m, int _n);
  QSize size();
  Matrix &operator=(const Matrix&);
private:
  std::vector<std::vector<int>> matrix;
  int m,n;
};

#endif // MATRIX_H
