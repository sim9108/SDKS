// JT_read1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <turbojpeg.h>
#include <vector>
#include <fstream>
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	/// virtual buffer <-- net, lib.. fake coding
	FILE* fp = fopen("test.jpg", "rb");
	if (!fp){
		std::cout << "file not found" << std::endl;
		return 0;
	}
	fseek(fp, 0, SEEK_END);
	auto fsize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	std::vector<unsigned char> all_data(fsize);
	fread(all_data.data(), fsize, 1, fp);
	fclose(fp);
	/////////////////////// virtual buffer end!!

	auto handleDecompress = tjInitDecompress();

	do{

		int w, h,subsamp;
		int ret = tjDecompressHeader2(handleDecompress, all_data.data(), all_data.size(), &w, &h, &subsamp);
		if (-1 == ret) break;

		std::vector<unsigned char> img_datas(w*h*3);
		ret = tjDecompress(handleDecompress, all_data.data(), all_data.size(), img_datas.data(), w, 0, h, 3, 0);
		if (-1 == ret) break;
		std::cout << "width:" << w << " height:" << h << " data size:" << img_datas.size() << std::endl;
		std::cout << "complete" << std::endl;

	} while (false);

	tjDestroy(handleDecompress);

	std::cin.get();
	return 0;
}

