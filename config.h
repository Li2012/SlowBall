#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <map>

const std::string unit_dat_file = "units.dat";

struct unit_config {
  std::string image;
  std::string unit_name;
  int unit_id;
};

class Config {
public:
  Config();
  unit_config get_unit_config(int unit_id) {
    return unit_config_map[unit_id];
  }

private:
  std::map<int, unit_config> unit_config_map;
};

#endif
