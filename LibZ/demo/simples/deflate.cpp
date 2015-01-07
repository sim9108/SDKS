#include  <zlib.h>
#include  <fstream>

int  _tmain(int  argc, _TCHAR* argv[])
{
	std::ifstream fp;
	fp.open("test.bmp", std::ios_base::binary);
	if (!fp) return 0;

	z_stream stream;
	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	stream.opaque = Z_NULL;

	// 메모리  할당
	int ret = deflateInit(&stream, Z_DEFAULT_COMPRESSION);
	if (Z_OK != ret){
		fp.close();
		return 0;
	}

	// 압축  코딩이... 위치하게  됨.
	const  int BUF = 1024;
	Bytef  in[BUF];
	Bytef  out[BUF];

	std::ofstream wp;
	wp.open("test.enc", std::ios_base::binary);

	do{
		fp.read((char*)in, BUF);
		int readsize = (int)fp.gcount();

		//입력  버퍼  설정
		stream.avail_in = readsize;
		stream.next_in = in;

		int flush = fp.eof() ? Z_FINISH : Z_NO_FLUSH;
		do{// 모든  입력  버퍼  연산  실행
			//입력  버퍼  설정
			stream.avail_out = BUF;
			stream.next_out = out;
			ret = deflate(&stream, flush);

			//실행된  결과  저장
			int dsize = BUF - stream.avail_out;
			wp.write((const  char*)out, dsize);
		} while (0 != stream.avail_in);
	} while (Z_STREAM_END != ret);

	deflateEnd(&stream);// 메모리  해제

	return 0;
}