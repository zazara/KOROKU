#include <cairomm/context.h>
#include <cmath>
#include <ctime>
#include <glibmm/main.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/window.h>

class TransparentWindow : public Gtk::Window {
public:
  TransparentWindow();
  virtual ~TransparentWindow();

  // protected:
  //   bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr) override;
};

TransparentWindow::TransparentWindow() {
  set_app_paintable(true);
  // 透過
  Glib::RefPtr<Gdk::Visual> visual = get_screen()->get_rgba_visual();
  gtk_widget_set_visual(GTK_WIDGET(gobj()), visual->gobj());
  this->show_all();
}
TransparentWindow::~TransparentWindow() {}