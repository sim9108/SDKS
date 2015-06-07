#pragma once

#include <png.h>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <functional>

typedef unsigned char BYTE;
typedef BYTE  PIXEL;
typedef std::vector<PIXEL> LINE;
typedef std::vector<LINE> IMAGE;

void error_handler(png_structp ptr, png_const_charp cmt);
void warning_handler(png_structp ptr, png_const_charp cmt);

class PngWriter
{
private:
	png_structp write_ptr_;
	png_infop  info_ptr_;

	bool failed_;
	const char* error_;
public:
	PngWriter();
	~PngWriter();

	PngWriter& operator >> (FILE* fp);

	template<typename FN>
	PngWriter&
		operator <<(FN&& fn){
			try{
				fn(this->write_ptr_, this->info_ptr_);
			}
			catch (std::runtime_error& ex)
			{
				this->failed_ = true;
				this->error_ = ex.what();
			}
			return *this;
		}
};

typedef std::function<void(png_structp, png_infop)> PNG_FN;

void set_bgr(png_structp, png_infop);
void write_info(png_structp, png_infop);

void write_end(png_structp, png_infop);

PNG_FN get_rowbytes(unsigned int& value);
PNG_FN set_IHDR(int width, int height, 
	int bit_depth = 8,
	int color_type = PNG_COLOR_TYPE_RGB_ALPHA,
	int interlace = PNG_INTERLACE_NONE,
	int compress_type = PNG_COMPRESSION_TYPE_DEFAULT, 
	int filter_type = PNG_FILTER_TYPE_DEFAULT);

PNG_FN write_rows(const LINE& line);
PNG_FN write_rows(const IMAGE& image);
