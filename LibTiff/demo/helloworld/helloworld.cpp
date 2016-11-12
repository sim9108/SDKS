// helloworld.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <tiffio.h>
#include <vector>
#include <Windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
	int a = GetTickCount();

	TIFF* tif = TIFFOpen("test.tif", "r"); 
	if (!tif){
		std::cout << "tif file error" << std::endl;
		return 0;
	}

	uint32 w{}, h{};
	TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
	TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);

	std::cout << "width:" << w << std::endl;
	std::cout << "height:" << h << std::endl;

	std::vector<uint32> raster( w*h );
	TIFFReadRGBAImage(tif, w, h, raster.data(), 0);
	TIFFClose(tif);

	int b = GetTickCount();
	printf("\n Decoding time: %u msec\n\n", b - a);  system("pause");

	return 0;
}

