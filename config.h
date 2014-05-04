#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <map>

#include <SDL2/SDL.h>

const std::string unit_dat_file = "units.dat";

struct unit_config {
  std::string unit_image;
  std::string unit_name;
  int unit_id;
};

struct terrain_config {
  std::string terrain_image_path;
  std::string terrain_name;
  int terrain_id;
  SDL_Surface* terrain_surface;
};

class Config {
public:
  Config();
  unit_config get_unit_config(int unit_id) {
    return unit_config_map_[unit_id];
  }

private:
  std::map<int, unit_config> unit_config_map_;
  std::map<int, terrain_config> terrain_config_map_;
};

#endif
