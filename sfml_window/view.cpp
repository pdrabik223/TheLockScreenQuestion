//
// Created by piotr on 24/10/2021.
//
#include "view.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <iostream>

void Dot::Draw(sf::RenderWindow &window, const sf::Vector2f &screen_placement) {

  sf::CircleShape circle(radius);
  circle.setPointCount(10);

  switch (state) {
  case State::FREE:
    circle.setFillColor(sf::Color::Blue);
    break;
  case State::OCCUPIED:
    circle.setFillColor(sf::Color::Green);
    break;
  }
  circle.setPosition(screen_placement.x + radius, screen_placement.y + radius);
  circle.setScale(1, 1);
  window.draw(circle);
}
const pm::Coord &Dot::GetPlacement() const { return placement; }
void Dot::SetRadius(float radius) { Dot::radius = radius; }

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

  printf("window size x: %u\twindow size y: %u\n", window.getSize().x,
         window.getSize().y);

  float dot_radius = window.getSize().x < window.getSize().y
                         ? window.getSize().x / 80
                         : window.getSize().y / 80;

  sf::Vector2f frame_shift = {dot_radius * 10, dot_radius * 10};

  printf("frame shift x: %f\tframe shift: %f\n", frame_shift.x, frame_shift.y);

  sf::Vector2f dot_shift = {
      ((float)window.getSize().x  / (float)shape_.x),
      ((float)window.getSize().y  / (float)shape_.y)};

  printf("dot shift x: %f\tdot shift: %f\n", dot_shift.x, dot_shift.y);

  for (auto dot : dots_) {

    sf::Vector2f placement = {
        (float)frame_shift.x + (dot.GetPlacement().x * dot_shift.x),
        (float)frame_shift.y + (dot.GetPlacement().y * dot_shift.y)};

    dot.radius = dot_radius;

    dot.Draw(window, placement);
  }
}