#ifndef MAP_H_
#define MAP_H_

#include <vector>
#include <iostream>

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
  Location(int x, int y) {
    if (x < 0 || y < 0) {
      this->x = -1; 
      this->y = -1;
      return;
    }
    this->x = x; this->y = y;
  }

  Location GetLocation(const Direction& d);

  int x;
  int y;

};


std::ostream& operator<<(std::ostream& stream, const Location& l);

inline bool operator==(const Location& lhs, const Location& rhs) { 
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline bool operator!=(const Location& lhs, const Location& rhs) { 
  return !(lhs == rhs);
}

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

#endif
