//
// Created by piotr on 23/10/2021.
//
#include "conio.h"
#include "sfml_window/window.h"
#include <string>

void DisplayPin(const std::vector<unsigned> &pin){
    printf("pin: ");
  for (auto item:pin) {
    printf("%d ",item);
  }
  printf("\n");
}

int main() {

  Window screen(800, 800);
  Lock test(3, 7);
  screen.PushFrame(View(test));

  while (1 < 2) {
    std::string command;
    std::getline(std::cin,command);
    switch (command[0]) {
    case 'q':
      goto fin;
    case 'c':
      test.Clear();
      screen.PushFrame(test);
      break;
    case 'p':
      DisplayPin(test.GetEmptyDots());
      break;
    default:

      int value = std::stoi(command);
      if (value > test.GetSize())
        break;
      if (test.CheckInput(value))
        test.PushPin(value);
      else
        printf("incorrect input\n");
      screen.PushFrame(test);
      break;
    }
  }
fin:
  return 0;
}

