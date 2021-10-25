//
// Created by piotr on 24/10/2021.
//
#include "view.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <cmath>
#include <iostream>
#define PI 3.14159265

sf::Font View::font_;
void Dot::Draw(sf::RenderWindow &window, const sf::Vector2f &screen_placement) {

  sf::CircleShape circle(radius);
  circle.setPointCount(30);

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

  sf::VertexArray lines(sf::LinesStrip, 2);
  lines[0].position = {screen_start.x , screen_start.y};
  lines[0].color = sf::Color::Cyan;
  lines[1].position = {screen_finish.x, screen_finish.y};
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

  if (!font_.loadFromFile("../sfml_window/Georama-Medium.ttf"))
    throw "bad file";
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

  dot_radius = (window.getSize().x / shape_.x < window.getSize().y / shape_.y
                    ? window.getSize().x / shape_.x
                    : window.getSize().y / shape_.y);
  dot_radius /= 20;

  sf::Vector2f dot_shift = {((float)window.getSize().x / (float)shape_.x),
                            ((float)window.getSize().y / (float)shape_.y)};

  sf::Vector2f frame_shift = {dot_shift.x / 2 - dot_radius * 2,
                              dot_shift.y / 2 -  dot_radius * 2};

  for (auto line : lines_) {

    sf::Vector2f start_placement = {
        (float)frame_shift.x + (line.GetStart().x * dot_shift.x) + dot_radius*2,
        (float)frame_shift.y + (line.GetStart().y * dot_shift.y) + dot_radius*2};

    sf::Vector2f finish_placement = {
        (float)frame_shift.x + (line.GetFinish().x * dot_shift.x)+ dot_radius*2,
        (float)frame_shift.y + (line.GetFinish().y * dot_shift.y)+ dot_radius*2};

    line.Draw(window, start_placement, finish_placement);
  }
  int i = 0;
  for (auto dot : dots_) {

    sf::Vector2f placement = {
        (float)frame_shift.x + (dot.GetPlacement().x * dot_shift.x) +
            dot_radius,
        (float)frame_shift.y + (dot.GetPlacement().y * dot_shift.y) +
            dot_radius};

    dot.radius = dot_radius;

    dot.Draw(window, placement);
    DisplayLabel(i++, placement, dot_radius, dot.state == Dot::State::OCCUPIED,
                 window);
  }
}
void View::DisplayLabel(int label, sf::Vector2f placement, float dot_radius,
                        bool is_occupied, sf::RenderWindow &window) {
  std::string raw_text = std::to_string(label);
  sf::Text text(raw_text, font_);

  text.setPosition(placement.x + (dot_radius * 3),
                   placement.y - (dot_radius * 3));
  text.setCharacterSize(dot_radius * 3);
  //  text.setStyle(sf::Text::Bold);

  if (is_occupied)
    text.setFillColor(sf::Color::Cyan);
  else
    text.setFillColor(sf::Color::White);
  window.draw(text);
}
