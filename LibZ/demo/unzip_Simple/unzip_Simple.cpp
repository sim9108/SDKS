// unzip_Simple.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <unzip.h>
#include <fstream>
#include <iostream>
#include <string>

bool extractZip(const std::string zippath, const std::string targetdir)
{
	unzFile uf = unzOpen(zippath.c_str());

	int ret = unzGoToFirstFile(uf);
	if (UNZ_OK != ret){
		unzClose(uf);
		return true;
	}

	do{
		unz_file_info info;
		char filename[256];
		char comment[256];
		unzGetCurrentFileInfo(uf, &info,
			filename, sizeof(filename),
			nullptr,0,
			comment, sizeof(comment)
			);

		unzOpenCurrentFile(uf);/////---@@@@
		// 폴더 생성
		std::ofstream ofs(targetdir + filename, std::ios_base::binary);
		// 기록 일자 변경...
		
		char buf[4096];
		for (;;){
			int len = unzReadCurrentFile(uf, buf, sizeof(buf));///--^^^^^^
			if (0 == len) break;
			ofs.write(buf, len);
		}
		unzCloseCurrentFile(uf);/////---@@@@

	} while (UNZ_OK == unzGoToNextFile(uf));

	unzClose(uf);
	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	extractZip("c.zip", ".\\test\\");

	return 0;
}

