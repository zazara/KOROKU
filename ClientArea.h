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
  void undo();
  void redo();
};

ClientArea::ClientArea() {
  this->button = Gtk::Button("Test");
  this->pack_start(canvas);
  this->pack_start(sideMenu, false, false);
  this->sideMenu.editBox.undo.signal_clicked().connect(
      sigc::mem_fun(*this, &ClientArea::undo));
  this->sideMenu.editBox.redo.signal_clicked().connect(
      sigc::mem_fun(*this, &ClientArea::redo));
}

void ClientArea::undo() { this->canvas.undo(); }
void ClientArea::redo() { this->canvas.redo(); }
} // namespace KOROKU
