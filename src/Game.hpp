/*
 * Game.hpp
 *
 * Created on: 12 Oct 2019
 *     Author: Dimitri Kourkoulis
 *    License: BSD 3-Clause License (see LICENSE file)
 */
#pragma once

#include <small3d/Renderer.hpp>

class Game {

private:

  int posX, posY;

  std::vector<std::string> map;

public:
  Game();
  init();
  terminate();
  process();
  render();
}
