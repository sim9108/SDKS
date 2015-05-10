// core_write_mem.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <jpeglib.h>
#include <iostream>
#include <stdexcept>
#include <vector>

void exit_handler(j_common_ptr cinfo){
	char msg[JMSG_LENGTH_MAX];
	cinfo->err->format_message(cinfo, msg);
	throw std::runtime_error(msg);
}

class Result_ptr{
private:
	typedef unsigned char* MY_T;
	MY_T result_;
public:
	Result_ptr() :result_{ nullptr }
	{
	}

	Result_ptr(std::nullptr_t) :result_{ nullptr }
	{
	}

	MY_T* operator & (){ // &
		return &this->result_;
	}

	operator MY_T(){ // 변환 연산자.
		return this->result_;
	}

	MY_T* operator->(){
		return &this->result_;
	}

	~Result_ptr(){
		if (!result_) return;
		free(result_);
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	jpeg_compress_struct cinfo;

	jpeg_create_compress(&cinfo);

	try{
		jpeg_error_mgr jerr;
		jpeg_std_error(&jerr);
		jerr.error_exit = &exit_handler;
		cinfo.err = &jerr;

		Result_ptr result = nullptr;
		unsigned long result_size = 0;////?????

		jpeg_mem_dest(&cinfo, &result, &result_size);

		///// feak start <---net, screen..
		cinfo.image_width = 320;
		cinfo.image_height = 240;
		cinfo.input_components = 3;
		cinfo.in_color_space = JCS_RGB;
		jpeg_set_defaults(&cinfo);

		jpeg_set_quality(&cinfo, 100,true);

		auto row_stride = cinfo.image_width*cinfo.input_components;
		std::vector<unsigned char> datas(row_stride);

		int val{ 0 };
		for (auto& item : datas){
			item = (++val) % 256;
		}

		unsigned char* ma[1];
		ma[0] = datas.data();

		//////// feak end...

		jpeg_start_compress(&cinfo, true);
		try{
			while (cinfo.next_scanline < cinfo.image_height){
				jpeg_write_scanlines(&cinfo, ma, 1);
			}
		}
		catch (std::runtime_error&){
			jpeg_finish_compress(&cinfo);
			throw;
		}
		jpeg_finish_compress(&cinfo);
		////

		FILE* fp = fopen("test.jpg", "wb");
		fwrite(result, result_size, 1, fp);
		fclose(fp);
	}
	catch (std::runtime_error& ex){
		std::cout << "error:" << ex.what() << std::endl;
	}

	jpeg_destroy_compress(&cinfo);


	return 0;
}

