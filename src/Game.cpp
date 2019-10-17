/*
 * Game.cpp
 *
 * Created on: 12 Oct 2019
 *     Author: Dimitri Kourkoulis
 *    License: BSD 3-Clause License (see LICENSE file)
 */

#include "Game.hpp"
#include <small3d/Logger.hpp>
#include <cmath>

#define FULL_ROTATION 6.28f // More or less 360 degrees in radians
#define CAMERA_ROTATION_SPEED 0.1f
#define CAMERA_SPEED 0.95f
#define ENEMY_SPEED 0.3f
#define TOUCH_DISTANCE 1.7f
#define SHOOT_DURATION 4

using namespace small3d;

Game::Game() {

  manRunning = new SceneObject("manRunning", "resources/anthropoid_run/anthropoid", 11, "", 0);
  manRunning->offset = glm::vec3(1.0f, -1.0f, -3.0f);
  manRunning->startAnimating();

  gun = new SceneObject("gun", "resources/gun.obj");

  small3d::initLogger();

  renderer = &small3d::Renderer::getInstance("Groom", 800, 600, 1.0f,
    1.0f, 60.0f, -1.0f, "resources/shaders/", 240);

  map.load("resources/map.txt");

  cube = Model("resources/cube.obj");

  renderer->generateTexture("tileTexture", Image("resources/images/tile.png"));

  renderer->cameraPosition.y = -0.1f;

  Enemy enemy;

  enemy.coords = glm::ivec2(5, 5);
  enemy.position = glm::vec2(1.0f, 1.0f);

  enemies.push_back(enemy);

  enemy.coords = glm::ivec2(5, 7);
  enemy.position = glm::vec2(1.0f, 1.0f);
  enemies.push_back(enemy);

  enemy.coords = glm::ivec2(5, 9);
  enemy.position = glm::vec2(1.0f, 1.0f);
  enemies.push_back(enemy);

  enemy.coords = glm::ivec2(10, 10);
  enemy.position = glm::vec2(1.0f, 1.0f);
  enemies.push_back(enemy);

  enemy.coords = glm::ivec2(8, 7);
  enemy.position = glm::vec2(1.0f, 1.0f);
  enemies.push_back(enemy);

  gunshot = Sound("resources/sounds/0438.ogg");

}

Game::~Game() {
  delete manRunning;
  delete gun;
}

GLFWwindow* Game::getWindow() {
  return renderer->getWindow();
}

void Game::init() {
  
  for (std::vector<Enemy>::iterator enemy = enemies.begin(); enemy != enemies.end(); ++enemy) {
    enemy->dead = false;
    enemy->position = glm::vec2(1.0f, 1.0f);
  }
  enemies[0].coords = glm::ivec2(5,5);
  enemies[1].coords = glm::ivec2(5,7);
  enemies[2].coords = glm::ivec2(5,9);
  enemies[3].coords = glm::ivec2(10,10);
  enemies[4].coords = glm::ivec2(8,7);
  inMenu = false;
  died = false;
  won = false;
  numDead = 0;
}

void Game::terminate() {
  inMenu = true;
}

void Game::process(const KeyInput& input) {
  
  if (!inMenu) {
    
    if (input.space) {
      shootCount = SHOOT_DURATION;
      gunshot.play();
    }
    else if (shootCount) {
      --shootCount;
    }
    
    if (input.left) {
      renderer->cameraRotation.y -= CAMERA_ROTATION_SPEED;
      
      if (renderer->cameraRotation.y < -FULL_ROTATION)
        renderer->cameraRotation.y = 0.0f;
      
    }
    else if (input.right) {
      renderer->cameraRotation.y += CAMERA_ROTATION_SPEED;
      
      if (renderer->cameraRotation.y > FULL_ROTATION)
        renderer->cameraRotation.y = 0.0f;
      
    }
    
    if (input.up) {
      
      renderer->cameraPosition.x += sin(renderer->cameraRotation.y) * CAMERA_SPEED;
      renderer->cameraPosition.z -= cos(renderer->cameraRotation.y) * CAMERA_SPEED;
      
    }
    else if (input.down) {
      renderer->cameraPosition.x -= sin(renderer->cameraRotation.y) * CAMERA_SPEED;
      renderer->cameraPosition.z += cos(renderer->cameraRotation.y) * CAMERA_SPEED;
    }
    
    if (renderer->cameraPosition.x < -4.0f) {
      
      if (playerCoords.x > 0 &&
          map.getLocation(playerCoords.x - 1, playerCoords.y) != '#') {
        --playerCoords.x;
        renderer->cameraPosition.x = 4.0f;
      }
      else {
        renderer->cameraPosition.x = -4.0f;
      }
    }
    
    if (renderer->cameraPosition.x > 4.0f) {
      if (playerCoords.x < xMapSize - 1 &&
          map.getLocation(playerCoords.x + 1, playerCoords.y) != '#') {
        ++playerCoords.x;
        renderer->cameraPosition.x = -4.0f;
      }
      else {
        renderer->cameraPosition.x = 4.0f;
      }
    }
    
    if (renderer->cameraPosition.z > 4.0f) {
      
      if (playerCoords.y < yMapSize - 1 &&
          map.getLocation(playerCoords.x, playerCoords.y + 1) != '#') {
        ++playerCoords.y;
        renderer->cameraPosition.z = -4.0f;
      }
      else {
        renderer->cameraPosition.z = 4.0f;
      }
    }
    
    if (renderer->cameraPosition.z < -4.0f) {
      
      if (playerCoords.y > 0 &&
          map.getLocation(playerCoords.x, playerCoords.y - 1) != '#') {
        --playerCoords.y;
        renderer->cameraPosition.z = 4.0f;
      }
      else {
        renderer->cameraPosition.z = -4.0f;
      }
    }
    
    gun->rotation = renderer->cameraRotation;
    gun->rotation.x += shootCount * 0.3f;
    gun->offset = renderer->cameraPosition;
    gun->offset.y -= 0.9f;
    gun->offset.x += sin(renderer->cameraRotation.y) * 1.3f;
    gun->offset.z -= cos(renderer->cameraRotation.y) * 1.3f;
    
    bool killedOne = false;
    
    
    for (std::vector<Enemy>::iterator enemy = enemies.begin(); enemy != enemies.end(); ++enemy) {
      int diffxcoords = playerCoords.x - enemy->coords.x;
      int diffycoords = playerCoords.y - enemy->coords.y;
      
      if (!enemy->dead) {
        
        if (abs(diffxcoords) > 0) {
          enemy->position.x = enemy->position.x +
          ENEMY_SPEED * abs(diffxcoords) / diffxcoords;
          if (enemy->position.x < -4.0f) {
            --enemy->coords.x;
            enemy->position.x = 4.0f;
          }
          if (enemy->position.x > 4.0f) {
            ++enemy->coords.x;
            enemy->position.x = -4.0f;
          }
        }
        else {
          float diffx = renderer->cameraPosition.x - enemy->position.x;
          if (abs(diffx) > TOUCH_DISTANCE) {
            enemy->position.x += ENEMY_SPEED * abs(diffx) / diffx;
            
          }
        }
        
        if (abs(diffycoords) > 0) {
          enemy->position.y = enemy->position.y +
          ENEMY_SPEED * abs(diffycoords) / diffycoords;
          if (enemy->position.y < -4.0f) {
            --enemy->coords.y;
            enemy->position.y = 4.0f;
          }
          if (enemy->position.y > 4.0f) {
            ++enemy->coords.y;
            enemy->position.y = -4.0f;
          }
        }
        else {
          float diffy = renderer->cameraPosition.z - enemy->position.y;
          if (abs(diffy) > TOUCH_DISTANCE) {
            enemy->position.y += ENEMY_SPEED * abs(diffy) / diffy;
            
          }
          else if (!enemy->dead) {
            died = true;
            terminate();
          }
        }
      }
      enemy->diffxcoords = playerCoords.x - enemy->coords.x;
      enemy->diffycoords = playerCoords.y - enemy->coords.y;
      
      enemy->inRange = pow(enemy->diffxcoords, 2) + pow(enemy->diffycoords, 2) < 2 * pow(localCoordRadius, 2);
      
      float distanceX = renderer->cameraPosition.x - enemy->position.x + enemy->diffxcoords * 8.0f;
      float distanceY = renderer->cameraPosition.z - enemy->position.y + enemy->diffycoords * 8.0f;
      enemy->dotp = -distanceY * sin(renderer->cameraRotation.y) - distanceX * cos(renderer->cameraRotation.y);
      
      if (abs(enemy->dotp) < 1.0f && shootCount == SHOOT_DURATION && !killedOne) {
        enemy->dead = true;
        killedOne = true;
        ++numDead;
        if (numDead == 6) {
          won = true;
          terminate();
        }
      }
    }
  }
  else {
    if (input.enter) {
      init();
    }
  }
}

void Game::renderEnv() {
  
  const char* region = map.getRegion(playerCoords.x, playerCoords.y, localCoordRadius);
  
  int length = 2 * localCoordRadius + 1;
  
  for (int y = 0; y < length; ++y) {
    for (int x = 0; x < length; ++x) {
      if (y * length + x >= 0 && y * length + x < xMapSize * yMapSize) {
        if (region[y * length + x] == '#') {
          
          for (int didx = 0; didx < 3; ++didx) {
            
            for (int idx = 0; idx < 3; ++idx) {
              // top
              // renderer->render(cube, glm::vec3(-30.0f + x * 12.0f + 4 * idx, 3.0f, -18.0f + y * 12.0f - didx * 4),
              //  glm::vec3(0.0f, 0.0f, 0.0f), "tileTexture");
              // bottom
              renderer->render(cube, glm::vec3(-30.0f + x * 12.0f + 4 * idx, -1.0f, -18.0f + y * 12.0f - didx * 4),
                               glm::vec3(0.0f, 0.0f, 0.0f), "tileTexture");
            }
          }
        }
        else {
          renderer->renderRectangle("tileTexture", glm::vec3(-30.0f + x * 12.0f, 3.0f, 6.0f - (2 - y) * 12.0f),
                                    glm::vec3(-18.0f + x * 12.0f, 3.0f, -6.0f - (2 - y) * 12.0f), true);
          renderer->renderRectangle("tileTexture", glm::vec3(-30.0f + x * 12.0f, -1.0f, -6.0f - (2 - y) * 12.0f),
                                    glm::vec3(-18.0f + x * 12.0f, -1.0f, 6.0f - (2 - y) * 12.0f), true);
        }
      }
    }
  }
}

void Game::render() {
  
  if (!inMenu) {
    
    renderEnv();
    renderer->render(*gun, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    
    for (std::vector<Enemy>::iterator enemy = enemies.begin(); enemy != enemies.end(); ++enemy) {
      //Enemy *enemy = &enemies[3];
      if (enemy->inRange) {
        manRunning->offset.x = -enemy->diffxcoords * 8.0f + enemy->position.x;
        manRunning->offset.z = -enemy->diffycoords * 8.0f + enemy->position.y;
        
        int ycoeff = 0;
        
        if (enemy->diffxcoords < 0 || (enemy->diffxcoords == 0 && renderer->cameraPosition.x - enemy->position.x < 0)) {
          manRunning->rotation.y = 1.7f;
          if (abs(enemy->diffxcoords) <= abs(enemy->diffycoords)) {
            ycoeff = -1;
          }
        }
        else {
          manRunning->rotation.y = -1.7f;
          if (abs(enemy->diffxcoords) <= abs(enemy->diffycoords)) {
            ycoeff = 1;
          }
        }
        
        if (enemy->diffycoords < 0 || (enemy->diffycoords == 0 && renderer->cameraPosition.z - enemy->position.y < 0)) {
          manRunning->rotation.y -= ycoeff * 0.85f;
        }
        else {
          manRunning->rotation.y += ycoeff * 0.85f;
        }
        
        if (enemy->dead) {
          manRunning->rotation.x = 1.75f;
          manRunning->rotation.y = 0.0f;
          manRunning->offset.y = -0.9f;
          manRunning->stopAnimating();
        }
        else {
          manRunning->rotation.x = 0.0f;
          manRunning->startAnimating();
          manRunning->animate();
        }
        renderer->render(*manRunning, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
      }
    }
    
    /*std::string cameraPosStr = "x: ";
     cameraPosStr += floatToStr(renderer->cameraPosition.x);
     cameraPosStr += " z: ";
     cameraPosStr += floatToStr(renderer->cameraPosition.z);
     cameraPosStr += " coordX: " + intToStr(playerCoords.x);
     cameraPosStr += " coordY: " + intToStr(playerCoords.y);
     cameraPosStr += " difx: " + intToStr(diffxcoords) + " dify: " + intToStr(diffycoords);
     
     renderer->write(cameraPosStr, glm::vec3(1.0f, 1.0f, 1.0f),
     glm::vec2(-0.2f, -0.6f), glm::vec2(1.0f, -0.8f));
     */
    
    //renderer->write(floatToStr(enemies[3].dotp), glm::vec3(1.0f, 1.0f, 1.0f),
    //  glm::vec2(-0.2f, -0.6f), glm::vec2(1.0f, -0.8f));
  }
  else {
    renderer->clearScreen(glm::vec4(0.5f, 0.0f, 0.0f, 1.0f));
    renderer->write("GLOOM!", glm::vec3(1.0f, 0.0f, 0.0f),
                    glm::vec2(-0.8f, 0.6f), glm::vec2(0.8f, 0.0f));
    
    if (won) {
      renderer->write("You won", glm::vec3(1.0f, 0.0f, 0.0f),
                      glm::vec2(-0.6f, -0.2f), glm::vec2(0.6f, -0.4f));
    }
  }
  renderer->swapBuffers();
  
}
