#include "game_engine.h"

GameEngine::GameEngine(int board_size, int ai_lvl)
{
  Q_UNUSED(ai_lvl);
  board = new Matrix(board_size,board_size);
}

GameEngine::~GameEngine()
{
  delete board;
}

int *GameEngine::takeBoardElement(int i, int j)
{
  return board->take(i,j);
}
