#include "game.h"

#include <SDL2/SDL_image.h>

Game::Game() {
  window_ = SDL_CreateWindow("SlowBall",100, 100, kWindowWidth, kWindowHeight, SDL_WINDOW_OPENGL);
  renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawColor(renderer_, 0xff, 0xff, 0xff, 0xff);
  SDL_RenderClear(renderer_);
  SDL_RenderPresent(renderer_);
  //RenderUnit(config_.get_unit_config(1));
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
  //while (1) {
  SDL_RenderClear(renderer_);
  //SDL_RenderCopy(renderer_, texture_, &image->clip_rect, &image->clip_rect);
  //SDL_RenderPresent(renderer_);
  //SDL_RenderClear(renderer_);
  SDL_RenderCopy(renderer_, texture_, &image->clip_rect, &rect);
  SDL_RenderPresent(renderer_);
  //}
}

void Game::RenderMap(const Map& m) {
  SDL_RenderClear(renderer_);
  for (int y = 0; y < m.get_h(); y++) {
    for (int x = 0; x < m.get_w(); x++) {
      Location location(x, y);
      Tile tile = m.GetTile(location);
      TerrainConfig* terrain_config = config_.get_terrain_config(tile.terrain);
      if (terrain_config->terrain_texture == nullptr) {
        SDL_Surface* image = IMG_Load(terrain_config->terrain_image_path.c_str());
        terrain_config->terrain_texture = SDL_CreateTextureFromSurface(renderer_, image);
std::cerr << "texture created for terrain" << terrain_config->terrain_id << std::endl;
        terrain_config->w = image->clip_rect.w;
        terrain_config->h = image->clip_rect.h;
        SDL_FreeSurface(image);
      }
      SDL_Rect src_rect;
      src_rect.x = 0; src_rect.y = 0;
      src_rect.w = terrain_config->w; src_rect.h = terrain_config->h;
      SDL_Rect dest_rect;
      if (y % 2 == 0) {
        dest_rect.x = terrain_config->w / 2 + terrain_config->w * x;
        dest_rect.y = terrain_config->h * y * 3 / 4;
      }
      else {
        dest_rect.x = terrain_config->w * x;
        dest_rect.y = terrain_config->h * y * 3 / 4;
      }
      dest_rect.w = terrain_config->w; dest_rect.h = terrain_config->h;
      SDL_RenderCopy(renderer_, terrain_config->terrain_texture, &src_rect, &dest_rect);
    }
  }
  SDL_RenderPresent(renderer_);
}

