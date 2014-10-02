#ifndef BATTLE_INSTANCE_H_
#define BATTLE_INSTANCE_H_

#include <map>
#include <vector>
#include <iostream>

#include "unit.h"

class BattleInstance {
 public:
  BattleInstance();
  ~BattleInstance() {}

 public:
  void AddUnitGroup(UnitGroup unit_group);
  void MoveUnitGroup(const Location &src, const Location &dest);

  std::vector<UnitGroup> unit_groups_vector_;
  Map map_;
  std::map<Location, int> location_to_index_map_;

  bool battle_done_;

  std::string DebugString(int spaces = 0) const;
};

std::ostream& operator<<(std::ostream& stream, const BattleInstance& bi);



#endif
