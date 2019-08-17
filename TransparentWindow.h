#include "ClientArea.h"
#include <cairomm/context.h>
#include <cmath>
#include <gtkmm.h>

namespace KOROKU {

class TransparentWindow : public Gtk::Window {
public:
  TransparentWindow();
  virtual ~TransparentWindow();
  KOROKU::Canvas canvas;
  Gtk::HScale hscale;
  Gtk::Box hbox;
  Gtk::Box *vbox;
  Gtk::Button button;
  KOROKU::ClientArea clientArea;
};

TransparentWindow::TransparentWindow() {
  set_app_paintable(true);
  // Transparent Process
  Glib::RefPtr<Gdk::Visual> visual = get_screen()->get_rgba_visual();
  gtk_widget_set_visual(GTK_WIDGET(gobj()), visual->gobj());
  this->button = Gtk::Button("Test");

  this->hbox = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 0);
  // White Box
  vbox = new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0);
  // Paint Process
  Glib::RefPtr<Gtk::CssProvider> css_provider = Gtk::CssProvider::create();
  css_provider->load_from_data("box {background-image: image(white);}");
  vbox->get_style_context()->add_provider(css_provider,
                                          GTK_STYLE_PROVIDER_PRIORITY_USER);

  hscale.set_range(1, 5);
  hscale.set_increments(1, 1);
  hscale.set_value(2);
  hbox.set_homogeneous(false);
  hbox.pack_start(canvas);
  vbox->pack_start(hscale, false, false);
  vbox->pack_start(button, false, false);
  hbox.pack_start(*vbox, false, false);
  // this->add(hbox);
  this->add(clientArea);
  this->show_all();
}
TransparentWindow::~TransparentWindow() {}

} // namespace KOROKU