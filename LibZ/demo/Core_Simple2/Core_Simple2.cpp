#include "stdafx.h"
#include <zlib.h>
#include <iostream>
#include <fstream>

int _tmain(int argc, _TCHAR* argv[]){
	z_stream stream;
	std::memset(&stream, 0, sizeof(z_stream));

	inflateInit(&stream);//pairing 1
	std::ifstream rp;
	rp.open("test.enc", std::ios_base::binary);

	std::ofstream wp;
	wp.open("test.bmp", std::ios_base::binary);

	const int BUF = 1024;
	Bytef in[BUF];
	Bytef out[BUF];

	int ret;
	do{
		rp.read(reinterpret_cast<char*>(in), BUF);
		int readsize = (int)rp.gcount();

		stream.avail_in = readsize;
		stream.next_in = in;
		
		do{
			stream.avail_out = BUF;
			stream.next_out = out;
			ret = inflate(&stream, Z_NO_FLUSH);
			int dsize = BUF - stream.avail_out;
			wp.write((const char*)out, dsize);
		} while (0 == stream.avail_out);
	} while (Z_STREAM_END != ret );

	inflateEnd(&stream);//pairing 1
	return 0;
}

