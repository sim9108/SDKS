// readbmpTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cdjpeg.h>

void emit_handle(j_common_ptr cinfo, int msg_level){
	char msg[JMSG_LENGTH_MAX];
	cinfo->err->format_message(cinfo, msg);
	std::cout << "msg:" << msg << " level:" << msg_level << std::endl;
}

void exit_handle(j_common_ptr cinfo){
	char msg[JMSG_LENGTH_MAX];
	cinfo->err->format_message(cinfo, msg);
	throw std::runtime_error(msg);
}

int _tmain(int argc, _TCHAR* argv[])
{
	// 에러 핸들 환경 설정
	jpeg_error_mgr jerr;
	jpeg_std_error(&jerr);
	jerr.error_exit = exit_handle;
	jerr.emit_message = emit_handle;

	jpeg_compress_struct cinfo;
	cinfo.err = &jerr;
	
	FILE* fp = fopen("result.jpg", "w+b");
	FILE* rp = fopen("test.bmp", "rb");
		
	try{
		jpeg_create_compress(&cinfo);		
		cjpeg_source_ptr ptr = jinit_read_bmp(&cinfo);				
		ptr->input_file = rp;
		ptr->start_input(&cinfo, ptr);
		(*cinfo.mem->realize_virt_arrays)((j_common_ptr)&cinfo);
		
		jpeg_set_defaults(&cinfo);
		jpeg_stdio_dest(&cinfo, fp);

		int quality = 100;
		jpeg_set_quality(&cinfo, quality, FALSE);
		jpeg_start_compress(&cinfo, TRUE);
		while (cinfo.next_scanline < cinfo.image_height) {			
			int num_scanlines = (*ptr->get_pixel_rows) (&cinfo, ptr);
			(void)jpeg_write_scanlines(&cinfo, ptr->buffer, num_scanlines);
		}

		ptr->finish_input(&cinfo, ptr);
		jpeg_finish_compress(&cinfo);

		jpeg_destroy_compress(&cinfo);
	}
	catch (std::runtime_error& ex){
		std::cout << ">>>error:" << ex.what() << std::endl;
		jpeg_destroy_compress(&cinfo);
	}

	fclose(rp);
	fclose(fp);
	return 0;
}

