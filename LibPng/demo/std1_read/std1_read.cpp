// std1_read.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "PngReader.h"

void print(png_structp read_ptr, png_infop info_ptr){
	auto width = png_get_image_width(read_ptr, info_ptr);
	auto height = png_get_image_height(read_ptr, info_ptr);
	auto color_type = png_get_color_type(read_ptr, info_ptr);
	auto bit_depth = png_get_bit_depth(read_ptr, info_ptr);//1,4,8,16
	auto bytes_stride = png_get_rowbytes(read_ptr, info_ptr);

	std::cout << "init: " << "width:" << width << " height:" << height << " color type:" << (int)color_type
		<< " bit_depth:" << (int)bit_depth << " bytes_stride:" << bytes_stride << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{	
	FILE* fp = fopen("test.png", "rb");
	if (!fp){
		std::cout << " file not found" << std::endl;
		return 0;
	}

	PNG::IMAGE datas;
	PNG::PngReader reader;
	reader << fp << PNG::read_info<<print <<PNG::set_rgba<< PNG::set_bgr << PNG::read_update_info<<print >> datas;
	

	fclose(fp);
	std::cin.get();

	return 0;
}

