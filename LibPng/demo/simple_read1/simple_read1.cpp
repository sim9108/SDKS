// simple_read1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <png.h>
#include <stdexcept>
#include <iostream>
#include <vector>

typedef std::vector<unsigned char> ROW_DATA;
typedef std::vector<ROW_DATA>  IMAGE_DATA;
typedef std::vector<unsigned char*>  PTR_DATA;

void error_handler(png_structp ptr, png_const_charp warning){
	throw std::runtime_error(warning);
}

void warning_handler(png_structp ptr, png_const_charp warning){
	std::cout << warning << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	png_structp read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	do{
		if (!read_ptr) break;

		png_set_error_fn(read_ptr, 0, error_handler, warning_handler);		
		png_infop info_ptr = png_create_info_struct(read_ptr);
		do{			
			if (!info_ptr) break;

			try{
				FILE* fp = fopen("test.png", "rb");
				if (!fp){
					std::cout << "file not found." << std::endl;
					break;
				}

				png_init_io(read_ptr, fp);

				///init 
				png_read_info(read_ptr, info_ptr);
				auto width = png_get_image_width(read_ptr, info_ptr);
				auto height = png_get_image_height(read_ptr, info_ptr);
				auto color_type = png_get_color_type(read_ptr, info_ptr);
				auto bit_depth = png_get_bit_depth(read_ptr, info_ptr);
				auto bytes_stride = png_get_rowbytes(read_ptr, info_ptr);
				std::cout << "update width:" << width << " height:" << height << " color_type:" << (int)color_type << " bit_depth:" << (int)bit_depth << " bytes_stride:" << bytes_stride << std::endl;
				
				//DECODE REQUIRE
				auto number_of_passess = png_set_interlace_handling(read_ptr);
				if ( !(PNG_COLOR_MASK_ALPHA & color_type))	png_set_add_alpha(read_ptr, 0xFF, PNG_FILLER_AFTER);
				if ( PNG_COLOR_TYPE_GRAY_ALPHA == color_type || PNG_COLOR_TYPE_GRAY == color_type) png_set_gray_to_rgb(read_ptr);				
				if (bit_depth>8) png_set_strip_16(read_ptr);

				if (PNG_COLOR_TYPE_PALETTE & color_type) png_set_expand(read_ptr);
				if (bit_depth<8) png_set_expand(read_ptr);
				if (png_get_valid(read_ptr, info_ptr, PNG_INFO_tRNS)) png_set_expand(read_ptr);

				//
				png_set_bgr(read_ptr);
								
				//update
				png_read_update_info(read_ptr, info_ptr);				

				width = png_get_image_width(read_ptr, info_ptr);
				height = png_get_image_height(read_ptr, info_ptr);
				color_type = png_get_color_type(read_ptr, info_ptr);
				bit_depth = png_get_bit_depth(read_ptr, info_ptr);
				bytes_stride = png_get_rowbytes(read_ptr, info_ptr);
				std::cout << "update width:" << width << " height:" << height << " color_type:" << (int)color_type << " bit_depth:" << (int)bit_depth <<" bytes_stride:" << bytes_stride << std::endl;

				///
				IMAGE_DATA datas(height);
				PTR_DATA datas_ptr;
				
				for (auto& item : datas){
					item.resize(bytes_stride);
					datas_ptr.push_back(item.data());
				}

				png_read_image(read_ptr, datas_ptr.data());
				png_read_end(read_ptr, info_ptr);
				fclose(fp);

				std::cout << "complete" << std::endl;

			}
			catch (std::runtime_error& ex){
				std::cout << ex.what() << std::endl;
				break;
			}
		} while (false);
		png_destroy_info_struct(read_ptr, &info_ptr);				

	} while (false);
	png_destroy_read_struct(&read_ptr, nullptr, nullptr);

	std::cin.get();
	return 0;
}

