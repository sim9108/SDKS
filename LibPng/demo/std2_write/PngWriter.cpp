#include "PngWriter.h"

void error_handler(png_structp ptr, png_const_charp cmt)
{
	throw std::runtime_error(cmt);
}

void warning_handler(png_structp ptr, png_const_charp cmt)
{
	std::cout << "warning:" << cmt << std::endl;
}

PngWriter::PngWriter()
:
write_ptr_{
			png_create_write_struct(
			PNG_LIBPNG_VER_STRING,
			0, error_handler, warning_handler
			)
		},
info_ptr_{
		png_create_info_struct(write_ptr_)
},
failed_{ false },
error_{ "" }
{
}


PngWriter::~PngWriter()
{
	png_destroy_info_struct(write_ptr_, &info_ptr_);
	png_destroy_write_struct(&write_ptr_, nullptr);
}

PngWriter& PngWriter::operator >> (FILE* fp){
	try{
		png_init_io(write_ptr_, fp);
	}
	catch (std::runtime_error& ex){
		this->failed_ = true;
		this->error_ = ex.what();
	}
	return *this;
}

void set_bgr(png_structp writer, png_infop){
	png_set_bgr(writer);
}

void write_info(png_structp writer, png_infop info){
	png_write_info(writer, info);
}

void write_end(png_structp writer, png_infop info){
	png_write_end(writer, info);
}

PNG_FN
get_rowbytes(unsigned int& value){
	auto fn = [&](png_structp writer, png_infop info){
		value = png_get_rowbytes(writer, info);
	};
	return fn;
}

PNG_FN set_IHDR(int width, int height,
	int bit_depth /*= 8*/,
	int color_type /*= PNG_COLOR_TYPE_RGB_ALPHA*/,
	int interlace /*= PNG_INTERLACE_NONE*/,
	int compress_type /*= PNG_COMPRESSION_TYPE_DEFAULT*/,
	int filter_type /*= PNG_FILTER_TYPE_DEFAULT*/){

	auto fn = [=](png_structp writer, png_infop info){
		png_set_IHDR(writer, info,
			width, height,
			bit_depth, color_type, interlace,
			compress_type, filter_type
			);
	};

	return fn;
}


PNG_FN
write_rows(const LINE& line){
	auto fn = [&](png_structp writer, png_infop info){
		auto height = png_get_image_height(writer, info);
		png_byte* m[1];
		m[0] = const_cast<png_byte*> (line.data());
		for (unsigned int h = 0; h < height; ++h){
			png_write_rows(writer, m, 1);
		}
	};

	return fn;
}

PNG_FN
write_rows(const IMAGE& image){
	auto fn = [&](png_structp writer, png_infop info){
		auto height = png_get_image_height(writer, info);
		png_byte* m[1];
		
		for (unsigned int h = 0; h < height; ++h){
			m[0] = const_cast<png_byte*> (image[h].data());
			png_write_rows(writer, m, 1);
		}
	};
	return fn;
}

