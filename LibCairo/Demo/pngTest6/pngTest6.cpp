
#include "pch.h"
#include "cairo_util.h"

int main() {
	{
		TimeCheck time;
		constexpr int WIDTH = 450;
		constexpr int HEIGHT = 900;
		constexpr int STRIDE = WIDTH * 4;

		std::vector<unsigned char> datas(HEIGHT*STRIDE);
		auto surface = create_surface(datas, CAIRO_FORMAT_ARGB32, SURFACE_SIZE{ WIDTH, HEIGHT }, STRIDE);
		auto cr = create(surface);

		rectangle(cr, POINT{ 0,0 }, SIZE{ WIDTH ,HEIGHT });
		source(cr, RGB{ 1,1,1 });
		fill(cr);
		
		source(cr, RGB{ 0,0,0 });
		font_face(cr, u8"serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
		font_size(cr, 40);
		translate(cr, POINT{ 40, 40 });

		
		font_face(cr, u8"mono", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
		font_size(cr, 12);
		show_text(cr, u8"+CTM rotation");

		constexpr auto NUM_STRINGS = 3;
		save(cr); 
		{
			font_face(cr, u8"serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
			font_size(cr, 40);
			for (int i = 0; i < NUM_STRINGS; ++i) {
				RADIAN angle{ i * 0.5*pi<double> / double(NUM_STRINGS - 1) };
				save(cr);
				rotate(cr, angle);
				move_to(cr, POINT{ 100, 0 });
				show_text(cr, u8"Text");
				restore(cr);
			}
		}
		restore(cr);

		translate(cr, POINT{ 0, HEIGHT / 3 });
		move_to(cr, POINT{ 0, 0 });
		show_text(cr, u8"+CTM rotation");
		rel_move_to(cr, POINT{ 0, 12 });
		show_text(cr, u8"-font rotation");

		save(cr);
		font_face(cr, u8"serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
		font_size(cr, 40);
		{
			cairo_matrix_t mat{};
			for (int i = 0; i < NUM_STRINGS; ++i) {
				RADIAN angle{ i * 0.5*pi<double> / double(NUM_STRINGS - 1) };
				save(cr);
				rotate(cr, angle);
				identity(mat);
				scale(mat, SCALE{ 40,40 });
				rotate(mat, -angle);
				font_matrix(cr, mat);
				move_to(cr, POINT{ 100, 0 });
				show_text(cr, u8"Text");
				restore(cr);
			}
		}
		restore(cr);

		translate(cr, POINT{ 0, HEIGHT / 3 });
		move_to(cr, POINT{ 0, 0 });
		show_text(cr, u8"+CTM rotation");
		rel_move_to(cr, POINT{ 0, 12 });
		show_text(cr, u8"-CTM rotation");

		save(cr);
		font_face(cr, u8"serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
		font_size(cr, 40);
		{
			for (int i = 0; i < NUM_STRINGS; ++i) {
				RADIAN angle{ i * 0.5*pi<double> / double(NUM_STRINGS - 1) };
				save(cr);
				rotate(cr, angle);
				move_to(cr, POINT{ 100, 0 });
				rotate(cr, -angle);
				show_text(cr, u8"Text");
				restore(cr);
			}
		}
		restore(cr);
		
		write_to_png(surface, "image.png"); 
	}
	return 0;
}