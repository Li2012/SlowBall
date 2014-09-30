#include "battle_instance.h"

BattleInstance::BattleInstance() : map_(15, 12) {

}

void BattleInstance::AddUnitGroup(UnitGroup unit_group) {
  // check valid location.
  if (!map_.IsValidLocation(unit_group.location_)) {
    return;
  }

  unit_groups_vector_.push_back(unit_group);
  location_to_index_map_[unit_group.location_] = unit_groups_vector_.size() - 1;
}

void BattleInstance::MoveUnitGroup(const Location &src, const Location &dest) {
  // check valid location.
  if ((!map_.IsValidLocation(src)) || (!map_.IsValidLocation(dest))) {
    return;
  }

  const auto& iter = location_to_index_map_.find(src);

  if (iter == location_to_index_map_.end()) {
    return;
  }
  const int index = iter->second;
  location_to_index_map_.erase(iter);
  location_to_index_map_[dest] = index;
  unit_groups_vector_[index].location_ = dest;
}



std::string BattleInstance::DebugString(int spaces) const {
  std::string s(spaces, ' ');
  std::stringstream ss;
  ss << s + "battle instance debug string";

  if (unit_groups_vector_.size() == 0) {
    ss << "\nthere's no unit group in this battle instance";
  }

  for (int i = 0; i < static_cast<int>(unit_groups_vector_.size()); i++) {
    const UnitGroup& unit_group = unit_groups_vector_[i];
    ss << "\n" + s + "unit_groups_vector_[" << i << "]\n" << unit_group.DebugString(2);
  }

  ss << "\n" + s + map_.DebugString(2);

  if (location_to_index_map_.size() == 0) {
    ss << "\nthere's no location registered";
  }

  for (const auto& pair : location_to_index_map_) {
    ss << "\n" + s << pair.first << " is registered to index: " << pair.second;
  }

  return ss.str();
}

std::ostream& operator<<(std::ostream& stream, const BattleInstance& bi) {
  return stream << bi.DebugString();

}
