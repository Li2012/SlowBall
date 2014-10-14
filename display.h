#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "map.h"

class BattleInstance;
class Config;
class UnitGroup;

struct Point {
  Point(int x_, int y_) : x(x_), y(y_) {}
  int x;
  int y;
};

class Display {
 public:
  Display();
  ~Display();

  void CreateRenderer(SDL_Window* window);

  void RenderBattleInstance(const BattleInstance& battle_instance, Config* config);

 private:
  void Render();
  void RenderClear();
  void PrepareRendererForUnitGroup(const UnitGroup& unit_group, Config* config);
  void PrepareRendererForMap(const Map& m, Config* config);
  void PrepareRendererForInfo();
  void PrepareRendererForText(std::string text, SDL_Rect dest_port, int x, int y,
                  SDL_Color text_color);

 public:
  void PrepareRendererForLocationInfo(Location location);

 public:
  void UpdateMousePoint(Point mouse_point);
  void UpdateBattleInstanceOffset(const BattleInstance& battle_instance,
                                  const int mouse_x, const int mouse_y);

  void ClearBattleInstanceOffset();

  Location PointToLocation(Point p);
  Point LocationToCenterPoint(Location location);
  long long DistSquare(Point p1, Point p2);

 private:
  SDL_Renderer* renderer_;
  TTF_Font *font_;

  SDL_Rect top_info_rect_;
  SDL_Rect bottom_info_rect_;
  SDL_Rect right_info_rect_;
  SDL_Rect main_rect_;
  SDL_Color text_color_;

  Point mouse_point_;
  int battle_instance_x_offset_;
  int battle_instance_y_offset_;

  void ChangeColor(SDL_Surface* image, SDL_PixelFormat* pixel_format);
};

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
struct ABGR {
  Uint8 A;
  Uint8 B;
  Uint8 G;
  Uint8 R;
};
#else
struct ABGR {
  Uint8 R;
  Uint8 G;
  Uint8 B;
  Uint8 A;
};
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN


#endif  // DISPLAY_H_
