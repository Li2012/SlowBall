#ifndef GAME_H_
#define GAME_H_

#include "config.h"
#include "map.h"

#include <SDL2/SDL.h>

const int kWindowWidth = 800;
const int kWindowHeight = 600;
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

  void RenderBattleInstance(BattleInstance* battle_instance);
  // void PrepareRendererForUnit(UnitConfig uc);
  void PrepareRendererForUnitGroup(const UnitGroup& unit_group);
  // void RenderUnit(UnitConfig uc);

  void PrepareRendererForMap(const Map& m);
  void RenderMap(const Map& m);
  // void RenderMap();

  void Render();
  void RenderClear();
  void UpdateBattleInstanceOffset(const BattleInstance& battle_instance);

  Location PointToLocation(Point p);
  Point LocationToCenterPoint(Location location);
  long long DistSquare(Point p1, Point p2);
  // Map map_;

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

#endif /* GAME_H_ */
