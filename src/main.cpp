#include <fstream>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <exception>

int posX, posY;

std::vector<std::string> map;

void render() {

  for (int y = 0; y < 3; ++y) {
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
}

int main(int argc, char** argv) {
  std::ifstream mapfile("map.txt");
  if (mapfile.is_open()) {
    
    std::string line;

    while(std::getline(mapfile, line)) {
      std::cout << line << " " << line.size() << std::endl;
      map.push_back(line);
    }

    
  }
  else {
    throw std::runtime_error("Could not open map file.");
  }

  if (mapfile.is_open()) mapfile.close();

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
  return 0;
}
