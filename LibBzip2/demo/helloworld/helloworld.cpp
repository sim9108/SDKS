// helloworld.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <bzlib.h>
#include <cstdio>

int _tmain(int argc, _TCHAR* argv[]){
	const int BUF = 1024;	
	int bzerror;
	BZFILE* bz = nullptr;
	FILE* fp = fopen("myfile.txt.bz2", "w");
	if (!fp){
		std::cout << "file io error" << std::endl;
		return 0;
	}

	try{		
		int blockSize100k{ 9 };
		int verbosity{ 0 };
		int workFactor{ 30 };

		bz = BZ2_bzWriteOpen(&bzerror, fp, blockSize100k, verbosity, workFactor);
		if (BZ_OK != bzerror) throw "error";

		std::string datas = "hello world! ¾È³çÇÏ¼¼¿ä.";
		do {
			BZ2_bzWrite(&bzerror, bz, reinterpret_cast<void*>(const_cast<char*>((datas.data()))), datas.size());
			if (BZ_IO_ERROR == bzerror) throw "error";
		} while (false);

		unsigned int  nbytes_in_lo32, nbytes_in_hi32;
		unsigned int  nbytes_out_lo32, nbytes_out_hi32;
		BZ2_bzWriteClose64(&bzerror, bz, 0, &nbytes_in_lo32, &nbytes_in_hi32, &nbytes_out_lo32, &nbytes_out_hi32);
		return 0;

	}
	catch (...){
		unsigned int  nbytes_in_lo32, nbytes_in_hi32;
		unsigned int  nbytes_out_lo32, nbytes_out_hi32;
		BZ2_bzWriteClose64(&bzerror, bz, 1, &nbytes_in_lo32, &nbytes_in_hi32, &nbytes_out_lo32, &nbytes_out_hi32);
		fclose(fp);

		std::cout << "error" << std::endl;
		return 0;
	}
}

