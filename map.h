#ifndef MAP_H_
#define MAP_H_

#include <vector>
#include <iostream>
#include <sstream>
#include <string>

//Zero indexed hex map.
// (0,0) is always at the north east of (0,1).
//         /\    /\    /\     |
//        /  \  /  \  /  \    |
//       /    \/    \/    \   |
//       |0,0  |1,0 | 2,0 |   |
//       |     |    |     |   |
//      /\    /\    /\    /   |
//     /  \  /  \  /  \  /    |
//    /    \/    \/    \/     |
//    |0,1  |1,1 | 2,1  |     |
//    |     |    |      |     |
//    \    /\    /\    /\     |
//     \  /  \  /  \  /  \    |
//      \/    \/    \/    \   |
//       |0,2  |1,2 | 2,2 |   |
//       |     |    |     |   |
//       \    /\    /\    /   |
//        \  /  \  /  \  /    |
//         \/    \/    \/     |


enum Direction {
  EAST, SOUTH_EAST, SOUTH_WEST, WEST, NORTH_WEST, NORTH_EAST
};

struct Location {
  Location() {
    i = -1;
    j = -1;
  }

  Location(int i, int j) {
    if (i < 0 || j < 0) {
      this->i = -1;
      this->j = -1;
      return;
    }
    this->i = i; this->j = j;
  }

  Location GetLocation(const Direction& d);

  std::string DebugString() const;

  // use i, j. since x, y are used for mouse point.
  // i is the horizontal index, and j is the vertical index.
  int i;
  int j;

};


std::ostream& operator<< (std::ostream& stream, const Location& l);

inline bool operator== (const Location& lhs, const Location& rhs) {
  return lhs.i == rhs.i && lhs.j == rhs.j;
}

inline bool operator!= (const Location& lhs, const Location& rhs) {
  return !(lhs == rhs);
}

inline bool operator< (const Location& lhs, const Location& rhs) {
  if (lhs.i < rhs.i) return true;
  if (lhs.i > rhs.i) return false;
  return lhs.j < rhs.j;
}

struct Tile {
  int terrain;
};

class Map {
 public:
  Map(int width, int height);
  Map(std::string map_file);

  void ReadMapFromStream(std::istream& in);
  void ReadMapFromFile(std::string map_file);

  Tile GetTile(Location location) const;
  int get_w() const {return w_;}
  //void set_w(int width) {w_ = width;}
  int get_h() const {return h_;}
  //void set_h(int height) {h_ = height;}

  bool IsValidLocation(const Location& location) const;

  std::string DebugString(int spaces = 0) const;


 private:
  int w_;
  int h_;
  std::vector<std::vector<Tile> > tiles_;
};


#endif
