/*
 * Map.hpp
 *
 * Created on: 12 Oct 2019
 *     Author: Dimitri Kourkoulis
 *    License: BSD 3-Clause License (see LICENSE file)
 */

#pragma once

#include <vector>

class Map {

private:
    std::vector<std::string> mapData;

public:
  Map();
  void load(std::string filepath);
};
