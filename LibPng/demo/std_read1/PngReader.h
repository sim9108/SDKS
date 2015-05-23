#pragma once
#include <png.h>
#include <vector>

namespace PNG{
	typedef std::vector<png_byte> ROW_DATA;
	typedef std::vector<ROW_DATA> IMAGE_DATA;


	struct Png_Error_Handler
	{
		static void error_handler(png_structp ptr, png_const_charp warning);
		static void warning_handler(png_structp ptr, png_const_charp warning);
	};

	class PngReader
	{
	private:
		png_structp struct_ptr_;
		png_infop info_ptr_;
		int pass_num_;

		bool isfailed_;

	public:
		PngReader();
		~PngReader();

		PngReader& operator <<(FILE*);
		operator bool() const;

		png_uint_32 width() const;
		png_uint_32 height() const;
		png_byte color_type() const;

		png_uint_32 bit_depth() const;
		png_size_t bytes_stride() const;
		
		typedef void FN(png_structp, png_infop);
		PngReader& operator <<(FN) ;
		PngReader& operator >>(IMAGE_DATA&);

	private:
		friend std::ostream& operator<< (std::ostream& t, PngReader& png);
		void print() const;
	};

	void rgba(png_structp, png_infop);
	void bgr(png_structp, png_infop);
	void update(png_structp, png_infop);

	std::ostream& operator<< (std::ostream& t, PngReader& png);
}