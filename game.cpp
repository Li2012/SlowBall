#include "game.h"

#include <SDL2/SDL_image.h>
#include <gflags/gflags.h>
#include <glog/logging.h>

#include "battle_instance.h"

DEFINE_int32(window_width, 800, "window width");
DEFINE_int32(window_height, 600, "window height");

Game::Game()
    // : map_(0, 0)
{
  CHECK((FLAGS_window_width == 800 && FLAGS_window_height == 600) ||
        (FLAGS_window_width == 1366 && FLAGS_window_height == 768))
      << "wrong window width and height";

  window_ = SDL_CreateWindow("SlowBall",          // titile
                             100,                 // x
                             100,                 // y
                             FLAGS_window_width,  // w
                             FLAGS_window_height, // h
                             SDL_WINDOW_OPENGL);  // flags

  CHECK(window_ != nullptr) << "NULL window_";

  renderer_ = SDL_CreateRenderer(window_,                     // window
                                 -1,                          // index
                                 SDL_RENDERER_ACCELERATED);   // flags

  CHECK(renderer_ != nullptr) << "NULL renderer_";

  SDL_RenderClear(renderer_);
  SDL_RenderPresent(renderer_);

  scrolling_sensitivity_ = 5;
  scrolling_margin_ = 15;
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

  while (!done) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT ) {
        LOG(INFO) << "quit event";
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
          LOG(INFO) << "keyboard quit event";
          done = true;
          break;
        default:
           // LOG(INFO) << "scan code: " << SDL_GetScancodeName(keyboard_event.keysym.scancode)
           //           << "key name: " << SDL_GetKeyName(keyboard_event.keysym.sym);
          break;
        }
      }
      else {
        LOG(INFO) << "in default";
      }
    }
    //UpdateOffset();
    current_tick = SDL_GetTicks();
    if (current_tick - prev_tick > 5) {
       LOG(INFO) << "in while loop before delay and prev tick is "
                 << prev_tick << " and current tick is: " << current_tick
                 << " and diff is " << current_tick - prev_tick;
    }
    int delay = kDelayBase - (current_tick - prev_tick);
    if (delay > 0) {
      SDL_Delay(delay);
    }
    prev_tick = SDL_GetTicks();
    // LOG(INFO) << "in while loop after delay and prev tick is "
    //           << prev_tick << " and current tick is: " << current_tick;

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

  Location src, dest;

  RenderBattleInstance(*battle_instance);

  while (!battle_instance->battle_done_) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT ) {
        LOG(INFO) << "quit event, quiting battle";
        battle_instance->battle_done_ = true;
      }
      else if (event.type == SDL_MOUSEMOTION) {
        SDL_MouseMotionEvent mouse_motion_event = event.motion;
        mouse_x_ = mouse_motion_event.x;
        mouse_y_ = mouse_motion_event.y;
        Location l = PointToLocation(Point(mouse_x_, mouse_y_));
        VLOG(2) << l;
      }
      else if (event.type == SDL_MOUSEBUTTONDOWN) {
        SDL_MouseButtonEvent mouse_button_event = event.button;
        if (mouse_button_event.button == SDL_BUTTON_LEFT) {
          mouse_x_ = mouse_button_event.x;
          mouse_y_ = mouse_button_event.y;
          src = PointToLocation(Point(mouse_x_, mouse_y_));
        }
      }
      else if (event.type == SDL_MOUSEBUTTONUP) {
        SDL_MouseButtonEvent mouse_button_event = event.button;
        if (mouse_button_event.button == SDL_BUTTON_LEFT) {
          mouse_x_ = mouse_button_event.x;
          mouse_y_ = mouse_button_event.y;
          dest = PointToLocation(Point(mouse_x_, mouse_y_));
          VLOG(2) << "moving from " << src << " to " << dest;
          battle_instance->MoveUnitGroup(src, dest);
        }
      }
      else {

      }
    }
    UpdateBattleInstanceOffset(*battle_instance);
    RenderBattleInstance(*battle_instance);
    current_tick = SDL_GetTicks();
    if (current_tick - prev_tick > 5) {
       LOG(INFO) << "in while loop before delay and prev tick is "
                 << prev_tick << " and current tick is: " << current_tick
                 << " and diff is " << current_tick - prev_tick;
    }
    int delay = kDelayBase - (current_tick - prev_tick);
    if (delay > 0) {
      SDL_Delay(delay);
    }
    prev_tick = SDL_GetTicks();
  }

  return;
}

void Game::RenderBattleInstance(const BattleInstance& battle_instance) {
  RenderClear();
  PrepareRendererForMap(battle_instance.map_);
  for (const UnitGroup& unit_group : battle_instance.unit_groups_vector_) {
    PrepareRendererForUnitGroup(unit_group);
  }
  PrepareRendererForInfo();
  Render();
}

void Game::PrepareRendererForUnitGroup(const UnitGroup& unit_group) {
  int unit_id = unit_group.unit_.unit_id_;
  Location location = unit_group.location_;
  Point center = LocationToCenterPoint(location);
  UnitConfig* unit_config = config_.get_unit_config(unit_id);
  if (unit_config->unit_texture == nullptr) {
    LOG(INFO) << "loading unit image for unit id:" << unit_id;
    SDL_Surface* image = IMG_Load(unit_config->unit_image.c_str());
    CHECK(image != nullptr) << "failed loading image for unit id: " << unit_id
                            << " and file: " << unit_config->unit_image;
    SDL_PixelFormat* pixel_format = image->format;

    CHECK_EQ(SDL_PIXELTYPE(pixel_format->format), SDL_PIXELTYPE_PACKED32);
    CHECK_EQ(SDL_PIXELORDER(pixel_format->format), SDL_PACKEDORDER_ABGR);
    CHECK_EQ(SDL_PIXELLAYOUT(pixel_format->format), SDL_PACKEDLAYOUT_8888);

    CHECK_EQ(SDL_BITSPERPIXEL(pixel_format->format), 32);
    CHECK_EQ(SDL_BYTESPERPIXEL(pixel_format->format), 4);

    unit_config->unit_texture = SDL_CreateTextureFromSurface(renderer_, image);
    CHECK_EQ(image->clip_rect.w, kTileWidth);
    CHECK_EQ(image->clip_rect.h, kTileHeight);
    SDL_FreeSurface(image);
  }
  SDL_Rect src_rect;
  src_rect.x = 0; src_rect.y = 0;
  src_rect.w = kTileWidth; src_rect.h = kTileHeight;

  SDL_Rect dest_rect;
  dest_rect.x = center.x - kTileWidth / 2;
  dest_rect.y = center.y - kTileHeight / 2;
  dest_rect.x += battle_instance_x_offset_;
  dest_rect.y += battle_instance_y_offset_;

  dest_rect.w = kTileWidth; dest_rect.h = kTileHeight;
  SDL_RenderCopy(renderer_, unit_config->unit_texture, &src_rect, &dest_rect);
}

void Game::PrepareRendererForMap(const Map& m) {
  for (int j = 0; j < m.get_h(); j++) {
    for (int i = 0; i < m.get_w(); i++) {
      Location location(i, j);
      Point center = LocationToCenterPoint(location);
      Tile tile = m.GetTile(location);
      TerrainConfig* terrain_config = config_.get_terrain_config(tile.terrain);
      if (terrain_config->terrain_texture == nullptr) {
        SDL_Surface* image = IMG_Load(terrain_config->terrain_image_path.c_str());
        CHECK(image != nullptr) << "failed loading image for terrain: " << tile.terrain
                            << " and file: " << terrain_config->terrain_image_path;
        terrain_config->terrain_texture = SDL_CreateTextureFromSurface(renderer_, image);
        LOG(INFO) << "texture created for terrain" << terrain_config->terrain_id;
        CHECK_EQ(kTileWidth, image->clip_rect.w);
        CHECK_EQ(kTileHeight, image->clip_rect.h);
        SDL_FreeSurface(image);
      }
      SDL_Rect src_rect;
      src_rect.x = 0; src_rect.y = 0;
      src_rect.w = kTileWidth; src_rect.h = kTileHeight;
      SDL_Rect dest_rect;

      dest_rect.x = center.x - kTileWidth / 2;
      dest_rect.y = center.y - kTileHeight / 2;
      dest_rect.x += battle_instance_x_offset_;
      dest_rect.y += battle_instance_y_offset_;

      dest_rect.w = kTileWidth; dest_rect.h = kTileHeight;
      SDL_RenderCopy(renderer_, terrain_config->terrain_texture, &src_rect, &dest_rect);
    }
  }
}

void Game::PrepareRendererForInfo() {
  SDL_SetRenderDrawColor(renderer_, 0xff, 0xff, 0xff, 0xff);
  SDL_Rect top_info_rect;
  top_info_rect.x = 0;
  top_info_rect.y = 0;
  top_info_rect.w = FLAGS_window_width;
  top_info_rect.h = kTopInfoHeight;
  SDL_RenderFillRect(renderer_, &top_info_rect);

  SDL_Rect bottom_info_rect;
  bottom_info_rect.x = 0;
  bottom_info_rect.y = FLAGS_window_height - kBottomInfoHeight;
  bottom_info_rect.w = FLAGS_window_width;
  bottom_info_rect.h = kBottomInfoHeight;
  SDL_RenderFillRect(renderer_, &bottom_info_rect);

  SDL_Rect right_info_rect;
  right_info_rect.x = FLAGS_window_width - kRightInfoWidth;
  right_info_rect.y = kTopInfoHeight;
  right_info_rect.w = kRightInfoWidth;
  right_info_rect.h = FLAGS_window_height - kTopInfoHeight - kBottomInfoHeight;
  SDL_RenderFillRect(renderer_, &right_info_rect);

  SDL_SetRenderDrawColor(renderer_, 0x0, 0x0, 0x0, 0xff);
  SDL_RenderDrawRect(renderer_, &top_info_rect);
  SDL_RenderDrawRect(renderer_, &bottom_info_rect);
  SDL_RenderDrawRect(renderer_, &right_info_rect);
  SDL_SetRenderDrawColor(renderer_, 0xff, 0xff, 0xff, 0xff);
}

void Game::RenderMap(const Map& m) {
  SDL_RenderClear(renderer_);
  PrepareRendererForMap(m);
  SDL_RenderPresent(renderer_);
}

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
  if (mouse_x_ > FLAGS_window_width - scrolling_margin_ &&
      battle_instance_x_offset_ > FLAGS_window_width - (map.get_w() + 1) * 72) {
    battle_instance_x_offset_inc = -1;
  }
  if (mouse_y_ > FLAGS_window_height - scrolling_margin_ &&
      battle_instance_y_offset_ > FLAGS_window_height - (map.get_h() + 1) * 72 * 3 / 4) {
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

  VLOG(2) << "x offset is: " << battle_instance_x_offset_ << " and y offset is: " << battle_instance_y_offset_;
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
  return Point(x + kTileWidth / 2, y + kTileHeight / 2 + kTopInfoHeight);
}

long long Game::DistSquare(Point p1, Point p2) {
  return (p1.x - p2.x) * (p1.x - p2.x) +
      (p1.y - p2.y) * (p1.y - p2.y);
}

void Game::ChangeColor(SDL_Surface* image, SDL_PixelFormat* pixel_format) {
  ABGR* pixels = static_cast<ABGR*>(image->pixels);
  for (int i = 0; i < image->w * image->h; i++) {
    if (pixels[i].R > pixels[i].G && pixels[i].R > pixels[i].B) {
      Uint8 tmp = pixels[i].R;
      pixels[i].R = pixels[i].G;
      pixels[i].G = tmp;
    }
  }
}
