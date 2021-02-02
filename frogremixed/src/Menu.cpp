/*
 * Menu.cpp
 *
 *  Created on: 2018/11/10
 *      Author: Dimitri Kourkoulis
 *     License: BSD 3-Clause License (see LICENSE file)
 */

#include "Menu.hpp"

using namespace small3d;

static const std::string PRESS_ENTER_TEXTURE_NAME = "menu_press_enter";
static const std::string FROGGER_TEXTURE_NAME = "menu_frogger";
static const std::string NOTE_TEXTURE_NAME = "menu_note";

Menu::Menu(Renderer* renderer) {
  this->renderer = renderer;
  renderer->generateTexture(FROGGER_TEXTURE_NAME, "FROG REMIXED",
    glm::vec3(0.0f, 1.0f, 0.0f), 200);
  renderer->generateTexture(PRESS_ENTER_TEXTURE_NAME, "PRESS ENTER TO PLAY",
    glm::vec3(1.0f, 1.0f, 0.0f), 200);
  renderer->generateTexture(NOTE_TEXTURE_NAME, "BY DIMI309 FOR THE FALL 2018 GAMEDEV CHALLENGE",
    glm::vec3(1.0f, 1.0f, 0.0f), 80);

  renderer->createRectangle(textRect1, glm::vec3(-0.4f, 0.8f, -1.0f),
    glm::vec3(0.4f, 0.4f, -1.0f));
  renderer->createRectangle(textRect2, glm::vec3(-0.4f, 0.35f, -1.0f),
    glm::vec3(0.4f, 0.30f, -1.0f));
  renderer->createRectangle(textRect3, glm::vec3(-0.4f, 0.25f, -1.0f),
    glm::vec3(0.4f, -0.1f, -1.0f));

}

void Menu::render() {
  renderer->render(textRect1, FROGGER_TEXTURE_NAME, false);

  renderer->render(textRect2, NOTE_TEXTURE_NAME, false);

  renderer->render(textRect3, PRESS_ENTER_TEXTURE_NAME, false);

}
