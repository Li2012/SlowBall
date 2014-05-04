#include "map.h"


Location Location::GetLocation(Direction direction) {
  switch (direction) {
  case EAST:
    return Location(x + 1, y);
  case SOUTH_EAST:
    if (y % 2 == 0) {
      return Location(x - 1, y + 1);
    }
    return Location(x, y + 1);
  case SOUTH_WEST:
    if (y % 2 == 0) {
      return Location(x, y + 1);
    }
    return Location(x - 1, y + 1);
  case WEST:
    return Location(x - 1, y);
  case NORTH_WEST:
    if (y % 2 == 0) {
      return Location(x, y - 1);
    }
    return Location(x - 1, y - 1);
  case NORTH_EAST:
    if (y % 2 == 0) {
      return Location(x + 1, y - 1);
    }
    return Location(x, y - 1);
  default:
    return Location(-1, -1);
  }
}

Map::Map(int width, int height): w_(width), h_(height) {
  for (int i = 0; i < height; i++) {
    tiles_.push_back(std::vector<Tile> ());
  }
}
