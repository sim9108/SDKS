#pragma once
#include <iostream>
#include <memory>
#include <chrono>
#include <cairo.h>
#include <vector>
#include <string>

template<class T>
constexpr T pi = T(3.1415926535897932385L);  // variable template

struct RGBA { double red, green, blue, alpha; };
struct RGB { double red, green, blue; };
struct POINT { double x, y; };
struct REL_POINT{ double x, y; };
struct SCALE{ double sx, sy; };
struct SURFACE_SIZE { int width , height; };
struct SIZE { double width, height; };

struct RADIAN;
struct DEGRESS {
	double value_ = 0.0;
	explicit DEGRESS(double angle);
	DEGRESS(const RADIAN& angle);
	DEGRESS& operator=(const RADIAN& value);
	double value() const;
};
DEGRESS operator-(const DEGRESS& obj);
DEGRESS operator*(const double& s, const DEGRESS& obj);
DEGRESS operator*(const DEGRESS& obj, const double& s);
DEGRESS operator/(const DEGRESS& obj, const double& s);
DEGRESS operator+(const DEGRESS& left, const DEGRESS& right);

struct RADIAN { 
	double value_ = 0.0;
	explicit RADIAN(double angle);
	RADIAN(const DEGRESS& angle);
	RADIAN& operator=(const DEGRESS& value);
	double value() const;
};

RADIAN operator-(const RADIAN& obj);
RADIAN operator*(const double& s, const RADIAN& obj);
RADIAN operator*(const RADIAN& obj, const double& s);
RADIAN operator/(const RADIAN& obj, const double& s);
RADIAN operator+(const RADIAN& left, const RADIAN& right);

struct DASH{
	std::vector<double> datas;
	double offset = 0.0;
};

std::shared_ptr <cairo_surface_t> create_surface(cairo_surface_t* ptr = nullptr);
std::shared_ptr <cairo_surface_t> create_surface(cairo_format_t	format, const SURFACE_SIZE& size);
std::shared_ptr <cairo_surface_t> create_surface(std::vector<unsigned char>& data, cairo_format_t	format, const SURFACE_SIZE& size, int stride);
std::shared_ptr <cairo_surface_t> create_surface(const char	*filename);

std::shared_ptr <cairo_pattern_t> create_pattern();
std::shared_ptr <cairo_pattern_t> create_pattern(std::shared_ptr <cairo_pattern_t>& cr);
std::shared_ptr <cairo_pattern_t> create_pattern(std::shared_ptr <cairo_surface_t>& cr);
std::shared_ptr <cairo_pattern_t> create_pattern(const RGB& color);
std::shared_ptr <cairo_pattern_t> create_pattern(const RGBA& color);
std::shared_ptr <cairo_pattern_t> create_pattern(const POINT& p1, const POINT& p2);
std::shared_ptr <cairo_pattern_t> create_pattern(const POINT& c1, double r1, const POINT& c2, double r2);

std::shared_ptr<cairo_t> create(std::shared_ptr <cairo_surface_t>& sf);
std::shared_ptr<cairo_path_t> copy_path(std::shared_ptr <cairo_t>& cr);

std::shared_ptr <cairo_font_options_t> create_font_options();
std::shared_ptr <cairo_font_options_t> create_font_options(std::shared_ptr <cairo_font_options_t>& src);

int width(std::shared_ptr <cairo_surface_t>& surface);
int height(std::shared_ptr <cairo_surface_t>& surface);

cairo_status_t status(std::shared_ptr <cairo_t>& cr );
std::string status_to_string(const cairo_status_t& err);

void line_width(std::shared_ptr <cairo_t>& cr, double val);
double line_width(std::shared_ptr <cairo_t>& cr);

void line_limit(std::shared_ptr <cairo_t>& cr, double val);
void line_cap(std::shared_ptr <cairo_t>& cr, const   cairo_line_cap_t& val);
void line_join(std::shared_ptr <cairo_t>& cr, const   cairo_line_join_t& val);
void op(std::shared_ptr <cairo_t>& cr, const   cairo_operator_t& val);
void tolerance(std::shared_ptr <cairo_t>& cr, double val);

void arc(std::shared_ptr <cairo_t>& cr, const POINT& cen, double radius, const RADIAN& angle1, const RADIAN& angle2);
void arc_negative(std::shared_ptr <cairo_t>& cr, const POINT& cen, double radius, const RADIAN& angle1, const RADIAN& angle2);
void rectangle(std::shared_ptr <cairo_t>& cr, const POINT& start, const SIZE& size);

void stroke(std::shared_ptr <cairo_t>& cr);
void fill(std::shared_ptr <cairo_t>& cr);
void clip(std::shared_ptr <cairo_t>& cr);
void reset_clip(std::shared_ptr <cairo_t>& cr);
void new_path(std::shared_ptr <cairo_t>& cr);
void new_sub_path(std::shared_ptr <cairo_t>& cr);
void close_path(std::shared_ptr <cairo_t>& cr);
void fill_rule(std::shared_ptr <cairo_t>& cr, const cairo_fill_rule_t& val);
void fill_preserve(std::shared_ptr <cairo_t>& cr);
void stroke_preserve(std::shared_ptr <cairo_t>& cr);
void clip_preserve(std::shared_ptr <cairo_t>& cr);

void save(std::shared_ptr <cairo_t>& cr);
void restore(std::shared_ptr <cairo_t>& cr);
void push_group(std::shared_ptr <cairo_t>& cr);
void pop_group(std::shared_ptr <cairo_t>& cr);

void line_to(std::shared_ptr <cairo_t>& cr, const POINT& pt);
void move_to(std::shared_ptr <cairo_t>& cr, const POINT& pt);
void curve_to(std::shared_ptr <cairo_t>& cr, const POINT& p1, const POINT& p2, const POINT& p3);

void line_to(std::shared_ptr <cairo_t>& cr, const REL_POINT& pt);
void move_to(std::shared_ptr <cairo_t>& cr, const REL_POINT& pt);
void curve_to(std::shared_ptr <cairo_t>& cr, const REL_POINT& p1, const REL_POINT& p2, const REL_POINT& p3);


void rel_line_to(std::shared_ptr <cairo_t>& cr, const POINT& pt);
void rel_move_to(std::shared_ptr <cairo_t>& cr, const POINT& pt);
void rel_curve_to(std::shared_ptr <cairo_t>& cr, const POINT& p1, const POINT& p2, const POINT& p3);

void user_to_device(std::shared_ptr <cairo_t>& cr, double& x, double& y);
void user_to_device_distance(std::shared_ptr <cairo_t>& cr, double& x, double& y);
void device_to_user(std::shared_ptr <cairo_t>& cr, double& x, double& y);
void device_to_user_distance(std::shared_ptr <cairo_t>& cr, double& x, double& y);

void dash(std::shared_ptr <cairo_t>& cr, const DASH& datas);
void font_face(std::shared_ptr <cairo_t>& cr, const std::string_view& family, cairo_font_slant_t   slant,	cairo_font_weight_t  weight);
void font_size(std::shared_ptr <cairo_t>& cr, double size);
void text_extents(std::shared_ptr <cairo_t>& cr, const std::string_view& utf8, cairo_text_extents_t& extents);
void show_text(std::shared_ptr <cairo_t>& cr, const std::string_view& utf8);
void font_options(std::shared_ptr <cairo_t>& cr, const std::shared_ptr <cairo_font_options_t >& src);
void font_extents(std::shared_ptr <cairo_t>& cr, cairo_font_extents_t& extents);
void glyph_extents(std::shared_ptr <cairo_t>& cr, const std::vector<cairo_glyph_t>& glyphs, cairo_text_extents_t& extents);
void glyph_extents(std::shared_ptr <cairo_t>& cr, const cairo_glyph_t& glyph, cairo_text_extents_t& extents);

void show_glyphs(std::shared_ptr <cairo_t>& cr, const std::vector<cairo_glyph_t>& glyphs);
void show_glyphs(std::shared_ptr <cairo_t>& cr, const cairo_glyph_t& glyph);

void glyph_path(std::shared_ptr <cairo_t>& cr, std::vector<cairo_glyph_t>& glyphs);
void glyph_path(std::shared_ptr <cairo_t>& cr, cairo_glyph_t& glyph);


void text_path(std::shared_ptr <cairo_t>& cr, const std::string_view& utf8);

void init(cairo_matrix_t& mat,	double  xx, double  yx,
													double  xy, double  yy,
													double  x0, double  y0);

void init_translate(cairo_matrix_t& mat, const POINT& val);
void init_scale(cairo_matrix_t& mat, const SCALE& val);
void init_rotate(cairo_matrix_t& mat, const RADIAN& val);


DASH dash(std::shared_ptr <cairo_t>& cr);

void begin_path(std::shared_ptr <cairo_pattern_t>& cr);
void end_path(std::shared_ptr <cairo_pattern_t>& cr);
void move_to(std::shared_ptr <cairo_pattern_t>& cr, const POINT& pt);
void line_to(std::shared_ptr <cairo_pattern_t>& cr, const POINT& pt);
void curve_to(std::shared_ptr <cairo_pattern_t>& cr, const POINT& p1, const POINT& p2, const POINT& p3);
void filter(std::shared_ptr <cairo_pattern_t > & cr, const cairo_filter_t& val);
void extend(std::shared_ptr <cairo_pattern_t > & cr, const cairo_extend_t& val);
void corner_color(std::shared_ptr <cairo_pattern_t > & cr, unsigned int corner_num, const RGB& color);
void corner_color(std::shared_ptr <cairo_pattern_t > & cr, unsigned int corner_num, const RGBA& color);
void control_point(std::shared_ptr <cairo_pattern_t > & cr, unsigned int corner_num, const POINT& pt);
void add_color_stop(std::shared_ptr <cairo_pattern_t > & cr, double offset, const RGB& color);
void add_color_stop(std::shared_ptr <cairo_pattern_t > & cr, double offset, const RGBA& color);

void matrix(std::shared_ptr <cairo_pattern_t > & cr, const cairo_matrix_t& mat);

void translate(std::shared_ptr <cairo_t>& cr, const POINT& pt);
void translate(cairo_matrix_t& mat, const POINT& val);
void scale(std::shared_ptr <cairo_t>& cr, const SCALE& scale);
void scale(cairo_matrix_t& mat, const SCALE& val);
void rotate(std::shared_ptr <cairo_t>& cr, const RADIAN& angle);
void rotate(cairo_matrix_t& mat, const RADIAN& val);

void identity(std::shared_ptr <cairo_t>& cr);
void identity(cairo_matrix_t& mat);
void transform(std::shared_ptr <cairo_t>& cr, cairo_matrix_t& mat);

void font_matrix(std::shared_ptr <cairo_t>& cr, cairo_matrix_t& mat);

void source(std::shared_ptr <cairo_t>& cr, std::shared_ptr <cairo_surface_t>& src, const POINT& pt);
void source(std::shared_ptr <cairo_t>& cr, const RGBA& color);
void source(std::shared_ptr <cairo_t>& cr, const RGB& color);
void source(std::shared_ptr <cairo_t>& cr, std::shared_ptr <cairo_pattern_t>& src);

void paint(std::shared_ptr <cairo_t>& cr);
void paint(std::shared_ptr <cairo_t>& cr, double alpha);
int write_to_png(std::shared_ptr <cairo_surface_t>& surface, const char* filename);

struct TimeCheck {
	~TimeCheck();
private:
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
};