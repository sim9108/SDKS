// Simple_write1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <png.h>
#include <stdexcept>
#include <iostream>
#include <vector>

typedef unsigned char BYTE;
typedef BYTE   PIXEL;
typedef std::vector<PIXEL> LINE;

void error_handler(png_structp ptr, png_const_charp warning){
	throw std::runtime_error(warning);
}

void warning_handler(png_structp ptr, png_const_charp warning){
	std::cout << warning << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	png_structp write_ptr = png_create_write_struct(
		PNG_LIBPNG_VER_STRING,
		0, 
		nullptr,
		nullptr
		);

	do{
		if (!write_ptr) break;

		png_set_error_fn(write_ptr, 0, error_handler, warning_handler);
		png_infop info_ptr = png_create_info_struct(write_ptr);
		try{

			FILE* fp = fopen("test.png", "wb");
			if (!fp){
				std::cout << "file not found." << std::endl;
				break;
			}

			png_init_io(write_ptr, fp);

			// Encode Requirement
			auto width = 320;
			auto height = 240;
			auto bit_depth = 8;
					
			png_set_IHDR(
				write_ptr,info_ptr,
				width, height, bit_depth,
				PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
				PNG_COMPRESSION_TYPE_DEFAULT,
				PNG_FILTER_TYPE_DEFAULT);

			png_set_bgr(write_ptr);

			//update
			png_write_info(write_ptr, info_ptr);

			auto bytes_stride = png_get_rowbytes(write_ptr, info_ptr);
			LINE rows(bytes_stride);
			png_byte* m[1];
			for (int h = 0; h < height; ++h){
				m[0] = rows.data();
				png_write_rows(write_ptr, m, 1);
			}
			png_write_end(write_ptr, info_ptr);
			fclose(fp);

		} 
		catch (std::runtime_error& ex){
			std::cout << "error:" << ex.what() << std::endl;
		}
		png_destroy_info_struct(write_ptr, &info_ptr);

	} while (false);
	png_destroy_write_struct(&write_ptr, nullptr);

	return 0;
}

