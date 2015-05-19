// simple_read1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <png.h>
#include <stdexcept>
#include <iostream>
#include <vector>

typedef std::vector<png_byte> ROW_DATA;
typedef std::vector<ROW_DATA> IMAGE_DATA;

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
			FILE* fp = fopen("test.png", "rb");
			if (!fp){
				std::cout << "file not found." << std::endl;
				break;
			}

			try{

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
				if ( !(PNG_COLOR_MASK_ALPHA & color_type))	png_set_add_alpha(read_ptr, 0xFF, PNG_FILLER_AFTER);
				if ( PNG_COLOR_TYPE_GRAY_ALPHA == color_type || PNG_COLOR_TYPE_GRAY == color_type) png_set_gray_to_rgb(read_ptr);	
				if (PNG_COLOR_TYPE_PALETTE & color_type) png_set_palette_to_rgb(read_ptr);
				if (png_get_valid(read_ptr, info_ptr, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(read_ptr);
				if (bit_depth>8) png_set_strip_16(read_ptr);				
				if (bit_depth<8) png_set_expand(read_ptr);
				
				auto number_of_passess = png_set_interlace_handling(read_ptr);

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
				IMAGE_DATA rows(height);
				for (auto& item : rows){
					item.resize(bytes_stride);
				}

				png_byte* m[1];
				for (int pass = 0; pass < number_of_passess; ++pass){
				for (unsigned int h = 0; h < height; ++h){		
					m[0] = rows[h].data();
					png_read_rows(read_ptr, m, nullptr,1);
					}
				};

				png_read_end(read_ptr, info_ptr);
				std::cout << "complete" << std::endl;

			}
			catch (std::runtime_error& ex){
				std::cout << ex.what() << std::endl;
			}

			fclose(fp);

		} while (false);
		png_destroy_info_struct(read_ptr, &info_ptr);				

	} while (false);
	png_destroy_read_struct(&read_ptr, nullptr, nullptr);

	std::cin.get();
	return 0;
}

