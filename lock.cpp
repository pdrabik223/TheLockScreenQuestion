//
// Created by piotr on 24/10/2021.
//

#include "lock.h"
Lock::Lock(const pm::Coord &shape) : shape_(shape) {
  dots_.reserve(GetSize());
  lines_.reserve(GetSize() - 1);

  for (int d = 0; d < GetSize(); ++d) {
    dots_[d] = false;
  }
}
unsigned Lock::GetSize() const { return shape_.x * shape_.y; }
const std::vector<bool> &Lock::GetDots() const { return dots_; }
const std::vector<Lock::Line> &Lock::GetLines() const { return lines_; }
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
      if (dy / dx == ddy / ddx) {
        if (sqrt(pow(ddx, 2)) + pow(ddy, 2) < sqrt(pow(dx, 2)) + pow(dy, 2))
          dots_[i] = true;
      }
    }
  }

  dots_[Int(new_gesture.first)] = true;
  dots_[Int(new_gesture.second)] = true;

  lines_.push_back(new_gesture);
}

Lock::Lock(unsigned int x, unsigned y) : shape_(x, y) {
  dots_.reserve(GetSize());
  lines_.reserve(GetSize() - 1);

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

  if (!pin_.empty())
    PushLine({{(int)pin_.back() % shape_.x, (int)pin_.back() / shape_.x},
              {(int)f % shape_.x, (int)f / shape_.x}});
  else
    dots_[f] = true;

  pin_.push_back(f);
}
void Lock::Clear() {
  lines_.clear();
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
  Pin empty_list;
  for (int i = 0; i < GetSize(); ++i) {
    if (!dots_[i])
      empty_list.push_back(i);
  }
  return empty_list;
}
float Lock::SecurityStatus() {
  float security_sum = 0;
  for (auto d : dots_)
    if (d)
      security_sum += 2;

  for (const auto &l : lines_) {
    security_sum +=
        sqrt(pow(l.first.x - l.second.x, 2) + pow(l.first.y - l.second.y, 2));
  }


  return security_sum;
}
