
#include "pch.h"
#include "cairo_util.h"
void draw_stars(std::shared_ptr<cairo_t>& cr, int width, int height);

int main() {
	{
		TimeCheck time;
		constexpr int WIDTH = 600;
		constexpr int HEIGHT = 275;
		constexpr int STRIDE = WIDTH * 4;

		std::vector<unsigned char> datas(HEIGHT*STRIDE);
		auto surface = create_surface(datas, CAIRO_FORMAT_ARGB32, SURFACE_SIZE{ WIDTH, HEIGHT }, STRIDE);
		auto cr = create(surface);
		
		rectangle(cr, POINT{ 0,0 }, SIZE{ WIDTH ,HEIGHT });
		source(cr, RGB{ 1,1,1 });
		fill(cr);

		draw_stars(cr, WIDTH, HEIGHT);
		write_to_png(surface, "image.png"); 
	}
	return 0;
}

void
star_path(std::shared_ptr<cairo_t>& cr)
{
	int i;
	double theta = 4 * pi<double> / 5.0;

	move_to(cr, POINT{ 0, 0 });
	for (i = 0; i < 4; i++) {
		rel_line_to(cr, POINT{ 1.0, 0 });
		rotate(cr, RADIAN{ theta });
	}
	close_path(cr);
}

void draw_stars(std::shared_ptr<cairo_t>& cr, int width, int height) {
	source(cr, RGB{ 0, 0, 0 });

	save(cr);
	{
		translate(cr, POINT{ 5, height / 2.6 });
		scale(cr, SCALE{ double(height), double(height) });
		star_path(cr);
		fill_rule(cr, CAIRO_FILL_RULE_WINDING);
		fill(cr);
	}
	restore(cr);

	save(cr);
	{
		translate(cr, POINT{ width - height - 5.0, height / 2.6 });
		scale(cr, SCALE{ double(height), double(height) });
		star_path(cr);
		fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
		fill(cr);
	}
	restore(cr);
}
