/*
 * Map.cpp
 *
 * Created on: 12 Oct 2019
 *     Author: Dimitri Kourkoulis
 *    License: BSD 3-Clause License (see LICENSE file)
 */

#include "Map.hpp"
#include <fstream>
#include <small3d/Logger.hpp>

using namespace small3d;

Map::Map() {
  
}

void Map::load(std::string filepath) {
  mapData.clear();
  std::ifstream mapfile(filepath);
  if (mapfile.is_open()) {
    std::string line;
    while(std::getline(mapfile, line)) {
      mapData.push_back(line);
    }
    LOGDEBUG("Loaded map.");
    mapfile.close();
  }
  else {
    throw std::runtime_error("Could not open map file.");
  }
}
