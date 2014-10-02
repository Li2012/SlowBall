#include "config.h"

#include <fstream>
#include <iostream>

Config::Config() {
  ReadUnitMap();
  ReadTerrainMap();
}

Config::~Config() {
  for (auto it = terrain_config_map_.begin(); it != terrain_config_map_.end(); it++) {
    SDL_DestroyTexture(it->second.terrain_texture);
std::cerr << "texture destroy for terrain" << it->second.terrain_id << std::endl;
    it->second.terrain_texture = nullptr;
  }

  for (auto it = unit_config_map_.begin(); it != unit_config_map_.end(); it++) {
    SDL_DestroyTexture(it->second.unit_texture);
std::cerr << "texture destroy for unit" << it->second.unit_id << std::endl;
    it->second.unit_texture = nullptr;
  }

}

UnitConfig* Config::get_unit_config(int unit_id) {
  const auto& iter = unit_config_map_.find(unit_id);
  if (iter == unit_config_map_.end()) {
    return nullptr;
  }
  return &iter->second;
}

TerrainConfig* Config::get_terrain_config(int terrain_id) {
  const auto& iter = terrain_config_map_.find(terrain_id);
  if (iter == terrain_config_map_.end()) {
    return nullptr;
  }
  return &iter->second;
}

void Config::ReadUnitMap() {
  std::ifstream fin_dat(kUnitDataFile.c_str());
  while (! fin_dat.eof()) {
    std::string config_file_name;
    fin_dat >> config_file_name;
    std::ifstream fin_cfg(config_file_name.c_str());
    UnitConfig uc;
    uc.unit_texture = nullptr;
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

void Config::ReadTerrainMap() {
  std::ifstream fin_dat(kTerrainDataFile.c_str());
  while (! fin_dat.eof()) {
    std::string config_file_name;
    fin_dat >> config_file_name;
    std::ifstream fin_cfg(config_file_name.c_str());
    TerrainConfig terrain_config;
    terrain_config.terrain_texture = nullptr;
    while (! fin_cfg.eof()) {
      std::string key;
      fin_cfg >> key;
      if (key == "image") {
        fin_cfg >> terrain_config.terrain_image_path;
      }
      if (key == "name") {
        fin_cfg >> terrain_config.terrain_name;
      }
      if (key == "tid") {
        fin_cfg >> terrain_config.terrain_id;
      }
    }
    terrain_config_map_[terrain_config.terrain_id] = terrain_config;
  }
}
