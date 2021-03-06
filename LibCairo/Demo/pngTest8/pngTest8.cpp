
#include "pch.h"
#include <functional>
#include <vector>
#include "cairo_util.h"

void fn_source_path(std::shared_ptr<cairo_t >&cr);
void fn_stroke(std::shared_ptr<cairo_t >&cr);
void fn_scale_both_set_line_width_stroke(std::shared_ptr<cairo_t >&cr);
void fn_scale_both_set_line_width_double_stroke(std::shared_ptr<cairo_t >&cr);
void fn_save_scale_path_restore_set_line_width_stroke(std::shared_ptr<cairo_t >&cr);
void fn_scale_path_set_line_width_stroke(std::shared_ptr<cairo_t >&cr);

int main() {
	{
		TimeCheck time;

		std::vector < std::function<void(std::shared_ptr<cairo_t >&)>> fns = { 
			fn_source_path,
			fn_stroke, 
			fn_scale_both_set_line_width_stroke,
			fn_scale_both_set_line_width_double_stroke,
			fn_save_scale_path_restore_set_line_width_stroke,
			fn_scale_path_set_line_width_stroke
		};

		const int size = fns.size();
		const int WIDTH = 140;
		const int HEIGHT = int(68.833*size);
		const int STRIDE = WIDTH * 4;

		std::vector<unsigned char> datas(HEIGHT*STRIDE);
		auto surface = create_surface(datas, CAIRO_FORMAT_ARGB32, SURFACE_SIZE{ WIDTH, HEIGHT }, STRIDE);
		auto cr = create(surface);

		rectangle(cr, POINT{ 0,0 }, SIZE{ double(WIDTH) ,double(HEIGHT) });
		source(cr, RGB{ 1,1,1 });
		fill(cr);
		
		source(cr, RGB{0,0,0 });

		int  idx{ 0 };
		for ( auto&& fn: fns) {
			save(cr);
			translate(cr, POINT{ WIDTH / 2, (idx + 0.5)*(HEIGHT / size) });
			fn(cr);			
			restore(cr);
			++idx;
			
			if (auto err = status(cr)) {
				std::cout << "Cairo is unhappy after pipeline:" << status_to_string(err);
				break;
			}
		}		
		
		write_to_png(surface, "image.png"); 
	}
	return 0;
}

constexpr auto LINE_WIDTH = 13;


void spline_path(std::shared_ptr<cairo_t >&cr) {
	save(cr); 
	{
		translate(cr, POINT{ -106.0988385, -235.84433 });
		move_to(cr, POINT{ 49.517857, 235.84433 });
		curve_to(cr, POINT{ 86.544809, 175.18401 }, POINT{ 130.19603, 301.40165 }, POINT{ 162.67982, 240.42946 });
	}
	restore(cr);
}

void fn_source_path(std::shared_ptr<cairo_t >&cr) {
	spline_path(cr);
	line_width(cr, 1.0);
	stroke(cr);
}

void fn_stroke(std::shared_ptr<cairo_t >&cr) {
	spline_path(cr);
	line_width(cr, LINE_WIDTH);
	stroke(cr);
}
void fn_scale_both_set_line_width_stroke(std::shared_ptr<cairo_t >&cr) {
	scale(cr, SCALE{ 0.5, 0.5 });
	spline_path(cr);
	line_width(cr, LINE_WIDTH);
	stroke(cr);
}

void fn_scale_both_set_line_width_double_stroke(std::shared_ptr<cairo_t >&cr) {
	scale(cr, SCALE{ 0.5, 0.5 });
	spline_path(cr);
	line_width(cr, 2 * LINE_WIDTH);
	stroke(cr);
}

void fn_save_scale_path_restore_set_line_width_stroke(std::shared_ptr<cairo_t >&cr) {
	save(cr);
	{
		scale(cr, SCALE{ 0.5, 1.0 });
		spline_path(cr);
	}
	restore(cr);

	line_width(cr, LINE_WIDTH);
	stroke(cr);
}

void fn_scale_path_set_line_width_stroke(std::shared_ptr<cairo_t >&cr) {
	scale(cr, SCALE{ 0.5, 1.0 });
	line_width(cr, LINE_WIDTH);
	spline_path(cr);
	stroke(cr);
}