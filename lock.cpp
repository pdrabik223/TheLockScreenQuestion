//
// Created by piotr on 24/10/2021.
//

#include "lock.h"
#include <clock/clock.h>
#include <map>

Lock::Lock(const pm::Coord &shape) : shape_(shape) {
  dots_.reserve(GetSize());


  for (int d = 0; d < GetSize(); ++d) {
    dots_[d] = false;
  }
}
unsigned Lock::GetSize() const { return shape_.x * shape_.y; }
const std::vector<bool> &Lock::GetDots() const { return dots_; }
std::vector<Lock::Line> Lock::GetLines() const {

  Blockade lines;
  if(pin_.size() <2 )return {};
  auto s = pin_[0];
  for (int p = 1; p < pin_.size(); ++p) {
    auto f = pin_[p];
    lines.push_back({{(int)s % shape_.x, (int)s / shape_.x},
                        {(int)f % shape_.x, (int)f / shape_.x}});
    s = f;
  }

  return lines;
}
pm::Coord Lock::GetShape() const { return shape_; }
void Lock::PushLine(const Lock::Line &new_gesture) {

  // for now,
  // gen dx, dy
  // for every non visited dot, check if fits inside this dx,dy
  // so connection (s,f) = s(0,0) , f(2,2) dx = abs(0-2); dy = abs(0-2)
  // than go true every non visited dot, if there is  connection (s,d) that dx'
  // = dx and dy'=dy if distance beetwen d and s is < than s and f mark that dot
  // as visited

  float dx = abs(new_gesture.first.x - new_gesture.second.x);
  float dy = abs(new_gesture.first.y - new_gesture.second.y);

  for (int i = 0; i < GetSize(); i++) {
    if (!dots_[i]) {

      float ddx = abs(new_gesture.first.x - (i % shape_.x));
      float ddy = abs(new_gesture.first.y - (i / shape_.x));

      if (dy * ddx == ddy * dx) {
        if (sqrt(pow(ddx, 2)) + pow(ddy, 2) < sqrt(pow(dx, 2)) + pow(dy, 2))
          dots_[i] = true;
      }
    }
  }

  dots_[Int(new_gesture.first)] = true;
  dots_[Int(new_gesture.second)] = true;


}

Lock::Lock(unsigned int x, unsigned y) : shape_(x, y) {
  dots_.reserve(GetSize());

  for (int d = 0; d < GetSize(); ++d) {
    dots_.emplace_back(false);
  }
}
void Lock::PushPin(unsigned int s, unsigned int f) {

  if (!pin_.empty()) {
    if (pin_.back() != s)
      throw std::invalid_argument("symbol must be continuous");
  }

  PushLine({{(int)s % shape_.x, (int)s / shape_.y},
            {(int)f % shape_.x, (int)f / shape_.x}});

  pin_.push_back(s);
  pin_.push_back(f);
}

void Lock::PushPin(unsigned int f) {
  AUTO_CLOCK;
  if (!pin_.empty())
    PushLine({{(int)pin_.back() % shape_.x, (int)pin_.back() / shape_.x},
              {(int)f % shape_.x, (int)f / shape_.x}});
  else
    dots_[f] = true;

  pin_.push_back(f);
}
void Lock::Clear() {

  for (int i = 0; i < GetSize(); ++i) {
    dots_[i] = false;
  }
  pin_.clear();
}
bool Lock::CheckInput(unsigned int input) {

  if (input < 0)
    return false;
  if (input >= GetSize())
    return false;
  return !dots_[input];
}
Lock::Pin Lock::GetEmptyDots() {
  AUTO_CLOCK;
  Pin empty_list;
  for (int i = 0; i < GetSize(); ++i) {

    if (!dots_[i])

      empty_list.push_back(i);
  }
  return empty_list;
}

double Lock::SecurityStatus() {
  double security_sum = 0;
  for (auto d : dots_)
    if (d)
      security_sum += 1;

  // lines are grouped based of their angle between the bottom of the screen
  std::map<double, int> line_group;

  for (const auto &l : GetLines()) {
    security_sum +=
        sqrt(pow(l.first.x - l.second.x, 2) + pow(l.first.y - l.second.y, 2)) /
        sqrt(GetSize());
    double delta_x = l.second.x - l.first.x;
    double delta_y = (l.second.y + l.first.y) * -1;
    double theta_radians = atan2(delta_y, delta_x);
    theta_radians = abs(theta_radians);

    if (line_group.find(theta_radians) == line_group.begin())
      line_group.emplace(theta_radians, 1);
    else
      line_group[theta_radians]++;
  }

  for (auto lg : line_group) {

    // that's -0.33 for every repetition
    if (lg.second == 1)
      lg.second--;
    //    if(lg.second == 0) // bad idea; solutions generated using this are
    //    shitty
    //      lg.second++;
    security_sum -= lg.second / sqrt(GetSize());
  }

  return security_sum;
}

std::vector<std::pair<Lock::Pin, double>> Lock::GenerateLocks() {
  AUTO_CLOCK;
  std::vector<std::pair<Pin, double>> output_vector;

  Pin possible;
  if (pin_.empty())
    possible = GetEmptyDots();
  else
    possible = GenPossibleMoves(pin_.back());

  for (auto move : possible) {
    Lock temp(*this);
    temp.PushPin(move);

    Lock::Pin temp_pin = temp.GetPin();

    if (temp_pin.size() >= 4) {
      output_vector.emplace_back(
          std::pair<Lock::Pin, double>(temp_pin, temp.SecurityStatus()));
    }
    for (const auto &pin : temp.GenerateLocks())
      output_vector.push_back(pin);
  }
  return output_vector;
}
const Lock::Pin &Lock::GetPin() const { return pin_; }

Lock::Pin Lock::GenPossibleMoves(unsigned position) {
  AUTO_CLOCK;
  Pin empty_dots = GetEmptyDots();

  int x = position % shape_.x;
  int y = position / shape_.x;

  for (int d = empty_dots.size() - 1; d >= 0; --d) {

    float dx = abs(x - (int)empty_dots[d] % shape_.x);
    float dy = abs(y - (int)empty_dots[d] / shape_.x);

    for (int i = 0; i < GetSize(); ++i) {
      if (!dots_[i]) {

        float ddx = abs(x - (i % shape_.x));
        float ddy = abs(y - (i / shape_.x));

        if (dy * ddx == ddy * dx) {
          if (sqrt(pow(ddx, 2)) + pow(ddy, 2) < sqrt(pow(dx, 2)) + pow(dy, 2))
            empty_dots.erase(empty_dots.begin() + d);
        }
      }
    }
  }

  return empty_dots;
}
