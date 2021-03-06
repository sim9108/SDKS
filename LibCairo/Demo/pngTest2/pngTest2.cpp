
#include "pch.h"
#include "cairo_util.h"
void draw_splines(std::shared_ptr<cairo_t>& cr, int width, int height);

int main() {
	{
		TimeCheck time;
		constexpr int WIDTH = 600;
		constexpr int HEIGHT = 600;
		constexpr int STRIDE = WIDTH * 4;

		std::vector<unsigned char> datas(HEIGHT*STRIDE);
		auto surface = create_surface(datas, CAIRO_FORMAT_ARGB32, SURFACE_SIZE{ WIDTH, HEIGHT }, STRIDE);
		auto cr = create(surface);
		
		rectangle(cr, POINT{ 0,0 }, SIZE{ WIDTH ,HEIGHT });
		source(cr, RGB{ 1,1,1 });
		fill(cr);

		draw_splines(cr, WIDTH, HEIGHT);
		write_to_png(surface, "image.png"); 
	}
	return 0;
}

void
draw_spline(std::shared_ptr<cairo_t>& cr, double height)
{
	move_to(cr, POINT{ 0, .1 * height });
	height = .8 * height;
	rel_curve_to(cr,
		POINT{ -height / 2, height / 2 },
		POINT{ height / 2, height / 2 },
		POINT{ 0, height });
	stroke(cr);
}

void draw_splines(std::shared_ptr<cairo_t>& cr, int width, int height) {
	int i;
	double values[5] = { .1, .5, 1, 5, 10 };
	double l_width = .08 * width;
	double gap = width / 6;

	source(cr, RGB{ 0, 0, 0 });
	line_width(cr, l_width);

	translate(cr, POINT{ gap, 0 });
	for (i = 0; i < 5; i++) {
		tolerance(cr, values[i]);
		draw_spline(cr, height);
		translate(cr, { gap, 0 });
	}
}
