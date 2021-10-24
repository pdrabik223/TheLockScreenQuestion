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
    letter = getchar();
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

      if (s_val == -1) {
        f_val = -1;
        s_val = value;
        break;
      } else {
        f_val = value;
        test.PushPin(s_val, f_val);
        s_val = f_val;
        screen.PushFrame(test);
      }
      break;
    }
  }
fin:
  return 0;
}