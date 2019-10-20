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
#include "Enemy.hpp"
#include <vector>
#include <small3d/Sound.hpp>

class Game {

private:

  small3d::Renderer *renderer;
  Map map;
  small3d::Model cube;
  small3d::Model plane;
  const int localCoordRadius = 4;
  const float coord3dradius = 4.0f;
  int shootCount = 0;
  
  int xMapSize;
  int yMapSize;

  glm::ivec2 playerCoords;
  
  small3d::SceneObject *manRunning;
  small3d::SceneObject *gun;

  small3d::Sound gunshot;
  
  void renderEnv();
  std::vector<Enemy> enemies;
  
  bool died = false;
  bool won = false;
  
  bool inMenu = true;
  
  int numDead = 0;
  
  int dieCount = 0;
  int winCount = 0;
  

public:
  Game();
  ~Game();
  GLFWwindow* getWindow();
  void init();
  void terminate();
  void process(const KeyInput &input);
  void render();
};
