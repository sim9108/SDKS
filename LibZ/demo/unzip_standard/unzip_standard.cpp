// unzip_standard.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <iostream>
#include "UzContainer.h"

int _tmain(int argc, _TCHAR* argv[])
{
	unzFile uf = unzOpen("c.zip");

	UzContainer uzdatas{ uf };
	for (auto& m : uzdatas){
		m.save_file(".\\test\\");
	}	

	unzClose(uf);
	return 0;
}

