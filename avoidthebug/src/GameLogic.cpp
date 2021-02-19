/*
 *  GameLogic.cpp
 *
 *  Created on: 2014/11/09
 *      Author: Dimitri Kourkoulis
 *     License: BSD 3-Clause License (see LICENSE file)
 */

#define MAX_Z -1.0f
#define MIN_Z -24.0f

#define GROUND_Y -1.0f
#define FULL_ROTATION 6.28f // More or less 360 degrees in radians

#define BUG_ROTATION_SPEED 0.12f
#define BUG_DIVE_TILT -0.8f
#define BUG_SPEED 0.08f
#define BUG_DIVE_DURATION 30
#define BUG_START_DIVE_DISTANCE 0.6f
#define BUG_FLIGHT_HEIGHT 1.4f

#define GOAT_ROTATION_SPEED 0.1f
#define GOAT_SPEED 0.05f

#define RESULT_TEXT_NAME "resutlText"

#include <memory>
#include <stdexcept>
#include <cmath>
#include "GameLogic.hpp"

using namespace small3d;

namespace AvoidTheBug3D {
  
  GameLogic::GameLogic() :
    goat("goat", "resources/models/goat.glb", "Cube", "Armature.001", "Armature.001Action", 2),
    bug("bug", "resources/models/bug.glb", "Cube"),
    tree("tree", "resources/models/tree.glb", "Cube", "", "", 2),
    bahSound("resources/sounds/bah.ogg"){
    
    renderer = &Renderer::getInstance("Avoid the Bug 3D", 854, 480);
    renderer->cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);

    renderer->createRectangle(startScreenRect, glm::vec3(-1.0f, 1.0f, 1.0f),
      glm::vec3(1.0f, -1.0f, 1.0f));
    renderer->createRectangle(msgRect,
      glm::vec3(-0.95f, -0.6f, -0.5f), glm::vec3(0.0f, -0.8f, -0.5f));
    renderer->createRectangle(skyRect, glm::vec3(-1.0f, 1.0f, 1.0f),
      glm::vec3(1.0f, -1.0f, 1.0f));
    renderer->createRectangle(groundRect, glm::vec3(-25.0f, GROUND_Y, MIN_Z),
      glm::vec3(25.0f, GROUND_Y, MAX_Z));

    Image goatTexture("resources/models/goat.png");
    renderer->generateTexture("goatTexture", goatTexture);

    Image treeTexture("resources/models/tree.png");
    renderer->generateTexture("treeTexture", treeTexture);
    
    Image startScreenTexture("resources/images/startScreen.png");
    renderer->generateTexture("startScreen", startScreenTexture);
    
    bugVerticalSpeed = BUG_FLIGHT_HEIGHT / BUG_DIVE_DURATION;
    
    tree.offset = glm::vec3(2.6f, GROUND_Y, -8.0f);
    tree.rotation = glm::vec3(0.0f, 0.5f, 0.0f);
    
    gameState = START_SCREEN;
    
    startSeconds = 0;
    seconds = 0;
    
    lightModifier = -0.01f;
  }

  GameLogic::~GameLogic() {
    renderer->clearBuffers(goat);
    renderer->clearBuffers(bug);
    renderer->clearBuffers(tree);
  }
  
  void GameLogic::initGame() {
    goat.offset = glm::vec3(-1.2f, GROUND_Y, -4.0f);
    bug.offset = glm::vec3(0.5f, GROUND_Y + BUG_FLIGHT_HEIGHT, -18.0f);
    
    bugState = FLYING_STRAIGHT;
    bugPreviousState = FLYING_STRAIGHT;
    bugFramesInCurrentState = 1;
    
    startSeconds = glfwGetTime();
  }
  
  void GameLogic::moveGoat(const KeyInput &keyInput) {
    goat.stopAnimating();
    
    if (keyInput.left) {
      goat.rotation.y += GOAT_ROTATION_SPEED;
      
      while (tree.containsCorners(goat)) {
        goat.rotation.y -= GOAT_ROTATION_SPEED;
      }
      
      if (goat.rotation.y > FULL_ROTATION)
        goat.rotation.y = 0.0f;
      goat.startAnimating();
      
    } else if (keyInput.right) {
      goat.rotation.y -= GOAT_ROTATION_SPEED;
      while (tree.containsCorners(goat)) {
        goat.rotation.y += GOAT_ROTATION_SPEED;
      }
      
      
      if (goat.rotation.y < -FULL_ROTATION)
        goat.rotation.y = 0.0f;
      goat.startAnimating();
    }
    
    if (keyInput.up) {
      
      goat.offset.x -= sin(goat.rotation.y) * GOAT_SPEED;
      goat.offset.z -= cos(goat.rotation.y) * GOAT_SPEED;
      
      while (tree.containsCorners(goat)) {
        goat.offset.x += sin(goat.rotation.y) * GOAT_SPEED;
        goat.offset.z += cos(goat.rotation.y) * GOAT_SPEED;
      }
      
      goat.startAnimating();
      
    } else if (keyInput.down) {
      goat.offset.x += sin(goat.rotation.y) * GOAT_SPEED;
      goat.offset.z += cos(goat.rotation.y) * GOAT_SPEED;
      
      while (tree.containsCorners(goat)) {
        goat.offset.x -= sin(goat.rotation.y) * GOAT_SPEED;
        goat.offset.z -= cos(goat.rotation.y) * GOAT_SPEED;
      }
      
      goat.startAnimating();
    }
    
    if (goat.offset.z < MIN_Z + 1.0f)
      goat.offset.z = MIN_Z + 1.0f;
    if (goat.offset.z > MAX_Z - 1.0f)
      goat.offset.z = MAX_Z - 1.0f;
    
    if (goat.offset.x < goat.offset.z)
      goat.offset.x = goat.offset.z;
    if (goat.offset.x > -(goat.offset.z))
      goat.offset.x = -(goat.offset.z);
    
    goat.animate();
    
  }
  
  void GameLogic::moveBug() {
    
    float xDistance = bug.offset.x - goat.offset.x;
    float zDistance = bug.offset.z - goat.offset.z;
    float distance = sqrt(xDistance * xDistance + zDistance * zDistance);
    
    float goatRelX = xDistance / distance;
    float goatRelZ = zDistance / distance;
    
    float bugDirectionX = sin(bug.rotation.y);
    float bugDirectionZ = cos(bug.rotation.y);
    
    float dotPosDir = goatRelX * bugDirectionX + goatRelZ * bugDirectionZ; // dot product
    
    // Bug state: decide
    if (bugState == bugPreviousState) {
      ++bugFramesInCurrentState;
    } else {
      bugFramesInCurrentState = 1;
    }
    
    bugPreviousState = bugState;
    
    if (bugState == DIVING_DOWN) {
      if (goat.contains(bug.offset)) {
        bahSound.play();
        seconds = static_cast<int>(glfwGetTime() - startSeconds);
        gameState = START_SCREEN;
        renderer->generateTexture(RESULT_TEXT_NAME, "Goat not bitten for " + std::to_string(seconds) + 
          " seconds", glm::vec3(0.5f, 1.0f, 0.0f));
      }
      
      if (bugFramesInCurrentState > BUG_DIVE_DURATION / 2) {
        bugState = DIVING_UP;
      }
    } else if (bugState == DIVING_UP) {
      if (goat.contains(bug.offset)) {
        bahSound.play();
        seconds = static_cast<int>(glfwGetTime() - startSeconds);
        gameState = START_SCREEN;
        renderer->generateTexture(RESULT_TEXT_NAME, "Goat not bitten for " + std::to_string(seconds) +
          " seconds", glm::vec3(0.5f, 1.0f, 0.0f));
      }
      
      if (bugFramesInCurrentState > BUG_DIVE_DURATION / 2) {
        bugState = FLYING_STRAIGHT;
        bug.offset.y = GROUND_Y + BUG_FLIGHT_HEIGHT; // Correction of possible rounding errors
      }
    } else {
      
      if (distance > BUG_START_DIVE_DISTANCE) {
        if (dotPosDir < 0.98f) {
          bugState = TURNING;
        } else {
          bugState = FLYING_STRAIGHT;
        }
      } else {
        bugState = DIVING_DOWN;
      }
      
    }
    
    // Bug state: represent
    
    bug.rotation.x = 0;
    
    if (bugState == TURNING) {
      
      bug.rotation.y += BUG_ROTATION_SPEED;
      
    } else if (bugState == DIVING_DOWN) {
      bug.rotation.x = BUG_DIVE_TILT;
      bug.offset.y -= bugVerticalSpeed;
    } else if (bugState == DIVING_UP) {
      bug.rotation.x = -BUG_DIVE_TILT;
      bug.offset.y += bugVerticalSpeed;
    }
    
    if (bug.rotation.y > FULL_ROTATION)
      bug.rotation.y = 0.0f;
    
    
    bug.offset.x -= sin(bug.rotation.y) * BUG_SPEED;
    bug.offset.z -= cos(bug.rotation.y) * BUG_SPEED;
    
    if (bug.offset.z < MIN_Z)
      bug.offset.z = MIN_Z;
    if (bug.offset.z > MAX_Z)
      bug.offset.z = MAX_Z;
    
    if (bug.offset.x < bug.offset.z)
      bug.offset.x = bug.offset.z;
    if (bug.offset.x > -(bug.offset.z))
      bug.offset.x = -(bug.offset.z);
    
  }
  
  void GameLogic::processGame(const KeyInput &keyInput) {
    moveBug();
    moveGoat(keyInput);
  }
  
  void GameLogic::processStartScreen(const KeyInput &keyInput) {
    if (keyInput.enter) {
      initGame();
      gameState = PLAYING;
    }
  }
  
  void GameLogic::process(const KeyInput &keyInput) {
    switch (gameState) {
    case START_SCREEN:
      processStartScreen(keyInput);
      break;
    case PLAYING:
      processGame(keyInput);
      break;
    default:
      throw std::runtime_error("Unrecognised game state");
      break;
    }
  }
  
  void GameLogic::render() {
    renderer->clearScreen(glm::vec4(0.4f, 0.37f, 1.0f, 1.0f));
    
    if (gameState == START_SCREEN) {
      
      renderer->render(startScreenRect, glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), "startScreen", false);
      
      if (seconds != 0) {
        
        renderer->render(msgRect, RESULT_TEXT_NAME, false);
        
      }
      
    } else {
      
      // Draw the ground
      renderer->render(groundRect, glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec4(0.0f, 0.5f, 0.0f, 1.0f));
      
      renderer->render(goat, "goatTexture");
      renderer->render(bug, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
      renderer->render(tree, "treeTexture");
      
    }
    renderer->swapBuffers();
  }
  
}
