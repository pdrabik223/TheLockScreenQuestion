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
bool Lock::PushLine(const Lock::Line &new_gesture) {

  // todo boundaries check
  // todo passing thru check

  if (dots_[Int(new_gesture.first)])
    return false;
  if (dots_[Int(new_gesture.second)])
    return false;

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