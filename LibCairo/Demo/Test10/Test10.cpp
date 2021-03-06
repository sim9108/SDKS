
#include "pch.h"
#include "cairo_util.h"

int main() {
	{
		TimeCheck time;
		auto surface = create_surface(CAIRO_FORMAT_ARGB32, SURFACE_SIZE{ 256, 256 });
		auto cr = create(surface);
		
		line_width(cr, 6);

		rectangle(cr, POINT{ 12, 12 }, SIZE{ 232, 70 });
		new_sub_path(cr); arc(cr, POINT{ 64, 64 }, 40, RADIAN{ 0 }, RADIAN{ 2 * pi<double> });
		new_sub_path(cr); arc_negative(cr, POINT{ 192, 64 }, 40, RADIAN{ 0 }, RADIAN{ -2 * pi<double> });

		fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
		source(cr, RGB{ 0, 0.7, 0 }); fill_preserve(cr);
		source(cr, RGB{ 0, 0, 0 }); stroke(cr);

		translate(cr, POINT{ 0, 128 });
		rectangle(cr, POINT{ 12, 12 }, SIZE{ 232, 70 });
		new_sub_path(cr); arc(cr, POINT{ 64, 64 }, 40, RADIAN{ 0 }, RADIAN{ 2*pi<double> });
		new_sub_path(cr); arc_negative(cr, POINT{ 192, 64 }, 40, RADIAN{ 0 }, RADIAN{ -2 * pi<double> });

		fill_rule(cr, CAIRO_FILL_RULE_WINDING);
		source(cr, RGB{ 0, 0, 0.9 }); fill_preserve(cr);
		source(cr, RGB{ 0, 0, 0 }); stroke(cr);
			   		
		write_to_png(surface, "image.png");
	}
	return 0;
}

