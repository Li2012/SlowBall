#ifndef GAME_H_
#define GAME_H_

#include "config.h"
#include "map.h"

#include <SDL2/SDL.h>

const int kWindowWidth = 800;
const int kWindowHeight = 600;

class Game {
public:
  Game();
  ~Game();
  int RunGame();
  void RenderUnit(unit_config uc);
  void RenderMap(const Map& m);
private:
  SDL_Window* window_;
  SDL_Renderer* renderer_;
  SDL_Texture* texture_;

  Config config_;
};

#endif /* GAME_H_ */
