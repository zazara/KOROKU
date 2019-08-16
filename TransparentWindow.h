#include <cairomm/context.h>
#include <cmath>
#include <ctime>
#include <glibmm/main.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/window.h>

namespace KOROKU {

class TransparentWindow : public Gtk::Window {
public:
  TransparentWindow();
  virtual ~TransparentWindow();
};

TransparentWindow::TransparentWindow() {
  set_app_paintable(true);
  // Transparent Process
  Glib::RefPtr<Gdk::Visual> visual = get_screen()->get_rgba_visual();
  gtk_widget_set_visual(GTK_WIDGET(gobj()), visual->gobj());

  this->show_all();
}
TransparentWindow::~TransparentWindow() {}

} // namespace KOROKU