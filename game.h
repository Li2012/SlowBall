#ifndef GAME_H_
#define GAME_H_

#include <SDL2/SDL.h>

#include "config.h"
#include "map.h"

const int kRefreshRate = 60;
const int kDelayBase = 1000 / kRefreshRate;
const int kTileWidth = 72;
const int kTileHeight = 72;

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

  void Render();
  void RenderClear();
  void UpdateBattleInstanceOffset(const BattleInstance& battle_instance);

  Location PointToLocation(Point p);
  Point LocationToCenterPoint(Location location);
  long long DistSquare(Point p1, Point p2);

private:
  SDL_Window* window_;
  SDL_Renderer* renderer_;
  SDL_Texture* texture_;

  int mouse_x_;
  int mouse_y_;

  int battle_instance_x_offset_;
  int battle_instance_y_offset_;
  int scrolling_margin_;
  int scrolling_sensitivity_;

  Config config_;
};

struct ABGR {
  Uint8 R;
  Uint8 G;
  Uint8 B;
  Uint8 A;
};

#endif /* GAME_H_ */
