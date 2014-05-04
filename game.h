#ifndef GAME_H_
#define GAME_H_

#include "config.h"
#include "map.h"

#include <SDL2/SDL.h>

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

  Config config;
};

#endif /* GAME_H_ */
