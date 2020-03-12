#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <matrix.h>
#include <ai_engine.h>

class GameEngine
{
public:
  GameEngine(int board_size, int ai_lvl);
  ~GameEngine();
  int *takeBoardElement(int i, int j);
  //void CheckWin();
private:
  AiEngine bot;
  Matrix *board;
};

#endif // GAME_ENGINE_H
