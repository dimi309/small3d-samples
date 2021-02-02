/*
 *  GameLogic.hpp
 *
 *  Created on: 2014/11/09
 *      Author: Dimitri Kourkoulis
 *     License: BSD 3-Clause License (see LICENSE file)
 */

#pragma once

#include <memory>
#include <small3d/Logger.hpp>
#include <small3d/SceneObject.hpp>
#include <small3d/Renderer.hpp>
#include <small3d/Sound.hpp>

#include "KeyInput.hpp"

using namespace small3d;

namespace AvoidTheBug3D {

  /**
   * @class	GameLogic
   *
   * @brief	The main body of the sample game.
   *
   */

  class GameLogic {

  private:

    SceneObject goat, bug, tree;
    
    Sound bahSound;

    enum GameState {
      START_SCREEN, PLAYING
    };
    GameState gameState;

    enum BugState {
      FLYING_STRAIGHT, TURNING, DIVING_DOWN, DIVING_UP
    };
    BugState bugState, bugPreviousState;
    int bugFramesInCurrentState;
    float bugVerticalSpeed;

    Model skyRect, groundRect, msgRect, startScreenRect;

    double startSeconds;

    int seconds;

    void initGame();

    void processGame(const KeyInput &keyInput);

    void processStartScreen(const KeyInput &keyInput);

    void moveGoat(const KeyInput &keyInput);

    void moveBug();

  public:

    Renderer *renderer;

    /**
     * Constructor
     */
    GameLogic();

    /**
     * Destructor
     */
    ~GameLogic();

    /**
     * Process conditions and set up the next frame, also taking into consideration
     * the input from the keyboard
     * 
     * @param	keyInput	The keyboard input
     */
    void process(const KeyInput &keyInput);

    /**
     * @fn	void GameLogic::render();
     *
     * @brief	Renders the current state of the game on the screen.
     *
     */

    void render();

    float lightModifier;
  };

} /* namespace AvoidTheBug3D */

