// helloworld.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <tiffio.h>
#include <vector>

int _tmain(int argc, _TCHAR* argv[])
{
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

	std::vector<uint32> raster(w*h);
	TIFFReadRGBAImage(tif, w, h, raster.data(), 0);
	TIFFClose(tif);

	return 0;
}

