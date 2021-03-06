// Test1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "cairo_util.h"

int main() {
	{
		TimeCheck time;
		auto surface = create_surface(CAIRO_FORMAT_ARGB32, SURFACE_SIZE{ 256, 256 });
		auto cr = create(surface);
		
		dash(cr, DASH{ 
			{ 
					50.0,  /* ink */
					10.0,  /* skip */
					10.0,  /* ink */
					10.0   /* skip*/
			}, 
			-50.0 });
		line_width(cr, 10.0);

		move_to(cr, POINT{ 128.0,25.6 });
		line_to(cr, POINT{ 230.4,230.4 });
		rel_line_to(cr, POINT{ -102.4, 0.0 });
		curve_to(cr, POINT{ 51.2, 230.4 }, POINT{ 51.2, 128.0 }, POINT{ 128.0, 128.0 });
		stroke(cr);
			   		
		write_to_png(surface, "image.png");
	}
	return 0;
}

