// standard_write1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "PngWriter.h"
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	PNG::PngWriter writer;
	FILE* fp = fopen("test.png", "wb");
	if (!fp){
		std::cout << "file not found." << std::endl;
		return 0;
	}

	auto width = 320;
	auto height = 240;
	auto bit_depth = 8;
	PNG::LINE rows(width*4);

	writer >> fp << 
		[&](png_structp write_ptr, png_infop info_ptr){
			png_set_IHDR(
				write_ptr, info_ptr,
				width, height, bit_depth,
				PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
				PNG_COMPRESSION_TYPE_DEFAULT,
				PNG_FILTER_TYPE_DEFAULT);
	} << PNG::set_bgr << PNG::write_info << rows;

	fclose(fp);
	std::cout << "complete" << std::endl;
	std::cin.get();

	return 0;
}

