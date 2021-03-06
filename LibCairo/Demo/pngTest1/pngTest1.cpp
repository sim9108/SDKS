
#include "pch.h"
#include "cairo_util.h"
void draw_spiral(std::shared_ptr<cairo_t>& cr, int width, int height);

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

		draw_spiral(cr, WIDTH, HEIGHT);
		write_to_png(surface, "image.png");
	}
	return 0;
}

void draw_spiral(std::shared_ptr<cairo_t>& cr, int width, int height) {
	double wd = .02 * width;
	double hd = .02 * height;
	width -= 2;
	height -= 2;

	move_to(cr, POINT{ width + 1.0, 1.0 - hd });
	for (int i = 0; i < 9; i++) {
		rel_line_to(cr, POINT{ 0, height - hd * (2 * i - 1) });
		rel_line_to(cr, POINT{ -(width - wd * (2 * i)), 0 });
		rel_line_to(cr, POINT{ 0, -(height - hd * (2 * i)) });
		rel_line_to(cr, POINT{ width - wd * (2 * i + 1), 0 });
	}

	source(cr, RGB{ 0, 0, 1 });
	stroke(cr);
}
