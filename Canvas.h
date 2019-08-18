#include <bits/stdc++.h>
#include <cairomm/context.h>
#include <glibmm/main.h>
#include <gtkmm/drawingarea.h>
#include <tuple>
#include <vector>

namespace KOROKU {

class Brush {
public:
  Brush(double, double, double, double, double);
  bool addPoint(double, double);
  virtual ~Brush();
  double width;
  double red;
  double green;
  double blue;
  double alpha;
  std::vector<std::tuple<double, double>> stroke;
};

Brush::Brush(double width, double red, double green, double blue,
             double alpha) {
  this->width = width;
  this->red = red;
  this->green = green;
  this->blue = blue;
  this->alpha = alpha;
}

// add point to Brush
bool Brush::addPoint(double x, double y) {
  this->stroke.push_back(std::make_tuple(x, y));
  return true;
}

Brush::~Brush() {}

class Canvas : public Gtk::DrawingArea {
public:
  Canvas();
  virtual ~Canvas();
  void undo();
  void redo();
  bool canUndo();
  bool canRedo();
  void resetRedoBrush();

protected:
  bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr) override;
  virtual bool on_mouse_move(GdkEventMotion *event);
  virtual bool on_mouse_press(GdkEventButton *event);

private:
  std::vector<Brush> Brushes;     // Process
  std::vector<Brush> redoBrushes; // redoProcess
};

Canvas::Canvas() {
  add_events(Gdk::BUTTON_PRESS_MASK | Gdk::POINTER_MOTION_MASK |
             Gdk::BUTTON_RELEASE_MASK);
  signal_button_press_event().connect(
      sigc::mem_fun(*this, &Canvas::on_mouse_press));
  signal_motion_notify_event().connect(
      sigc::mem_fun(*this, &Canvas::on_mouse_move));
}

Canvas::~Canvas() {}

bool Canvas::on_draw(const Cairo::RefPtr<Cairo::Context> &cr) {
  cr->set_source_rgba(1.0, 1.0, 1.0, 0.5);
  cr->paint();
  Brushes[0].red;
  for (auto BrushesItr = this->Brushes.begin();
       BrushesItr != this->Brushes.end(); ++BrushesItr) {
    cr->set_source_rgba(BrushesItr->red, BrushesItr->green, BrushesItr->blue,
                        BrushesItr->alpha);
    cr->set_line_width(BrushesItr->width);
    cr->set_line_cap(Cairo::LINE_CAP_ROUND);
    cr->set_line_join(Cairo::LINE_JOIN_ROUND);

    for (auto PointItr = BrushesItr->stroke.begin();
         PointItr != BrushesItr->stroke.end(); ++PointItr) {
      cr->line_to(std::get<0>(*PointItr), std::get<1>(*PointItr));
    }
    cr->stroke();
  }
  return true;
}

bool Canvas::on_mouse_move(GdkEventMotion *event) {
  if (event->state & Gdk::EventMask::BUTTON_PRESS_MASK) {
    Brush *CurrentBrush = &this->Brushes.back();
    CurrentBrush->addPoint(event->x, event->y);
    queue_draw();
  }
  return true;
}

bool Canvas::on_mouse_press(GdkEventButton *event) {
  if (event->button == 1) {
    Brush brush = Brush(1, 0, 0, 0, 1.0);
    brush.addPoint(event->x, event->y);
    this->Brushes.push_back(brush);
    this->resetRedoBrush();
    queue_draw();
  }
  return true;
}

void Canvas::undo() {
  if (this->canUndo()) {
    this->redoBrushes.push_back(this->Brushes.back());
    this->Brushes.pop_back();
    this->queue_draw();
  }
}
void Canvas::redo() {
  if (this->canRedo()) {
    this->Brushes.push_back(this->redoBrushes.back());
    this->redoBrushes.pop_back();
    this->queue_draw();
  }
}

bool Canvas::canUndo() { return !this->Brushes.empty(); }
bool Canvas::canRedo() { return !this->redoBrushes.empty(); }

void Canvas::resetRedoBrush() { this->redoBrushes.clear(); }

} // namespace KOROKU
