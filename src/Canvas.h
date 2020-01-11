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
#define _USE_MATH_DEFINES
#include "Brush.h"
#include <bits/stdc++.h>
#include <cairomm/context.h>
#include <cmath>
#include <gtkmm.h>
#include <vector>
extern Glib::RefPtr<Gtk::Builder> refBuilder; // glade ref
namespace KOROKU {

enum BrushSelect {
  ERASER_BRUSH = 0,
  PRIMARY_BRUSH = 1,
  SECONDARY_BRUSH = 2,
};

// クラス
class Canvas : public Gtk::DrawingArea {
public:
  Canvas();
  virtual ~Canvas();
  Canvas(BaseObjectType *obj, const Glib::RefPtr<Gtk::Builder> glade_xml);

protected:
  bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr) override;

private:
  KOROKU::Brush primary_brush, secondary_brush, eraser_brush;
  virtual bool on_mouse_move(GdkEventMotion *event);
  virtual bool on_mouse_press(GdkEventButton *event);
  void set_primary_color();
  void set_secondary_color();
  void set_layer_select();
  bool set_brush_width(Gtk::ScrollType, double);
  bool set_brush_opacity(Gtk::ScrollType, double);
  void undo();
  void redo();
  void reset_redo_brushes();
  bool set_background_opacity(Gtk::ScrollType, double);
  void clear_layer();
  void erase_all();
  void show_save_dialog();
  void show_about();
  void quit_about_dialog();

  int brush_select;
  ELayer layer_select;
  int mouse_x;
  int mouse_y;
  double background_opacity;
  bool is_eraser_mode;

  std::vector<Brush> brushes;      // Process
  std::vector<Brush> redo_brushes; // redoProcess

  // UI Parts
  Gtk::ColorButton *primary_color_button;
  Gtk::ColorButton *secondary_color_button;
  Gtk::RadioButton *primary_select_button;
  Gtk::RadioButton *secondary_select_button;
  Gtk::CheckButton *primary_layer_select_button;
  Gtk::CheckButton *secondary_layer_select_button;
  Gtk::CheckButton *eraser_check_button;
  Gtk::Scale *brush_width_scale;
  Gtk::Scale *brush_opacity_scale;
  Gtk::Button *undo_button;
  Gtk::Button *redo_button;
  Gtk::Scale *background_opacity_scale;
  Gtk::Button *erase_all_button;
  Gtk::Button *clear_layer_button;
  Gtk::Button *save_button;
  Gtk::FileChooserDialog *file_chooser_dialog;
  Gtk::Button *confirm_button;
  Gtk::Button *cancel_button;
  Gtk::Button *about_button;
  Gtk::AboutDialog *about_dialog;
};

Canvas::Canvas() {
  add_events(Gdk::BUTTON_PRESS_MASK | Gdk::POINTER_MOTION_MASK |
             Gdk::BUTTON_RELEASE_MASK);
}

Canvas::Canvas(BaseObjectType *obj, const Glib::RefPtr<Gtk::Builder> glade_xml)
    : Gtk::DrawingArea(obj) {

  // add events
  add_events(Gdk::BUTTON_PRESS_MASK | Gdk::POINTER_MOTION_MASK |
             Gdk::BUTTON_RELEASE_MASK);

  // mouse events
  signal_button_press_event().connect(
      sigc::mem_fun(*this, &Canvas::on_mouse_press));
  signal_motion_notify_event().connect(
      sigc::mem_fun(*this, &Canvas::on_mouse_move));

  // init
  brush_select = PRIMARY_BRUSH;
  layer_select = PRIMARY_LAYER;
  eraser_brush = KOROKU::Brush(10, 1.0, 1.0, 1.0, 0.5, true, PRIMARY_LAYER);
  secondary_brush = KOROKU::Brush(3, 0, 0, 1.0, 0.3, false, SECONDARY_LAYER);
  mouse_x = -50;
  mouse_y = -50;
  background_opacity = 0.5;
  is_eraser_mode = false;

  // color button
  refBuilder->get_widget("primary_color_button", primary_color_button);
  refBuilder->get_widget("secondary_color_button", secondary_color_button);
  primary_color_button->set_color(Gdk::Color("black"));
  secondary_color_button->set_color(Gdk::Color("blue"));
  primary_color_button->signal_color_set().connect(
      sigc::mem_fun(*this, &Canvas::set_primary_color));
  secondary_color_button->signal_color_set().connect(
      sigc::mem_fun(*this, &Canvas::set_secondary_color));

  // select button
  refBuilder->get_widget("primary_select_button", primary_select_button);
  refBuilder->get_widget("secondary_select_button", secondary_select_button);
  refBuilder->get_widget("eraser_check_button", eraser_check_button);
  primary_select_button->signal_clicked().connect(
      sigc::mem_fun(*this, &Canvas::set_layer_select));
  secondary_select_button->signal_clicked().connect(
      sigc::mem_fun(*this, &Canvas::set_layer_select));
  eraser_check_button->signal_clicked().connect(
      sigc::mem_fun(*this, &Canvas::set_layer_select));

  // select layer button
  refBuilder->get_widget("primary_layer_select_button",
                         primary_layer_select_button);
  refBuilder->get_widget("secondary_layer_select_button",
                         secondary_layer_select_button);
  primary_layer_select_button->signal_clicked().connect(
      sigc::mem_fun(*this, &Canvas::queue_draw));
  secondary_layer_select_button->signal_clicked().connect(
      sigc::mem_fun(*this, &Canvas::queue_draw));

  // brush width scale
  refBuilder->get_widget("brush_width_scale", brush_width_scale);
  brush_width_scale->signal_change_value().connect(
      sigc::mem_fun(*this, &Canvas::set_brush_width));

  // brush opacity scale
  refBuilder->get_widget("brush_opacity_scale", brush_opacity_scale);
  brush_opacity_scale->signal_change_value().connect(
      sigc::mem_fun(*this, &Canvas::set_brush_opacity));

  // redo & undo
  refBuilder->get_widget("undo_button", undo_button);
  refBuilder->get_widget("redo_button", redo_button);
  undo_button->signal_clicked().connect(sigc::mem_fun(*this, &Canvas::undo));
  redo_button->signal_clicked().connect(sigc::mem_fun(*this, &Canvas::redo));

  // background opacity scale
  refBuilder->get_widget("background_opacity_scale", background_opacity_scale);
  background_opacity_scale->signal_change_value().connect(
      sigc::mem_fun(*this, &Canvas::set_background_opacity));
  // clear layer button
  refBuilder->get_widget("clear_layer_button", clear_layer_button);
  clear_layer_button->signal_clicked().connect(
      sigc::mem_fun(*this, &Canvas::clear_layer));
  // erase all button
  refBuilder->get_widget("erase_all_button", erase_all_button);
  erase_all_button->signal_clicked().connect(
      sigc::mem_fun(*this, &Canvas::erase_all));

  // save button
  refBuilder->get_widget("save_button", save_button);
  save_button->signal_clicked().connect(
      sigc::mem_fun(*this, &Canvas::show_save_dialog));
  // file chooser dialog
  refBuilder->get_widget("file_chooser_dialog", file_chooser_dialog);

  // // canfirm & cancel button
  file_chooser_dialog->add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);
  file_chooser_dialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);

  // about button
  refBuilder->get_widget("about_button", about_button);
  refBuilder->get_widget("about_dialog", about_dialog);
  about_button->signal_clicked().connect(
      sigc::mem_fun(*this, &Canvas::show_about));
}

Canvas::~Canvas() {}

// draw
bool Canvas::on_draw(const Cairo::RefPtr<Cairo::Context> &cr) {
  cr->set_source_rgba(1.0, 1.0, 1.0, background_opacity);
  cr->paint();
  std::vector<Brush> primary_brushes;
  std::vector<Brush> secondary_brushes;

  // sort brush
  for (long unsigned brush_idx = 0; brush_idx < this->brushes.size();
       brush_idx++) {
    if (this->brushes[brush_idx].brush_layer == PRIMARY_LAYER) {
      primary_brushes.push_back(this->brushes[brush_idx]);
    } else {
      secondary_brushes.push_back(this->brushes[brush_idx]);
    }
  }

  // secondary draw
  if (secondary_layer_select_button->get_active()) {
    cr->push_group();
    for (long unsigned index = 0; index < secondary_brushes.size(); index++) {
      auto current_brush = secondary_brushes[index];
      if (current_brush.is_eraser) {
        cr->set_operator(Cairo::OPERATOR_SOURCE);
        cr->set_source_rgba(0, 0, 0, 0);
      } else {
        cr->set_operator(Cairo::OPERATOR_OVER);
        // set cairo property
        cr->set_source_rgba(current_brush.red, current_brush.green,
                            current_brush.blue, current_brush.alpha);
      }

      cr->set_line_width(current_brush.width);
      cr->set_line_cap(Cairo::LINE_CAP_ROUND);
      cr->set_line_join(Cairo::LINE_JOIN_ROUND);

      for (long unsigned point_index = 0;
           point_index < current_brush.points.size(); point_index++) {
        cr->line_to(current_brush.points[point_index].x,
                    current_brush.points[point_index].y);
      }
      cr->stroke();
    }
    cr->pop_group_to_source();
    cr->paint();
  }

  // primary draw
  if (primary_layer_select_button->get_active()) {
    cr->push_group();
    for (long unsigned index = 0; index < primary_brushes.size(); index++) {
      auto current_brush = primary_brushes[index];
      if (current_brush.is_eraser) {
        cr->set_operator(Cairo::OPERATOR_SOURCE);
        cr->set_source_rgba(0, 0, 0, 0);
      } else {
        cr->set_operator(Cairo::OPERATOR_OVER);
        // set cairo property
        cr->set_source_rgba(current_brush.red, current_brush.green,
                            current_brush.blue, current_brush.alpha);
      }

      cr->set_line_width(current_brush.width);
      cr->set_line_cap(Cairo::LINE_CAP_ROUND);
      cr->set_line_join(Cairo::LINE_JOIN_ROUND);

      for (long unsigned point_index = 0;
           point_index < current_brush.points.size(); point_index++) {
        cr->line_to(current_brush.points[point_index].x,
                    current_brush.points[point_index].y);
      }
      cr->stroke();
    }
    cr->pop_group_to_source();
    cr->paint();
  }

  // draw pointer circle
  cr->set_line_width(1);
  cr->set_source_rgba(0, 0, 0, 1);
  switch (brush_select) {
  case PRIMARY_BRUSH:
    cr->arc(mouse_x, mouse_y, primary_brush.width / 2, 0, 2 * M_PI);
    break;
  case SECONDARY_BRUSH:
    cr->arc(mouse_x, mouse_y, secondary_brush.width / 2, 0, 2 * M_PI);
    break;
  default:
    cr->arc(mouse_x, mouse_y, eraser_brush.width / 2, 0, 2 * M_PI);
    break;
  }

  cr->stroke();

  return true;
}

// マウス押下時
bool Canvas::on_mouse_press(GdkEventButton *event) {
  if (event->button == 1) {              // clear
    if (brush_select == PRIMARY_BRUSH) { // clear

      Brush brush = Brush(this->primary_brush.width, this->primary_brush.red,
                          this->primary_brush.green, this->primary_brush.blue,
                          this->primary_brush.alpha,
                          this->primary_brush.is_eraser, PRIMARY_LAYER);

      brush.add_point(event->x, event->y);
      this->brushes.push_back(brush);

    } else if (brush_select == SECONDARY_BRUSH) {
      Brush brush =
          Brush(this->secondary_brush.width, this->secondary_brush.red,
                this->secondary_brush.green, this->secondary_brush.blue,
                this->secondary_brush.alpha, this->secondary_brush.is_eraser,
                SECONDARY_LAYER);

      brush.add_point(event->x, event->y);
      this->brushes.push_back(brush);
    } else {
      Brush brush = Brush(this->eraser_brush.width, this->eraser_brush.red,
                          this->eraser_brush.green, this->eraser_brush.blue,
                          this->eraser_brush.alpha,
                          this->eraser_brush.is_eraser, layer_select);
      brush.add_point(event->x, event->y);
      this->brushes.push_back(brush);
    }

    this->reset_redo_brushes();
    queue_draw();
  }
  return true;
}

bool Canvas::on_mouse_move(GdkEventMotion *event) {
  this->mouse_x = event->x;
  this->mouse_y = event->y;
  if (event->state & Gdk::EventMask::BUTTON_PRESS_MASK) {
    Brush *current_brush = &this->brushes.back();
    current_brush->add_point(event->x, event->y);
  }
  queue_draw();
  return true;
}

void Canvas::set_primary_color() {
  primary_brush.red = primary_color_button->get_color().get_red() / 65535.0;
  primary_brush.green = primary_color_button->get_color().get_green() / 65535.0;
  primary_brush.blue = primary_color_button->get_color().get_blue() / 65535.0;
}

void Canvas::set_secondary_color() {
  secondary_brush.red = secondary_color_button->get_color().get_red() / 65535.0;
  secondary_brush.green =
      secondary_color_button->get_color().get_green() / 65535.0;
  secondary_brush.blue =
      secondary_color_button->get_color().get_blue() / 65535.0;
}

void Canvas::set_layer_select() {
  if (eraser_check_button->get_active()) {
    is_eraser_mode = true;
    brush_select = ERASER_BRUSH;
    brush_width_scale->set_value(eraser_brush.width);
    brush_opacity_scale->set_value(eraser_brush.alpha);
    brush_opacity_scale->set_sensitive(false);
  } else {
    is_eraser_mode = false;
  }

  if (primary_select_button->get_active()) {
    layer_select = PRIMARY_LAYER;
    if (!is_eraser_mode) {
      brush_select = PRIMARY_BRUSH;
      brush_width_scale->set_value(primary_brush.width);
      brush_opacity_scale->set_value(primary_brush.alpha);
      brush_opacity_scale->set_sensitive(true);
    }
  } else if (secondary_select_button->get_active()) {
    layer_select = SECONDARY_LAYER;
    if (!is_eraser_mode) {
      brush_select = SECONDARY_BRUSH;
      brush_width_scale->set_value(secondary_brush.width);
      brush_opacity_scale->set_value(secondary_brush.alpha);
      brush_opacity_scale->set_sensitive(true);
    }
  }
}

bool Canvas::set_brush_width(Gtk::ScrollType scroll, double value) {
  if (brush_select == PRIMARY_BRUSH) {
    primary_brush.width = value;
  } else if (brush_select == SECONDARY_BRUSH) {
    secondary_brush.width = value;
  } else {
    eraser_brush.width = value;
  }
  return true;
}

bool Canvas::set_brush_opacity(Gtk::ScrollType scroll, double value) {
  if (brush_select == PRIMARY_BRUSH) {
    primary_brush.alpha = value;
  } else if (brush_select == SECONDARY_BRUSH) {
    secondary_brush.alpha = value;
  } else {
    eraser_brush.alpha = value;
  }
  return true;
}

void Canvas::undo() {
  if (!this->brushes.empty()) {
    this->redo_brushes.push_back(this->brushes.back());
    this->brushes.pop_back();
    this->queue_draw();
  }
}

void Canvas::redo() {
  if (!this->redo_brushes.empty()) {
    this->brushes.push_back(this->redo_brushes.back());
    this->redo_brushes.pop_back();
    this->queue_draw();
  }
}

void Canvas::reset_redo_brushes() { this->redo_brushes.clear(); }

bool Canvas::set_background_opacity(Gtk::ScrollType scroll, double value) {
  background_opacity = value;
  this->queue_draw();
  return true;
}

void Canvas::clear_layer() {
  if (this->brushes.size() != 0) {
    long unsigned brush_idx = this->brushes.size() - 1;
    while (true) {
      if (this->brushes[brush_idx].brush_layer == this->layer_select) {
        this->brushes.erase(this->brushes.begin() + brush_idx);
      }
      if (brush_idx == 0) {
        break;
      }
      brush_idx--;
    }
    this->queue_draw();
  }
}

void Canvas::erase_all() {
  this->brushes.clear();
  this->reset_redo_brushes();
  this->queue_draw();
}

void Canvas::show_save_dialog() {
  auto save_result = this->file_chooser_dialog->run();
  this->file_chooser_dialog->hide();
  if (save_result == Gtk::RESPONSE_OK) {
    auto file_name = this->file_chooser_dialog->get_filename();
    auto export_surface = Cairo::ImageSurface::create(
        Cairo::FORMAT_ARGB32, this->get_width(), this->get_height());
    Cairo::RefPtr<Cairo::Context> export_context =
        Cairo::Context::create(export_surface);
    this->on_draw(export_context);
    export_surface->write_to_png(file_name);
  }
}

void Canvas::show_about() {
  this->about_dialog->run();
  this->about_dialog->hide();
}

void Canvas::quit_about_dialog() {}

} // namespace KOROKU
