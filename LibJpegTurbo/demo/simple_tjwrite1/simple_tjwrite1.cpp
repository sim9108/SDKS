// simple_tjwrite1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <turbojpeg.h>
#include <vector>
#include <fstream>

int _tmain(int argc, _TCHAR* argv[])
{
	// fake data
	auto width = 320;
	auto height = 240;
	auto compent = 3;
	auto row_stirde = width*compent;

	std::vector<unsigned char> datas(row_stirde*height);

	int val{ 0 };
	for (auto& item : datas){
		item = ++val % 256;
	}
	////////////////////////

	auto maxsize = TJBUFSIZE(width, height);
	std::vector<unsigned char> output(maxsize);

	auto thandle = tjInitCompress();

	do{
		unsigned long csize{ 0 };
		int jpegQuality = 100;
		int ret = tjCompress(thandle,
			datas.data(),
			width, row_stirde,
			height,
			compent, /// input
			output.data(),
			&csize,
			0,
			jpegQuality,
			0
			);
		if (-1 == ret) break;

		std::ofstream fp("test.jpg", std::ios_base::binary);
		if (!fp) break;
		fp.write( reinterpret_cast<char*>( output.data() ), csize);
		fp.close();

	} while (false);

	tjDestroy(thandle);

	return 0;
}

