
#include "stdafx.h"
#include <zlib.h>
#include <iostream>
#include <fstream>

int _tmain(int argc, _TCHAR* argv[]){
	z_stream stream;
	std::memset(&stream, 0, sizeof(z_stream));

	deflateInit(&stream, Z_DEFAULT_COMPRESSION);//pairing 1
	std::ifstream rp;
	rp.open("test.bmp", std::ios_base::binary);

	std::ofstream wp;
	wp.open("test.enc", std::ios_base::binary);

	const int BUF = 1024;
	Bytef in[BUF];
	Bytef out[BUF];
	int ret;
	do{	
		rp.read(reinterpret_cast<char*>(in), BUF);
		int readsize =(int) rp.gcount();
				
		stream.avail_in = readsize;
		stream.next_in = in;

		int flush = rp.eof()? Z_FINISH:Z_NO_FLUSH;
		do{
			stream.avail_out = BUF;
			stream.next_out = out;
			ret = deflate(&stream, flush);

			int dsize = BUF - stream.avail_out;
			wp.write((const char*)out, dsize);
		} while (0 == stream.avail_out);
	} while (Z_STREAM_END != ret );

	deflateEnd(&stream);//pairing 1
	return 0;
}

