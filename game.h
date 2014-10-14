#ifndef GAME_H_
#define GAME_H_

#include <SDL2/SDL.h>

#include "config.h"
#include "display.h"
#include "map.h"

const int kRefreshRate = 60;
const int kDelayBase = 1000 / kRefreshRate;
const int kTileWidth = 72;
const int kTileHeight = 72;
const int kTopInfoHeight = 100;
const int kBottomInfoHeight = 100;
const int kRightInfoWidth = 300;

const int scrolling_margin = 15;
const int scrolling_sensitivity = 5;

class BattleInstance;

class Game {
 public:
  Game();
  ~Game();
  int RunGame();
  void RunBattleInstance(BattleInstance* battle_instance);

 private:
  SDL_Window* window_;
  Display display_;

  int mouse_x_;
  int mouse_y_;

  Config config_;
};

#endif // GAME_H_
