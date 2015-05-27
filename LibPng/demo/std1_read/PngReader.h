#pragma once
#include <png.h>
#include <vector>
// 반복적인 부분 요소 지원(메모리, 에러 핸들링,reading)
// 추가로 필요한 확장_ 열어버리자.

namespace PNG{
	typedef unsigned char BYTE;
	typedef BYTE   PIXEL;
	typedef std::vector<PIXEL> LINE;
	typedef std::vector<LINE> IMAGE;

	typedef void PNG_OP(png_structp, png_infop);

	class PngReader
	{
	private:
		png_structp read_ptr_;
		png_infop info_ptr_;

		bool failed_;
		const char* error_;
	public:
		PngReader();
		~PngReader();

		PngReader& operator <<(FILE* fp);
		PngReader& operator << (PNG_OP fn);

		operator bool();// bool 변환 연산자.
		const char* error_string();

		PngReader& operator >>(IMAGE& datas);		
	};

	void read_update_info(png_structp read_ptr, png_infop info_ptr);
	void read_info(png_structp read_ptr, png_infop info_ptr);
	void set_bgr(png_structp read_ptr, png_infop info_ptr);
	void set_rgba(png_structp read_ptr, png_infop info_ptr);
}