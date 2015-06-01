#pragma once
#include <png.h>
#include <vector>

namespace PNG{
	typedef unsigned char BYTE;
	typedef BYTE PIXEL;
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

		operator bool() const;
		const char* error_string() const;

		PngWriter& operator >> (FILE* fp);
	};

}