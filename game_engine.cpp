#include "game_engine.h"

GameEngine::GameEngine(int board_size, int ai_lvl)
{
  Q_UNUSED(ai_lvl);
  board_matrix = new Matrix(board_size,board_size);
  for(int i(board_size/2 - 50); i < board_size/2 + 100; i += 100)
    for(int j(board_size/2 - 50); j < board_size/2 + 100; j += 100)
    *(board_matrix->take(i,j)) = 1;
  wdg = new board(nullptr, board_size, board_matrix);
}

GameEngine::~GameEngine()
{
  delete board_matrix;
  delete wdg;
}

int *GameEngine::takeBoardElement(int i, int j)
{
  return board_matrix->take(i,j);
}

QWidget *GameEngine::getBoard()
{
  return wdg;
}
