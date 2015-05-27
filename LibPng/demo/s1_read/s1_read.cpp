// s1_read.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <png.h>
#include <stdexcept>
#include <iostream>
#include <vector>

typedef unsigned char BYTE;
typedef BYTE   PIXEL;
typedef std::vector<PIXEL> LINE;
typedef std::vector<LINE> IMAGE;


void error_handler(png_structp ptr, png_const_charp cmt){
	throw std::runtime_error(cmt);
}

void warning_handler(png_structp ptr, png_const_charp cmt){
	std::cout <<"warning:"<< cmt << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	png_structp read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	do{
		if (!read_ptr) break;

		png_set_error_fn(read_ptr, nullptr, error_handler, warning_handler);
		png_infop info_ptr = png_create_info_struct(read_ptr);
		try{
			FILE* fp = fopen("test.png", "rb");
			if (!fp){
				std::cout << "file not found." << std::endl;
				break;
			}

			png_init_io(read_ptr, fp);

			//init
			png_read_info(read_ptr, info_ptr);
			auto width = png_get_image_width(read_ptr, info_ptr);
			auto height = png_get_image_height(read_ptr, info_ptr);
			auto color_type = png_get_color_type(read_ptr, info_ptr);
			auto bit_depth = png_get_bit_depth(read_ptr, info_ptr);//1,4,8,16
			auto bytes_stride = png_get_rowbytes(read_ptr, info_ptr);

			std::cout << "init: " << "width:" << width << " height:" << height << " color type:" << (int)color_type
				<< " bit_depth:" << (int)bit_depth << " bytes_stride:" << bytes_stride << std::endl;


			// Decode Requirement
			if (!(PNG_COLOR_MASK_ALPHA& color_type)) png_set_add_alpha(read_ptr, 0xFF, PNG_FILLER_AFTER);
			if (PNG_COLOR_TYPE_GRAY == color_type || PNG_COLOR_TYPE_GRAY_ALPHA == color_type) png_set_gray_to_rgb(read_ptr);
			if (PNG_COLOR_TYPE_PALETTE == color_type) png_set_palette_to_rgb(read_ptr);
			if (png_get_valid(read_ptr, info_ptr, PNG_INFO_tRNS))  png_set_tRNS_to_alpha(read_ptr);
			if (bit_depth > 8) png_set_strip_16(read_ptr);
			if (bit_depth < 8) png_set_expand(read_ptr);

			auto pass = png_set_interlace_handling(read_ptr);
			png_set_bgr(read_ptr);
			
			/// update
			png_read_update_info(read_ptr, info_ptr);
			width = png_get_image_width(read_ptr, info_ptr);
			height = png_get_image_height(read_ptr, info_ptr);
			color_type = png_get_color_type(read_ptr, info_ptr);
			bit_depth = png_get_bit_depth(read_ptr, info_ptr);//1,4,8,16
			bytes_stride = png_get_rowbytes(read_ptr, info_ptr);

			std::cout << "upate: " << "width:" << width << " height:" << height << " color type:" << (int)color_type
				<< " bit_depth:" << (int)bit_depth << " bytes_stride:" << bytes_stride << std::endl;

			// image read
			IMAGE datas(height);
			for (auto& item : datas){
				item.resize(bytes_stride);
			}

			png_byte* m[1];
			for (int ip = 0; ip < pass; ++ip){ //pass count
				for (unsigned int h = 0; h < height; ++h){
					m[0] =  datas[h].data() ;
					png_read_rows(read_ptr, m, nullptr, 1);
				}
			}

			png_read_end(read_ptr, info_ptr);
			fclose(fp);
			std::cout << "complete" << std::endl;
						
		}
		catch (std::runtime_error& ex){
			std::cout <<"error:"<< ex.what() << std::endl; // exception-safty
		}
		png_destroy_info_struct(read_ptr, &info_ptr);
		

	} while (false);

	png_destroy_read_struct(&read_ptr, nullptr, nullptr);
	std::cin.get();

	return 0;
}

