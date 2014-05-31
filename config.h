#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <map>

#include <SDL2/SDL.h>

const std::string kUnitDataFile = "units.dat";
const std::string kTerrainDataFile = "terrain.dat";

struct unit_config {
  std::string unit_image;
  std::string unit_name;
  int unit_id;
};

struct TerrainConfig {
  std::string terrain_image_path;
  std::string terrain_name;
  int terrain_id;
  SDL_Texture* terrain_texture;
  int w, h;
};

class Config {
public:
  Config();
  ~Config();
  unit_config get_unit_config(int unit_id) {
    return unit_config_map_[unit_id];
  }

  TerrainConfig* get_terrain_config(int terrain_id) {
    return &terrain_config_map_[terrain_id];
  }

private:
  void ReadUnitMap();
  void ReadTerrainMap();

  std::map<int, unit_config> unit_config_map_;
  std::map<int, TerrainConfig> terrain_config_map_;
};

#endif
