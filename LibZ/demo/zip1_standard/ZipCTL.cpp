#include "ZipCTL.h"
#include "zip_utils.h"
#include <filesystem>

void operator <<(ZipCTL::NewCTL& f, std::ifstream& fp){
	const int BUFSIZE = 4096;
	Bytef buf[BUFSIZE];
	do{
		fp.read(reinterpret_cast<char*>(buf), BUFSIZE);
		std::size_t size = static_cast<std::size_t>(fp.gcount());
		zipWriteInFileInZip(f, buf, size);
	} while (!fp.eof());
}

ZipCTL::ZipCTL(std::string fname, int flags/* = APPEND_STATUS_CREATE*/)
: zf_{ zipOpen(fname.data(), flags) }
{
}

ZipCTL::~ZipCTL()
{
	if (!this->zf_) return;
	zipClose(this->zf_, "global_comment");
}

ZipCTL::operator zipFile& (){
	return this->zf_;
}

ZipCTL::NewCTL::NewCTL(ZipCTL& zipctl, std::string in_name, std::string out_name)
: zipctl_{ zipctl }, destory_{ true }
{
	zip_fileinfo info;
	info << std::tr2::sys::path(out_name);

	zipOpenNewFileInZip(zipctl,
		in_name.c_str(),
		&info,
		nullptr,
		0,
		nullptr,
		0,
		"local comment",
		Z_DEFLATED,
		Z_DEFAULT_COMPRESSION
		);
}

ZipCTL::NewCTL::NewCTL(NewCTL&& rhs)
:zipctl_{ rhs.zipctl_ }, destory_{true}
{
	rhs.destory_ = false;
}

ZipCTL::NewCTL::~NewCTL(){
	if (!this->destory_) return;
	zipCloseFileInZip(this->zipctl_);
}

ZipCTL::NewCTL
ZipCTL::factory_new(std::string in_name, std::string out_name){
	return ZipCTL::NewCTL(*this, in_name, out_name);
}