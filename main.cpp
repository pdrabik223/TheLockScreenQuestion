//
// Created by piotr on 23/10/2021.
//
#include "conio.h"
#include "sfml_window/window.h"
#include <string>

int main() {

  Window screen(800, 800);
  Lock test(3, 3);
  screen.PushFrame(View(test));

  int s_val = -1;
  int f_val = -1;

  while (1 < 2) {
    char letter;
    letter = getch();
    switch (letter) {
    case 'q':
      goto fin;
    case 'c':
      test.Clear();
      screen.PushFrame(test);
      break;
    default:
      int value = letter - 48;
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