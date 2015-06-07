// std2_write.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "PngWriter.h"

int _tmain(int argc, _TCHAR* argv[])
{
	PngWriter png;
	FILE* fp = fopen("test.png", "wb");
	if (!fp){
		std::cout << "file not access." << std::endl;
		return 0;
	}

	unsigned int rows_bytes;

	png >> fp
		<< set_IHDR(320, 240)
		<< set_bgr
		<< write_info
		<< get_rowbytes(rows_bytes);

	LINE row(rows_bytes);
	png << write_rows(row)
		<< write_end;


	return 0;
}

