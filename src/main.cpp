/*
 * KeyInput.hpp
 *
 *  Created on: 12 Oct 2019
 *      Author: Dimitri Kourkoulis
 *     License: BSD 3-Clause License (see LICENSE file)
 */
#pragma once

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <exception>
#include <small3d/Renderer.hpp>

using namespace small3d;

bool downkey, leftkey, rightkey, upkey, esckey, enterkey;

void keyCallback(GLFWwindow* window, int key, int scancode, int action,
		 int mods) {
  if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    downkey = true;
  if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    upkey = true;
  if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    leftkey = true;
  if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    rightkey = true;
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    esckey = true;
  if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    enterkey = true;
  if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
    downkey = false;
  if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
    upkey = false;
  if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
    leftkey = false;
  if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
    rightkey = false;
  if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
    esckey = false;
  if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE)
    enterkey = false;
  
}

int posX, posY;

std::vector<std::string> map;

void render() {

  for (int y = -1; y < 2; ++y) {
    if (posY + y >= 0 && posY + y < map.size()) {
      for (int x = 0; x < 3; ++x) {
	if (posX + x >= 0 && posX + x < map[posY + y].size()) {
	  std::cout << map[posY + y][posX + x] ;
	}
      }
      std::cout << std::endl;
    }
  }
  
  std::cout << "x: " << posX << " y: " <<  posY << std::endl;
}


int main(int argc, char** argv) {

  Renderer *renderer = &Renderer::getInstance("Groom");

  GLFWwindow *window = renderer->getWindow();

  glfwSetKeyCallback(window, keyCallback);

  std::ifstream mapfile("resources/map.txt");
  if (mapfile.is_open()) {
    
    std::string line;

    while(std::getline(mapfile, line)) {
      std::cout << line << " " << line.size() << std::endl;
      map.push_back(line);
    }
  }
  else {
    throw std::runtime_error("Could not open map file.");
  }

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }
  
  
  

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
  return 0;
}
