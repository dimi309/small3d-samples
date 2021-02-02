/*
 *  main.cpp
 *
 *  Created on: 2014/11/09
 *      Author: Dimitri Kourkoulis
 *     License: BSD 3-Clause License (see LICENSE file)
 */

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "GameLogic.hpp"
#include "KeyInput.hpp"

#include <GLFW/glfw3.h>

using namespace std;
using namespace ChaseTheGoat3D;
using namespace small3d;

const uint32_t frameRate = 60;

KeyInput input;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        input.down = true;
    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        input.up = true;
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        input.left = true;
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        input.right = true;
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
        input.enter = true;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        input.esc = true;
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        input.space = true;    
    if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
        input.down = false;
    if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
        input.up = false;
    if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
        input.left = false;
    if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
        input.right = false;
    if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE)
        input.enter = false;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        input.esc = false;
    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
        input.space = false;
   
}


int main(int argc, char **argv) {

  try {
    initLogger();
    GameLogic gameLogic;

    // seconds, for setting the framerate
    double seconds = glfwGetTime();
    double prevSeconds = seconds;
    double secondsInterval = 1.0 / frameRate;

    GLFWwindow* window = gameLogic.renderer->getWindow();

    glfwSetKeyCallback(window, keyCallback);

    while (!glfwWindowShouldClose(window) && !input.esc) {

      glfwPollEvents();

      seconds = glfwGetTime();
      if (seconds - prevSeconds > secondsInterval) {
	gameLogic.process(input);
	prevSeconds = seconds;
	gameLogic.render();
      }
    }
  }
  catch (std::runtime_error &e) {
    LOGERROR(e.what());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
