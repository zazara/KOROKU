#include "BackgroundColorBox.h"
#include "BrushBox.h"
#include "EditBox.h"
#include <gtkmm.h>
namespace KOROKU {
class SideMenu : public Gtk::Box {
public:
  SideMenu();

private:
  Gtk::Scale hscale;
  Gtk::Frame editFrame, brushFrame, backgroundColorFrame;
  KOROKU::EditBox editBox;
  KOROKU::BrushBox brushBox;
  KOROKU::BackgroundColorBox backgroundColorBox;
};

SideMenu::SideMenu() : Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0) {
  Glib::RefPtr<Gtk::CssProvider> css_provider = Gtk::CssProvider::create();
  css_provider->load_from_data("box {background-image: image(white);}");
  this->get_style_context()->add_provider(css_provider,
                                          GTK_STYLE_PROVIDER_PRIORITY_USER);

  hscale.set_range(1, 5);
  hscale.set_increments(1, 1);
  hscale.set_value(2);
  editFrame.set_label("編集");
  editFrame.add(this->editBox);
  brushFrame.set_label("ブラシ");
  brushFrame.add(this->brushBox);
  backgroundColorFrame.set_label("背景");
  backgroundColorFrame.add(this->backgroundColorBox);
  this->pack_start(editFrame, false, false);
  this->pack_start(brushFrame, false, false);
  this->pack_start(backgroundColorFrame, false, false);
  this->pack_start(hscale, false, false);
}
} // namespace KOROKU
