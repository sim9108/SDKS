// read1_mem.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <jpeglib.h>
#include <stdexcept>
#include <iostream>
#include <vector>

void exit_handle(j_common_ptr cinfo){
	char msg[JMSG_LENGTH_MAX];
	cinfo->err->format_message(cinfo, msg);
	throw std::runtime_error(msg);
}

int _tmain(int argc, _TCHAR* argv[])
{
	/// buffer data---start
	FILE* fp = fopen("test.jpg", "rb");
	if (!fp){
		std::cout << "file not found." << std::endl;
		return 0;
	}
	fseek(fp, 0, SEEK_END);
	auto fsize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	std::vector<unsigned char> all_data(fsize);
	fread(all_data.data(), fsize, 1, fp);
	fclose(fp);
	//// buffer data---end


	jpeg_decompress_struct cinfo;
	jpeg_create_decompress(&cinfo);

	try{
		jpeg_error_mgr jerr;
		jpeg_std_error(&jerr);
		jerr.error_exit = &exit_handle;
		cinfo.err = &jerr;

		jpeg_mem_src(&cinfo, all_data.data(), all_data.size()); ///!!!! diff
		jpeg_read_header(&cinfo, true);
		cinfo.out_color_space = JCS_RGB;

		jpeg_start_decompress(&cinfo);

		auto width = cinfo.output_width;
		auto height = cinfo.output_height;
		auto row_bytes = width*cinfo.output_components;
		std::cout << "width:" << width << std::endl;
		std::cout << "height:" << height << std::endl;
		std::cout << "row_bytes:" << row_bytes << std::endl;

		unsigned char* ma[1];
		std::vector<unsigned char> m(row_bytes);
		ma[0] = m.data();

		while (cinfo.output_scanline < cinfo.output_height){			
			jpeg_read_scanlines(&cinfo, ma, 1);
		}

		jpeg_finish_decompress(&cinfo);
	}
	catch (std::runtime_error& ex){
		std::cout << "error:" << ex.what() << std::endl;
	}

	jpeg_destroy_decompress(&cinfo);

	std::cin.get();
	return 0;
}

