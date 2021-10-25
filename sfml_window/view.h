//
// Created by piotr on 24/10/2021.
//

#ifndef THELOCKSCREENQUESTION_SFML_WINDOW_VIEW_H_
#define THELOCKSCREENQUESTION_SFML_WINDOW_VIEW_H_
#include "../lock.h"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <coord/coord.h>

struct Dot {
  enum class State { FREE, OCCUPIED };

  Dot(const pm::Coord &placement, bool is_occupied) : placement(placement) {
    state = is_occupied ? State::OCCUPIED : State::FREE;
  };

  Dot(const Dot &other) = default;
  Dot &operator=(const Dot &other) = default;

  const pm::Coord &GetPlacement() const;
  void SetRadius(float radius);
  void Draw(sf::RenderWindow &window, const sf::Vector2f &screen_placement);

  pm::Coord placement;
  State state = State::FREE;
  float radius = 10;
};
struct Line {
  Line(const pm::Coord &start, const pm::Coord &finish)
      : start(start), finish(finish){};
  Line(const Lock::Line &line);
  Line(const Line &other) = default;
  Line &operator=(const Line &other) = default;

  void Draw(sf::RenderWindow &window, sf::Vector2f screen_start, sf::Vector2f screen_finish);
  const pm::Coord &GetStart() const;
  const pm::Coord &GetFinish() const;
  pm::Coord start;
  pm::Coord finish;

  float width = 10;
};

class View {
public:
  View() = default;
  View(const View &other) = default;
  View &operator=(const View &other) = default;
  View(const Lock &lock);
  void Draw(sf::RenderWindow &window);
  void DisplayLabel(int label, sf::Vector2f placement,float dot_radius ,bool is_occupied,
                    sf::RenderWindow &window);
protected:
  pm::Coord shape_;
  std::vector<Dot> dots_;
  std::vector<Line> lines_;
  static sf::Font font_;
};
#endif // THELOCKSCREENQUESTION_SFML_WINDOW_VIEW_H_
