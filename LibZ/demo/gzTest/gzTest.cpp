// gzTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <zlib.h>
#include <fstream>
#include <string>

void makegz(const char* fname){
	std::ifstream rp(fname, std::ios_base::binary);
	if (!rp) return;

	std::string gzname = fname;
	gzname += ".gz";
	gzFile fp = gzopen(gzname.data(), "wb");

	const int BUF = 1024;
	char in[BUF];

	do{
		rp.read(in, BUF);
		int readsize = (int)rp.gcount();
		gzwrite(fp, in, readsize);

	} while (!rp.eof());
	gzclose(fp);
}

void readgz(const char* fname){
	gzFile fp = gzopen(fname, "rb");

	std::string readname = fname;
	readname += ".txt";
	std::ofstream wp(readname.data(), std::ios_base::binary);

	const int BUF = 1024;
	char in[BUF];
	do{
		int readsize = gzread(fp, in, BUF);
		wp.write(in, readsize);		
	} while (!gzeof(fp));

	gzclose(fp);
}

int _tmain(int argc, _TCHAR* argv[])
{
	makegz("test.txt");
	readgz("test.txt.gz");
	return 0;
}
