#include "TransparentWindow.h"
#include <gtkmm/application.h>

int main(int argc, char **argv) {
  auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

  TransparentWindow win;
  win.show();

  return app->run(win);
}