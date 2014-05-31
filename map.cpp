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
    std::vector<Tile> row(width);
    for (int j = 0; j < width; j++) {
      Tile t;
      t.terrain = ((346 * i + 573 * j) % 79) % 3 + 1;
      row[j] = t;
    }
    tiles_.push_back(row);
  }
}

Tile Map::GetTile(Location location) const {
  return tiles_[location.y][location.x];
}
