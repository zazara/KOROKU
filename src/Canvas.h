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
#include <iostream>
#include <tuple>
#include <vector>

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

  int brush_select;
  int mouse_x;
  int mouse_y;
  std::vector<Brush> brushes;      // Process
  std::vector<Brush> redo_brushes; // redoProcess
};                                 // namespace KOROKU

// コンストラクタ
Canvas::Canvas() {
  add_events(Gdk::BUTTON_PRESS_MASK | Gdk::POINTER_MOTION_MASK |
             Gdk::BUTTON_RELEASE_MASK);
}

// コンストラクタ
Canvas::Canvas(BaseObjectType *obj, const Glib::RefPtr<Gtk::Builder> glade_xml)
    : Gtk::DrawingArea(obj) {
  add_events(Gdk::BUTTON_PRESS_MASK | Gdk::POINTER_MOTION_MASK |
             Gdk::BUTTON_RELEASE_MASK);
  signal_button_press_event().connect(
      sigc::mem_fun(*this, &Canvas::on_mouse_press));
  signal_motion_notify_event().connect(
      sigc::mem_fun(*this, &Canvas::on_mouse_move));
  brush_select = PRIMARY_BRUSH;
  eraser_brush.width = 10;
  mouse_x = -50;
  mouse_y = -50;
}

// デストラクタ
Canvas::~Canvas() {}

// 描画
bool Canvas::on_draw(const Cairo::RefPtr<Cairo::Context> &cr) {
  cr->set_source_rgba(1.0, 1.0, 1.0, 0.5);
  cr->paint();

  // draw all brush
  for (auto brush_itr = this->brushes.begin(); brush_itr != this->brushes.end();
       ++brush_itr) {
    // eraser or else
    if (brush_itr->is_eraser) {
      cr->set_operator(Cairo::OPERATOR_SOURCE);
    } else {
      cr->set_operator(Cairo::OPERATOR_OVER);
    }

    // set cairo property
    cr->set_source_rgba(brush_itr->red, brush_itr->green, brush_itr->blue,
                        brush_itr->alpha);
    cr->set_line_width(brush_itr->width);
    cr->set_line_cap(Cairo::LINE_CAP_ROUND);
    cr->set_line_join(Cairo::LINE_JOIN_ROUND);

    // draw current brush
    for (auto point_itr = brush_itr->points.begin();
         point_itr != brush_itr->points.end(); ++point_itr) {
      cr->line_to(point_itr->x, point_itr->y);
    }
    cr->stroke();
  }

  // draw pointer circle
  cr->set_line_width(1);
  cr->set_source_rgba(0, 0, 0, 1);
  cr->arc(mouse_x, mouse_y, primary_brush.width / 2, 0, 2 * M_PI); // 要修正
  cr->stroke();

  return true;
}

// マウス押下時
bool Canvas::on_mouse_press(GdkEventButton *event) {
  if (event->button == 1) {              // clear
    if (brush_select == PRIMARY_BRUSH) { // clear

      Brush brush =
          Brush(this->primary_brush.width, this->primary_brush.red,
                this->primary_brush.green, this->primary_brush.blue,
                this->primary_brush.alpha, this->primary_brush.is_eraser);

      brush.add_point(event->x, event->y);
      this->brushes.push_back(brush);

    } else if (brush_select == SECONDARY_BRUSH) {
      Brush brush =
          Brush(this->secondary_brush.width, this->secondary_brush.red,
                this->secondary_brush.green, this->secondary_brush.blue,
                this->secondary_brush.alpha, this->secondary_brush.is_eraser);

      brush.add_point(event->x, event->y);
      this->brushes.push_back(brush);
    } else {
      Brush brush =
          Brush(this->eraser_brush.width, this->eraser_brush.red,
                this->eraser_brush.green, this->eraser_brush.blue,
                this->eraser_brush.alpha, this->eraser_brush.is_eraser);
      brush.add_point(event->x, event->y);
      this->brushes.push_back(brush);
    }

    // this->reset_redo_brush();
    queue_draw();
  }
  return true;
}

bool Canvas::on_mouse_move(GdkEventMotion *event) {
  this->mouse_x = event->x;
  this->mouse_x = event->y;
  if (event->state & Gdk::EventMask::BUTTON_PRESS_MASK) {
    Brush *current_brush = &this->brushes.back();
    current_brush->add_point(event->x, event->y);
  }
  queue_draw();
  return true;
}

} // namespace KOROKU
