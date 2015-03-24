// hellworld.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <jasper/jasper.h>

int _tmain(int argc, _TCHAR* argv[])
{
	jas_init();

	do {
		jas_setdbglevel(1);

		jas_stream_t* in = jas_stream_fopen("test.jpg", "rb");
		if (!in){
			std::cout << "file error";
			break;
		}

		jas_tmr_t dectmr;
		jas_tmr_start(&dectmr);

		int in_fmt = jas_image_getfmt(in);
		jas_image_t* in_img = jas_image_decode(in, in_fmt, nullptr);
		jas_tmr_stop(&dectmr);

		{
			jas_stream_t* out = jas_stream_fopen("test.jp2", "w+b");
			jas_tmr_t enctmr;
			jas_tmr_start(&enctmr);
			jas_image_encode(in_img, out, jas_image_strtofmt((char*)"jp2"), nullptr);
			jas_stream_flush(out);
			jas_tmr_stop(&enctmr);
			jas_stream_close(out);
		}

		jas_stream_close(in);

		jas_image_destroy(in_img);
		jas_image_clearfmts();
	} while (false);

	jas_cleanup();

	std::cin.get();


	return 0;
}

