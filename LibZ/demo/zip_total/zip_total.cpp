// zip_total.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "WrapZip.h"


int _tmain(int argc, _TCHAR* argv[])
{
	{
		ZipCTL ctl("C.zip");
		ctl.append(".");
	}

	{
		UnzipCTL ctl("C.zip");		
		ctl.extract(".\\output");
	}

	return 0;
}

