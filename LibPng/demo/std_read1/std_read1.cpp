// std_read1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "PngReader.h"
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{

	FILE* fp = fopen("test.png", "rb");
	if (!fp){
		std::cout << "file not found." << std::endl;
		return 0;
	}

	PNG::PngReader png;

	png << fp;

	std::cout <<"init:" << png;
	png << PNG::rgba << PNG::bgr << PNG::update;
	std::cout <<"update:"<< png;

	PNG::IMAGE_DATA data;
	png >> data;

	fclose(fp);

	std::cout << "complete." << std::endl;
	std::cin.get();
	return 0;
}

