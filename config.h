#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <map>

#include <SDL2/SDL.h>

const std::string kUnitDataFile = "units.dat";
const std::string kTerrainDataFile = "terrain.dat";

struct UnitConfig {
  std::string unit_image;
  std::string unit_name;
  int unit_id;
  SDL_Texture* unit_texture;
  int w, h;
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
  UnitConfig* get_unit_config(int unit_id);
  TerrainConfig* get_terrain_config(int terrain_id);


private:
  void ReadUnitMap();
  void ReadTerrainMap();

  // map from unit id to unit config.
  std::map<int, UnitConfig> unit_config_map_;

  // map from terrain id to terrain config.
  std::map<int, TerrainConfig> terrain_config_map_;
};

#endif
