/*
 * Game.cpp
 *
 * Created on: 12 Oct 2019
 *     Author: Dimitri Kourkoulis
 *    License: BSD 3-Clause License (see LICENSE file)
 */

#include "Game.hpp"
#include <small3d/Logger.hpp>

#define FULL_ROTATION 6.28f // More or less 360 degrees in radians
#define CAMERA_ROTATION_SPEED 0.1f
#define CAMERA_SPEED 0.95f

using namespace small3d;

Game::Game() {

  small3d::initLogger();

  renderer = &small3d::Renderer::getInstance("Groom", 800, 600, 1.0f,
    1.0f, 60.0f, -1.0f, "resources/shaders/", 240);

  map.load("resources/map.txt");

  cube = Model("resources/cube.obj");

  renderer->generateTexture("tileTexture", Image("resources/images/tile.png"));

  renderer->cameraPosition.y = 3.2f;
 
}

GLFWwindow* Game::getWindow() {
  return renderer->getWindow();
}

void Game::init() {
}

void Game::terminate() {
}

void Game::process(const KeyInput &input) {
  
  if (input.left) {
    renderer->cameraRotation.y -= CAMERA_ROTATION_SPEED;
    
    if (renderer->cameraRotation.y < -FULL_ROTATION)
      renderer->cameraRotation.y = 0.0f;
    
  } else if (input.right) {
    renderer->cameraRotation.y += CAMERA_ROTATION_SPEED;
    
    if (renderer->cameraRotation.y > FULL_ROTATION)
      renderer->cameraRotation.y = 0.0f;
    
  }
  
  if (input.up) {
    
    renderer->cameraPosition.x += sin(renderer->cameraRotation.y) * CAMERA_SPEED;
    renderer->cameraPosition.z -= cos(renderer->cameraRotation.y) * CAMERA_SPEED;
    
  } else if (input.down) {
    renderer->cameraPosition.x -= sin(renderer->cameraRotation.y) * CAMERA_SPEED;
    renderer->cameraPosition.z += cos(renderer->cameraRotation.y) * CAMERA_SPEED;
  }
  
  if (renderer->cameraPosition.x < -4.0f ) {
    
    if (playerCoords.x > 0 &&
        map.getLocation(playerCoords.x - 1, playerCoords.y) != '#') {
      --playerCoords.x;
      renderer->cameraPosition.x = 0.0f;
    }
    else {
      renderer->cameraPosition.x = -4.0f;
    }
  }
  
  if (renderer->cameraPosition.x > 4.0f ) {
    if (playerCoords.x < xMapSize - 1 &&
    map.getLocation(playerCoords.x + 1, playerCoords.y) != '#') {
      ++playerCoords.x;
      renderer->cameraPosition.x = 0.0f;
    }
    else {
      renderer->cameraPosition.x = 4.0f;
    }
  }
  
  if (renderer->cameraPosition.z > 4.0f ) {
    
    if (playerCoords.y < yMapSize - 1 &&
    map.getLocation(playerCoords.x, playerCoords.y + 1) != '#') {
      ++playerCoords.y;
      renderer->cameraPosition.z = 0.0f;
    }
    else {
      renderer->cameraPosition.z = 4.0f;
    }
  }
  
  if (renderer->cameraPosition.z < -4.0f ) {
    
    if (playerCoords.y > 0 &&
    map.getLocation(playerCoords.x, playerCoords.y - 1) != '#') {
      --playerCoords.y;
      renderer->cameraPosition.z = 0.0f;
    }
    else {
      renderer->cameraPosition.z = -4.0f;
    }
  }
}

void Game::renderEnv(int radius) {
  
  const char* region = map.getRegion(playerCoords.x, playerCoords.y, radius);

  int length = 2 * radius + 1;

  for (int y = 0; y < length; ++y) {
    for (int x = 0; x < length; ++x) {
      if (y * length + x >= 0 && y * length + x < xMapSize * yMapSize) {
      if (region[y * length + x] == '#') {

        for (int didx = 0; didx < 3; ++didx) {
         
          for (int idx = 0; idx < 3; ++idx) {
            // top
            renderer->render(cube, glm::vec3(-30.0f + x * 12.0f + 4 * idx, 3.0f, -18.0f + y * 12.0f - didx * 4),
              glm::vec3(0.0f, 0.0f, 0.0f), "tileTexture");
            // bottom
            renderer->render(cube, glm::vec3(-30.0f + x * 12.0f + 4 * idx, -1.0f, -18.0f + y * 12.0f - didx * 4),
              glm::vec3(0.0f, 0.0f, 0.0f), "tileTexture");
          }
        }
      }
      else {
        renderer->renderRectangle("tileTexture", glm::vec3(-30.0f + x * 12.0f, 7.0f, 6.0f - (2 - y) * 12.0f),
          glm::vec3(-18.0f + x * 12.0f, 7.0f, -6.0f - (2 - y) * 12.0f), true);
        renderer->renderRectangle("tileTexture", glm::vec3(-30.0f + x * 12.0f, -1.0f, -6.0f - (2 - y) * 12.0f),
          glm::vec3(-18.0f + x * 12.0f, -1.0f, 6.0f - (2 - y) * 12.0f), true);
      }
      }
    }
  }
}

void Game::render() {
  renderEnv(2);
  
  std::string cameraPosStr = "x: ";
  /*cameraPosStr += floatToStr(renderer->cameraPosition.x);
  cameraPosStr += " z: ";
  cameraPosStr += floatToStr(renderer->cameraPosition.z);
  cameraPosStr += " coordX: " + intToStr(playerCoords.x);
  cameraPosStr += " coordY: " + intToStr(playerCoords.y);
  renderer->write(cameraPosStr, glm::vec3(1.0f, 1.0f, 1.0f),
                  glm::vec2(0.2f, -0.8f), glm::vec2(1.0f, -1.0f));
  */
  renderer->swapBuffers();
  
}
