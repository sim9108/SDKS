#include "PngReader.h"

namespace PNG{
	void error_handler(png_structp ptr, png_const_charp cmt){
		throw std::runtime_error(cmt);
	}

	void warning_handler(png_structp ptr, png_const_charp cmt){
		std::cout << "warning:" << cmt << std::endl;
	}


	PngReader::PngReader()
		: // 객체 초기화 목록
		read_ptr_{ png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr) },
		info_ptr_{ png_create_info_struct(read_ptr_) },
		failed_(false),
		error_("")
	{
		png_set_error_fn(read_ptr_, nullptr, error_handler, warning_handler);
	}


	PngReader::~PngReader()
	{
		png_destroy_info_struct(read_ptr_, &info_ptr_);
		png_destroy_read_struct(&read_ptr_, nullptr, nullptr);
	}

	PngReader& 
	PngReader::operator<<(FILE* fp){
		try{
			png_init_io(read_ptr_, fp);
		}
		catch (std::runtime_error& ex){
			this->failed_ = true;
			this->error_ = ex.what();
		}
		return *this;
	}

	PngReader::operator bool(){
		return !this->failed_;
	}

	const char* 
	PngReader::error_string(){
			return this->error_;
	}



	PngReader& 
	PngReader::operator >>(IMAGE& datas){
		try{
			auto height = png_get_image_height(read_ptr_, info_ptr_);
			auto bytes_stride = png_get_rowbytes(read_ptr_, info_ptr_);
			auto pass = png_set_interlace_handling(read_ptr_);

			datas.resize(height);
			for (auto& item : datas){
				item.resize(bytes_stride);
			}
			png_byte* m[1];
			for (int ip = 0; ip < pass; ++ip){ //pass count
				for (unsigned int h = 0; h < height; ++h){
					m[0] = datas[h].data();
					png_read_rows(read_ptr_, m, nullptr, 1);
				}
			}
			png_read_end(read_ptr_, info_ptr_);
		}
		catch (std::runtime_error& ex){
			this->failed_ = true;
			this->error_ = ex.what();
		}

		return *this;
	}
	
	PngReader& 
	PngReader::operator << (PNG_OP fn){
		try{
			fn(this->read_ptr_, this->info_ptr_); 
		}
		catch (std::runtime_error& ex){
			this->failed_ = true;
			this->error_ = ex.what();
		}
		return *this;	
	}

	void set_bgr(png_structp read_ptr, png_infop info_ptr){
		png_set_bgr(read_ptr);
	}

	void read_info(png_structp read_ptr, png_infop info_ptr){
		png_read_info(read_ptr, info_ptr);
	}

	void read_update_info(png_structp read_ptr, png_infop info_ptr){
		png_read_update_info(read_ptr, info_ptr);
	}

	void set_rgba(png_structp read_ptr, png_infop info_ptr){
		auto color_type = png_get_color_type(read_ptr, info_ptr);
		auto bit_depth = png_get_bit_depth(read_ptr, info_ptr);//1,4,8,16


		if (!(PNG_COLOR_MASK_ALPHA& color_type)) png_set_add_alpha(read_ptr, 0xFF, PNG_FILLER_AFTER);
		if (PNG_COLOR_TYPE_GRAY == color_type || PNG_COLOR_TYPE_GRAY_ALPHA == color_type) png_set_gray_to_rgb(read_ptr);
		if (PNG_COLOR_TYPE_PALETTE == color_type) png_set_palette_to_rgb(read_ptr);
		if (png_get_valid(read_ptr, info_ptr, PNG_INFO_tRNS))  png_set_tRNS_to_alpha(read_ptr);
		if (bit_depth > 8) png_set_strip_16(read_ptr);
		if (bit_depth < 8) png_set_expand(read_ptr);

		auto pass = png_set_interlace_handling(read_ptr);
	}
}