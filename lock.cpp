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

  if (!pin_.empty()){
    if(pin_.back() != s)
    throw std::invalid_argument("symbol must be continuous");
  }

  PushLine({{(int)s % shape_.x, (int)s / shape_.y},
            {(int)f % shape_.x, (int)f / shape_.x}});

  pin_.push_back(s);
  pin_.push_back(f);
}

void Lock::PushPin(unsigned int f) {

  if(!pin_.empty()) {
    PushLine({{(int)pin_.back() % shape_.x, (int)pin_.back() / shape_.x},
              {(int)f % shape_.x, (int)f / shape_.x}});
  }else{
    dots_[f] = true;
  }
  pin_.push_back(f);
}
void Lock::Clear() {
  lines_.clear();
  for (int i = 0; i < GetSize(); ++i) {
    dots_[i] = false;
  }
  pin_.clear();
}
