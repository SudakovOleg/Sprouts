#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <matrix.h>
#include <ai_engine.h>
#include "board.h"

class GameEngine
{
public:
  GameEngine(int board_size, int ai_lvl);
  ~GameEngine();
  int *takeBoardElement(int i, int j);
  QWidget *getBoard();
  //void CheckWin();
private:
  AiEngine bot;
  Matrix *board_matrix;
  board *wdg;
};

#endif // GAME_ENGINE_H
