/* KOROKU
 *
 * Copyright (C) 2019 zazara
 *
 * This file is part of KOROKU.
 *
 * KOROKU is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * KOROKU is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with KOROKU.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Canvas.h"
#include "TransparentWindow.h"
#include <iostream>

Glib::RefPtr<Gtk::Builder> refBuilder;

int main(int argc, char **argv) {
  auto app = Gtk::Application::create(argc, argv);

  try {
    // refBuilder = Gtk::Builder::create_from_file("src/glade_project.glade");
    refBuilder = Gtk::Builder::create_from_resource("/ui/glade_project.glade");
  } catch (const Glib::FileError &ex) {
    std::cerr << "FileError: " << ex.what() << std::endl;
    return 1;
  }

  KOROKU::TransparentWindow *m_main = nullptr;
  refBuilder->get_widget_derived("main_window", m_main);

  Gtk::Box *side_menu = nullptr;
  refBuilder->get_widget("side_menu", side_menu);
  Glib::RefPtr<Gtk::CssProvider> css_provider = Gtk::CssProvider::create();
  css_provider->load_from_resource("/ui/style.css");
  side_menu->get_style_context()->add_provider(
      css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);

  KOROKU::Canvas *canvas = nullptr;
  refBuilder->get_widget_derived("canvas", canvas);

  app->run(*m_main);
}