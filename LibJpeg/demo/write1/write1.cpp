// write1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <jpeglib.h>
#include <stdexcept>
#include <vector>

void exit_handle(j_common_ptr cinfo){
	char msg[JMSG_LENGTH_MAX];
	cinfo->err->format_message(cinfo, msg);
	throw std::runtime_error(msg);
}

int _tmain(int argc, _TCHAR* argv[]){

	FILE* fp = fopen("test.jpg", "wb");
	if (!fp){
		std::cout << "file open error" << std::endl;
		return 0;
	}

	struct jpeg_compress_struct cinfo;
	jpeg_create_compress(&cinfo);

	try{
		jpeg_error_mgr jerr;
		jpeg_std_error(&jerr);
		jerr.error_exit = &exit_handle;
		cinfo.err = &jerr;

		// 입력 설정
		jpeg_stdio_dest(&cinfo, fp);

		// 환경 설정
		cinfo.image_width = 320;
		cinfo.image_height = 240;
		cinfo.input_components = 3;
		cinfo.in_color_space = JCS_RGB;
		jpeg_set_defaults(&cinfo);

		jpeg_set_quality(&cinfo, 100, true);

		// 압축 시작
		jpeg_start_compress(&cinfo, TRUE);

		unsigned char* ma[1];
		auto row_stride = cinfo.image_width*cinfo.input_components;
		std::vector<unsigned char> datas(row_stride);

		int val{ 0 };
		for (auto& item : datas){
			item = (++val) % 255;
		}
		ma[0] = datas.data();

		while (cinfo.next_scanline < cinfo.image_height) {
			jpeg_write_scanlines(&cinfo, ma, 1);
		}

		jpeg_finish_compress(&cinfo);
	}
	catch(std::runtime_error& ex){
		std::cout << "error:" << ex.what() << std::endl;
	}

	jpeg_destroy_compress(&cinfo);
	fclose(fp);

	std::cout << "complete" << std::endl;
	std::cin.get();
	return 0;
}

