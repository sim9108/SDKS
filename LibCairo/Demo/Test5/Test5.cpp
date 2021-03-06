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
		
		auto image = create_surface("test.png");
		auto w = width(image);
		auto h = height(image);
		scale(cr, SCALE{ 256.0 / w, 256.0 / h });
		source(cr, image, POINT{ 0, 0 });
		paint(cr);
		
		write_to_png(surface, "image.png");
	}
	return 0;
}

