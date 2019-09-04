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
#include <cairomm/context.h>

#include <gtkmm.h>

namespace KOROKU {

class TransparentWindow : public Gtk::ApplicationWindow {
public:
  TransparentWindow();
  virtual ~TransparentWindow();
  TransparentWindow(BaseObjectType *obj,
                    Glib::RefPtr<Gtk::Builder> const &m_builder_)
      : Gtk::ApplicationWindow(obj) {
    this->set_default_size(800, 500);
    set_app_paintable(true);
    // Transparent Process
    Glib::RefPtr<Gdk::Visual> visual = get_screen()->get_rgba_visual();
    gtk_widget_set_visual(GTK_WIDGET(gobj()), visual->gobj());

    this->show_all();
  };

private:
}; // namespace KOROKU

TransparentWindow::TransparentWindow() {
  this->set_default_size(800, 500);
  set_app_paintable(true);
  // Transparent Process
  Glib::RefPtr<Gdk::Visual> visual = get_screen()->get_rgba_visual();
  gtk_widget_set_visual(GTK_WIDGET(gobj()), visual->gobj());

  this->show_all();
}
TransparentWindow::~TransparentWindow() {}

} // namespace KOROKU