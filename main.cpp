#include "Canvas.h"
#include "TransparentWindow.h"
#include <gtkmm/application.h>

int main(int argc, char **argv) {
  auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

  KOROKU::TransparentWindow win;
  KOROKU::Canvas canvas;
  win.add(canvas);
  win.show_all();

  return app->run(win);
}