#ifndef MAP_H_
#define MAP_H_

#include <vector>

enum Direction {
  EAST, SOUTH_EAST, SOUTH_WEST, WEST, NORTH_WEST, NORTH_EAST
};

struct Location {
  Location(int x, int y) {
    this->x = x; this->y = y;
  }

  Location GetLocation(Direction d);

  int x;
  int y;
};

struct Tile {
  int terrain;
};

class Map {
public:
  Map(int width, int height);

  Tile GetTile(Location location) const;
  int get_w() const {return w_;}
  void set_w(int width) {w_ = width;}
  int get_h() const {return h_;}
  void set_h(int height) {h_ = height;}


private:
  int w_;
  int h_;
  std::vector<std::vector<Tile> > tiles_;
};

// haha


#endif
