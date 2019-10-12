/*
 * main.cpp
 *
 * Created on: 12 Oct 2019
 *     Author: Dimitri Kourkoulis
 *    License: BSD 3-Clause License (see LICENSE file)
 */

#include "KeyInput.hpp"

using namespace small3d;

KeyInput input;

bool downkey, leftkey, rightkey, upkey, esckey, enterkey;

void keyCallback(GLFWwindow* window, int key, int scancode, int action,
		 int mods) {
  if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    input.downkey = true;
  if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    input.upkey = true;
  if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    input.leftkey = true;
  if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    input.rightkey = true;
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    input.esckey = true;
  if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    input.enterkey = true;
  if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
    input.downkey = false;
  if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
    input.upkey = false;
  if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
    input.leftkey = false;
  if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
    input.rightkey = false;
  if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
    input.esckey = false;
  if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE)
    input.enterkey = false;
  
}

int main(int argc, char** argv) {

  Renderer *renderer = &Renderer::getInstance("Groom");

  GLFWwindow *window = renderer->getWindow();

  glfwSetKeyCallback(window, keyCallback);
  
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    if (input.esckey) {
      break;
    }
  }  

  return 0;
}
