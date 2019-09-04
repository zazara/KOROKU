/* KOROKU
 *
 * Copyright (C) 2019 zazara
 *
 * This file is part of KOROKU.
 *
 * KOROKU is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * KOROKU is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with KOROKU.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtkmm.h>
#include <map>
#include <vector>

namespace KOROKU {
typedef struct point {
  int x;
  int y;
} Point;

class Brush {

public:
  int width;
  double red;
  double green;
  double blue;
  double alpha;
  bool is_eraser;
  std::vector<Point> points;

  Brush(/* args */);
  Brush(double, double, double, double, double, bool);
  bool add_point(int, int);
  ~Brush();
};

Brush::Brush(/* args */) {
  this->width = 1;
  this->red = 0;
  this->green = 0;
  this->blue = 0;
  this->alpha = 1;
  this->is_eraser = false;
}

Brush::Brush(double width, double red, double green, double blue, double alpha,
             bool is_eraser) {
  this->width = width;
  this->red = red;
  this->green = green;
  this->blue = blue;
  this->alpha = alpha;
  this->is_eraser = is_eraser;
}
Brush::~Brush() {}

bool Brush::add_point(int x, int y) {
  this->points.push_back({x, y});
  return true;
}

} // namespace KOROKU
