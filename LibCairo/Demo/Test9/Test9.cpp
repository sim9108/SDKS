
#include "pch.h"
#include "cairo_util.h"

int main() {
	{
		TimeCheck time;
		auto surface = create_surface(CAIRO_FORMAT_ARGB32, SURFACE_SIZE{ 256, 256 });
		auto cr = create(surface);
		
		move_to(cr, POINT{ 128.0,25.6 });
		line_to(cr, POINT{ 230.4, 230.4 });
		rel_line_to(cr, POINT{ -102.4,0 });
		curve_to(cr, POINT{ 51.2, 230.4 }, POINT{ 51.2, 128.0 }, POINT{ 128.0, 128.0 });
		close_path(cr);

		move_to(cr, POINT{ 64.0, 25.6 });
		rel_line_to(cr, POINT{ 51.2, 51.2 });
		rel_line_to(cr, POINT{ -51.2, 51.2 });
		rel_line_to(cr, POINT{ -51.2, -51.2 });
		close_path(cr);

		line_width(cr, 10.0);
		source(cr, RGB{ 0, 0, 1 });
		fill_preserve(cr);
		source(cr, RGB{ 0, 0, 0 });
		stroke(cr);
			   		
		write_to_png(surface, "image.png");
	}
	return 0;
}

