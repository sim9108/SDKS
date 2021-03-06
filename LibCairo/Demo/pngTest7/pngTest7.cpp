
#include "pch.h"
#include "cairo_util.h"

void draw_outlines (std::shared_ptr <cairo_t>& cr, int surface_width, int surface_height);
std::shared_ptr <cairo_pattern_t> create_gradient(double width, double height);
void draw_flat (std::shared_ptr <cairo_t>& cr, double width, double height);
void draw_tent (std::shared_ptr <cairo_t>& cr, double width, double height);
void draw_cylinder (std::shared_ptr <cairo_t>& cr, double width, double height);

int main() {
	{
		TimeCheck time;
		constexpr int WIDTH =750;
		constexpr int HEIGHT = 500;
		constexpr int STRIDE = WIDTH * 4;

		std::vector<unsigned char> datas(HEIGHT*STRIDE);
		auto surface = create_surface(datas, CAIRO_FORMAT_ARGB32, SURFACE_SIZE{ WIDTH, HEIGHT }, STRIDE);
		auto cr = create(surface);

		rectangle(cr, POINT{ 0,0 }, SIZE{ WIDTH ,HEIGHT });
		source(cr, RGB{ 1,1,1 });
		fill(cr);
		
		draw_outlines (cr, WIDTH, HEIGHT);
		
		write_to_png(surface, "image.png"); 
	}
	return 0;
}

std::shared_ptr <cairo_pattern_t> create_gradient(double width, double height) {
	std::shared_ptr <cairo_pattern_t> gradient = create_pattern(POINT{ 0,0 }, POINT{ width,0 });
	add_color_stop(gradient, 0.0,  RGB{ 0, 0, 0 });
	add_color_stop(gradient, 0.5, RGB{ 1, 1, 1 });
	add_color_stop(gradient, 1.0, RGB{ 0, 0, 0 });
	return gradient;
}

void draw_flat(std::shared_ptr <cairo_t>& cr, double width, double height) {
	const double hwidth = width / 2.0;
	rectangle(cr, POINT{ 0, hwidth }, SIZE{ width, height - hwidth });
	fill(cr);
}

void draw_tent(std::shared_ptr <cairo_t>& cr, double width, double height) {
	const double hwidth = width / 2.0;
	move_to(cr, POINT{ 0, hwidth });
	rel_line_to(cr, POINT{ hwidth, -hwidth });
	rel_line_to(cr, POINT{ hwidth, hwidth });
	rel_line_to(cr, POINT{ 0, height - hwidth });
	rel_line_to(cr, POINT{ -hwidth, -hwidth });
	rel_line_to(cr, POINT{ -hwidth, hwidth });
	close_path(cr);

	fill(cr);
}

void draw_outlines (std::shared_ptr <cairo_t>& cr, int surface_width, int surface_height){
	const double width= surface_width / 4.0;
	const double  height = surface_height;
	auto gradient = create_gradient(width, height);

	source(cr, gradient);
	draw_flat(cr, width, height);
	
	const double pad = (surface_width - (3 * width)) / 2.0;
	translate(cr, POINT{ width + pad, 0 });
	source(cr, gradient);
	draw_tent(cr, width, height);

	translate(cr, POINT{ width + pad, 0 });
	source(cr, gradient);
	draw_cylinder(cr, width, height);
}

void draw_cylinder(std::shared_ptr <cairo_t>& cr, double width, double height) {
	const double hwidth = width / 2.0;

	move_to(cr, POINT{ 0, hwidth });
	rel_curve_to(cr, POINT{ 0, -hwidth }, POINT{ width, -hwidth }, POINT{ width, 0});
	rel_line_to(cr, POINT{ 0, height - hwidth });
	rel_curve_to(cr,
		POINT{ 0, -hwidth }, POINT{ -width, -hwidth }, POINT{ -width, 0 });
	close_path(cr);

	fill(cr);
}