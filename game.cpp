#include "game.h"

#include <iostream>
#include <SDL2/SDL_image.h>

Game::Game() : map_(0, 0) {
  window_ = SDL_CreateWindow("SlowBall", 100, 100, kWindowWidth, kWindowHeight, SDL_WINDOW_OPENGL);
  renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawColor(renderer_, 0xff, 0xff, 0xff, 0xff);
  SDL_RenderClear(renderer_);
  SDL_RenderPresent(renderer_);

  x_offset_ = 0;
  y_offset_ = 0;
  scrolling_sensitivity_ = 5;
  scrolling_margin_ = 15;
  //RenderUnit(config_.get_unit_config(1));
}

Game::~Game() {
  SDL_DestroyWindow(window_);
  SDL_DestroyRenderer(renderer_);
}

int Game::RunGame() {
  bool done = false;
  SDL_Event event;
  int prev_tick = SDL_GetTicks();
  int current_tick = SDL_GetTicks();
  mouse_x_ = 0;
  mouse_y_ = 0;

  RenderMap();
  while (!done) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT ) {
        std::cout << "quit event" << std::endl;
        done = true;
      }

      else if (event.type == SDL_MOUSEMOTION) {
        SDL_MouseMotionEvent mouse_motion_event = event.motion;
        mouse_x_ = mouse_motion_event.x;
        mouse_y_ = mouse_motion_event.y;
      }

      else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
        SDL_KeyboardEvent keyboard_event = event.key;
        SDL_Keysym key_symbol = keyboard_event.keysym;

        // can call SDL_GetScancodeName(keyboard_event.keysym.scancode)
        // and SDL_GetKeyName(keyboard_event.keysym.sym) to get symbols.
        switch (key_symbol.sym) {
        case SDLK_ESCAPE:
          std::cout << "keyboard quit event" << std::endl;
          done = true;
          break;
        default:
           // std::cout << "scan code: " << SDL_GetScancodeName(keyboard_event.keysym.scancode)
           //           << "key name: " << SDL_GetKeyName(keyboard_event.keysym.sym) << std::endl;
          break;
        }
      }
      else {
        std::cout << "in default" << std::endl;
      }
    }
    UpdateOffset();
    current_tick = SDL_GetTicks();
    if (current_tick - prev_tick > 5) {
       std::cout << "in while loop before delay and prev tick is "
                 << prev_tick << " and current tick is: " << current_tick
                 << " and diff is " << current_tick - prev_tick << std::endl;
    }
    int delay = kDelayBase - (current_tick - prev_tick);
    if (delay > 0) {
      SDL_Delay(delay);
    }
    prev_tick = SDL_GetTicks();
    // std::cout << "in while loop after delay and prev tick is "
    //           << prev_tick << " and current tick is: " << current_tick << std::endl;

  }
  return 0;
}

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

      dest_rect.x += x_offset_;
      dest_rect.y += y_offset_;

      dest_rect.w = terrain_config->w; dest_rect.h = terrain_config->h;
      SDL_RenderCopy(renderer_, terrain_config->terrain_texture, &src_rect, &dest_rect);
    }
  }
  SDL_RenderPresent(renderer_);
}

void Game::RenderMap() {
  int prev_tick = SDL_GetTicks();
  RenderMap(map_);
  std::cout << "rendering takes "
            << SDL_GetTicks() - prev_tick << std::endl;
}

void Game::UpdateOffset() {
  int x_offset_inc = 0;
  int y_offset_inc = 0;
  if (mouse_x_ > kWindowWidth - scrolling_margin_ &&
      x_offset_ > kWindowWidth - (map_.get_w() + 1) * 72) {
    x_offset_inc = -1;
  }
  if (mouse_y_ > kWindowHeight - scrolling_margin_ &&
      y_offset_ > kWindowHeight - (map_.get_h() + 1) * 72 * 3 / 4) {
    y_offset_inc = -1;
  }
  if (mouse_x_ < scrolling_margin_ && x_offset_ < 0) {
    x_offset_inc = 1;
  }
  if (mouse_y_ < scrolling_margin_ && y_offset_ < 0) {
    y_offset_inc = 1;
  }

  if (x_offset_inc == 0 && y_offset_inc == 0) {
    return;
  }


  x_offset_ += x_offset_inc * scrolling_sensitivity_;
  y_offset_ += y_offset_inc * scrolling_sensitivity_;
  RenderMap();

  // std::cout << "x offset is: " << x_offset_ << " and y offset is: " << y_offset_ << std::endl;
}
