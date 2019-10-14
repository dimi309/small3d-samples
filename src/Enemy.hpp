/*
 * Enemy.hpp
 *
 * Created on: 14 Oct 2019
 *     Author: Dimitri Kourkoulis
 *    License: BSD 3-Clause License (see LICENSE file)
 */
#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

class Enemy {

private:

  glm::ivec2 coords = glm::ivec2(0, 0);
  glm::vec2 position = glm::vec2(0.0f, 0.0f);

public:
  Enemy();


};


