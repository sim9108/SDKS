#include "PngWriter.h"
#include <stdexcept>
#include <iostream>

namespace PNG{
	void error_handler(png_structp ptr, png_const_charp cmt){
		throw std::runtime_error(cmt);
	}

	void warning_handler(png_structp ptr, png_const_charp cmt){
		std::cout << "warning:" << cmt << std::endl;
	}

	PngWriter::PngWriter()
		: // 객체 초기화 목록
		write_ptr_{ png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr) },
		info_ptr_{ png_create_info_struct(write_ptr_) },
		failed_(false),
		error_("")
	{
		png_set_error_fn(write_ptr_, nullptr, error_handler, warning_handler);
	}


	PngWriter::~PngWriter()
	{
		png_destroy_info_struct(write_ptr_, &info_ptr_);
		png_destroy_write_struct(&write_ptr_, nullptr);
	}
	
	PngWriter&
	PngWriter::operator>>(FILE* fp){
		try{
			png_init_io(write_ptr_, fp);
		}
		catch (std::runtime_error& ex){
			this->failed_ = true;
			this->error_ = ex.what();
		}
		return *this;
	}

	PngWriter::operator bool(){
		return !this->failed_;
	}

	const char*
	PngWriter::error_string(){
		return this->error_;
	}

	PngWriter&
	PngWriter::operator <<(IMAGE& datas){
		try{
			auto height = png_get_image_height(write_ptr_, info_ptr_);
			auto bytes_stride = png_get_rowbytes(write_ptr_, info_ptr_);

			if (datas.size() < height) throw std::runtime_error("operator <<(IMAGE& datas) height error");
			for (auto& item : datas){
				if (item.size() < bytes_stride)  throw std::runtime_error("operator <<(IMAGE& datas) bytes_stride error");
			}
			png_byte* m[1];
			for (unsigned int h = 0; h < height; ++h){
				m[0] = datas[h].data();
				png_write_rows(write_ptr_, m, 1);
			}
			png_write_end(write_ptr_, info_ptr_);
		}
		catch (std::runtime_error& ex){
			this->failed_ = true;
			this->error_ = ex.what();
		}

		return *this;
	}

	PngWriter&
	PngWriter::operator <<(LINE& line){
		try{				
			auto bytes_stride = png_get_rowbytes(write_ptr_, info_ptr_);
			if (line.size() < bytes_stride)  throw std::runtime_error("operator <<(IMAGE& datas) bytes_stride error");

			auto height = png_get_image_height(write_ptr_, info_ptr_);
			png_byte* m[1];
			for (unsigned int h = 0; h < height; ++h){
				m[0] = line.data();
				png_write_rows(write_ptr_, m, 1);
			}
			png_write_end(write_ptr_, info_ptr_);
		}
		catch (std::runtime_error& ex){
			this->failed_ = true;
			this->error_ = ex.what();
		}
		return *this;
	}


	void set_bgr(png_structp write_ptr, png_infop info_ptr){
		png_set_bgr(write_ptr);
	}

	void write_info(png_structp write_ptr, png_infop info_ptr){
		png_write_info(write_ptr, info_ptr);
	}
	
	CALLFN get_height(unsigned int & mvalue){
		auto fn = [&](png_structp write_ptr, png_infop info_ptr){
			mvalue = png_get_image_height(write_ptr, info_ptr);
		};
		return fn;
	}

	CALLFN get_width(unsigned int & mvalue){
		auto fn = [&](png_structp write_ptr, png_infop info_ptr){
			mvalue = png_get_image_width(write_ptr, info_ptr);
		};
		return fn;
	}


	CALLFN set_IDHR(png_uint_32 width, png_uint_32 height, int bit_depth,
		int color_type, int interlace_method, int compression_method,
		int filter_method){

		auto fn = [=](png_structp write_ptr, png_infop info_ptr){
			png_set_IHDR(
				write_ptr, info_ptr,
				width, height, bit_depth,
				color_type, interlace_method,
				compression_method,
				filter_method
				);
		};
		return fn;
	}


}