
#include "pch.h"
#include "cairo_util.h"

int main() {
	{
		TimeCheck time;
		auto surface = create_surface(CAIRO_FORMAT_ARGB32, SURFACE_SIZE{ 256, 256 });
		auto cr = create(surface);
		
		line_width(cr, 30.0);
		line_cap(cr, CAIRO_LINE_CAP_BUTT); /* default */
		move_to(cr, POINT{ 64.0, 50.0 }); line_to(cr, POINT{ 64.0, 200.0 });
		stroke(cr);

		line_cap(cr, CAIRO_LINE_CAP_ROUND);
		move_to(cr, POINT{ 128.0, 50.0 }); line_to(cr, POINT{ 128.0, 200.0 });
		stroke(cr);

		line_cap(cr, CAIRO_LINE_CAP_SQUARE);
		move_to(cr, POINT{ 192.0, 50.0 }); line_to(cr, POINT{ 192.0, 200.0 });
		stroke(cr);

		/* draw helping lines */
		source(cr, RGB{ 1, 0.2, 0.2 });
		line_width(cr, 2.56);
		move_to(cr, POINT{ 64.0, 50.0 }); line_to(cr, POINT{ 64.0, 200.0 });
		move_to(cr, POINT{ 128.0, 50.0 });  line_to(cr, POINT{ 128.0, 200.0 });
		move_to(cr, POINT{ 192.0, 50.0 }); line_to(cr, POINT{ 192.0, 200.0 });
		stroke(cr);

		write_to_png(surface, "image.png");
	}
	return 0;
}

