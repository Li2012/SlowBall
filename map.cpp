#include "map.h"


Location Location::GetLocation(const Direction& direction) {
  if (x == -1 or y == -1) {
    return Location(-1, -1);
  }

  switch (direction) {
  case EAST:
    return Location(x + 1, y);
  case SOUTH_EAST:
    if (y % 2 == 0) {
      return Location(x + 1, y + 1);
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

std::string Location::DebugString() const {
  std::stringstream ss;
  std::string result;

  ss << "(" << x << "," << y << ")";
  ss >> result;

  return result;
}

std::ostream& operator<<(std::ostream& stream, const Location& l) {
  return stream << l.DebugString();
}

Map::Map(int width, int height): w_(width), h_(height) {
  for (int y = 0; y < height; y++) {
    std::vector<Tile> row(width);
    for (int x = 0; x < width; x++) {
      Tile t;
      t.terrain = ((346 * y + 573 * x) % 79) % 3 + 1;
      row[x] = t;
    }
    tiles_.push_back(row);
  }
}

Tile Map::GetTile(Location location) const {
  return tiles_[location.y][location.x];
}

bool Map::IsValidLocation(const Location& location) const {
  return location.x >= 0 && location.y >= 0 && location.x < w_ && location.y < h_;
}

std::string Map::DebugString(int spaces) const {
  std::stringstream ss;
  std::string s(spaces, ' ');

  ss << s + "map debug string\n";

  for (int y = 0; y < h_; y++) {
    ss << s;
    if (y % 2 == 0) {
      ss << " ";
    }
    for (int x = 0; x < w_; x++) {
      ss << tiles_[y][x].terrain << " ";
    }
    ss << "\n";
  }
  return ss.str();
}
