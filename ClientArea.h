#include "SideMenu.h"
#include <gtkmm.h>
namespace KOROKU {
class ClientArea : public Gtk::Box {
public:
  ClientArea();
  KOROKU::Canvas canvas;
  Gtk::Box *vbox;
  Gtk::Button button;
  KOROKU::SideMenu sideMenu;
};

ClientArea::ClientArea() {
  this->button = Gtk::Button("Test");
  this->pack_start(canvas);
  this->pack_start(sideMenu, false, false);
}
} // namespace KOROKU
