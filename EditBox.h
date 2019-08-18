#include <gtkmm.h>
namespace KOROKU {
class EditBox : public Gtk::Box {
public:
  EditBox();
  Gtk::Button undo;
  Gtk::Button redo;
  void onUndoClicked();
  void onRedoClicked();
};

EditBox::EditBox() : Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 0) {
  this->undo = Gtk::Button("Undo");
  this->redo = Gtk::Button("Redo");
  // this->undo.signal_clicked().connect(sigc::mem_fun(*this, &Canvas::undo));
  this->pack_start(undo);
  this->pack_start(redo);
}

} // namespace KOROKU
