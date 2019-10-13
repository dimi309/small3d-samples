/*
 * Game.hpp
 *
 * Created on: 12 Oct 2019
 *     Author: Dimitri Kourkoulis
 *    License: BSD 3-Clause License (see LICENSE file)
 */
#pragma once

#include "Map.hpp"
#include <small3d/Renderer.hpp>
#include <small3d/Model.hpp>
#include "KeyInput.hpp"
#include <small3d/SceneObject.hpp>

class Game {

private:

  small3d::Renderer *renderer;
  Map map;
  small3d::Model cube;
  
  const int xMapSize = 29;
  const int yMapSize = 11;

  glm::ivec2 playerCoords = glm::ivec2(3, 3);
  glm::vec2 playerPosition = glm::vec2(0.0f, 0.0f);
  
  small3d::SceneObject *manRunning;
  
  void renderEnv(int radius);
  
  

public:
  Game();
  ~Game();
  GLFWwindow* getWindow();
  void init();
  void terminate();
  void process(const KeyInput &input);
  void render();
};
