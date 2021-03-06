
#include "pch.h"
#include "cairo_util.h"

int main() {
	{
		TimeCheck time;
		auto surface = create_surface(CAIRO_FORMAT_ARGB32, SURFACE_SIZE{ 256, 256 });
		auto cr = create(surface);
		
		font_face(cr, u8"나눔바른고딕OTF", CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_BOLD);
		font_size(cr, 40.0);

		move_to(cr, POINT{ 10.0, 135.0 });
		show_text(cr, u8"안녕");

		move_to(cr, POINT{ 70.0, 165.0 });
		text_path(cr, u8"하세요");
		source(cr, RGB{ 0.5, 0.5, 1 });
		fill_preserve(cr);
		source(cr, RGB{ 0, 0, 0 });
		line_width(cr, 2.56);
		stroke(cr);

		/* draw helping lines */
		source(cr, RGBA{ 1, 0.2, 0.2, 0.6 });
		arc(cr, POINT{ 10.0, 135.0 }, 5.12, RADIAN{ 0 }, RADIAN{ 2 * pi<double> });
		close_path(cr);
		arc(cr, POINT{ 70.0, 165.0 }, 5.12, RADIAN{ 0 }, RADIAN{ 2 * pi<double> });
		fill(cr);

		write_to_png(surface, "image.png");
	}
	return 0;
}

