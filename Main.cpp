#include <iostream>

#include "config.h"
#include "game.h"
#include "map.h"

#include "battle_instance.h"

int main() {
  BattleInstance bi;
  UnitGroup ug;

  ug.location_ = Location(2, 1);

  //  bi.unit_groups_vector_.push_back(ug);
  bi.AddUnitGroup(ug);
  std::cout << bi << std::endl;

  bi.MoveUnitGroup(Location(2, 1), Location(2, 2));
  std::cout << bi << std::endl;

  std::cout << "hello" << std::endl;

  Game g;
  //  Map m(15, 10);
  g.map_ = bi.map_;
  //g.RenderMap(bi.map_);
  g.RunGame();


  return 0;
}
