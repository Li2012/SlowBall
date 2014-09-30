#ifndef GAME_H_
#define GAME_H_

#include "config.h"
#include "map.h"

#include <SDL2/SDL.h>

const int kWindowWidth = 800;
const int kWindowHeight = 600;
const int kRefreshRate = 60;
const int kDelayBase = 1000 / kRefreshRate;

class Game {
public:
  Game();
  ~Game();
  int RunGame();
  void RenderUnit(unit_config uc);
  void RenderMap(const Map& m);
  void RenderMap();
  void UpdateOffset();

  Map map_;

private:
  SDL_Window* window_;
  SDL_Renderer* renderer_;
  SDL_Texture* texture_;

  int mouse_x_;
  int mouse_y_;

  int x_offset_;
  int y_offset_;
  int scrolling_margin_;
  int scrolling_sensitivity_;

  Config config_;
};

#endif /* GAME_H_ */
