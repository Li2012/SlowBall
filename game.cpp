#include "game.h"

#include <iostream>
#include <SDL2/SDL_image.h>

#include "battle_instance.h"

Game::Game()
    // : map_(0, 0)
{
  window_ = SDL_CreateWindow("SlowBall", 100, 100, kWindowWidth, kWindowHeight, SDL_WINDOW_OPENGL);
  renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawColor(renderer_, 0xff, 0xff, 0xff, 0xff);
  SDL_RenderClear(renderer_);
  SDL_RenderPresent(renderer_);

  scrolling_sensitivity_ = 5;
  scrolling_margin_ = 15;
  //RenderUnit(*config_.get_unit_config(1));
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

  //RenderMap();
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
    //UpdateOffset();
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


void Game::RunBattleInstance(BattleInstance* battle_instance) {
  battle_instance_x_offset_ = 0;
  battle_instance_y_offset_ = 0;
  mouse_x_ = 0;
  mouse_y_ = 0;

  SDL_Event event;
  int prev_tick = SDL_GetTicks();
  int current_tick = SDL_GetTicks();

  RenderBattleInstance(battle_instance);

  while (!battle_instance->battle_done_) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT ) {
        std::cout << "quit event, quiting battle" << std::endl;
        battle_instance->battle_done_ = true;
      }
      else if (event.type == SDL_MOUSEMOTION) {
        SDL_MouseMotionEvent mouse_motion_event = event.motion;
        mouse_x_ = mouse_motion_event.x;
        mouse_y_ = mouse_motion_event.y;
        Location l = PointToLocation(Point(mouse_x_, mouse_y_));
        std::cout << l << std::endl;
      }
      else {

      }
    }
    UpdateBattleInstanceOffset(*battle_instance);
    RenderBattleInstance(battle_instance);
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
  }

  return;
}

void Game::RenderBattleInstance(BattleInstance* battle_instance) {
  RenderClear();
  PrepareRendererForMap(battle_instance->map_);
  for (const UnitGroup& unit_group : battle_instance->unit_groups_vector_) {
    PrepareRendererForUnitGroup(unit_group);
  }
  Render();
}

// void Game::PrepareRendererForUnit(UnitConfig uc) {
//   SDL_Surface* image = IMG_Load(uc.unit_image.c_str());
//   texture_ = SDL_CreateTextureFromSurface(renderer_, image);
//   SDL_Rect rect(image->clip_rect);
//   // rect.x += rect.w;
//   // rect.y += rect.h;

//   SDL_FreeSurface(image);
//   //while (1) {
//   //SDL_RenderCopy(renderer_, texture_, &image->clip_rect, &image->clip_rect);
//   //SDL_RenderPresent(renderer_);
//   //SDL_RenderClear(renderer_);
//   SDL_RenderCopy(renderer_, texture_, &image->clip_rect, &rect);
// }

void Game::PrepareRendererForUnitGroup(const UnitGroup& unit_group) {
  int unit_id = unit_group.unit_.unit_id_;
  Location location = unit_group.location_;
  int i = location.i; int j = location.j;
  UnitConfig* unit_config = config_.get_unit_config(unit_id);
  if (unit_config->unit_texture == nullptr) {
    std::cout << "loading unit image for unit id:" << unit_id << std::endl;
    SDL_Surface* image = IMG_Load(unit_config->unit_image.c_str());
    unit_config->unit_texture = SDL_CreateTextureFromSurface(renderer_, image);
    unit_config->w = image->clip_rect.w;
    unit_config->h = image->clip_rect.h;
    SDL_FreeSurface(image);
  }
  SDL_Rect src_rect;
  src_rect.x = 0; src_rect.y = 0;
  src_rect.w = unit_config->w; src_rect.h = unit_config->h;

  SDL_Rect dest_rect;
  if (j % 2 == 0) {
    dest_rect.x = unit_config->w / 2 + unit_config->w * i;
    dest_rect.y = unit_config->h * j * 3 / 4;
  }
  else {
    dest_rect.x = unit_config->w * i;
    dest_rect.y = unit_config->h * j * 3 / 4;
  }

  dest_rect.x += battle_instance_x_offset_;
  dest_rect.y += battle_instance_y_offset_;

  dest_rect.w = unit_config->w; dest_rect.h = unit_config->h;
  SDL_RenderCopy(renderer_, unit_config->unit_texture, &src_rect, &dest_rect);
}


// void Game::RenderUnit(UnitConfig uc)
// {
//   SDL_RenderClear(renderer_);
//   PrepareRendererForUnit(uc);
//   SDL_RenderPresent(renderer_);
//   //}
// }

void Game::PrepareRendererForMap(const Map& m) {
  for (int j = 0; j < m.get_h(); j++) {
    for (int i = 0; i < m.get_w(); i++) {
      Location location(i, j);
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
      if (j % 2 == 0) {
        dest_rect.x = terrain_config->w / 2 + terrain_config->w * i;
        dest_rect.y = terrain_config->h * j * 3 / 4;
      }
      else {
        dest_rect.x = terrain_config->w * i;
        dest_rect.y = terrain_config->h * j * 3 / 4;
      }

      dest_rect.x += battle_instance_x_offset_;
      dest_rect.y += battle_instance_y_offset_;

      dest_rect.w = terrain_config->w; dest_rect.h = terrain_config->h;
      SDL_RenderCopy(renderer_, terrain_config->terrain_texture, &src_rect, &dest_rect);
    }
  }
}


void Game::RenderMap(const Map& m) {
  SDL_RenderClear(renderer_);
  PrepareRendererForMap(m);
  SDL_RenderPresent(renderer_);
}

// void Game::RenderMap() {
//   int prev_tick = SDL_GetTicks();
//   RenderMap(map_);
//   std::cout << "rendering takes "
//             << SDL_GetTicks() - prev_tick << std::endl;
// }

void Game::Render() {
  SDL_RenderPresent(renderer_);
}

void Game::RenderClear() {
  SDL_RenderClear(renderer_);
}


void Game::UpdateBattleInstanceOffset(const BattleInstance& battle_instance) {
  const Map& map = battle_instance.map_;
  int battle_instance_x_offset_inc = 0;
  int battle_instance_y_offset_inc = 0;
  if (mouse_x_ > kWindowWidth - scrolling_margin_ &&
      battle_instance_x_offset_ > kWindowWidth - (map.get_w() + 1) * 72) {
    battle_instance_x_offset_inc = -1;
  }
  if (mouse_y_ > kWindowHeight - scrolling_margin_ &&
      battle_instance_y_offset_ > kWindowHeight - (map.get_h() + 1) * 72 * 3 / 4) {
    battle_instance_y_offset_inc = -1;
  }
  if (mouse_x_ < scrolling_margin_ && battle_instance_x_offset_ < 0) {
    battle_instance_x_offset_inc = 1;
  }
  if (mouse_y_ < scrolling_margin_ && battle_instance_y_offset_ < 0) {
    battle_instance_y_offset_inc = 1;
  }

  if (battle_instance_x_offset_inc == 0 && battle_instance_y_offset_inc == 0) {
    return;
  }


  battle_instance_x_offset_ += battle_instance_x_offset_inc * scrolling_sensitivity_;
  battle_instance_y_offset_ += battle_instance_y_offset_inc * scrolling_sensitivity_;
  //RenderMap();

  // std::cout << "x offset is: " << x_offset_ << " and y offset is: " << y_offset_ << std::endl;
}

Location Game::PointToLocation(Point p) {
  p.x -= battle_instance_x_offset_;
  p.y -= battle_instance_y_offset_;

  int x = p.x; int y = p.y;

  // j = y / 0.75h
  int j1 = y * 4 / 3 / kTileHeight;
  int i1;
  if (j1 % 2 == 0) {
    i1 = (x - kTileWidth / 2) / kTileWidth;
  }
  else {
    i1 = x / kTileWidth;
  }

  int j2 = (y - kTileHeight / 4) * 4 / 3 / kTileHeight;
  int i2;
  if (j2 % 2 == 0) {
    i2 = (x - kTileWidth / 2) / kTileWidth;
  }
  else {
    i2 = x / kTileWidth;
  }

  Location l1(i1, j1);
  Location l2(i2, j2);
  Point location1_center = LocationToCenterPoint(l1);
  Point location2_center = LocationToCenterPoint(l2);

  // return the closer location.
  if (DistSquare(location1_center, p) < DistSquare(location2_center, p)) {
    return l1;
  }
  return l2;
}

Point Game::LocationToCenterPoint(Location location) {
  int i = location.i; int j = location.j;
  int x = 0;
  int y = 0;
  if (j % 2 == 0) {
    x = kTileWidth / 2 + kTileWidth * i;
    y = kTileHeight * j * 3 / 4;
  }
  else {
    x = kTileWidth * i;
    y = kTileHeight * j * 3 / 4;
  }
  // std::cout << "calling LocationToCenterPoint, location is " << location
  //           << " and returning " << x + kTileWidth / 2 << "," << y + kTileHeight / 2 << std::endl;
  return Point(x + kTileWidth / 2, y + kTileHeight / 2);
}

long long Game::DistSquare(Point p1, Point p2) {
  return (p1.x - p2.x) * (p1.x - p2.x) +
      (p1.y - p2.y) * (p1.y - p2.y);
}
