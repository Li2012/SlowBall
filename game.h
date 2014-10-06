#ifndef GAME_H_
#define GAME_H_

#include <SDL2/SDL.h>

#include "config.h"
#include "map.h"

const int kRefreshRate = 60;
const int kDelayBase = 1000 / kRefreshRate;
const int kTileWidth = 72;
const int kTileHeight = 72;
const int kTopInfoHeight = 20;
const int kBottomInfoHeight = 20;
const int kRightInfoWidth = 80;

class BattleInstance;
class UnitGroup;

struct Point {
  Point(int x_, int y_) : x(x_), y(y_) {}
  int x;
  int y;
};

class Game {
public:
  Game();
  ~Game();
  int RunGame();
  void RunBattleInstance(BattleInstance* battle_instance);

  void RenderBattleInstance(const BattleInstance& battle_instance);
  void PrepareRendererForUnitGroup(const UnitGroup& unit_group);

  void PrepareRendererForMap(const Map& m);
  void RenderMap(const Map& m);

  void PrepareRendererForInfo();

  void Render();
  void RenderClear();
  void UpdateBattleInstanceOffset(const BattleInstance& battle_instance);

  Location PointToLocation(Point p);
  Point LocationToCenterPoint(Location location);
  long long DistSquare(Point p1, Point p2);

private:
  SDL_Window* window_;
  SDL_Renderer* renderer_;

  int mouse_x_;
  int mouse_y_;

  int battle_instance_x_offset_;
  int battle_instance_y_offset_;
  int scrolling_margin_;
  int scrolling_sensitivity_;

  Config config_;

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


#endif // GAME_H_
