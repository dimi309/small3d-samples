/*
 * Game.cpp
 *
 * Created on: 12 Oct 2019
 *     Author: Dimitri Kourkoulis
 *    License: BSD 3-Clause License (see LICENSE file)
 */

#include "Game.cpp"
#include <fstream>
#include <vector>

Game::Game() {

  initLogger();
  
  std::ifstream mapfile("resources/map.txt");
  if (mapfile.is_open()) {
    std::string line;
    while(std::getline(mapfile, line)) {
      map.push_back(line);
    }
    LOGDEBUG("Loaded map.");
    mapfile.close();
  }
  else {
    throw std::runtime_error("Could not open map file.");
  }

}

Game::init() {
}

Game::terminate() {
}

Game::process() {
    /*if (mapfile.is_open()) mapfile.close();

  posX = 0;
  posY = 0;
  
  char c;
  while ((c = getchar()) != 27) {
    switch(c) {
    case 'q':
      posX--;
      break;
    case 'd':
      posX++;
      break;
    case 'z':
      ++posY;
      break;
    case 's':
      --posY;
      break;
    default:
      //do nothing
      break;
    }
    render();
  }
  */
}

Game::render() {
  /*
for (int y = -1; y < 2; ++y) {
    if (posY + y >= 0 && posY + y < map.size()) {
      for (int x = 0; x < 3; ++x) {
	if (posX + x >= 0 && posX + x < map[posY + y].size()) {
	  std::cout << map[posY + y][posX + x] ;
	}
      }
      std::cout << std::endl;
    }
  }
  
  std::cout << "x: " << posX << " y: " <<  posY << std::endl;
  */
}
