
#include "pch.h"
#include <vector>
#include "cairo_util.h"

struct  hex_color_t{ uint8_t r, g, b;} ;
enum checked_status_t { UNCHECKED, CHECKED };

hex_color_t BG_COLOR = { 0xd4, 0xd0, 0xc8 };
hex_color_t HI_COLOR_1 = { 0xff, 0xff, 0xff };
hex_color_t HI_COLOR_2 = { 0xd4, 0xd0, 0xc8 };
hex_color_t LO_COLOR_1 = { 0x80, 0x80, 0x80 };
hex_color_t LO_COLOR_2 = { 0x40, 0x40, 0x40 };
hex_color_t BLACK = { 0, 0, 0 };

#define CHECK_BOX_SIZE 13
#define CHECK_COLOR BLACK
#define RADIO_SIZE CHECK_BOX_SIZE
#define RADIO_DOT_COLOR BLACK

void set_hex_color(std::shared_ptr< cairo_t>& cr, const  hex_color_t& color);
void draw_bevels(std::shared_ptr< cairo_t>& cr, int width, int height);
void groovy_box(std::shared_ptr< cairo_t>& cr, int x, int y, int width, int height);
void check_box(std::shared_ptr< cairo_t>& cr, int x, int y, checked_status_t checked);
void bevel_box(std::shared_ptr< cairo_t>& cr, int x, int y, int width, int height);
void flat_box(std::shared_ptr< cairo_t>& cr, int x, int y, int width, int height);
void radio_button(std::shared_ptr< cairo_t>& cr, int x, int y, checked_status_t checked);
void flat_circle(std::shared_ptr< cairo_t>& cr, int x, int y, int width);
void bevel_circle(std::shared_ptr< cairo_t>& cr, int x, int y, int width);

int main() {
	{
		TimeCheck time;

		constexpr int WIDTH = 100;
		constexpr int HEIGHT = 70;
		constexpr int STRIDE = WIDTH * 4;

		std::vector<unsigned char> datas(HEIGHT*STRIDE);
		auto surface = create_surface(datas, CAIRO_FORMAT_ARGB32, SURFACE_SIZE{ WIDTH, HEIGHT }, STRIDE);
		auto cr = create(surface);

		rectangle(cr, POINT{ 0,0 }, SIZE{ WIDTH, HEIGHT });
		set_hex_color(cr, BG_COLOR);
		fill(cr);

		draw_bevels(cr, WIDTH, HEIGHT);

		write_to_png(surface, "image.png"); 
	}
	return 0;
}

void set_hex_color(std::shared_ptr< cairo_t>& cr, const  hex_color_t& color) {
	source(cr, RGB{ color.r / 255.0, color.g / 255.0, color.b / 255.0 });
}

void bevel_circle(std::shared_ptr< cairo_t>& cr, int x, int y, int width) {
	double radius = (width - 1) / 2.0 - 0.5;

	save(cr);
	{
		line_width(cr, 1);

		/* Fill and Highlight */
		set_hex_color(cr, HI_COLOR_1);
		arc(cr, POINT{ x + radius + 1.5, y + radius + 1.5 }, radius,
			RADIAN{ 0 }, RADIAN{ 2 * pi<double> });
		fill(cr);

		/* 2nd highlight */
		set_hex_color(cr, HI_COLOR_2);
		arc(cr, POINT{ x + radius + 0.5, y + radius + 0.5 }, radius,
			RADIAN{ 0 }, RADIAN{ 2 * pi<double> });
		stroke(cr);

		/* 1st lowlight */
		set_hex_color(cr, LO_COLOR_1);
		arc(cr, POINT{ x + radius + 0.5, y + radius + 0.5 }, radius,
			RADIAN{ 3 * pi<double>/4.0 }, RADIAN{ 7 * pi<double> / 4.0 });
		stroke(cr);

		/* 2nd lowlight */
		set_hex_color(cr, LO_COLOR_2);
		arc(cr, POINT{ x + radius + 1.5, y + radius + 1.5 }, radius,
			RADIAN{ 3 * pi<double> / 4.0 }, RADIAN{ 7 * pi<double> / 4.0 });
		stroke(cr);
	}
	restore(cr);
}

void flat_circle(std::shared_ptr< cairo_t>& cr, int x, int y, int width) {
	double radius = (width - 1) / 2.0;

	save(cr);
	{
		/* Fill background */
		set_hex_color(cr, HI_COLOR_1);
		arc(cr, POINT{ x + radius + 0.5, y + radius + 0.5 }, radius - 1,
			RADIAN{ 0 }, RADIAN{ 2 * pi<double> });
		fill(cr);

		/* Fill background */
		line_width(cr, 1.0);
		set_hex_color(cr, BLACK);
		arc(cr, POINT{ x + radius + 0.5, y + radius + 0.5 }, radius - 1,
			RADIAN{ 0 }, RADIAN{ 2 * pi<double> });
		stroke(cr);
	}	
	restore(cr);
}

void radio_button(std::shared_ptr< cairo_t>& cr, int x, int y, checked_status_t checked) {
	save(cr);
	do{
		bevel_circle(cr, x, y, RADIO_SIZE);
		if (!checked)  break;
		
		set_hex_color(cr, RADIO_DOT_COLOR);
		arc(cr,
			POINT{ x + (RADIO_SIZE - 1) / 2.0 + 0.5,	y + (RADIO_SIZE - 1) / 2.0 + 0.5 },
			(RADIO_SIZE - 1) / 2.0 - 3.5 ,
			RADIAN{ 0 }, RADIAN{ 2*pi<double> });
		fill(cr);
	} while (false);
	restore(cr);
}

void flat_box(std::shared_ptr< cairo_t>& cr, int x, int y, int width, int height) {
	save(cr);
	{
		/* Fill background */
		set_hex_color(cr, HI_COLOR_1);
		rectangle(cr, POINT{ x + 1.0, y + 1.0 }, SIZE{ width - 2.0, height - 2.0 });
		fill(cr);

		/* Stroke outline */
		line_width(cr, 1.0);
		set_hex_color(cr, BLACK);
		rectangle(cr, POINT{ x + 1.5, y + 1.5 }, SIZE{width - 3.0, height - 3.0 });
		stroke(cr);
	}
	restore(cr);
}

void bevel_box(std::shared_ptr< cairo_t>& cr, int x, int y, int width, int height) {
	save(cr);
	{
		line_width(cr, 1.0);
		line_cap(cr, CAIRO_LINE_CAP_SQUARE);

		/* Fill and highlight */
		set_hex_color(cr, HI_COLOR_1);
		rectangle(cr, POINT{ double(x), double(y) }, SIZE{ double(width), double(height) });
		fill(cr);

		/* 2nd highlight */
		set_hex_color(cr, HI_COLOR_2);
		move_to(cr, POINT{ x + 1.5, y + height - 1.5 });
		rel_line_to(cr, POINT{ width - 3.0, 0 });
		rel_line_to(cr, POINT{ 0, -(height - 3.0) });
		stroke(cr);

		/* 1st lowlight */
		set_hex_color(cr, LO_COLOR_1);
		move_to(cr, POINT{ x + .5, y + height - 1.5 });
		rel_line_to(cr, POINT{ 0, -(height - 2.0) });
		rel_line_to(cr, POINT{ width - 2.0, 0 });
		stroke(cr);

		/* 2nd lowlight */
		set_hex_color(cr, LO_COLOR_2);
		move_to(cr, POINT{ x + 1.5, y + height - 2.5 });
		rel_line_to(cr, POINT{ 0, -(height - 4.0) });
		rel_line_to(cr, POINT{ width - 4.0, 0 });
		stroke(cr);
	}	
	restore(cr);
}

void check_box(std::shared_ptr< cairo_t>& cr, int x, int y, checked_status_t checked) {
	save(cr);
	do{
		bevel_box(cr, x, y, CHECK_BOX_SIZE, CHECK_BOX_SIZE);
		if (!checked)  break;
		
		set_hex_color(cr, CHECK_COLOR);
		move_to(cr, POINT{ x + 3.0, y + 5.0 });
		rel_line_to(cr, POINT{ 2.5, 2 });
		rel_line_to(cr, POINT{ 4.5, -4 });
		rel_line_to(cr, POINT{ 0, 3 });
		rel_line_to(cr, POINT{ -4.5, 4 });		
		rel_line_to(cr, POINT{ -2.5, -2 });

		close_path(cr);
		fill(cr);
	} while (false);
	restore(cr);
}

void groovy_box(std::shared_ptr< cairo_t>& cr, int x, int y, int width, int height) {
	save(cr);
	{
		/* Highlight */
		set_hex_color(cr, HI_COLOR_1);
		line_width(cr, 2);
		rectangle(cr, POINT{ x + 1.0, y + 1.0 }, SIZE{ width - 2.0, height - 2.0 });
		stroke(cr);

		/* Lowlight */
		set_hex_color(cr, LO_COLOR_1);
		line_width(cr, 1);
		rectangle(cr, POINT{ x + 0.5, y + 0.5 }, SIZE{ width - 2.0, height - 2.0 });
		stroke(cr);
	}
	restore(cr);
}

void draw_bevels(std::shared_ptr< cairo_t>& cr, int width, int height) {
	int check_room = (width - 20) / 3;
	int check_pad = (check_room - CHECK_BOX_SIZE) / 2;

	groovy_box(cr, 5, 5, width - 10, height - 10);

	check_box(cr, 10 + check_pad, 10 + check_pad, UNCHECKED);
	check_box(cr, check_room + 10 + check_pad, 10 + check_pad, CHECKED);

	flat_box(cr, 2 * check_room + 10 + check_pad, 10 + check_pad,	CHECK_BOX_SIZE, CHECK_BOX_SIZE);

	radio_button(cr, 10 + check_pad, check_room + 10 + check_pad, UNCHECKED);
	radio_button(cr, check_room + 10 + check_pad, check_room + 10 + check_pad, CHECKED);
	flat_circle(cr, 2 * check_room + 10 + check_pad, check_room + 10 + check_pad, CHECK_BOX_SIZE);
}