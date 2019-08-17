#include <gtkmm.h>

namespace KOROKU {
class BrushBox : public Gtk::Box {
private:
  Gtk::ColorButton primaryBrush, secondaryBrush;
  Gtk::Box primaryBox, secondaryBox;
  Gtk::RadioButton primaryButton, secondaryButton;

public:
  BrushBox();
  virtual ~BrushBox();
};

BrushBox::BrushBox() : Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 0) {
  this->primaryBox = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0);
  this->secondaryBox = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0);
  this->primaryBrush.set_color(Gdk::Color("black"));
  this->secondaryBrush.set_color(Gdk::Color("white"));

  Gtk::RadioButton::Group selector = primaryButton.get_group();
  secondaryButton.set_group(selector);
  this->primaryBox.pack_start(primaryBrush);
  this->primaryBox.pack_start(primaryButton, false, false);
  this->secondaryBox.pack_start(secondaryBrush);
  this->secondaryBox.pack_start(secondaryButton, false, false);
  this->pack_start(primaryBox);
  this->pack_start(secondaryBox);
}

BrushBox::~BrushBox() {}

} // namespace KOROKU
