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
  region = new char[(1 + 2 * maxRegionRadius) *
		    (1 + 2 * maxRegionRadius)];
  
}

Map::~Map() {
  if (region) delete[] region;
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

const char* Map::getRegion(int coordx, int coordy, int radius) {

  if (radius > maxRegionRadius) {
    throw std::runtime_error("getRegion given region radius exceeded maximum possible value.");
  }

  int dimension = 1 + 2 * radius;

  for (int y = 0; y < dimension; ++y) {
    for (int x = 0; x < dimension; ++x) {
      region[y * dimension + x] = mapData[coordy - radius + y][coordx - radius + x];
    }
  }
  
  return region;
}

char Map::getLocation(int coordx, int coordy) {
  return mapData[coordy][coordx];
}
