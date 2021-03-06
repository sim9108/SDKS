// Test1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "cairo_util.h"

int main() {
	{
		TimeCheck time;
		auto surface = create_surface(CAIRO_FORMAT_ARGB32, SURFACE_SIZE{ 256, 256 });
		auto image = create_surface("test.png");
		auto w = width(image);
		auto h = height(image);
		
		auto cr = create(surface);		
		translate(cr, POINT{ 128.0, 128.0 });
		rotate(cr, DEGRESS{ 45 });
		scale(cr, SCALE{ 256.0 / w, 256.0 / h });
		translate(cr, POINT{ -0.5*w, -0.5*h });
		source(cr, image, POINT{ 0, 0 });
		paint(cr);

		write_to_png(surface, "image.png");
	}
	return 0;
}

