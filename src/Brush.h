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
  double width;
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
