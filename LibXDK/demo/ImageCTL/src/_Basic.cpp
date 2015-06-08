#include "_Basic.h"
#include <jpeglib.h>
#include <vector>

//_Basic

void exit_handle(j_common_ptr cinfo){
	throw "error";
}

_Basic::MAP_FN
_Basic::map_ = {
		{ "readfile", { "readfile(fname:UTF8STRING):IMAGE", &readfile } },
		{ "writefile", { "writefile(img:IMAGE,fname:UTF8STRING):bool", &writefile } },
};

_Basic::MAP_GET_FN
_Basic::get_map_ = {
		{ "interface", { "interface:UTF8STRING", &get_interface } },
		
};

_Basic::MAP_SET_FN
_Basic::set_map_ = {
};

_Basic::_Basic(_MOAFactory* mobj)
	:_BaseXValue(mobj, "Basic")
{
}

_Basic::~_Basic()
{
}

HRESULT __stdcall
_Basic::CallHandler(PMoaMmCallInfo callPtr){
	std::string cmd;
	this->utils_.SymbolToString(callPtr->methodSelector, cmd);

	auto iter = map_.find(cmd);
	if (map_.end() != iter){
		return (this->*map_[cmd].second)(*callPtr);
	}

	return kMoaErr_FuncNotFound;
}

HRESULT __stdcall
_Basic::GetProp(ConstPMoaMmValue selfRef, MoaMmSymbol propName, MoaLong indexCount, ConstPMoaMmValue pIndexValues, PMoaMmValue pResult){
	std::string cmd;
	this->utils_.SymbolToString(propName, cmd);
	GetRef ref{ *selfRef, propName, indexCount, *pIndexValues, *pResult };

	auto iter = get_map_.find(cmd);
	if (get_map_.end() != iter){
		return (this->*get_map_[cmd].second)(ref);
	}

	return kMoaMmErr_PropertyNotFound;
}

HRESULT __stdcall
_Basic::SetProp(MoaMmSymbol propName, MoaLong indexCount, ConstPMoaMmValue pIndexValues, ConstPMoaMmValue pNewValue){
	std::string cmd;
	this->utils_.SymbolToString(propName, cmd);

	SetRef ref{ propName, indexCount, *pIndexValues, *pNewValue };
	auto iter = set_map_.find(cmd);
	if (set_map_.end() != iter){
		return (this->*set_map_[cmd].second)(ref);
	}

	return kMoaMmErr_PropertyNotFound;
}

void
_Basic::get_interface(std::stringstream& ss){
	ss << "-- Basic\n";
	HELPTEXT(ss, map_, set_map_, get_map_);
}

HRESULT
_Basic::get_interface(GetRef& ref){
	std::stringstream ss;
	get_interface(ss);
	this->utils_.write(ss, ref);
	return kMoaErr_NoErr;
}

HRESULT
_Basic::readfile(MoaMmCallInfo& callPtr){
	std::wstring file;
	bool bret = this->utils_.read(callPtr, 0, file);
	if (!bret) return kMoaErr_BadParam;

	std::locale loc("");
	std::string cfile = wchar_to_ansi(file, loc);

	FILE* fp = fopen(cfile.data(), "rb");
	if (!fp){
		this->utils_.ValueRelease(callPtr);
		return kMoaErr_NoErr;
	}

	jpeg_decompress_struct cinfo;
	jpeg_create_decompress(&cinfo);

	bool bstart{ false };

	try{
		jpeg_error_mgr jerr;
		jpeg_std_error(&jerr);
		jerr.error_exit = &exit_handle;
		cinfo.err = &jerr;

		jpeg_stdio_src(&cinfo, fp);
		jpeg_read_header(&cinfo, true);
		cinfo.out_color_space = JCS_EXT_BGRA;

		jpeg_start_decompress(&cinfo);
		
		auto width = cinfo.output_width;
		auto height = cinfo.output_height;

		bstart = true;
		this->utils_.NewImage(width, height, 32, 0, NULL, &callPtr.resultValue);
		ImageLock bimg{ *this, callPtr.resultValue };

		unsigned char* ma[1];
		while (cinfo.output_scanline < cinfo.output_height){
			ma[0] = bimg.ptr<unsigned char>(cinfo.output_scanline);
			if (!ma[0]) break;
			jpeg_read_scanlines(&cinfo, ma, 1);
		}

		jpeg_finish_decompress(&cinfo);
	}
	catch (...){
		if (!bstart){
			this->utils_.ValueRelease(callPtr);
		}
	}

	jpeg_destroy_decompress(&cinfo);
	fclose(fp);

	return kMoaErr_NoErr;
}

HRESULT
_Basic::writefile(MoaMmCallInfo& callPtr){
	ImageLock bimg{ *this,  callPtr, 0 };
	if (!bimg){
		return kMoaErr_BadParam;
	}

	std::wstring file;
	bool bret = this->utils_.read(callPtr, 1, file);//!!!
	if (!bret) return kMoaErr_BadParam;

	std::locale loc("");
	std::string cfile = wchar_to_ansi(file, loc);

	FILE* fp = fopen(cfile.data(), "wb");
	if (!fp){
		return kMoaErr_BadParam;
	}
	struct jpeg_compress_struct cinfo;
	jpeg_create_compress(&cinfo);

	try{
		jpeg_error_mgr jerr;
		jpeg_std_error(&jerr);
		jerr.error_exit = &exit_handle;
		cinfo.err = &jerr;

		// io
		jpeg_stdio_dest(&cinfo, fp);

		// setting
		cinfo.image_width = bimg.width();
		cinfo.image_height = bimg.height();
		cinfo.input_components = 4;
		cinfo.in_color_space = JCS_EXT_BGRA;
		jpeg_set_defaults(&cinfo);

		jpeg_set_quality(&cinfo, 100, true);

		// start compress
		jpeg_start_compress(&cinfo, true);

		// real image 
		unsigned char* ma[1];

		for (int k = 0; k < bimg.height(); ++k){
			ma[0] = bimg.ptr<unsigned char>(k);
			if (!ma[0]) break;
			jpeg_write_scanlines(&cinfo, ma, 1);			
		}		

		// end compress
		jpeg_finish_compress(&cinfo);

		this->utils_.write(true, callPtr);///!!!!
	}
	catch (...){
		this->utils_.write(false, callPtr);
	}

	jpeg_destroy_compress(&cinfo);
	fclose(fp);

	return kMoaErr_NoErr;
}
