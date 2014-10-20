#include "display.h"

#include <SDL2/SDL_image.h>
#include <gflags/gflags.h>
#include <glog/logging.h>

#include "battle_instance.h"
#include "config.h"
#include "game.h"
#include "util.h"

DECLARE_int32(window_width);
DECLARE_int32(window_height);

Display::Display() : mouse_point_(0, 0) {
  CHECK(TTF_Init() == 0);
  //font_ = TTF_OpenFont("Bretan.otf", 16);
  font_ = TTF_OpenFont("DejaVuSans.ttf", 16);
  CHECK(font_ != nullptr);

  renderer_ = nullptr;
  battle_instance_ = nullptr;

  main_rect_.x = 0;
  main_rect_.y = kTopInfoHeight;
  main_rect_.w = FLAGS_window_width - kRightInfoWidth;
  main_rect_.h = FLAGS_window_height - kBottomInfoHeight - kTopInfoHeight;

  top_info_rect_.x = 0;
  top_info_rect_.y = 0;
  top_info_rect_.w = FLAGS_window_width;
  top_info_rect_.h = kTopInfoHeight;

  bottom_info_rect_.x = 0;
  bottom_info_rect_.y = FLAGS_window_height - kBottomInfoHeight;
  bottom_info_rect_.w = FLAGS_window_width;
  bottom_info_rect_.h = kBottomInfoHeight;

  right_info_rect_.x = FLAGS_window_width - kRightInfoWidth;
  right_info_rect_.y = kTopInfoHeight;
  right_info_rect_.w = kRightInfoWidth;
  right_info_rect_.h = FLAGS_window_height - kTopInfoHeight - kBottomInfoHeight;

  text_color_.r = 0;
  text_color_.g = 0;
  text_color_.b = 0;
  text_color_.a = 255;
}

Display::~Display() {
  SDL_DestroyRenderer(renderer_);
  renderer_ = nullptr;
}

void Display::CreateRenderer(SDL_Window* window) {
  CHECK(renderer_ == nullptr);
  renderer_ = SDL_CreateRenderer(window,                      // window
                                 -1,                          // index
                                 SDL_RENDERER_ACCELERATED);   // flags

  CHECK(renderer_ != nullptr) << "NULL renderer_";

  SDL_SetRenderDrawColor(renderer_, 0xff, 0xff, 0xff, 0xff);
}

void Display::Render() {
  SDL_RenderPresent(renderer_);
}

void Display::RenderClear() {
  SDL_RenderClear(renderer_);
}

void Display::SetBattleInstance(BattleInstance* battle_instance) {
  battle_instance_ = battle_instance;
}

void Display::ClearBattleInstance() {
  battle_instance_ = nullptr;
}

void Display::RenderBattleInstance(Config* config) {
  CHECK(battle_instance_ != nullptr);
  PrepareRendererForMap(battle_instance_->map_, config);
  for (const UnitGroup& unit_group : battle_instance_->unit_groups_vector_) {
    PrepareRendererForUnitGroup(unit_group, config);
  }
  PrepareRendererForInfo();
  Location mouse_location = PointToLocation(mouse_point_);
  PrepareRendererForLocationInfo(mouse_location);
  Render();
  RenderClear();
}

void Display::PrepareRendererForUnitGroup(const UnitGroup& unit_group, Config* config) {
  int unit_id = unit_group.unit_.unit_id_;
  Location location = unit_group.location_;
  Point center = LocationToCenterPoint(location);
  UnitConfig* unit_config = config->get_unit_config(unit_id);
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
  SDL_RenderSetViewport(renderer_, &main_rect_);
  SDL_RenderCopy(renderer_, unit_config->unit_texture, &src_rect, &dest_rect);
  // SDL_RenderSetViewport(renderer_, &dest_rect);
  // SDL_RenderCopy(renderer_, unit_config->unit_texture, nullptr, nullptr);
   SDL_RenderSetViewport(renderer_, nullptr);

  // Render();
}

void Display::PrepareRendererForMap(const Map& m, Config* config) {
  SDL_RenderSetViewport(renderer_, &main_rect_);
  for (int j = 0; j < m.get_h(); j++) {
    for (int i = 0; i < m.get_w(); i++) {
      Location location(i, j);
      Point center = LocationToCenterPoint(location);
      Tile tile = m.GetTile(location);
      TerrainConfig* terrain_config = config->get_terrain_config(tile.terrain);
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
  SDL_RenderSetViewport(renderer_, nullptr);
}

void Display::PrepareRendererForInfo() {
  // Draw background box.
  SDL_SetRenderDrawColor(renderer_, 0xff, 0xff, 0xff, 0xff);

  SDL_RenderFillRect(renderer_, &top_info_rect_);
  SDL_RenderFillRect(renderer_, &bottom_info_rect_);
  SDL_RenderFillRect(renderer_, &right_info_rect_);

  // Draw border.
  SDL_SetRenderDrawColor(renderer_, 0x0, 0x0, 0x0, 0xff);
  SDL_RenderDrawRect(renderer_, &top_info_rect_);
  SDL_RenderDrawRect(renderer_, &bottom_info_rect_);
  SDL_RenderDrawRect(renderer_, &right_info_rect_);

  // Set back draw color.
  SDL_SetRenderDrawColor(renderer_, 0xff, 0xff, 0xff, 0xff);

  PrepareRendererForText("hello", right_info_rect_, 5, 5, text_color_);
  // LOG_EVERY_N(INFO, 100) << "r: " << (int)text_color_.r
  //           << "\ng: " << (int)text_color_.g
  //           << "\nb: " << (int)text_color_.b
  //           << "\na: " << (int)text_color_.a;

}

void Display::PrepareRendererForText(std::string text, SDL_Rect dest_port, int x, int y,
                         SDL_Color text_color) {
  // LOG_EVERY_N(INFO, 20) << "in rendertext\ntext is " << text
  //           << "\nr: " << (int)text_color_.r
  //           << "\ng: " << (int)text_color_.g
  //           << "\nb: " << (int)text_color_.b
  //           << "\na: " << (int)text_color_.a
  //           << "\nx is " << x << " and y is " << y;

  SDL_Surface* text_surface = TTF_RenderText_Solid(font_, text.c_str(), text_color);
  CHECK(text_surface != nullptr);
  SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer_, text_surface);
  CHECK(text_texture != nullptr);
  SDL_RenderSetViewport(renderer_, &dest_port);
  SDL_Rect text_dest;
  text_dest.x = x;
  text_dest.y = y;
  text_dest.w = text_surface->w;
  text_dest.h = text_surface->h;
  SDL_RenderCopy(renderer_, text_texture, 0, &text_dest);
  SDL_RenderSetViewport(renderer_, nullptr);
}

void Display::PrepareRendererForLocationInfo(Location location) {
  PrepareRendererForText(location.DebugString(), right_info_rect_, 5, 50, text_color_);

  // Unit info. if the location is occupied by some unit.
  if (battle_instance_->location_to_index_map_.find(location) ==
      battle_instance_->location_to_index_map_.end()) {
    return;
  }
  const int unit_index = battle_instance_->location_to_index_map_[location];
  const UnitGroup& unit_group = battle_instance_->unit_groups_vector_[unit_index];
  PrepareRendererForText("num:" + SimpleIntToString(unit_group.num_),
                         right_info_rect_, 5, 70, text_color_);
  PrepareRendererForText("unit_name:" + unit_group.unit_.unit_name_,
                         right_info_rect_, 5, 90, text_color_);
  PrepareRendererForText("hp:" + SimpleIntToString(unit_group.unit_.current_hp_)
                         + "/" + SimpleIntToString(unit_group.unit_.max_hp_),
                         right_info_rect_, 5, 110, text_color_);
}

void Display::UpdateMousePoint(const Point mouse_point) {
  mouse_point_ = mouse_point;
}

void Display::UpdateBattleInstanceOffset(const BattleInstance& battle_instance,
                                         const Point mouse_point) {
  const Map& map = battle_instance.map_;
  int battle_instance_x_offset_inc = 0;
  int battle_instance_y_offset_inc = 0;
  if (mouse_point.x > FLAGS_window_width - scrolling_margin &&
      battle_instance_x_offset_ > FLAGS_window_width - kRightInfoWidth - (map.get_w() + 1) * 72) {
    battle_instance_x_offset_inc = -1;
  }
  if (mouse_point.y > FLAGS_window_height - scrolling_margin &&
      battle_instance_y_offset_ > FLAGS_window_height - kBottomInfoHeight - kTopInfoHeight
      - (map.get_h() + 1) * 72 * 3 / 4) {
    battle_instance_y_offset_inc = -1;
  }
  if (mouse_point.x < scrolling_margin && battle_instance_x_offset_ < 0) {
    battle_instance_x_offset_inc = 1;
  }
  if (mouse_point.y < scrolling_margin && battle_instance_y_offset_ < 0) {
    battle_instance_y_offset_inc = 1;
  }

  if (battle_instance_x_offset_inc == 0 && battle_instance_y_offset_inc == 0) {
    return;
  }

  battle_instance_x_offset_ += battle_instance_x_offset_inc * scrolling_sensitivity;
  battle_instance_y_offset_ += battle_instance_y_offset_inc * scrolling_sensitivity;

  VLOG(2) << "x offset is: " << battle_instance_x_offset_ << " and y offset is: " << battle_instance_y_offset_;
}

void Display::ClearBattleInstanceOffset() {
  battle_instance_x_offset_ = 0;
  battle_instance_y_offset_ = 0;
}

Location Display::PointToLocation(Point p) {
  p.x -= battle_instance_x_offset_;
  p.y -= battle_instance_y_offset_;
  p.y -= kTopInfoHeight;

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

Point Display::LocationToCenterPoint(Location location) {
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
  return Point(x + kTileWidth / 2,
               y + kTileHeight / 2);
}

long long Display::DistSquare(Point p1, Point p2) {
  return (p1.x - p2.x) * (p1.x - p2.x) +
      (p1.y - p2.y) * (p1.y - p2.y);
}

void Display::ChangeColor(SDL_Surface* image, SDL_PixelFormat* pixel_format) {
  ABGR* pixels = static_cast<ABGR*>(image->pixels);
  for (int i = 0; i < image->w * image->h; i++) {
    if (pixels[i].R > pixels[i].G && pixels[i].R > pixels[i].B) {
      Uint8 tmp = pixels[i].R;
      pixels[i].R = pixels[i].G;
      pixels[i].G = tmp;
    }
  }
}
