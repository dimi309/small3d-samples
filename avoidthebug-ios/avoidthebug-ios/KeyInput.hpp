/*
 * KeyInput.hpp
 *
 *  Created on: 23 Sept 2014
 *      Author: Dimitri Kourkoulis
 *     License: BSD 3-Clause License (see LICENSE file)
 */
#pragma once

namespace AvoidTheBug3D {
  /**
   * @typedef	struct KeyInput
   *
   * @brief	Structure holding the state of keys on the keyboard that are related to the game.
   */

  typedef struct KeyInput {
    bool up, down, left, right, enter, esc;

    KeyInput() {
      up = false;
      down = false;
      left = false;
      right = false;
      enter = false;
      esc = false;
    }

  } KeyInput;

}
