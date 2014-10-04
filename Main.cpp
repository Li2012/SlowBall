#include <iostream>

#include <gflags/gflags.h>
#include <glog/logging.h>

#include "battle_instance.h"
#include "config.h"
#include "game.h"
#include "map.h"

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);

  BattleInstance bi;
  UnitGroup ug;
  ug.unit_.unit_id_ = 1;
  ug.location_ = Location(2, 1);

  bi.AddUnitGroup(ug);
  bi.MoveUnitGroup(Location(2, 1), Location(2, 2));

  ug.location_ = Location(3, 5);
  bi.AddUnitGroup(ug);
  bi.MoveUnitGroup(Location(3, 5), Location(0, 0));
  ug.unit_.unit_id_ = 2;
  ug.location_ = Location(9, 4);
  bi.AddUnitGroup(ug);
  ug.location_ = Location(9, 7);
  bi.AddUnitGroup(ug);
  ug.location_ = Location(1, 7);
  bi.AddUnitGroup(ug);

  Game g;
  //  Map m(15, 10);
  //g.map_ = bi.map_;
  //g.RenderMap(bi.map_);
  //g.RunGame();
  g.RunBattleInstance(&bi);

  return 0;
}
