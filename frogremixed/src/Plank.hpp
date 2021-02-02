/*
 * Plank.hpp
 *
 *  Created on: 2018/11/10
 *      Author: Dimitri Kourkoulis
 *     License: BSD 3-Clause License (see LICENSE file)
 */

#include <small3d/SceneObject.hpp>
#include <string>
#include <small3d/Renderer.hpp>

using namespace small3d;

class Plank : public SceneObject {
private:
  glm::vec4 colour;
  Renderer *renderer = nullptr;
public:
  Plank();
  ~Plank();
  void render(Renderer *renderer);
  float speed = 0.0f;
  void move();
};
