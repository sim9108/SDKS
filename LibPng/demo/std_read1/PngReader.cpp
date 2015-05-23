#include "PngReader.h"
#include <stdexcept>
#include <iostream>

namespace PNG{

	void
		Png_Error_Handler::error_handler(png_structp ptr, png_const_charp warning){
			throw std::runtime_error(warning);
		}

	void
		Png_Error_Handler::warning_handler(png_structp ptr, png_const_charp warning){
			std::cout << warning << std::endl;
		}


	PngReader::PngReader()
		:
		struct_ptr_{ png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr) },
		info_ptr_{ png_create_info_struct(struct_ptr_) },
		pass_num_{ 1 },
		isfailed_{false}
	{
		png_set_error_fn(this->struct_ptr_, 0, Png_Error_Handler::error_handler, Png_Error_Handler::warning_handler);
	}


	PngReader::~PngReader()
	{
		png_destroy_info_struct(struct_ptr_, &info_ptr_);
		png_destroy_read_struct(&struct_ptr_, nullptr, nullptr);
	}

	PngReader&
		PngReader::operator << (FILE* fp){
			png_init_io(this->struct_ptr_, fp);
			png_read_info(this->struct_ptr_, this->info_ptr_);
			pass_num_ = png_set_interlace_handling(this->struct_ptr_);
			return  *this;
		}

	png_uint_32
		PngReader::width() const{
			return png_get_image_width(this->struct_ptr_, this->info_ptr_);
		}

	png_uint_32
		PngReader::height() const{
			return png_get_image_height(this->struct_ptr_, this->info_ptr_);
		}

	png_byte
		PngReader::color_type() const{
			return png_get_color_type(this->struct_ptr_, this->info_ptr_);
		}

	png_uint_32
		PngReader::bit_depth() const{
			return png_get_bit_depth(this->struct_ptr_, this->info_ptr_);
		}

	png_size_t
		PngReader::bytes_stride() const{
			return png_get_rowbytes(this->struct_ptr_, this->info_ptr_);
		}

	PngReader&
		PngReader::operator<<(FN fn2) {
			fn2(this->struct_ptr_, this->info_ptr_);
			return *this;
		}

	void bgr(png_structp struct_ptr, png_infop info_ptr){
		png_set_bgr(struct_ptr);
	}
	

	PngReader::operator bool() const{
		return !this->isfailed_;
	}



	void
		PngReader::print() const{
			auto width = png_get_image_width(this->struct_ptr_, this->info_ptr_);
			auto height = png_get_image_height(this->struct_ptr_, this->info_ptr_);
			auto color_type = png_get_color_type(this->struct_ptr_, this->info_ptr_);
			auto bit_depth = png_get_bit_depth(this->struct_ptr_, this->info_ptr_);
			auto bytes_stride = png_get_rowbytes(this->struct_ptr_, this->info_ptr_);
			std::cout << "width:" << width << " height:" << height << " color_type:" << (int)color_type << " bit_depth:" << (int)bit_depth << " bytes_stride:" << bytes_stride << std::endl;
	}

	void update(png_structp struct_ptr, png_infop info_ptr){
			png_read_update_info(struct_ptr, info_ptr);
		}

	void rgba(png_structp struct_ptr, png_infop info_ptr){
		auto width = png_get_image_width(struct_ptr, info_ptr);
		auto height = png_get_image_height(struct_ptr, info_ptr);
		auto color_type = png_get_color_type(struct_ptr, info_ptr);
		auto bit_depth = png_get_bit_depth(struct_ptr, info_ptr);
		auto bytes_stride = png_get_rowbytes(struct_ptr, info_ptr);

		// DECODE REQUIRE
		if (!(PNG_COLOR_MASK_ALPHA & color_type))	png_set_add_alpha(struct_ptr, 0xFF, PNG_FILLER_AFTER);
		if (PNG_COLOR_TYPE_GRAY_ALPHA == color_type || PNG_COLOR_TYPE_GRAY == color_type) png_set_gray_to_rgb(struct_ptr);
		if (PNG_COLOR_TYPE_PALETTE & color_type) png_set_palette_to_rgb(struct_ptr);
		if (png_get_valid(struct_ptr, info_ptr, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(struct_ptr);
		if (bit_depth > 8) png_set_strip_16(struct_ptr);
		if (bit_depth < 8) png_set_expand(struct_ptr);
	}

	PngReader&
		PngReader::operator >>(IMAGE_DATA& rows){
			try{
				auto bytes_stride = png_get_rowbytes(this->struct_ptr_, this->info_ptr_);
				auto height = png_get_image_height(this->struct_ptr_, this->info_ptr_);

				rows.resize(height);
				for (auto& item : rows){
					item.resize(bytes_stride);
				}

				png_byte* m[1];
				for (int pass = 0; pass < this->pass_num_; ++pass){
					for (unsigned int h = 0; h < height; ++h){
						m[0] = rows[h].data();
						png_read_rows(this->struct_ptr_, m, nullptr, 1);
					}
				};

				png_read_end(this->struct_ptr_, this->info_ptr_);
			}
			catch (...){
				isfailed_ = true;
			}

			return *this;
		}


	std::ostream& operator<< (std::ostream& t, PngReader& png){
		png.print();
		return t;
	}
}


