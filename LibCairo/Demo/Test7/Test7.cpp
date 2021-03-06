// Test1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "cairo_util.h"

int main() {
	{
		TimeCheck time;

		double x = 25.6, y = 128.0;
		double x1 = 102.4, y1 = 230.4,
					x2 = 153.6, y2 = 25.6,
					x3 = 230.4, y3 = 128.0;
		
		auto surface = create_surface(CAIRO_FORMAT_ARGB32, SURFACE_SIZE{ 256, 256 });
		auto cr = create(surface);

		move_to(cr, POINT{ x,y });
		curve_to(cr, POINT{ x1,y1 }, POINT{ x2,y2 }, POINT{ x3,y3 });
		
		line_width(cr, 10.0);
		stroke(cr);

		source(cr, RGBA{ 1, 0.2,0.2, 0.6 });
		line_width(cr, 6.0);
		move_to(cr, POINT{ x,y }); line_to(cr, POINT{ x1,y1 });
		move_to(cr, POINT{ x2,y2 }); line_to(cr, POINT{ x3,y3 });
		stroke(cr);
			   		
		write_to_png(surface, "image.png");
	}
	return 0;
}

