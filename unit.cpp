#include "unit.h"

Unit::Unit() {
  unit_name_ = "dummy";
  player_id_ = -1;
}

std::string Unit::DebugString(int spaces) const {
  std::string s(spaces, ' ');
  std::stringstream resultstream;
  resultstream << s + "unit debug string\n";
  resultstream << s + "unit_name_: " + unit_name_ + "\n";
  resultstream << s + "player_id_: " << player_id_ << "\n";
  return resultstream.str();
}


std::string UnitGroup::DebugString(int spaces) const {
  std::string s(spaces, ' ');
  std::stringstream resultstream;
  resultstream << s + "unit group debug string\n";
  resultstream << s + "unit_:\n" + unit_.DebugString(2 + spaces) + "\n";
  resultstream << s + "num_: " << num_ << "\n";
  resultstream << s << "location_: " << location_;

   return  resultstream.str();
;
}
