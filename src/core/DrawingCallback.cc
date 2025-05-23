/*
 *  OpenSCAD (www.openscad.org)
 *  Copyright (C) 2009-2011 Clifford Wolf <clifford@clifford.at> and
 *                          Marius Kintel <marius@kintel.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  As a special exception, you have permission to link this program
 *  with the CGAL library and distribute executables, as long as you
 *  follow the requirements of the GNU GPL in regard to all of the
 *  software in the executable aside from CGAL.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#include "core/DrawingCallback.h"

#include <memory>
#include <cmath>
#include <vector>

#include "geometry/Polygon2d.h"

DrawingCallback::DrawingCallback(unsigned long fn, double size) :
  pen(Vector2d(0, 0)), offset(Vector2d(0, 0)), advance(Vector2d(0, 0)), fn(fn), size(size)
{
}

DrawingCallback::~DrawingCallback()
{
}

void DrawingCallback::start_glyph()
{
  this->polygon = std::make_shared<Polygon2d>();
  // FIXME: Why do we think that a glyph is sanitized?
  // This is technically not true, since we don't maintain correct values for the 'positive' flag.
  this->polygon->setSanitized(true);
}

void DrawingCallback::finish_glyph()
{
  if (this->outline.vertices.size() > 0) {
    this->polygon->addOutline(this->outline);
    this->outline.vertices.clear();
  }
  if (this->polygon->outlines().size() == 0) {
    this->polygon = nullptr;
  }
  if (this->polygon) {
    this->polygons.push_back(this->polygon);
    this->polygon = nullptr;
  }
}

std::vector<std::shared_ptr<const Polygon2d>> DrawingCallback::get_result()
{
  return this->polygons;
}

void DrawingCallback::set_glyph_offset(double offset_x, double offset_y)
{
  offset = Vector2d(offset_x, offset_y);
}

void DrawingCallback::add_glyph_advance(double advance_x, double advance_y)
{
  advance += Vector2d(advance_x, advance_y);
}

void DrawingCallback::add_vertex(const Vector2d& v)
{
  this->outline.vertices.push_back(size * (v + offset + advance));
}

void DrawingCallback::move_to(const Vector2d& to)
{
  if (this->outline.vertices.size() > 0) {
    this->polygon->addOutline(this->outline);
    this->outline.vertices.clear();
  }
  add_vertex(to);
  pen = to;
}

void DrawingCallback::line_to(const Vector2d& to)
{
  add_vertex(to);
  pen = to;
}

// Quadric Bezier curve
void DrawingCallback::curve_to(const Vector2d& c1, const Vector2d& to)
{
  // NOTE - this could be done better using a chord length iteration (uniform in space) to implement $fa (lot of work, little gain)
  for (unsigned long idx = 1; idx <= fn; ++idx) {
    const double a = idx * (1.0 / (double)fn);
    add_vertex(pen * pow(1 - a, 2) +
               c1 * 2 * pow(1 - a, 1) * a +
               to * pow(a, 2));
  }
  pen = to;
}

// Cubic Bezier curve
void DrawingCallback::curve_to(const Vector2d& c1, const Vector2d& c2, const Vector2d& to)
{
  // NOTE - this could be done better using a chord length iteration (uniform in space) to implement $fa (lot of work, little gain)
  for (unsigned long idx = 1; idx <= fn; ++idx) {
    const double a = idx * (1.0 / (double)fn);
    add_vertex(pen * pow(1 - a, 3) +
               c1 * 3 * pow(1 - a, 2) * a +
               c2 * 3 * pow(1 - a, 1) * pow(a, 2) +
               to * pow(a, 3));
  }
  pen = to;
}
