/*
 * Menu.hpp
 *
 *  Created on: 2018/11/10
 *      Author: Dimitri Kourkoulis
 *     License: BSD 3-Clause License (see LICENSE file)
 */

#include <string>
#include <small3d/Renderer.hpp>

using namespace small3d;

class Menu {
  Renderer *renderer;
  Model textRect1, textRect2, textRect3;
public:
  Menu(Renderer *renderer);
  void render();
};
