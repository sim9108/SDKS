
#include "pch.h"
#include <vector>
#include "cairo_util.h"

void fill_ring(std::shared_ptr<cairo_t >&cr);
void fill_star(std::shared_ptr<cairo_t >&cr);

void ring_path(std::shared_ptr<cairo_t >&cr);
void star_path(std::shared_ptr<cairo_t >&cr);

void clip_to_top_and_bottom(std::shared_ptr<cairo_t >&cr, int width, int height);
void clip_to_left_and_right(std::shared_ptr<cairo_t >&cr, int width, int height);

int main() {
	{
		TimeCheck time;

		const int WIDTH = 600;
		const int HEIGHT =600;
		const int STRIDE = WIDTH * 4;

		std::vector<unsigned char> datas(HEIGHT*STRIDE);
		auto result = create_surface(datas, CAIRO_FORMAT_ARGB32, SURFACE_SIZE{ WIDTH, HEIGHT }, STRIDE);
		auto cr = create(result);

		auto ring_over_star = create_surface(CAIRO_FORMAT_ARGB32, SURFACE_SIZE{ WIDTH, HEIGHT });
		{
			auto cr_ros = create(ring_over_star);
			clip_to_top_and_bottom(cr_ros, WIDTH, HEIGHT);
			fill_star(cr_ros);
			fill_ring(cr_ros);
			write_to_png(ring_over_star, "ring_over_star.png");
		}

		auto  star_over_ring = create_surface(CAIRO_FORMAT_ARGB32, SURFACE_SIZE{ WIDTH, HEIGHT });
		{
			auto cr_sor = create(star_over_ring);
			clip_to_left_and_right(cr_sor, WIDTH, HEIGHT);
			fill_ring(cr_sor);
			fill_star(cr_sor);
			write_to_png(star_over_ring, "star_over_ring.png");
		}
			   	
		op(cr, CAIRO_OPERATOR_ADD);
		source(cr, ring_over_star, POINT{ 0,0 });
		paint(cr);

		source(cr, star_over_ring, POINT{ 0,0 });
		paint(cr);
		write_to_png(result, "image.png");
	}
	return 0;
}

void ring_path(std::shared_ptr<cairo_t >&cr) {
	move_to(cr, POINT{ 200.86568, 667.80795 });
	curve_to(cr,
		POINT{ 110.32266, 562.62134 },
		POINT{ 122.22863, 403.77940 },
		POINT{ 227.41524, 313.23637 });
	curve_to(cr,
		POINT{ 332.60185, 222.69334 },
		POINT{ 491.42341, 234.57563 },
		POINT{ 581.96644, 339.76224 });
	curve_to(cr,
		POINT{ 672.50948, 444.94884 },
		POINT{ 660.64756, 603.79410 },
		POINT{ 555.46095, 694.33712 });
	curve_to(cr,
		POINT{ 450.27436, 784.88016 },
		POINT{ 291.40871, 772.99456 },
		POINT{ 200.86568, 667.80795 });
	close_path(cr);

	move_to(cr, POINT{ 272.14411, 365.19927 });
	curve_to(cr,
		POINT{ 195.64476, 431.04875 },
		POINT{ 186.97911, 546.57972 },
		POINT{ 252.82859, 623.07908 });
	curve_to(cr,
		POINT{ 318.67807, 699.57844 },
		POINT{ 434.23272, 708.22370 },
		POINT{ 510.73208, 642.37422 });
	curve_to(cr,
		POINT{ 587.23144, 576.52474 },
		POINT{ 595.85301, 460.99047 },
		POINT{ 530.00354, 384.49112 });
	curve_to(cr,
		POINT{ 464.15406, 307.99176 },
		POINT{ 348.64347, 299.34979 },
		POINT{ 272.14411, 365.19927 });
	close_path(cr);
}

void star_path(std::shared_ptr<cairo_t >&cr) {
	cairo_matrix_t matrix;

	init(matrix,
		0.647919, -0.761710,
		0.761710, 0.647919,
		-208.7977, 462.0608);
	transform(cr, matrix);

	move_to(cr, POINT{ 505.80857, 746.23606 });
	line_to(cr, POINT{ 335.06870, 555.86488 });
	line_to(cr, POINT{ 91.840384, 635.31360 });
	line_to(cr, POINT{ 282.21157, 464.57374 });
	line_to(cr, POINT{ 202.76285, 221.34542 });
	line_to(cr, POINT{ 373.50271, 411.71660 });
	line_to(cr, POINT{ 616.73103, 332.26788 });
	line_to(cr, POINT{ 426.35984, 503.00775 });
	line_to(cr, POINT{ 505.80857, 746.23606 });
	close_path(cr);
}

void clip_to_top_and_bottom(std::shared_ptr<cairo_t >&cr, int width, int height) {
	move_to(cr, POINT{ 0, 0 });
	line_to(cr, POINT{ (double)width, 0 });
	line_to(cr, POINT{ 0, (double)height });
	line_to(cr, POINT{ (double)width, (double)height });
	close_path(cr);
	clip(cr);
	new_path(cr);
}

void clip_to_left_and_right(std::shared_ptr<cairo_t >&cr, int width, int height) {
	move_to(cr, POINT{ 0, 0 });
	line_to(cr, POINT{ 0, (double)height });
	line_to(cr, POINT{ (double)width, 0 });
	line_to(cr, POINT{ (double)width, (double)height });
	close_path(cr);
	clip(cr);
	new_path(cr);
}

void fill_star(std::shared_ptr<cairo_t >&cr) {
	save(cr);
	{
		translate(cr, POINT{ -90, -205 });
		star_path(cr);
		source(cr, RGBA{ 0.0, 0.0, (double)0xae / 0xff, 0.55135137 });
		fill(cr);
	}
	restore(cr);
}

void fill_ring(std::shared_ptr<cairo_t >&cr) {
	save(cr);
	{
		translate(cr, POINT{ -90, -205 });
		ring_path(cr);
		source(cr, RGBA{ 1.0, 0.0, 0.0, 0.75 });
		fill(cr);
	}
	restore(cr);
}

