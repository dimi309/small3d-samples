#include <fstream>
#include <iostream>
#include <stdio.h>

int position[2];

int posX, posY;

void render() {
  std::cout << "x: " << posX << " y: " <<  posY << std::endl;
}

int main(int argc, char** argv) {
  std::ifstream map("map.txt");
  if (map.is_open()) {
    std::cout << "Map is open." << std::endl;
    char line[100];

    while(map.getline(line, 100)) {
      std::cout << line;
      std::string linestr(line);
      std::cout << linestr.size() << std::endl;
    }
  }
  else {
    std::cout << "Map is not open." << std::endl;
  }

  if (map.is_open()) map.close();

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
