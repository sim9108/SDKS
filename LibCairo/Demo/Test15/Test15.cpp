
#include "pch.h"
#include "cairo_util.h"

int main() {
	{
		TimeCheck time;
		auto surface = create_surface(CAIRO_FORMAT_ARGB32, SURFACE_SIZE{ 256, 256 });
		auto cr = create(surface);
		
		line_width(cr, 40.96);
		move_to(cr, POINT{ 76.8, 84.48 });
		rel_line_to(cr, POINT{ 51.2, -51.2 });
		rel_line_to(cr, POINT{ 51.2, 51.2 });
		line_join(cr, CAIRO_LINE_JOIN_MITER); /* default */
		stroke(cr);

		move_to(cr, POINT{ 76.8, 161.28 });
		rel_line_to(cr, POINT{ 51.2, -51.2 });
		rel_line_to(cr, POINT{ 51.2, 51.2 });
		line_join(cr, CAIRO_LINE_JOIN_BEVEL);
		stroke(cr);

		move_to(cr, POINT{ 76.8, 238.08 });
		rel_line_to(cr, POINT{ 51.2, -51.2 });
		rel_line_to(cr, POINT{ 51.2, 51.2 });
		line_join(cr, CAIRO_LINE_JOIN_ROUND);
		stroke(cr);

		write_to_png(surface, "image.png");
	}
	return 0;
}

