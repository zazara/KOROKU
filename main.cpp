#include "Canvas.h"
#include "TransparentWindow.h"
#include <gtkmm/application.h>

int main(int argc, char **argv) {
  auto app = Gtk::Application::create(argc, argv);

  KOROKU::TransparentWindow win;

  win.show_all();

  return app->run(win);
}