#ifndef UNIT_H_
#define UNIT_H_

#include <string>

#include "map.h"

class Unit {
 public:
  Unit();
  virtual ~Unit() {}

  std::string DebugString(int spaces = 0) const;

  // private:
 public:
  std::string unit_name_;

  int player_id_;
  int unit_id_;
  int max_hp_;
  int current_hp_;
  int attack_;
  int defense_;
  int current_movement_;
  int max_movement_;

};

class UnitGroup {
 public:
  UnitGroup() {}
  ~UnitGroup() {}

  std::string DebugString(int spaces = 0) const;

 public:
  Unit unit_;
  int num_;

  Location location_;
};



#endif
