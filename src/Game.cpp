/*
 * Game.cpp
 *
 * Created on: 12 Oct 2019
 *     Author: Dimitri Kourkoulis
 *    License: BSD 3-Clause License (see LICENSE file)
 */

#include "Game.hpp"
#include <small3d/Logger.hpp>

using namespace small3d;

Game::Game() {

  small3d::initLogger();

  renderer = &small3d::Renderer::getInstance("Groom", 800, 600, 1.0f,
    1.0f, 24.0f, -1.0f, "resources/shaders/", 240);

  map.load("resources/map.txt");

  cube = Model("resources/cube.obj");

  renderer->generateTexture("tileTexture", Image("resources/images/tile.png"));

  renderer->cameraPosition.y = 3.2f;
 // renderer->cameraPosition.x = 22.2f;
  renderer->cameraRotation.y = 1.7f;
 // renderer->cameraPosition.z = 4.0f;
}

GLFWwindow* Game::getWindow() {
  return renderer->getWindow();
}

void Game::init() {
}

void Game::terminate() {
}

void Game::process(const KeyInput &input) {
  
}

void Game::renderEnv(int radius) {

  const char* region = map.getRegion(2, 2, radius);

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
  renderer->swapBuffers();

}
