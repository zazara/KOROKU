#include <gtkmm.h>

namespace KOROKU {
class BackgroundColorBox : public Gtk::Box {
private:
  Gtk::ColorButton backgroundColorButton;
  Gtk::HScale backgroundColorScale;

public:
  BackgroundColorBox();
  ~BackgroundColorBox();
};

BackgroundColorBox::BackgroundColorBox()
    : Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0) {
  this->backgroundColorButton.set_color(Gdk::Color("white"));
  this->backgroundColorScale.set_range(1, 5);
  this->backgroundColorScale.set_increments(1, 1);
  this->backgroundColorScale.set_value(2);
  this->pack_start(this->backgroundColorButton);
  this->pack_start(this->backgroundColorScale);
}

BackgroundColorBox::~BackgroundColorBox() {}

} // namespace KOROKU
