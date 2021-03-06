
#include "pch.h"
#include "cairo_util.h"

int main() {
	{
		TimeCheck time;
		auto surface = create_surface(CAIRO_FORMAT_ARGB32, SURFACE_SIZE{ 256, 256 });
		auto cr = create(surface);
			
		font_face(cr, u8"맑은 고딕",
			CAIRO_FONT_SLANT_NORMAL,
			CAIRO_FONT_WEIGHT_NORMAL);

		const char *utf8 = u8"안녕";
		cairo_text_extents_t extents;
		font_size(cr, 100.0);
		text_extents(cr, utf8, extents);

		double x = 25.0;
		double y = 150.0;

		move_to(cr, POINT{ x, y });
		show_text(cr, utf8);

		/* draw helping lines */
		source(cr, RGBA{ 1, 0.2, 0.2, 0.6 });
		line_width(cr, 6.0);
		arc(cr, POINT{ x, y }, 10.0, RADIAN{ 0 }, RADIAN{ 2 * pi<double> });
		fill(cr); 

		move_to(cr, POINT{ x, y });
		rel_line_to(cr, POINT{ 0, -extents.height });
		rel_line_to(cr, POINT{ extents.width, 0 });
		rel_line_to(cr, POINT{ extents.x_bearing, -extents.y_bearing });
		stroke(cr);

		write_to_png(surface, "image.png");
	}
	return 0;
}

