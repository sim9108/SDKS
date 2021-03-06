
#include "pch.h"
#include "cairo_util.h"
void draw_caps_joins(std::shared_ptr<cairo_t>& cr, int width, int height);
void stroke_v_twice(std::shared_ptr<cairo_t>& cr, int width, int height);

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

		draw_caps_joins(cr, WIDTH, HEIGHT);
		write_to_png(surface, "image.png"); 
	}
	return 0;
}

void stroke_v_twice(std::shared_ptr<cairo_t>& cr, int width, int height) {
	move_to(cr, POINT{ 0, 0 });
	rel_line_to(cr, POINT{ width / 2.0, height / 2.0 });
	rel_line_to(cr, POINT{ width / 2.0, -height / 2.0 });

	save(cr);
	stroke(cr);
	restore(cr);

	save(cr);
	{
		line_width(cr, 2.0);
		line_cap(cr, CAIRO_LINE_CAP_BUTT);
		source(cr, RGB{ 1, 1, 1 });
		stroke(cr);
	}
	restore(cr);

	new_path(cr);
}

void draw_caps_joins(std::shared_ptr<cairo_t>& cr, int width, int height) {

	int linewidth = height / 12 & (~1);
	line_width(cr, linewidth);
	source(cr, RGB{ 0,0,0 });
	translate(cr, POINT{ double(linewidth),double(linewidth) });

	width -= 2 * linewidth;
	line_join(cr, CAIRO_LINE_JOIN_BEVEL);
	line_cap(cr, CAIRO_LINE_CAP_BUTT);
	stroke_v_twice(cr, width, height);

	translate(cr, POINT{ 0, height / 4.0 - linewidth });
	line_join(cr, CAIRO_LINE_JOIN_MITER);
	line_cap(cr, CAIRO_LINE_CAP_SQUARE);
	stroke_v_twice(cr, width, height);

	translate(cr, POINT{ 0, height / 4.0 - linewidth });
	line_join(cr, CAIRO_LINE_JOIN_ROUND);
	line_cap(cr, CAIRO_LINE_CAP_ROUND);
	stroke_v_twice(cr, width, height);
}
