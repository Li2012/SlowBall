#include "map.h"


Location Location::GetLocation(const Direction& direction) {
  if (i == -1 or j == -1) {
    return Location(-1, -1);
  }

  switch (direction) {
  case EAST:
    return Location(i + 1, j);
  case SOUTH_EAST:
    if (j % 2 == 0) {
      return Location(i + 1, j + 1);
    }
    return Location(i, j + 1);
  case SOUTH_WEST:
    if (j % 2 == 0) {
      return Location(i, j + 1);
    }
    return Location(i - 1, j + 1);
  case WEST:
    return Location(i - 1, j);
  case NORTH_WEST:
    if (j % 2 == 0) {
      return Location(i, j - 1);
    }
    return Location(i - 1, j - 1);
  case NORTH_EAST:
    if (j % 2 == 0) {
      return Location(i + 1, j - 1);
    }
    return Location(i, j - 1);
  default:
    return Location(-1, -1);
  }
}

std::string Location::DebugString() const {
  std::stringstream ss;
  std::string result;

  ss << "(" << i << "," << j << ")";
  ss >> result;

  return result;
}

std::ostream& operator<<(std::ostream& stream, const Location& l) {
  return stream << l.DebugString();
}

Map::Map(int width, int height): w_(width), h_(height) {
  for (int j = 0; j < height; j++) {
    std::vector<Tile> row(width);
    for (int i = 0; i < width; i++) {
      Tile t;
      t.terrain = ((346 * j + 573 * i) % 79) % 3 + 1;
      row[i] = t;
    }
    tiles_.push_back(row);
  }
}

Tile Map::GetTile(Location location) const {
  return tiles_[location.j][location.i];
}

bool Map::IsValidLocation(const Location& location) const {
  return location.i >= 0 && location.j >= 0 && location.i < w_ && location.j < h_;
}

std::string Map::DebugString(int spaces) const {
  std::stringstream ss;
  std::string s(spaces, ' ');

  ss << s + "map debug string\n";

  for (int j = 0; j < h_; j++) {
    ss << s;
    if (j % 2 == 0) {
      ss << " ";
    }
    for (int i = 0; i < w_; i++) {
      ss << tiles_[j][i].terrain << " ";
    }
    ss << "\n";
  }
  return ss.str();
}
