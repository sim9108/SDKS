
#include "pch.h"
#include "cairo_util.h"
void draw_hering(std::shared_ptr<cairo_t>& cr, int width, int height);

int main() {
	{
		TimeCheck time;
		constexpr int WIDTH = 300;
		constexpr int HEIGHT = 600;
		constexpr int STRIDE = WIDTH * 4;

		std::vector<unsigned char> datas(HEIGHT*STRIDE);
		auto surface = create_surface(datas, CAIRO_FORMAT_ARGB32, SURFACE_SIZE{ WIDTH, HEIGHT }, STRIDE);
		auto cr = create(surface);
		
		rectangle(cr, POINT{ 0,0 }, SIZE{ WIDTH ,HEIGHT });
		source(cr, RGB{ 1,1,1 });
		fill(cr);

		draw_hering(cr, WIDTH, HEIGHT);
		write_to_png(surface, "image.png"); 
	}
	return 0;
}


void draw_hering(std::shared_ptr<cairo_t>& cr, int width, int height) {

	RADIAN MAX_THETA  { 0.80*pi<double>*2 };
	auto LINES{ 32 };
	RADIAN THETA_INC{ 2.0 * MAX_THETA / (LINES - 1) };
	
	source(cr, RGB{ 0, 0, 0 });
	line_width(cr, 2);
	
	save(cr);
	{
		translate(cr, POINT{ width / 2.0, height / 2.0 });
		rotate(cr, MAX_THETA);
		for (int i = 0; i < LINES; i++) {
			move_to(cr, POINT{ -2.0 * width, 0 });
			line_to(cr, POINT{ 2.0* width, 0 });
			stroke(cr);
			rotate(cr, -THETA_INC);
		}
	}
	restore(cr);

	line_width(cr, 6);
	source(cr, RGB{ 1, 0, 0 });
	move_to(cr, POINT{ width / 4.0, 0 });
	rel_line_to(cr, POINT{ 0, double(height) });
	stroke(cr);

	move_to(cr, POINT{ 3 * width / 4.0, 0 });
	rel_line_to(cr, POINT{ 0, double(height) });
	stroke(cr);
}
