// read1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <jpeglib.h>
#include <iostream>
#include <vector>
#include <stdexcept>

struct ReadData
{
	using ROW_DATA = std::vector<unsigned char>;
	using IMAGE_DATA = std::vector<ROW_DATA>;

	IMAGE_DATA data_;
	unsigned int width_;
	unsigned int height_;
	unsigned int row_bytes_;
};


void exit_handle(j_common_ptr cinfo){
	char msg[JMSG_LENGTH_MAX];
	cinfo->err->format_message(cinfo, msg);
	throw std::runtime_error(msg);
}

int _tmain(int argc, _TCHAR* argv[])
{
	FILE* fp = fopen("test.jpg", "rb");
	if (!fp){
		std::cout << "file not found." << std::endl;
		return 0;
	}

	jpeg_decompress_struct cinfo;
	jpeg_create_decompress(&cinfo);
	
	try{
		jpeg_error_mgr jerr;
		jpeg_std_error(&jerr);
		jerr.error_exit = &exit_handle;
		cinfo.err = &jerr;

		jpeg_stdio_src(&cinfo, fp);
		jpeg_read_header(&cinfo, true);
		cinfo.out_color_space = JCS_RGB;

		jpeg_start_decompress(&cinfo);
		ReadData data;

		data.width_ = cinfo.output_width;
		data.height_ = cinfo.output_height;
		data.row_bytes_ = data.width_*cinfo.output_components;
		std::cout << "width:" << data.width_ << std::endl;
		std::cout << "height:" << data.height_ << std::endl;
		std::cout << "row_bytes:" << data.row_bytes_ << std::endl;

		unsigned char* ma[1];
		while (cinfo.output_scanline < cinfo.output_height){
			ReadData::ROW_DATA m(data.row_bytes_);
			ma[0] = m.data();
			jpeg_read_scanlines(&cinfo, ma, 1);
			data.data_.push_back(std::move(m));
		}

		jpeg_finish_decompress(&cinfo);
	}
	catch (std::runtime_error& ex){
		std::cout << "error:" << ex.what() << std::endl;
	}

	jpeg_destroy_decompress(&cinfo);
	fclose(fp);

	std::cin.get();
	return 0;
}

