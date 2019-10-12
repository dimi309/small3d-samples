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

  renderer = &small3d::Renderer::getInstance("Groom", 800, 600);

  map.load("resources/map.txt");

  const char* test = map.getRegion(4, 1, 1);
  
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

void Game::process() {
    /*if (mapfile.is_open()) mapfile.close();

  posX = 0;
  posY = 0;
  
  char c;
  while ((c = getchar()) != 27) {
    switch(c) {
    case 'q':
      posX--;
      break;
    case 'd':
      posX++;
      break;
    case 'z':
      ++posY;
      break;
    case 's':
      --posY;
      break;
    default:
      //do nothing
      break;
    }
    render();
  }
  */
}

void Game::renderEnv() {

}

void Game::render() {
  renderer->renderRectangle("tileTexture", glm::vec3(-2.0f, -1.0f, -10.0f),
                              glm::vec3(2.0f, -1.0f, -6.0f), true);
  renderer->renderRectangle("tileTexture", glm::vec3(2.0f, -1.0f, -10.0f),
                              glm::vec3(6.0f, -1.0f, -6.0f), true);
  
  renderer->renderRectangle("tileTexture", glm::vec3(-2.0f, -1.0f, -6.0f),
                              glm::vec3(2.0f, -1.0f, -2.0f), true);
  renderer->renderRectangle("tileTexture", glm::vec3(2.0f, -1.0f, -6.0f),
                              glm::vec3(6.0f, -1.0f, -2.0f), true);

  renderer->renderRectangle("tileTexture", glm::vec3(-2.0f, 3.0f, -10.0f),
                              glm::vec3(2.0f, -1.0f, -10.0f), true);
  renderer->renderRectangle("tileTexture", glm::vec3(-2.0f, 7.0f, -10.0f),
                              glm::vec3(2.0f, 3.0f, -10.0f), true);

  renderer->renderRectangle("tileTexture", glm::vec3(-2.0f, 7.0f, -6.0f),
			    glm::vec3(2.0f, 7.0f, -10.0f), true);
  
  renderer->swapBuffers();
  
}
