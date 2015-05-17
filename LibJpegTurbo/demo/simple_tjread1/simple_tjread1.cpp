// simple_tjread1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <turbojpeg.h>
#include <vector>
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	// fake coding
	FILE* fp = fopen("test.jpg", "rb");
	if (!fp){
		std::cout << "file not found." << std::endl;
		return 0;
	}

	fseek(fp, 0, SEEK_END);
	auto fsize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	std::vector<unsigned char> all_data(fsize);
	fread(all_data.data(), fsize, 1, fp);
	fclose(fp);
	/////

	auto thandle = tjInitDecompress();

	do{
		int w, h, subsamp;
		int ret = tjDecompressHeader2(thandle,
			all_data.data(), all_data.size(),
			&w, &h, &subsamp
			);
		if (-1 == ret) break;

		auto compcount = 3;
		std::vector<unsigned char> img_data(w*h * compcount);
		ret = tjDecompress(thandle,
			all_data.data(), all_data.size(),
			img_data.data(),
			w, 0, /// width
			h, compcount,
			0
			);
		if (-1 == ret) break;

		std::cout << "width:" << w<<" ";
		std::cout << "height:" << h << " ";
		std::cout << "data size:" << img_data.size()<< " ";
	} while (false);

	tjDestroy(thandle);

	std::cin.get();

	return 0;
}

