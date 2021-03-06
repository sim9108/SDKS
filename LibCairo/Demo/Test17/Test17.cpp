
#include "pch.h"
#include "cairo_util.h"

int main() {
	{
		TimeCheck time;
		auto surface = create_surface(CAIRO_FORMAT_ARGB32, SURFACE_SIZE{ 256, 256 });
		auto cr = create(surface);
		
		auto image = create_surface("test.png");
		auto w = width(image);
		auto h = height(image);

		auto pattern = create_pattern(image);
		extend(pattern, CAIRO_EXTEND_REPEAT);

		translate(cr, POINT{ 128.0, 128.0 });
		rotate(cr, RADIAN{ pi<double> / 4 });
		scale(cr, SCALE{ 1 / sqrt(2), 1 / sqrt(2) });
		translate(cr, POINT{ -128.0, -128.0 });

		cairo_matrix_t   mat;
		init_scale(mat, SCALE{ w / 256.0 * 5.0, h / 256.0 * 5.0 });
		matrix(pattern, mat);
		source(cr, pattern);

		rectangle(cr, POINT{ 0, 0 }, SIZE{ 256.0, 256.0 });
		fill(cr);

		write_to_png(surface, "image.png");
	}
	return 0;
}

