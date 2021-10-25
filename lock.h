//
// Created by piotr on 24/10/2021.
//

#ifndef THELOCKSCREENQUESTION__LOCK_H_
#define THELOCKSCREENQUESTION__LOCK_H_

#include <coord/coord.h>
#include <exception>
#include <vector>
class Lock {

public:
  using Line = std::pair<pm::Coord, pm::Coord>;
  using Blockade = std::vector<Line>;
  using Pin = std::vector<unsigned>;

  Lock(const pm::Coord &shape);
  Lock(unsigned x, unsigned y);

  unsigned GetSize() const;
  pm::Coord GetShape() const;
  const std::vector<bool> &GetDots() const;
  const std::vector<Line> &GetLines() const;
  bool GetDotState(const pm::Coord &position) const {
    return dots_[Int(position)];
  }

  void PushLine(const Lock::Line &new_gesture);
  void PushPin(unsigned s, unsigned f);
  void PushPin(unsigned f);
  void Clear() {
    lines_.clear();
    for (int i = 0; i < GetSize(); ++i) {
      dots_[i] = false;
    }
    pin_.clear();
  };

private:
  int Int(const pm::Coord &position) const { return position.ToInt(shape_.x); }

protected:
  pm::Coord shape_;
  std::vector<bool> dots_;
  Blockade lines_;
  Pin pin_;
};



#endif // THELOCKSCREENQUESTION__LOCK_H_
