//
// Created by piotr on 23/10/2021.
//
#include "sfml_window/window.h"
int main(){

  Window screen(800,800);
  Lock test(3,3);

  screen.PushFrame(View(test));




  return 0;
}