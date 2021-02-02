/*
 * Car.hpp
 *
 *  Created on: 2018/11/10
 *      Author: Dimitri Kourkoulis
 *     License: BSD 3-Clause License (see LICENSE file)
 */

#include <small3d/SceneObject.hpp>
#include <small3d/Model.hpp>
#include <small3d/Renderer.hpp>

using namespace small3d;

class Car : public SceneObject {
private:
  Model wheel;
  glm::vec4 colour;
  Renderer *renderer = nullptr;
public:
  Car();
  ~Car();
  void render(Renderer *renderer);
  float speed = 0.0f;
  void move();
};
