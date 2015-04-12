#include "_Basic.h"
#include <jpeglib.h>

//_Basic

void exit_handle(j_common_ptr cinfo){
	throw "error";
}

_Basic::MAP_FN
_Basic::map_ = {
		{ "readfile", { "readfile(fname:UTF8STRING):IMAGE", &readfile } },
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
			ma[0] = bimg.ptr<unsigned char>(
				cinfo.output_height - cinfo.output_scanline-1
				);
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

/*
HRESULT
_Basic::readfile(MoaMmCallInfo& callPtr){
	std::wstring file;
	bool bret = this->utils_.read(callPtr, 0, file);
	if (!bret) return kMoaErr_BadParam;
	
	std::locale loc("");
	std::string cfile = wchar_to_ansi(file, loc);

	Jpeg_Info info_;
	bret = info_.read(cfile);
	if (!bret){
		this->utils_.ValueRelease(callPtr);
		return kMoaErr_NoErr;
	}
	
	this->utils_.NewImage(info_.width(), info_.height(), 32, 0, NULL, &callPtr.resultValue);
	ImageLock bimg{ *this, callPtr.resultValue };

	auto size = info_.height();
	for (unsigned int y = 0; y < size; ++y){
		std::memcpy(bimg.ptr<BYTE>(y), info_.row_ptr(size - y - 1), info_.row_bytes());
	}
		
	return kMoaErr_NoErr;
}
*/
