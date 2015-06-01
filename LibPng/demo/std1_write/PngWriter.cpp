#include "PngWriter.h"
#include <iostream>
#include <stdexcept>

namespace PNG{

	void error_handler(png_structp ptr, png_const_charp cmt)
	{
		throw std::runtime_error(cmt);
	}
	void warning_handler(png_structp ptr, png_const_charp cmt)
	{
		std::cout << "warning:" << cmt << std::endl;
	}

	PngWriter::PngWriter()
		: 
	write_ptr_{
		png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, error_handler, warning_handler)
	},
	info_ptr_{
		png_create_info_struct(write_ptr_)
	},
	failed_{ false },
	error_{ "" }
	{
	}
	

	PngWriter::~PngWriter()
	{
		png_destroy_info_struct(write_ptr_, &info_ptr_);
		png_destroy_write_struct(&write_ptr_, nullptr);
	}
	

	PngWriter::operator bool() const {
		return !this->failed_;
	}

	const char*
	PngWriter::error_string() const{
		return this->error_;
	}

	PngWriter&
	PngWriter::operator >> (FILE* fp){
		try{
			png_init_io(this->write_ptr_, fp);
		}
		catch (std::runtime_error& ex){
			this->failed_ = true;
			this->error_ = ex.what();
		}
		return *this;
	}



}