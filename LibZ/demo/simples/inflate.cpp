#include  <zlib.h>
#include  <fstream>

int  _tmain(int  argc, _TCHAR* argv[])
{
	std::ifstream fp;
	fp.open("test.enc", std::ios_base::binary);
	if (!fp) return 0;

	z_stream stream;
	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	stream.opaque = Z_NULL;

	int ret = inflateInit(&stream);//메모리  할당
	if (Z_OK != ret){
		fp.close();
		return 0;
	}

	const  int BUF = 1024;
	Bytef in[BUF];
	Bytef out[BUF];
	std::ofstream wp;
	wp.open("test_inflate.bmp", std::ios_base::binary);

	do {
		fp.read((char*)in, BUF);
		int readsize = (int)fp.gcount();

		//입력  버퍼  설정
		stream.avail_in = readsize;
		stream.next_in = in;

		do {// 모든  입력  버퍼  연산  실행
			stream.avail_out = BUF;
			stream.next_out = out;
			ret = inflate(&stream, Z_NO_FLUSH);
			//실행된  결과  저장
			int dsize = BUF - stream.avail_out;
			wp.write((const  char*)out, dsize);
		} while (0 != stream.avail_in);

	} while (Z_STREAM_END != ret || !fp.eof());

	inflateEnd(&stream);//메모리  해제
	return 0;
}