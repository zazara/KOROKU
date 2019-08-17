#include "Canvas.h"
#include "TransparentWindow.h"
#include <gtkmm/application.h>
// 静的なイベントハブクラスを作ろう
int main(int argc, char **argv) {
  auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

  KOROKU::TransparentWindow win;

  win.show_all();

  return app->run(win);
}