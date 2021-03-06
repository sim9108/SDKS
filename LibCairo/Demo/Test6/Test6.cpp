// Test1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "cairo_util.h"

int main() {
	do{
		/* a custom shape that could be wrapped in a function */
		double x0 = 25.6,   /* parameters like cairo_rectangle */
					y0 = 25.6,
					rect_width = 204.8,
					rect_height = 204.8,
					radius = 102.4;   /* and an approximate curvature radius */
		if (!rect_width || !rect_height) break;

		TimeCheck time;
		auto surface = create_surface(CAIRO_FORMAT_ARGB32, SURFACE_SIZE{ 256, 256 });
		auto cr = create(surface);

		double x1= x0 + rect_width;
		double y1 = y0 + rect_height;
		
		if (rect_width / 2 < radius) {
			if (rect_height / 2 < radius) {
				move_to(cr, POINT{ x0, (y0 + y1) / 2 });
				curve_to(cr, POINT{ x0, y0 }, POINT{ x0, y0 }, POINT{ (x0 + x1) / 2, y0 });
				curve_to(cr, POINT{ x1, y0 }, POINT{ x1, y0 }, POINT{ x1, (y0 + y1) / 2 });
				curve_to(cr, POINT{ x1, y1 }, POINT{ x1, y1 }, POINT{ (x1 + x0) / 2, y1 });
				curve_to(cr, POINT{ x0, y1 }, POINT{ x0, y1 }, POINT{ x0, (y0 + y1) / 2 });
			}
			else {
				move_to(cr, POINT{ x0, y0 + radius });
				curve_to(cr, POINT{ x0, y0 }, POINT{ x0, y0 }, POINT{ (x0 + x1) / 2, y0 });
				curve_to(cr, POINT{ x1, y0 }, POINT{ x1, y0 }, POINT{ x1, y0 + radius });
				line_to(cr, POINT{ x1, y1 - radius });
				curve_to(cr, POINT{ x1, y1 }, POINT{ x1, y1 }, POINT{ (x1 + x0) / 2, y1 });
				curve_to(cr, POINT{ x0, y1 }, POINT{ x0, y1 }, POINT{ x0, y1 - radius });
			}
		}
		else {
			if (rect_height / 2 < radius) {
				move_to(cr, POINT{ x0, (y0 + y1) / 2 });
				curve_to(cr, POINT{ x0, y0 }, POINT{ x0, y0 }, POINT{ x0 + radius, y0 });
				line_to(cr, POINT{ x1 - radius, y0 });
				curve_to(cr, POINT{ x1, y0 }, POINT{ x1, y0 }, POINT{ x1, (y0 + y1) / 2 });
				curve_to(cr, POINT{ x1, y1 }, POINT{ x1, y1 }, POINT{ x1 - radius, y1 });
				line_to(cr, POINT{ x0 + radius, y1 });
				curve_to(cr, POINT{ x0, y1 }, POINT{ x0, y1 }, POINT{ x0, (y0 + y1) / 2 });
			}
			else {
				move_to(cr, POINT{ x0, y0 + radius });
				curve_to(cr, POINT{ x0, y0 }, POINT{ x0, y0 }, POINT{ x0 + radius, y0 });
				line_to(cr, POINT{ x1 - radius, y0 });
				curve_to(cr, POINT{ x1, y0 }, POINT{ x1, y0 }, POINT{ x1, y0 + radius });
				line_to(cr, POINT{ x1, y1 - radius });
				curve_to(cr, POINT{ x1, y1 }, POINT{ x1, y1 }, POINT{ x1 - radius, y1 });
				line_to(cr, POINT{ x0 + radius, y1 });
				curve_to(cr, POINT{ x0, y1 }, POINT{ x0, y1 }, POINT{ x0, y1 - radius });
			}
		}
		close_path(cr);
		source(cr, RGB{ 0.5, 0.5, 1 });
		fill_preserve(cr);
		source(cr, RGBA{ 0.5, 0, 0, 0.5 });
		line_width(cr, 10.0);
		stroke(cr);
			   		
		write_to_png(surface, "image.png");
	} while (false);
	return 0;
}

