// jp2_hello.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <openjp2/openjpeg.h>
#include <string.h>
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	opj_dparameters_t parameters;
	opj_set_default_decoder_parameters(&parameters);
	parameters.infile[0] = 0;
	strcat(parameters.infile, "test.jp2");
	parameters.decod_format = OPJ_CODEC_JP2;

	opj_stream_t* l_stream = opj_stream_create_default_file_stream(parameters.infile, 1);
	opj_codec_t* l_codec = opj_create_decompress(OPJ_CODEC_JP2);
	opj_setup_decoder(l_codec, &parameters);

	opj_image_t* image = nullptr;
	opj_read_header(l_stream, l_codec, &image);
	std::cout << "width:" << image->x1<<std::endl;
	std::cout << "height:" << image->y1 << std::endl;
	std::cout << "comp count:" << image->numcomps << std::endl;

	opj_decode(l_codec, l_stream, image);
	opj_end_decompress(l_codec, l_stream);

	opj_destroy_codec(l_codec);
	opj_stream_destroy(l_stream);
	opj_image_destroy(image);

	return 0;
}

