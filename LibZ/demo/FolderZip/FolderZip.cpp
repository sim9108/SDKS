// FolderZip.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <filesystem>
#include <iostream>
#include <vector>
#include <string>
#include "ZipCTL.h"

int _tmain(int argc, _TCHAR* argv[]){
	ZipCTL ctl("test.zip");
	ctl.make_zip(".");
	return 0;
}

