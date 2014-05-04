#include "config.h"

#include <fstream>

Config::Config() {
  std::ifstream fin_dat(unit_dat_file.c_str());
  while (! fin_dat.eof()) {
    std::string config_file_name;
    fin_dat >> config_file_name;
    std::ifstream fin_cfg(config_file_name.c_str());
    unit_config uc;
    while (! fin_cfg.eof()) {
      std::string key;
      fin_cfg >> key;
      if (key == "image") {
        fin_cfg >> uc.unit_image;
      }
      if (key == "name") {
        fin_cfg >> uc.unit_name;
      }
      if (key == "uid") {
        fin_cfg >> uc.unit_id;
      }
    }
    unit_config_map_[uc.unit_id] = uc;
  }
}
