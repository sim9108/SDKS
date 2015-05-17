// JT_write1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <turbojpeg.h>
#include <vector>
#include <fstream>

int _tmain(int argc, _TCHAR* argv[])
{
	///// fake start 
	auto width = 320;
	auto height = 240;
	auto compoents = 3;//rgb
	auto row_stirde = width*compoents;

	std::vector<unsigned char> datas(row_stirde * height);

	int val{ 0 };
	for (auto& item : datas){
		item = (++val) % 256;
	}
	//////// fake end...

	unsigned char * output = tjAlloc(TJBUFSIZE(width, height));
	auto handleCompress = tjInitCompress();
	unsigned long size = 0;
	int  flags = 0, jpegsubsamp = 0;
	int jpegQuality = 100;
	auto ret = tjCompress(handleCompress, datas.data(), width, row_stirde, height, compoents, output, &size, jpegsubsamp, jpegQuality, flags);
	tjDestroy(handleCompress);

	do{
		if (-1 == ret) break;
		std::ofstream fp("test.jpg",std::ios_base::binary);
		if (!fp) break;
		fp.write( reinterpret_cast<char*>(output), size);
		fp.close();
	} while (false);


	tjFree(output);
	return 0;
}

