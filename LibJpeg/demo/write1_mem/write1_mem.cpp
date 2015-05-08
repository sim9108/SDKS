// write1_mem.cpp : Defines the entry point for the console application.
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
	struct jpeg_compress_struct cinfo;
	jpeg_create_compress(&cinfo);

	unsigned char *inmem = nullptr;
	unsigned long inmem_size = 0;
	/////
	try{
		jpeg_error_mgr jerr;
		jpeg_std_error(&jerr);
		jerr.error_exit = &exit_handle;
		cinfo.err = &jerr;

		// 입력 설정
		jpeg_mem_dest(&cinfo, &inmem, &inmem_size);///!!!!

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
	catch (std::runtime_error& ex){
		std::cout << "error:" << ex.what() << std::endl;
	}

	jpeg_destroy_compress(&cinfo);
	
	if (inmem){
		FILE* fp = fopen("test.jpg", "wb");
		fwrite(inmem, inmem_size, 1, fp);
		fclose(fp);

		free(inmem);
		inmem = nullptr;
	}

	std::cout << "complete" << std::endl;
	std::cin.get();
	return 0;
}

