#include "cairo_util.h"


DEGRESS::DEGRESS(const RADIAN& angle)
	:value_{ angle.value()*180.0 / pi<double> }
{
}

RADIAN::RADIAN(const DEGRESS& angle) 
	: value_{ angle.value() * pi<double> / 180.0 }
{
}
DEGRESS::DEGRESS(double angle) : value_{ angle } {}

RADIAN::RADIAN(double angle) : value_{ angle } {}


DEGRESS& DEGRESS::operator=(const RADIAN& angle) {
	value_ = angle.value()*180.0 / pi<double>;
	return *this;
}

RADIAN operator-(const RADIAN& obj) {
	return RADIAN{ -obj.value() };
}

RADIAN operator*(const double& s, const RADIAN& obj) {
	return RADIAN{ s*obj.value() };
}

RADIAN operator*(const RADIAN& obj, const double& s) {
	return RADIAN{ s*obj.value() };
}

RADIAN operator/(const RADIAN& obj, const double& s) {
	return RADIAN{ obj.value()/ s };
}

DEGRESS operator-(const DEGRESS& obj) {
	return DEGRESS{ -obj.value() };
}

DEGRESS operator*(const double& s, const DEGRESS& obj) {
	return DEGRESS{ s*obj.value() };
}

DEGRESS operator*(const DEGRESS& obj, const double& s) {
	return DEGRESS{ s*obj.value() };
}

DEGRESS operator/(const DEGRESS& obj, const double& s) {
	return DEGRESS{obj.value()/ s };
}

DEGRESS operator+(const DEGRESS& left, const DEGRESS& right) {
	return DEGRESS{ left.value() +right.value() };
}

RADIAN operator+(const RADIAN& left, const RADIAN& right) {
	return RADIAN{ left.value() + right.value() };
}

RADIAN& RADIAN::operator=(const DEGRESS& angle) {
	value_ = angle.value() * pi<double> / 180.0;
	return *this;
}

double RADIAN::value() const {
	return value_;
}

double DEGRESS::value() const {
	return value_;
}



std::shared_ptr <cairo_surface_t> create_surface(cairo_surface_t* ptr) {
	return std::shared_ptr < cairo_surface_t>(ptr, [](auto p) {cairo_surface_destroy(p); });
}

std::shared_ptr <cairo_pattern_t> create_pattern() {
	return std::shared_ptr < cairo_pattern_t>(cairo_pattern_create_mesh(), [](auto p) { cairo_pattern_destroy(p); });
}

std::shared_ptr <cairo_pattern_t> create_pattern(std::shared_ptr <cairo_pattern_t>& cr) {
	return std::shared_ptr < cairo_pattern_t>(cairo_pattern_reference(cr.get()), [](auto p) { cairo_pattern_destroy(p); });
}

std::shared_ptr <cairo_font_options_t> create_font_options() {
	return std::shared_ptr < cairo_font_options_t>(cairo_font_options_create(), [](auto p) { cairo_font_options_destroy(p); });
}

std::shared_ptr <cairo_font_options_t> create_font_options(std::shared_ptr <cairo_font_options_t>& src) {
	return std::shared_ptr < cairo_font_options_t>(cairo_font_options_copy(src.get()), [](auto p) { cairo_font_options_destroy(p); });
}

void filter(std::shared_ptr <cairo_pattern_t > & cr, const cairo_filter_t& val) {
	cairo_pattern_set_filter(cr.get(), val);
}

void extend(std::shared_ptr <cairo_pattern_t > & cr, const cairo_extend_t& val) {
	cairo_pattern_set_extend(cr.get(), val);
}

void corner_color(std::shared_ptr <cairo_pattern_t > & cr, unsigned int corner_num, const RGB& color) {
	cairo_mesh_pattern_set_corner_color_rgb(cr.get(), corner_num, color.red, color.green, color.blue);
}

void control_point(std::shared_ptr <cairo_pattern_t > & cr, unsigned int corner_num, const POINT& pt) {
	cairo_mesh_pattern_set_control_point(cr.get(), corner_num, pt.x, pt.y);
}

void add_color_stop(std::shared_ptr <cairo_pattern_t > & cr, double offset, const RGB& color) {
	cairo_pattern_add_color_stop_rgb(cr.get(), offset, color.red, color.green, color.blue);
}

void add_color_stop(std::shared_ptr <cairo_pattern_t > & cr, double offset, const RGBA& color) {
	cairo_pattern_add_color_stop_rgba(cr.get(), offset, color.red, color.green, color.blue, color.alpha);
}

void corner_color(std::shared_ptr <cairo_pattern_t > & cr, unsigned int corner_num, const RGBA& color) {
	cairo_mesh_pattern_set_corner_color_rgba(cr.get(), corner_num, color.red, color.green, color.blue, color.alpha);
}

std::shared_ptr <cairo_pattern_t> create_pattern(std::shared_ptr <cairo_surface_t>& cr) {
	return std::shared_ptr < cairo_pattern_t>(cairo_pattern_create_for_surface(cr.get()), [](auto p) { cairo_pattern_destroy(p); });
}

std::shared_ptr <cairo_pattern_t> create_pattern(const RGB& color) {
	return std::shared_ptr < cairo_pattern_t>(cairo_pattern_create_rgb(color.red, color.green, color.blue), [](auto p) { cairo_pattern_destroy(p); });
}

std::shared_ptr <cairo_pattern_t> create_pattern(const RGBA& color) {
	return std::shared_ptr < cairo_pattern_t>(cairo_pattern_create_rgba(color.red, color.green, color.blue, color.alpha), [](auto p) { cairo_pattern_destroy(p); });
}

std::shared_ptr <cairo_pattern_t> create_pattern(const POINT& p1, const POINT& p2) {
	return std::shared_ptr < cairo_pattern_t>(cairo_pattern_create_linear(p1.x, p1.y, p2.x, p2.y), [](auto p) { cairo_pattern_destroy(p); });
}

std::shared_ptr <cairo_pattern_t> create_pattern(const POINT& c1, double r1, const POINT& c2, double r2) {
	return std::shared_ptr < cairo_pattern_t>(cairo_pattern_create_radial(c1.x, c1.y,r1, c2.x, c2.y, r2), [](auto p) { cairo_pattern_destroy(p); });
}

std::shared_ptr <cairo_surface_t> create_surface(cairo_format_t	format, const SURFACE_SIZE& size) {
	auto ptr = cairo_image_surface_create(format, size.width, size.height);
	return create_surface(ptr);
}

std::shared_ptr <cairo_surface_t> create_surface(std::vector<unsigned char>& data, cairo_format_t	format, const SURFACE_SIZE& size, int stride) {
	auto ptr = cairo_image_surface_create_for_data(data.data(), format, size.width, size.height, stride);
	return create_surface(ptr);
}

std::shared_ptr <cairo_surface_t> create_surface(const char	*filename) {
	auto ptr = cairo_image_surface_create_from_png(filename);
	return create_surface(ptr);
}

std::shared_ptr<cairo_t> create(std::shared_ptr <cairo_surface_t>& sf) {
	return std::shared_ptr<cairo_t>(cairo_create(sf.get()), [](auto p) {cairo_destroy(p); });
}

std::shared_ptr<cairo_path_t> copy_path(std::shared_ptr <cairo_t>& cr) {
	return std::shared_ptr<cairo_path_t>(cairo_copy_path(cr.get()), [](auto p) {cairo_path_destroy(p); });

}

int width(std::shared_ptr <cairo_surface_t>& surface) {
	return cairo_image_surface_get_width(surface.get());
}

int height(std::shared_ptr <cairo_surface_t>& surface) {
	return cairo_image_surface_get_height(surface.get());
}

void line_width(std::shared_ptr <cairo_t>& cr, double val) {
	cairo_set_line_width(cr.get(), val);
}

double line_width(std::shared_ptr <cairo_t>& cr) {
	return cairo_get_line_width(cr.get());
}

void line_limit(std::shared_ptr <cairo_t>& cr, double val) {
	cairo_set_miter_limit(cr.get(), val);
}

void tolerance(std::shared_ptr <cairo_t>& cr, double val) {
	cairo_set_tolerance(cr.get(), val);
}

void arc(std::shared_ptr <cairo_t>& cr, const POINT& cen,	double radius, const RADIAN& angle1, const RADIAN& angle2) {
	cairo_arc(cr.get(), cen.x, cen.y, radius, angle1.value(), angle2.value());
}

void arc_negative(std::shared_ptr <cairo_t>& cr, const POINT& cen, double radius, const RADIAN& angle1, const RADIAN& angle2) {
	cairo_arc_negative(cr.get(), cen.x, cen.y, radius, angle1.value(), angle2.value());
}

void rectangle(std::shared_ptr <cairo_t>& cr, const POINT& start, const SIZE& size) {
	cairo_rectangle(cr.get(), start.x, start.y, size.width, size.height);
}

void line_to(std::shared_ptr <cairo_t>& cr, const POINT& pt) {
	cairo_line_to(cr.get(),pt.x,pt.y);
}

void begin_path(std::shared_ptr <cairo_pattern_t>& cr) {
	cairo_mesh_pattern_begin_patch(cr.get());
}

void end_path(std::shared_ptr <cairo_pattern_t>& cr) {
	cairo_mesh_pattern_end_patch(cr.get());
}

void move_to(std::shared_ptr <cairo_pattern_t>& cr, const POINT& pt) {
	cairo_mesh_pattern_move_to(cr.get(), pt.x, pt.y);
}

void line_to(std::shared_ptr <cairo_pattern_t>& cr, const POINT& pt) {
	cairo_mesh_pattern_line_to(cr.get(), pt.x, pt.y);
}

void move_to(std::shared_ptr <cairo_t>& cr, const POINT& pt) {
	cairo_move_to(cr.get(), pt.x, pt.y);
}

void curve_to(std::shared_ptr <cairo_t>& cr, const POINT& p1, const POINT& p2, const POINT& p3) {
	cairo_curve_to(cr.get(), p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
}

void curve_to(std::shared_ptr <cairo_pattern_t>& cr, const POINT& p1, const POINT& p2, const POINT& p3) {
	cairo_mesh_pattern_curve_to(cr.get(), p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
}

void rel_line_to(std::shared_ptr <cairo_t>& cr, const POINT& pt) {
	cairo_rel_line_to(cr.get(), pt.x, pt.y);
}

void rel_move_to(std::shared_ptr <cairo_t>& cr, const POINT& pt) {
	cairo_rel_move_to(cr.get(), pt.x, pt.y);
}

void rel_curve_to(std::shared_ptr <cairo_t>& cr, const POINT& p1, const POINT& p2, const POINT& p3) {
	cairo_rel_curve_to(cr.get(), p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
}

void line_to(std::shared_ptr <cairo_t>& cr, const REL_POINT& pt) {
	cairo_rel_line_to(cr.get(), pt.x, pt.y);
}

void move_to(std::shared_ptr <cairo_t>& cr, const REL_POINT& pt) {
	cairo_rel_move_to(cr.get(), pt.x, pt.y);
}

void curve_to(std::shared_ptr <cairo_t>& cr, const REL_POINT& p1, const REL_POINT& p2, const REL_POINT& p3) {
	cairo_rel_curve_to(cr.get(), p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
}

void dash(std::shared_ptr <cairo_t>& cr, const DASH& val) {
	cairo_set_dash(cr.get(), &val.datas[0], val.datas.size(), val.offset);
}

DASH dash(std::shared_ptr <cairo_t>& cr) {
	auto size= cairo_get_dash_count(cr.get());
	DASH val{ std::vector<double>(size), 0.0 };
	cairo_get_dash(cr.get(), &val.datas[0], &val.offset);
	return val;
}


void stroke(std::shared_ptr <cairo_t>& cr) {
	cairo_stroke(cr.get());
}

void fill(std::shared_ptr <cairo_t>& cr) {
	cairo_fill(cr.get());
}

void clip(std::shared_ptr <cairo_t>& cr) {
	cairo_clip(cr.get());
}

void reset_clip(std::shared_ptr <cairo_t>& cr) {
	cairo_reset_clip(cr.get());
}

void fill_preserve(std::shared_ptr <cairo_t>& cr) {
	cairo_fill_preserve(cr.get());
}

void stroke_preserve(std::shared_ptr <cairo_t>& cr) {
	cairo_stroke_preserve(cr.get());
}

void clip_preserve(std::shared_ptr <cairo_t>& cr) {
	cairo_clip_preserve(cr.get());
}

void fill_rule(std::shared_ptr <cairo_t>& cr, const cairo_fill_rule_t& val) {
	cairo_set_fill_rule(cr.get(), val);
}

void line_cap(std::shared_ptr <cairo_t>& cr, const   cairo_line_cap_t& val) {
	cairo_set_line_cap(cr.get(), val);
}

void line_join(std::shared_ptr <cairo_t>& cr, const   cairo_line_join_t& val) {
	cairo_set_line_join(cr.get(), val);
}

void op(std::shared_ptr <cairo_t>& cr, const   cairo_operator_t& val) {
	cairo_set_operator(cr.get(), val);
}

void new_path(std::shared_ptr <cairo_t>& cr) {
	cairo_new_path(cr.get());
}

void new_sub_path(std::shared_ptr <cairo_t>& cr) {
	cairo_new_sub_path(cr.get());
}

void close_path(std::shared_ptr <cairo_t>& cr) {
	cairo_close_path(cr.get());
}

void translate(std::shared_ptr <cairo_t>& cr, const POINT& pt) {
	cairo_translate(cr.get(), pt.x, pt.y);
}

void scale(std::shared_ptr <cairo_t>& cr, const SCALE& scale) {
	cairo_scale(cr.get(), scale.sx, scale.sy);
}

void identity(std::shared_ptr <cairo_t>& cr) {
	cairo_identity_matrix(cr.get());
}

void rotate(std::shared_ptr <cairo_t>& cr, const RADIAN& angle) {
	cairo_rotate(cr.get(), angle.value());
}

void source(std::shared_ptr <cairo_t>& cr, std::shared_ptr <cairo_surface_t>& src, const POINT& pt) {
	cairo_set_source_surface(cr.get(), src.get(), pt.x, pt.y);
}

void source(std::shared_ptr <cairo_t>& cr, const RGBA& color) {
	cairo_set_source_rgba(cr.get(), color.red, color.green, color.blue, color.alpha);
}

void source(std::shared_ptr <cairo_t>& cr, const RGB& color) {
	cairo_set_source_rgb(cr.get(), color.red, color.green, color.blue);
}

void source(std::shared_ptr <cairo_t>& cr, std::shared_ptr <cairo_pattern_t>& src) {
	cairo_set_source(cr.get(), src.get());
}

void paint(std::shared_ptr <cairo_t>& cr) {
	cairo_paint(cr.get());
}

void paint(std::shared_ptr <cairo_t>& cr, double alpha) {
	cairo_paint_with_alpha(cr.get(), alpha);
}

int write_to_png(std::shared_ptr <cairo_surface_t>& surface, const char* filename) {
	return cairo_surface_write_to_png(surface.get(), filename);
}


void font_face(std::shared_ptr <cairo_t>& cr, const std::string_view& family, cairo_font_slant_t   slant, cairo_font_weight_t  weight) {
	cairo_select_font_face(cr.get(), family.data(), slant, weight);
}

void font_size(std::shared_ptr <cairo_t>& cr, double size) {
	cairo_set_font_size(cr.get(), size);
}

void text_extents(std::shared_ptr <cairo_t>& cr, const std::string_view& utf8, cairo_text_extents_t& extents) {
	cairo_text_extents(cr.get(), utf8.data(), &extents);
}

void show_text(std::shared_ptr <cairo_t>& cr, const std::string_view& utf8) {
	cairo_show_text(cr.get(), utf8.data());
}

void font_options(std::shared_ptr <cairo_t>& cr, const std::shared_ptr <cairo_font_options_t >& src) {
	cairo_set_font_options(cr.get(), src.get());
}

void font_extents(std::shared_ptr <cairo_t>& cr, cairo_font_extents_t& extents) {
	cairo_font_extents(cr.get(), &extents);
}

void glyph_extents(std::shared_ptr <cairo_t>& cr, const std::vector<cairo_glyph_t>& glyphs, cairo_text_extents_t& extents) {
	cairo_glyph_extents(cr.get(),glyphs.data(), glyphs.size(), &extents);
}

void show_glyphs(std::shared_ptr <cairo_t>& cr, const std::vector<cairo_glyph_t>& glyphs) {
	cairo_show_glyphs(cr.get(), glyphs.data(), glyphs.size());
}

void show_glyphs(std::shared_ptr <cairo_t>& cr, const cairo_glyph_t& glyph) {
	cairo_show_glyphs(cr.get(),&glyph, 1);
}

void glyph_path(std::shared_ptr <cairo_t>& cr, std::vector<cairo_glyph_t>& glyphs) {
	cairo_glyph_path(cr.get(), glyphs.data(), glyphs.size());
}

void glyph_path(std::shared_ptr <cairo_t>& cr, cairo_glyph_t& glyph) {
	cairo_glyph_path(cr.get(), &glyph, 1);
}

void glyph_extents(std::shared_ptr <cairo_t>& cr, const cairo_glyph_t& glyph, cairo_text_extents_t& extents) {
	cairo_glyph_extents(cr.get(), &glyph,1, &extents);
}

void text_path(std::shared_ptr <cairo_t>& cr, const std::string_view& utf8) {
	cairo_text_path(cr.get(), utf8.data());
}

void init(cairo_matrix_t& mat, double  xx, double  yx,
	double  xy, double  yy,
	double  x0, double  y0) {
	cairo_matrix_init(&mat, xx, yx, xy, yy, x0, y0);
}

void identity(cairo_matrix_t& mat) {
	cairo_matrix_init_identity(&mat);
}

void font_matrix(std::shared_ptr <cairo_t>& cr, cairo_matrix_t& mat) {
	cairo_set_font_matrix(cr.get(), &mat);
}

void init_translate(cairo_matrix_t& mat, const POINT& val) {
	cairo_matrix_init_translate(&mat, val.x, val.y);
}

void init_scale(cairo_matrix_t& mat, const SCALE& val) {
	cairo_matrix_init_scale(&mat, val.sx, val.sy);
}

void init_rotate(cairo_matrix_t& mat, const RADIAN& val) {
	cairo_matrix_init_rotate(&mat, val.value());
}

void translate(cairo_matrix_t& mat, const POINT& val) {
	cairo_matrix_translate(&mat, val.x, val.y);
}

void scale(cairo_matrix_t& mat, const SCALE& val) {
	cairo_matrix_scale(&mat, val.sx, val.sy);
}

void rotate(cairo_matrix_t& mat, const RADIAN& val) {
	cairo_matrix_rotate(&mat, val.value());
}

void matrix(std::shared_ptr <cairo_pattern_t > & cr, const cairo_matrix_t& mat) {
	cairo_pattern_set_matrix(cr.get(), &mat);
}

void save(std::shared_ptr <cairo_t>& cr) {
	cairo_save(cr.get());
}

void restore(std::shared_ptr <cairo_t>& cr) {
	cairo_restore(cr.get());
}

void push_group(std::shared_ptr <cairo_t>& cr) {
	cairo_push_group(cr.get());
}

void pop_group(std::shared_ptr <cairo_t>& cr) {
	cairo_pop_group(cr.get());
}

cairo_status_t status(std::shared_ptr <cairo_t>& cr) {
	return cairo_status(cr.get());
}

std::string status_to_string(const cairo_status_t& err) {
	return cairo_status_to_string(err);
}

void transform(std::shared_ptr <cairo_t>& cr, cairo_matrix_t& mat) {
	cairo_transform(cr.get(), &mat);
}

void user_to_device(std::shared_ptr <cairo_t>& cr, double& x, double& y) {
	cairo_user_to_device(cr.get(), &x, &y);
}

void user_to_device_distance(std::shared_ptr <cairo_t>& cr, double& x, double& y) {
	cairo_user_to_device_distance(cr.get(), &x, &y);
}

void device_to_user(std::shared_ptr <cairo_t>& cr, double& x, double& y) {
	cairo_device_to_user(cr.get(), &x, &y);
}

void device_to_user_distance(std::shared_ptr <cairo_t>& cr, double& x, double& y) {
	cairo_device_to_user_distance(cr.get(), &x, &y);
}

TimeCheck::~TimeCheck() {
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> diff = end - start;
		std::cout << " time : " << diff.count() << " s\n";
}
