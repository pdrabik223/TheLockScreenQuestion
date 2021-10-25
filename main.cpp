//
// Created by piotr on 23/10/2021.
//
#include "conio.h"
#include "sfml_window/window.h"
#include <string>
#include <utility>
void DisplayInSequence(const std::vector<Lock::Pin> &best_locks, Window &screen,
                       Lock &test);
void DisplayPin(const std::vector<unsigned> &pin) {
  printf("pin: ");
  for (auto item : pin) {
    printf("%d ", item);
  }
  printf("\n");
}

/// best one so far   {4,0,5,6,2,3,8,1,7}

int main() {

  Window screen(800, 800);
  Lock test(3, 3);
  screen.PushFrame(View(test));

  while (1 < 2) {
    std::string command;
    std::getline(std::cin, command);
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
    case 'l': {
      std::vector<std::pair<Lock::Pin, double>> locks = test.GenerateLocks();
      double best_lock = 0;
      unsigned partial_sum = 0;

      for (const auto &pair : locks) {
        partial_sum++;
        best_lock = best_lock > pair.second ? best_lock : pair.second;
      }

      std::vector<Lock::Pin> best_locks;

      for (const auto &pair : locks)
        if (pair.second == best_lock)
          best_locks.push_back(pair.first);

      printf("\tno generated correct locks : %d\n", partial_sum);
      printf("\tbest security award : %lf\n", best_lock);
      printf("\tno locks with best security award : %d\n", best_locks.size());

      DisplayInSequence(best_locks, screen, test);

    } break;

    default:

      int value = std::stoi(command);
      if (value > test.GetSize())
        break;
      if (test.CheckInput(value))
        test.PushPin(value);
      else
        printf("incorrect input\n");
      screen.PushFrame(test);
      printf("\t current security status: %f\n", test.SecurityStatus());
      break;
    }
  }
fin:
  return 0;
}



void DisplayInSequence(const std::vector<Lock::Pin> &best_locks, Window &screen,
                       Lock &test) {
  for (auto pin : best_locks) {
    test.Clear();
    for (auto p : pin) {
      test.PushPin(p);
      screen.PushFrame(test);
      getch();
    }
    printf("\tpin:\n");
    for (auto p : pin) {
      printf("\t%d, ", p);
    }
    printf("\n");
  }
}
