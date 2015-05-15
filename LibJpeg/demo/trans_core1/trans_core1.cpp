// trans_core1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <jpeglib.h>
#include <transupp.h>

#include <iostream>
#include <vector>
#include <stdexcept>

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

	jpeg_transform_info transformoption;
	JCOPY_OPTION copyoption;

	do{
		try{
			jpeg_error_mgr jerr;
			jpeg_std_error(&jerr);
			jerr.error_exit = &exit_handle;
			cinfo.err = &jerr;

			jpeg_stdio_src(&cinfo, fp);

			jcopy_markers_setup(&cinfo, copyoption);
			jpeg_read_header(&cinfo, true);

			if (!jtransform_request_workspace(&cinfo, &transformoption)) {
				break;
			}

		}
		catch (std::runtime_error& ex){
			std::cout << "error:" << ex.what() << std::endl;
		}

	} while (false);

	jpeg_destroy_decompress(&cinfo);
	fclose(fp);

	return 0;
}

