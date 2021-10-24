//
// Created by piotr on 24/10/2021.
//
#include "view.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

void Dot::Draw(sf::RenderWindow &window, const sf::Vector2f &screen_placement) {

  sf::CircleShape circle(radius);
  circle.setPointCount(100);

  switch (state) {
  case State::FREE:
    circle.setFillColor(sf::Color::Blue);
    break;
  case State::OCCUPIED:
    circle.setFillColor(sf::Color::Green);
    break;
  }
  circle.setPosition(screen_placement.x, screen_placement.y);
  window.draw(circle);
}
const pm::Coord &Dot::GetPlacement() const { return placement; }

void Line::Draw(sf::RenderWindow &window) {

  float length = sqrt(pow(start.x - finish.x, 2) + pow(start.y - finish.y, 2));
  sf::RectangleShape line({length, width});
  line.setPosition({(float)start.x, (float)start.y});

  line.setFillColor(sf::Color::Blue);
  line.rotate(cos(length / abs(start.x - finish.x)));
  window.draw(line);
}
Line::Line(const Lock::Line &line) {
  start = line.first;
  finish = line.second;
}

View::View(const Lock &lock) : shape_(lock.GetShape()) {

  dots_.reserve(lock.GetSize());

  for (int y = 0; y < lock.GetShape().y; y++)
    for (int x = 0; x < lock.GetShape().x; x++)
      dots_.push_back({{x, y}, lock.GetDotState({x, y})});

  for (const auto &l : lock.GetLines())
    lines_.emplace_back(l);
}

void View::Draw(sf::RenderWindow &window) {
  window.clear({255, 255, 255});

  pm::Coord frame_shift = {(int)window.getSize().x / 20,
                           (int)window.getSize().y / 20};

  pm::Coord dot_shift = {
      (int)((window.getSize().x - (frame_shift.x * 2)) / shape_.x),
      (int)((window.getSize().y - (frame_shift.y * 2)) / shape_.y)};

  float dot_radius = window.getSize().x / 10;

  for (auto dot : dots_) {

    sf::Vector2f placement = {
        (float)frame_shift.x + dot.GetPlacement().x * dot_shift.x,
        (float)frame_shift.y + dot.GetPlacement().y * dot_shift.y};
    dot.Draw(window, placement);
  }

}