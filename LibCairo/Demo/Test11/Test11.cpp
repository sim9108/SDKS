
#include "pch.h"
#include "cairo_util.h"

int main() {
	{
		TimeCheck time;
		auto surface = create_surface(CAIRO_FORMAT_ARGB32, SURFACE_SIZE{ 256, 256 });
		auto cr = create(surface);
		
		auto pat = create_pattern(POINT{ 0.0, 0.0 }, POINT{0.0, 256.0});
		add_color_stop(pat, 1, RGBA{ 0, 0, 0, 1 });
		add_color_stop(pat, 0, RGBA{ 1, 1, 1, 1 });
		rectangle(cr, POINT{ 0, 0 }, SIZE{ 256, 256 });
		source(cr, pat);
		fill(cr);

		pat = create_pattern(POINT{ 115.2, 102.4 }, 25.6, POINT{ 102.4, 102.4 }, 128.0);
		add_color_stop(pat, 0, RGBA{ 1, 1, 1, 1 });
		add_color_stop(pat, 1, RGBA{ 0, 0, 0, 1 });
		source(cr, pat);
		arc(cr, POINT{ 128.0, 128.0 }, 76.8, RADIAN{ 0 }, RADIAN{ 2 * pi<double> });
		fill(cr);
			   		
		write_to_png(surface, "image.png");
	}
	return 0;
}

