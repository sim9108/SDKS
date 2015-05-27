#pragma once
#include <png.h>
#include <vector>
#include <functional>

namespace PNG{
	typedef unsigned char BYTE;
	typedef BYTE   PIXEL;
	typedef std::vector<PIXEL> LINE;
	typedef std::vector<LINE> IMAGE;


	class PngWriter
	{
	private:
		png_structp write_ptr_;
		png_infop info_ptr_;

		bool failed_;
		const char* error_;

	public:
		PngWriter();
		~PngWriter();

		PngWriter& operator >>(FILE* fp);

		template<typename FN>
		PngWriter& operator << (FN&& fn){
			try{
				fn(this->write_ptr_, this->info_ptr_);
			}
			catch (std::runtime_error& ex){
				this->failed_ = true;
				this->error_ = ex.what();
			}
			return *this;
		}

		operator bool();// bool 변환 연산자.
		const char* error_string();

		PngWriter& operator <<(IMAGE& datas);
		PngWriter& operator <<(LINE& line);
	};

	void set_bgr(png_structp read_ptr, png_infop info_ptr);
	void write_info(png_structp read_ptr, png_infop info_ptr);
};