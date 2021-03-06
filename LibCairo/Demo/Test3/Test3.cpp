// Test1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "cairo_util.h"

int main() {
	{
		TimeCheck time;

		double xc = 128.0;
		double yc = 128.0;
		double radius = 100.0;
		DEGRESS angle1{ 45.0 };  /* angles are specified */
		DEGRESS angle2{ 180.0 };  /* in radians           */

		auto surface = create_surface(CAIRO_FORMAT_ARGB32, SURFACE_SIZE{ 256, 256 });
		auto cr = create(surface);		
		line_width(cr, 10.0);
		arc_negative(cr, POINT{ xc, yc }, radius, angle1, angle2);
		stroke(cr);
		
		// draw helping lines
		source(cr, RGBA{ 1, 0.2, 0.2, 0.6 });
		line_width(cr, 6.0);
		arc(cr, POINT{ xc, yc }, 10.0, DEGRESS{ 0 }, DEGRESS{ 360 });
		fill(cr);

		arc(cr, POINT{ xc, yc }, radius, angle1, angle1);
		line_to(cr, POINT{ xc, yc });
		arc(cr, POINT{ xc, yc },  radius, angle2, angle2);
		line_to(cr, POINT{ xc, yc });
		stroke(cr);

		write_to_png(surface, "image.png");
	}
	return 0;
}

