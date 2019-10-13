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
#define CAMERA_SPEED 0.65f

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
  
  if (renderer->cameraPosition.x < 0.0f ) {
    
    if (playerCoords.x > 0) {
      --playerCoords.x;
      renderer->cameraPosition.x = 10.0f;
    }
    else {
      renderer->cameraPosition.x = 0.0f;
    }
  }
  
  if (renderer->cameraPosition.x > 10.0f ) {
    if (playerCoords.x < xMapSize) {
      ++playerCoords.x;
      renderer->cameraPosition.x = 0.0f;
    }
    else {
      renderer->cameraPosition.x = 10.0f;
    }
  }
  
  if (renderer->cameraPosition.z > 10.0f ) {
    
    if (playerCoords.y < yMapSize) {
      ++playerCoords.y;
      renderer->cameraPosition.z = 0.0f;
    }
    else {
      renderer->cameraPosition.z = 10.0f;
    }
  }
  
  if (renderer->cameraPosition.z < -10.0f ) {
    
    if (playerCoords.y > 0) {
      --playerCoords.y;
      renderer->cameraPosition.z = 0.0f;
    }
    else {
      renderer->cameraPosition.z = -10.0f;
    }
  }
}

void Game::renderEnv(int radius) {
  
  int regCenterX = playerCoords.x;
  int regCenterY = playerCoords.y;
  
  if (regCenterX < radius) regCenterX = radius;
  if (regCenterY < radius) regCenterY = radius;
  
  if (regCenterX > xMapSize - radius) regCenterX = xMapSize - radius;
  if (regCenterY > yMapSize - radius) regCenterY = yMapSize -radius;

  const char* region = map.getRegion(regCenterX, regCenterY, radius);

  int length = 2 * radius + 1;

  for (int y = 0; y < length; ++y) {
    for (int x = 0; x < length; ++x) {
      if (region[y * length + x] == '#') {

        for (int didx = 0; didx < 3; ++didx) {
         
          for (int idx = 0; idx < 3; ++idx) {
            // top
            renderer->render(cube, glm::vec3(-18.0f + x * 12.0f + 4 * idx, 3.0f, -10.0f + y * 12.0f - didx * 4),
              glm::vec3(0.0f, 0.0f, 0.0f), "tileTexture");
            // bottom
            renderer->render(cube, glm::vec3(-18.0f + x * 12.0f + 4 * idx, -1.0f, -10.0f + y * 12.0f - didx * 4),
              glm::vec3(0.0f, 0.0f, 0.0f), "tileTexture");
          }
        }
      }
      else {
        renderer->renderRectangle("tileTexture", glm::vec3(-18.0f + x * 12.0f, 7.0f, 14.0f - (2 - y) * 12.0f),
          glm::vec3(-6.0f + x * 12.0f, 7.0f, 2.0f - (2 - y) * 12.0f), true);
        renderer->renderRectangle("tileTexture", glm::vec3(-18.0f + x * 12.0f, -1.0f, 2.0f - (2 - y) * 12.0f),
          glm::vec3(-6.0f + x * 12.0f, -1.0f, 14.0f - (2 - y) * 12.0f), true);
      }
    }
  }
}

void Game::render() {
  renderEnv(2);
  
  std::string cameraPosStr = "x: ";
  cameraPosStr += floatToStr(renderer->cameraPosition.x);
  cameraPosStr += " z: ";
  cameraPosStr += floatToStr(renderer->cameraPosition.z);
  
  renderer->write(cameraPosStr, glm::vec3(1.0f, 1.0f, 1.0f),
                  glm::vec2(0.4f, -0.9f), glm::vec2(1.0f, -1.0f));
  
  renderer->swapBuffers();
  

}
