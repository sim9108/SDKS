// Test1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "cairo_util.h"

int main() {
	{
		TimeCheck time;

		auto surface = create_surface(CAIRO_FORMAT_ARGB32, SURFACE_SIZE{ 256, 256 });
		auto cr = create(surface);

		arc(cr, POINT{ 128.0, 128.0 }, 76.8, DEGRESS{ 0 }, DEGRESS{ 360 });
		clip(cr);

		new_path(cr);  /* current path is not  consumed by cairo_clip() */
		rectangle(cr, POINT{ 0, 0 }, SIZE{ 255, 255 });
		fill(cr);
		
		source(cr, RGB{ 0, 1, 0 });
		move_to(cr, POINT{ 0, 0 });
		line_to(cr, POINT{ 256, 256 });
		move_to(cr, POINT{ 256, 0 });
		line_to(cr, POINT{ 0, 256 });
		line_width(cr, 10.0);
		stroke(cr);
		
		write_to_png(surface, "image.png");
	}
	return 0;
}

