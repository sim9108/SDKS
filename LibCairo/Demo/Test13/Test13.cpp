
#include "pch.h"
#include "cairo_util.h"

int main() {
	{
		TimeCheck time;
		auto surface = create_surface(CAIRO_FORMAT_ARGB32, SURFACE_SIZE{ 256, 256 });
		auto cr = create(surface);
		
		font_face(cr, u8"휴먼편지체",
			CAIRO_FONT_SLANT_NORMAL,
			CAIRO_FONT_WEIGHT_NORMAL);

		cairo_text_extents_t extents;
		const char *utf8 = u8"중간 정렬";
		font_size(cr, 52.0);
		text_extents(cr, utf8, extents);

		double x = 128.0 - (extents.width / 2 + extents.x_bearing);
		double y = 128.0 - (extents.height / 2 + extents.y_bearing);

		move_to(cr, POINT{ x, y });
		show_text(cr, utf8);

		/* draw helping lines */
		source(cr, RGBA{ 1, 0.2, 0.2, 0.6 });
		line_width(cr, 6.0);
		arc(cr, POINT{ x, y }, 10.0, RADIAN{ 0 }, RADIAN{ 2 * pi<double> });
		fill(cr);
		move_to(cr, POINT{ 128.0, 0 });
		rel_line_to(cr, POINT{ 0, 256 });
		move_to(cr, POINT{ 0, 128.0 });
		rel_line_to(cr, POINT{ 256, 0 });
		stroke(cr);

		write_to_png(surface, "image.png");
	}
	return 0;
}

