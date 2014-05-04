#include "game.h"

#include <SDL2/SDL_image.h>

Game::Game() {
  window_ = SDL_CreateWindow("SlowBall",100, 100, 400,300, SDL_WINDOW_OPENGL);
  renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawColor(renderer_, 0xff, 0xff, 0xff, 0xff);
  SDL_RenderClear(renderer_);
  SDL_RenderPresent(renderer_);
  RenderUnit(config.get_unit_config(2));
}

Game::~Game() {
  SDL_DestroyWindow(window_);
  SDL_DestroyRenderer(renderer_);
}

int Game::RunGame() {
  bool done = false;
  SDL_Event event;
  while (!done) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        done = true;
        break;
      default:
        break;
      }
    }
  }
  return 0;
}

void Game::RenderUnit(unit_config uc)
{
  SDL_Surface* image = IMG_Load(uc.image.c_str());
  texture_ = SDL_CreateTextureFromSurface(renderer_, image);
  SDL_FreeSurface(image);
  SDL_RenderClear(renderer_);
  SDL_RenderCopy(renderer_, texture_, &image->clip_rect, &image->clip_rect);
  SDL_RenderPresent(renderer_);
}
