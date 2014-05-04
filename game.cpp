#include "game.h"

#include <SDL2/SDL_image.h>

Game::Game() {
  window_ = SDL_CreateWindow("SlowBall",100, 100, 400,300, SDL_WINDOW_OPENGL);
  renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawColor(renderer_, 0xff, 0xff, 0xff, 0xff);
  SDL_RenderClear(renderer_);
  SDL_RenderPresent(renderer_);
  RenderUnit(config.get_unit_config(1));
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

#include <iostream>

void Game::RenderUnit(unit_config uc)
{
  SDL_Surface* image = IMG_Load(uc.unit_image.c_str());
  texture_ = SDL_CreateTextureFromSurface(renderer_, image);
  SDL_Rect rect(image->clip_rect);
  rect.x += rect.w;
  rect.y += rect.h;

  SDL_FreeSurface(image);
  while (1) {
  SDL_RenderClear(renderer_);
  SDL_RenderCopy(renderer_, texture_, &image->clip_rect, &image->clip_rect);
  SDL_RenderPresent(renderer_);
  SDL_RenderClear(renderer_);
  SDL_RenderCopy(renderer_, texture_, &image->clip_rect, &rect);
  SDL_RenderPresent(renderer_);
  }
}

void Game::RenderMap(const Map& m) {
}

