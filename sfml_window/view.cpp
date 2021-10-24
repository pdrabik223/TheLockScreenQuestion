//
// Created by piotr on 24/10/2021.
//
#include "view.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <cmath>
#include <iostream>
#define PI 3.14159265
void Dot::Draw(sf::RenderWindow &window, const sf::Vector2f &screen_placement) {

  sf::CircleShape circle(radius);
  circle.setPointCount(10);

  switch (state) {
  case State::FREE:
    circle.setFillColor(sf::Color::White);
    break;
  case State::OCCUPIED:
    circle.setFillColor(sf::Color::Cyan);
    break;
  }
  circle.setPosition(screen_placement.x, screen_placement.y);
  circle.setScale(1, 1);
  window.draw(circle);
}
const pm::Coord &Dot::GetPlacement() const { return placement; }
void Dot::SetRadius(float radius) { Dot::radius = radius; }

void Line::Draw(sf::RenderWindow &window, sf::Vector2f screen_start,
                sf::Vector2f screen_finish) {

  //  double length = sqrt(pow(screen_start.x - screen_finish.x, 2) +
  //                       pow(screen_start.y - screen_finish.y, 2));
  //
  //  sf::RectangleShape line({(float)length, width});
  //  line.setFillColor(sf::Color::Cyan);
  //
  //  line.setOrigin(width / 2, width / 2);
  //
  //  line.setPosition(
  //      {(float)screen_start.x + width, (float)screen_start.y + width});
  //
  //
  //
  //  double cos_angle = abs(screen_start.x - screen_finish.x) / length;
  //
  //  auto angle = sin(cos_angle) * (180 / PI);
  //  //  angle *= 180;
  //  //  angle /= PI;
  //
  //  line.rotate(angle);

  sf::VertexArray lines(sf::LinesStrip, 2);
  lines[0].position = {screen_start.x + width, screen_start.y + width};
  lines[0].color = sf::Color::Cyan;
  lines[1].position = {screen_finish.x + width, screen_finish.y + width};
  lines[1].color = sf::Color::Cyan;

  window.draw(lines);
}
Line::Line(const Lock::Line &line) {
  start = line.first;
  finish = line.second;
}
const pm::Coord &Line::GetStart() const { return start; }
const pm::Coord &Line::GetFinish() const { return finish; }

View::View(const Lock &lock) : shape_(lock.GetShape()) {

  dots_.reserve(lock.GetSize());

  for (int y = 0; y < lock.GetShape().y; y++)
    for (int x = 0; x < lock.GetShape().x; x++)
      dots_.push_back({{x, y}, lock.GetDotState({x, y})});

  for (const auto &l : lock.GetLines())
    lines_.emplace_back(l);
}

void View::Draw(sf::RenderWindow &window) {
  window.clear({40, 40, 40});

  float dot_radius;
  dot_radius = (window.getSize().x < window.getSize().y ? window.getSize().x
                                                        : window.getSize().y) /
               80;

  sf::Vector2f frame_shift = {dot_radius * 10, dot_radius * 10};

  sf::Vector2f dot_shift = {((float)window.getSize().x / (float)shape_.x),
                            ((float)window.getSize().y / (float)shape_.y)};

  for (auto line : lines_) {

    sf::Vector2f start_placement = {
        (float)frame_shift.x + (line.GetStart().x * dot_shift.x) + dot_radius,
        (float)frame_shift.y + (line.GetStart().y * dot_shift.y) + dot_radius};

    sf::Vector2f finish_placement = {
        (float)frame_shift.x + (line.GetFinish().x * dot_shift.x) + dot_radius,
        (float)frame_shift.y + (line.GetFinish().y * dot_shift.y) + dot_radius};

    line.Draw(window, start_placement, finish_placement);
  }

  for (auto dot : dots_) {

    sf::Vector2f placement = {
        (float)frame_shift.x + (dot.GetPlacement().x * dot_shift.x) +
            dot_radius,
        (float)frame_shift.y + (dot.GetPlacement().y * dot_shift.y) +
            dot_radius};

    dot.radius = dot_radius;

    dot.Draw(window, placement);
  }
}