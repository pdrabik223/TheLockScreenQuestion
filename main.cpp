//
// Created by piotr on 23/10/2021.
//
#include "sfml_window/window.h"
int main() {

  Window screen(800, 800);
  Lock test(3, 3);

  while (1 < 2) {
    screen.PushFrame(View(test));
    std::this_thread::sleep_for(std::chrono::milliseconds(32));

  }

  return 0;
}