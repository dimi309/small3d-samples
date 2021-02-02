/*
 *  KeyInput.hpp
 *
 *  Created on: 2014/09/23
 *      Author: Dimitri Kourkoulis
 *     License: BSD 3-Clause License (see LICENSE file)
 */
#pragma once

namespace ChaseTheGoat3D
{
  /**
   * @typedef	struct KeyInput
   *
   * @brief	Structure holding the state of keys on the keyboard that are related to the game.
   */

  typedef struct KeyInput
  {
    bool up, down, left, right, enter, space, esc;

    KeyInput()
    {
      up = false;
      down = false;
      left = false;
      right = false;
      enter = false;
      space = false;
      esc = false;
    }

  } KeyInput;

}
