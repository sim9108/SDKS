// Core_Standard1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DeflateCTL.h"
#include <iostream>


int _tmain(int argc, _TCHAR* argv[])
{
	{
		DeflateCTL ctl;
		std::string hello = "æ»≥Á«œººø‰.";

		ctl << hello << "1234" << DeflateCTL::endl;
		ctl.writefile("1234.enc");
	}

	{
		DeflateCTL ctl;
		std::ifstream rp{ "test.bmp", std::ios_base::binary };
		ctl << rp << DeflateCTL::endl;
		ctl.writefile("test.enc");
    }


	return 0;
}

