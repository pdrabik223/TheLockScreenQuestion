//
// Created by piotr on 24/10/2021.
//

#ifndef THELOCKSCREENQUESTION__LOCK_H_
#define THELOCKSCREENQUESTION__LOCK_H_

#include <coord/coord.h>
#include <vector>
class Lock {

public:
  using Line = std::pair<pm::Coord, pm::Coord>;
  using Blockade =  std::vector<Line>;

  Lock(const pm::Coord &shape);
  Lock(unsigned x,unsigned y);

  unsigned GetSize() const;
  pm::Coord GetShape() const;
  const std::vector<bool> &GetDots() const;
  const std::vector<Line> &GetLines() const;
  bool GetDotState(const pm::Coord& position) const {return dots_[Int(position)];}

  bool PushLine(const Lock::Line& new_gesture);
  void PushPin(unsigned s, unsigned f);
  void PushPin(unsigned f);
  void Clear(){
    lines_.clear();
    for (int i = 0; i < GetSize(); ++i) {
      dots_[i] = false;
    }
  };
private:
  int Int(const pm::Coord& position) const {return position.ToInt(shape_.x);}
protected:
  pm::Coord shape_;
  pm::Coord previous_dot_ = {-1,-1};
  std::vector<bool> dots_;
  Blockade lines_;
};

#endif // THELOCKSCREENQUESTION__LOCK_H_
