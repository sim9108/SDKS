// helloworld.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <jpeglib.h>
#include <stdexcept>
#include <iostream>
#include <vector>

struct ReadData{
	using ROW_DATA = std::vector<JSAMPLE>;
	using ROWS_DATA = std::vector< ROW_DATA >;
	ROWS_DATA rows_;

	unsigned int width_;
	unsigned int height_;
	unsigned int row_bytes_;
};

void exit_handle(j_common_ptr cinfo){
	char msg[JMSG_LENGTH_MAX];
	cinfo->err->format_message(cinfo, msg);
	throw std::runtime_error(msg);
}

int _tmain(int argc, _TCHAR* argv[]){	
	// File Open
	FILE* fp = fopen("test.jpg", "rb");
	if (!fp){
		std::cout << "test.jpg file error" << std::endl;
		return 0;
	}

	// 에러 핸들 환경 설정
	jpeg_error_mgr jerr;
	jpeg_std_error(&jerr);
	jerr.error_exit = exit_handle;

	// 압축 해제 구조
	jpeg_decompress_struct cinfo;
	cinfo.err = &jerr;
	
	ReadData datas;
	try{
		jpeg_create_decompress(&cinfo);
		jpeg_stdio_src(&cinfo, fp);

		jpeg_read_header(&cinfo, true);
		cinfo.out_color_space = JCS_RGB;

		jpeg_start_decompress(&cinfo);

		datas.width_ = cinfo.output_width;
		datas.height_ = cinfo.output_height;
		datas.row_bytes_ = datas.width_*cinfo.output_components;
		std::cout << "width:" << datas.width_ << " height:" << datas.height_ << std::endl;
		std::cout << "components:" << cinfo.output_components << std::endl;

		while (cinfo.output_scanline < cinfo.output_height){
			ReadData::ROW_DATA m(datas.row_bytes_);
			{
				JSAMPLE* ma[] = { m.data() };
				jpeg_read_scanlines(&cinfo, ma, 1);
			}
			datas.rows_.push_back(std::move(m));
		}
		jpeg_finish_decompress(&cinfo);

		jpeg_destroy_decompress(&cinfo);
		fclose(fp);
	}
	catch (std::runtime_error& ex){
		std::cout << "error:" << ex.what() << std::endl;
		jpeg_destroy_decompress(&cinfo);
		fclose(fp);
	}

	return 0;
}

