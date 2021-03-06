
#include "pch.h"
#include <vector>
#include "cairo_util.h"

void box_text(std::shared_ptr <cairo_t>& cr, const char *utf8, double x, double y);
void box_glyphs(std::shared_ptr <cairo_t>& cr, std::vector<cairo_glyph_t>& glyphs, double x, double y);

int main() {
	{
		TimeCheck time;

		constexpr char TEXT[] = u8"hello, world";

		constexpr int WIDTH = 450;
		constexpr int HEIGHT = 600;
		constexpr int STRIDE = WIDTH * 4;

		std::vector<unsigned char> datas(HEIGHT*STRIDE);
		auto surface = create_surface(datas, CAIRO_FORMAT_ARGB32, SURFACE_SIZE{ WIDTH, HEIGHT }, STRIDE);
		auto cr = create(surface);

		source(cr, RGB{ 0,0,0 });
		line_width(cr, 2.0);

		save(cr);
		{
			rectangle(cr, POINT{ 0,0 }, SIZE{ WIDTH, HEIGHT });
			source(cr, RGBA{ 0,0,0,0 });
			op(cr, CAIRO_OPERATOR_SOURCE);
			fill(cr);
		}
		restore(cr);

		font_face(cr, u8"sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
		font_size(cr, 40);
		
		cairo_text_extents_t extents;
		std::vector<cairo_glyph_t> glyphs{10 };

		double dx{ 0 }, dy{ 0 };
		int i{ 0 };
		for (auto&& item : glyphs) {
			item.x = dx;
			item.y = dy;
			item.index = i + 4;
			glyph_extents(cr, item, extents);
			dx += extents.x_advance;
			dy += extents.y_advance;
			++i;
		}

		cairo_font_extents_t f_extents;
		font_extents(cr, f_extents);
		auto height{ f_extents.height };

		box_text(cr, TEXT, 10, height);

		translate(cr, POINT{ 0, height });
		save(cr);
		{
			translate(cr, POINT{ 10, height });
			rotate(cr, DEGRESS{ 10 });
			box_text(cr, TEXT, 10, 0);
		}
		restore(cr);

		translate(cr, POINT{ 0, 2 * height });
		save(cr);
		{
			cairo_matrix_t matrix;
			identity(matrix);
			scale(matrix, SCALE{ 40, -40 });
			rotate(matrix, DEGRESS{ -10 });
			font_matrix(cr, matrix);
		}
		box_text(cr, TEXT, 10, height);
		restore(cr);

		translate(cr, POINT{ 0, 2 * height });
		box_glyphs(cr, glyphs, 10, height);

		translate(cr, POINT{ 10, 2 * height });
		save(cr);
		{
			rotate(cr, DEGRESS{ 10 });
			box_glyphs(cr, glyphs,  0, 0);
		}
		restore(cr);

		translate(cr, POINT{ 0, 2*height });
		i = 0;
		for (auto&& item : glyphs) {
			item.y += i *5;
			++i;
		}
		box_glyphs(cr, glyphs, 10, height);

		write_to_png(surface, "image.png"); 
	}
	return 0;
}

void box_text(std::shared_ptr <cairo_t>& cr, const char *utf8, double x, double y) {
	save(cr);
	{
		cairo_text_extents_t extents;
		text_extents(cr, utf8, extents);
		double linewidth = line_width(cr);
		rectangle(cr,
			POINT{ x + extents.x_bearing - linewidth,y + extents.y_bearing - linewidth },
			SIZE{ extents.width + 2 * linewidth,extents.height + 2 * linewidth }
		);
		stroke(cr);

		move_to(cr, POINT{ x, y });
		show_text(cr, utf8);
		move_to(cr, POINT{ x, y });
		text_path(cr, utf8);
		source(cr, RGB{ 1, 0, 0 });
		line_width(cr, 1.0);
		stroke(cr);
	}
	restore(cr);
}

void box_glyphs(std::shared_ptr <cairo_t>& cr, std::vector<cairo_glyph_t>& glyphs, double x, double y) {
	save(cr);
	{
		cairo_text_extents_t extents;
		glyph_extents(cr, glyphs, extents);
		auto linewidth = line_width(cr);
		rectangle(cr,
			POINT{ x + extents.x_bearing - linewidth, y + extents.y_bearing - linewidth },
			SIZE{ extents.width + 2 * linewidth, 	extents.height + 2 * linewidth });
		stroke(cr);

		for (auto&& mitem : glyphs) {
			mitem.x += x;
			mitem.y += y;
		}

		show_glyphs(cr, glyphs);
		glyph_path(cr, glyphs);
		source(cr, RGB{ 1, 0, 0 });
		line_width(cr, 1.0);
		stroke(cr);
		for (auto&& mitem : glyphs) {
			mitem.x -= x;
			mitem.y -= y;
		}
	}
	restore(cr);
}