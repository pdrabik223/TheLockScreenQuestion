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

/// best of  v1.0   {4,0,5,6,2,3,8,1,7}
/// best of  v1.5   {4,2,6,1,7,0,8,3,5}
// 14.259757
//  total no_of Patterns 389,112
//  I have 2,4k to much

/// todo 2. cmake "copy files automatically"
/// todo 3. "GenerateLocks()" is a bit slow, every single
/// todo 4. SecurityStatus() is not all-directional
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
      if (!test.GetPin().empty())
        DisplayPin(test.GenPossibleMoves(test.GetPin().back()));
      else
        printf("first enter first pin number");
      break;
    case 'l': {
      std::vector<std::pair<Lock::Pin, double>> locks ;

      for (int i = 0; i < 100; i++) {
        test.GenerateLocks();
        test.Clear();
      }
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
      DISPLAY_TIMINGS;
      DisplayInSequence(best_locks, screen, test);

    } break;
    case 'h':
      printf(" <q> to exit app\n <c> to clear screen\n <p> to display possible "
             "to connect dots\n <l> to calculate all possibilities\n <number> "
             "to connect to next dot manually\n");
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
