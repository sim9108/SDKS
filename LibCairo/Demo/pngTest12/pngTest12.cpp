
#include "pch.h"
#include <vector>
#include "cairo_util.h"

struct snap_path_closure_t {
	int first;
	int fill;
	std::shared_ptr< cairo_t> cr;
} ;

enum nested_style_t { NESTED_FILLS, NESTED_STROKES } ;
enum snapping_t { NO_SNAPPING, SNAPPING } ;

void draw(std::shared_ptr< cairo_t>& cr, int width, int height);
void draw_nested(std::shared_ptr< cairo_t>& cr, const nested_style_t& style, const snapping_t& snapping);
void snap_path_for_stroke(std::shared_ptr< cairo_t>& cr);
void snap_line_width(std::shared_ptr< cairo_t>& cr);
void snap_path_for_fill(std::shared_ptr< cairo_t>& cr);

void spc_move_to(snap_path_closure_t& spc, double x, double y);
void spc_line_to(snap_path_closure_t& spc, double x, double y);
void spc_curve_to(snap_path_closure_t& spc,	double x1, double y1_,
																			double x2, double y2,
																			double x3, double y3);
void spc_close_path(snap_path_closure_t& spc);
void spc_new_path_perhaps(snap_path_closure_t &spc);
void snap_point(snap_path_closure_t & spc, double& x, double& y);
void snap_point_for_stroke(std::shared_ptr< cairo_t>& cr, double& x, double& y);
void snap_point_for_fill(std::shared_ptr< cairo_t>& cr, double& x, double& y);

int main() {
	{
		TimeCheck time;

		constexpr int WIDTH = 175;
		constexpr int HEIGHT = 175;
		constexpr int STRIDE = WIDTH * 4;

		std::vector<unsigned char> datas(HEIGHT*STRIDE);
		auto surface = create_surface(datas, CAIRO_FORMAT_ARGB32, SURFACE_SIZE{ WIDTH, HEIGHT }, STRIDE);
		auto cr = create(surface);

		rectangle(cr, POINT{ 0,0 }, SIZE{ WIDTH, HEIGHT });
		source(cr, RGB{ 0,0,0 });
		fill(cr);

		draw(cr, WIDTH, HEIGHT);

		write_to_png(surface, "image.png"); 
	}
	return 0;
}

void snap_path_for_fill(std::shared_ptr< cairo_t>& cr) {
	snap_path_closure_t spc{ 1,1, cr };

	auto path = copy_path(cr);

	for (int i = 0; i < path->num_data; i += path->data[i].header.length) {
		cairo_path_data_t* data = &path->data[i];
		switch (data->header.type) {
		case CAIRO_PATH_MOVE_TO:
			spc_move_to(spc, data[1].point.x, data[1].point.y);
			break;
		case CAIRO_PATH_LINE_TO:
			spc_line_to(spc, data[1].point.x, data[1].point.y);
			break;
		case CAIRO_PATH_CURVE_TO:
			spc_curve_to(spc, data[1].point.x, data[1].point.y,
				data[2].point.x, data[2].point.y,
				data[3].point.x, data[3].point.y);
			break;
		case CAIRO_PATH_CLOSE_PATH:
			spc_close_path(spc);
			break;
		}
	}
	
}

void snap_point_for_fill(std::shared_ptr< cairo_t>& cr, double& x, double& y) {
	/* Convert to device space, round, then convert back to user space. */
	user_to_device(cr, x, y);
	x = floor(x + 0.5);
	y = floor(y + 0.5);
	device_to_user(cr, x, y);
}

void snap_point_for_stroke(std::shared_ptr< cairo_t>& cr, double& x, double& y) {
	/*
	 * Round in device space after adding the fractional portion of
	 * one-half the (device space) line width.
	 */
	double x_width_dev_2 =  line_width(cr);
	double y_width_dev_2 = x_width_dev_2;
	user_to_device_distance(cr, x_width_dev_2, y_width_dev_2);
	x_width_dev_2 *= 0.5;
	y_width_dev_2 *= 0.5;

	double x_offset = x_width_dev_2 - (int)(x_width_dev_2);
	double y_offset = y_width_dev_2 - (int)(y_width_dev_2);

	user_to_device(cr, x, y);
	x = floor(x + x_offset + 0.5);
	y = floor(y + y_offset + 0.5);
	x -= x_offset;
	y -= y_offset;
	device_to_user(cr, x, y);
}

void snap_point(snap_path_closure_t & spc, double& x, double& y) {
	if (spc.fill)
		snap_point_for_fill(spc.cr, x, y);
	else
		snap_point_for_stroke(spc.cr, x, y);
}

void spc_new_path_perhaps(snap_path_closure_t &spc) {
	if (!spc.first)  return;
	new_path(spc.cr);
	spc.first = 0;
}
void spc_close_path(snap_path_closure_t& spc) {
	spc_new_path_perhaps(spc);
	close_path(spc.cr);
}

void spc_curve_to(snap_path_closure_t& spc, double x1, double y1_,
																			double x2, double y2,
																			double x3, double y3) {
	spc_new_path_perhaps(spc);
	snap_point(spc, x1, y1_);
	snap_point(spc, x2, y2);
	snap_point(spc, x3, y3);
	curve_to(spc.cr, POINT{ x1, y1_ }, POINT{ x2, y2 }, POINT{ x3, y3 });
}

void spc_line_to(snap_path_closure_t& spc, double x, double y) {
	spc_new_path_perhaps(spc);
	snap_point(spc, x, y);
	line_to(spc.cr, POINT{ x, y });
}

void spc_move_to(snap_path_closure_t& spc, double x, double y) {
	spc_new_path_perhaps(spc);
	snap_point(spc, x, y);
	move_to(spc.cr, POINT{ x, y });
}

void snap_line_width(std::shared_ptr< cairo_t>& cr) {
	double x_width = line_width(cr);
	double y_width = x_width;

	user_to_device_distance(cr, x_width, y_width);

	/* If the line width is less than 1 then it will round to 0 and
	 * disappear. Instead, we clamp it to 1.0, but we must preserve
	 * its sign for the case of a reflecting transformation. */
	double x_width_snapped = floor(x_width + 0.5);
	if (fabs(x_width_snapped) < 1.0) {
		if (x_width > 0)
			x_width_snapped = 1.0;
		else
			x_width_snapped = -1.0;
	}

	double y_width_snapped = floor(y_width + 0.5);
	if (fabs(y_width_snapped) < 1.0) {
		if (y_width > 0)
			y_width_snapped = 1.0;
		else
			y_width_snapped = -1.0;
	}

	double x_error = fabs(x_width - x_width_snapped);
	double y_error = fabs(y_width - y_width_snapped);

	device_to_user_distance(cr, x_width_snapped, y_width_snapped);

	if (x_error > y_error)
		line_width(cr, x_width_snapped);
	else
		line_width(cr, y_width_snapped);
}

void snap_path_for_stroke(std::shared_ptr< cairo_t>& cr) {
	snap_path_closure_t spc{ 1,0,cr };
	snap_line_width(cr);

	auto path = copy_path(cr);

	for (int i = 0; i < path->num_data; i += path->data[i].header.length) {
		cairo_path_data_t* data = &path->data[i];
		switch (data->header.type) {
		case CAIRO_PATH_MOVE_TO:
			spc_move_to(spc, data[1].point.x, data[1].point.y);
			break;
		case CAIRO_PATH_LINE_TO:
			spc_line_to(spc, data[1].point.x, data[1].point.y);
			break;
		case CAIRO_PATH_CURVE_TO:
			spc_curve_to(spc, data[1].point.x, data[1].point.y,
				data[2].point.x, data[2].point.y,
				data[3].point.x, data[3].point.y);
			break;
		case CAIRO_PATH_CLOSE_PATH:
			spc_close_path(spc);
			break;
		}
	}
}

void draw_nested(std::shared_ptr< cairo_t>& cr, const nested_style_t& style, const snapping_t& snapping) {
#define NUM_BOXES 5
#define BOX_WIDTH 13
	/* We need non-integer scale factors to demonstrate anything interesting. */
#define SCALE_TWEAK 1.11

	double mscale, offset = SCALE_TWEAK * BOX_WIDTH / 2.0;

	save(cr);
	{
		line_width(cr, 1.0);

		for (int i = 0; i < NUM_BOXES; i++) {
			save(cr);
			{
				mscale = SCALE_TWEAK * (NUM_BOXES - i);			
				scale(cr, SCALE{ mscale, mscale });

				rectangle(cr, POINT{ 0, 0 }, SIZE{ BOX_WIDTH, BOX_WIDTH });

				if (style == NESTED_FILLS) {
					if (snapping == SNAPPING)
						snap_path_for_fill(cr);
					if (i % 2 == 0)
						source(cr, RGB{ 1, 1, 1 });
					else
						source(cr, RGB{ 0, 0, 0 });
					fill(cr);
				}
				else {
					if (snapping == SNAPPING) snap_path_for_stroke(cr);
					source(cr, RGB{ 1, 1, 1 });
					stroke(cr);
				}
			}
			restore(cr);

			translate(cr, POINT{ offset, offset });
		}
	}
	restore(cr);
}

void draw(std::shared_ptr< cairo_t>& cr, int width, int height) {
	translate(cr, POINT{ 6, 6 });
	draw_nested(cr, NESTED_FILLS, NO_SNAPPING);
	translate(cr, POINT{ width / 2.0, 0 });
	draw_nested(cr, NESTED_FILLS, SNAPPING);
	translate(cr, POINT{ -width / 2.0, height / 2.0 });
	draw_nested(cr, NESTED_STROKES, NO_SNAPPING);
	translate(cr, POINT{ width / 2.0, 0 });
	draw_nested(cr, NESTED_STROKES, SNAPPING);
}