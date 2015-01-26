#include "WrapZip.h"
#include <ctime>
#include <string>
#include <fstream>

zip_fileinfo& operator<< (zip_fileinfo& info, std::tr2::sys::path& mpath){
	memset(&info, 0, sizeof(zip_fileinfo));
	std::time_t tm = std::tr2::sys::last_write_time(mpath);
	struct tm* tdata = std::localtime(&tm);
	info.tmz_date.tm_year = tdata->tm_year;
	info.tmz_date.tm_mon = tdata->tm_mon;
	info.tmz_date.tm_mday = tdata->tm_mday;
	info.tmz_date.tm_hour = tdata->tm_hour;
	info.tmz_date.tm_min = tdata->tm_min;
	info.tmz_date.tm_sec = tdata->tm_sec;

	return info;
}

void folderlist(std::tr2::sys::path root, std::vector< std::tr2::sys::path>& filelist){
	using namespace std::tr2::sys;
	directory_iterator itr;
	directory_iterator path_end;

	for (itr = root; itr != path_end; ++itr){
		if (is_directory(itr->path())){
			folderlist(*itr, filelist);
			continue;
		}
		filelist.push_back(itr->path());
	}
}

ZipCTL::ZipCTL(std::string fname, ZipFlag flags/* = ZipFlag::ZF_NEW*/)
	: zf_{ zipOpen(fname.data(), static_cast<int>(flags) ) }
{
}

ZipCTL::~ZipCTL()
{
	if (!this->zf_) return;
	zipClose(this->zf_, nullptr);
}

ZipCTL::operator zipFile& (){
	return this->zf_;
}

bool
ZipCTL::operator!() {
	return !this->zf_;
}

void 
ZipCTL::append(std::tr2::sys::path fname){
	using namespace std::tr2::sys;
	bool bdir = is_directory(fname);

	switch (bdir){
	case false:
		append_file(fname);
		break;
	case true:
		{
			std::vector< std::tr2::sys::path> filelist;
			folderlist(fname, filelist);
			for (auto& mitem : filelist){
				append_file(mitem, fname);
			}
			break;
		}
	}
}

void
ZipCTL::append_file(std::tr2::sys::path fname, std::tr2::sys::path root){
	using namespace std::tr2::sys;

	zip_fileinfo info;
	info << fname;
		
	auto size = root.string().size()+1 ;
	std::tr2::sys::path savepath = fname.string().substr(size);

	zipOpenNewFileInZip(this->zf_,
		savepath.string().c_str(),
		&info,
		nullptr,0,
		nullptr,0,
		nullptr,
		Z_DEFLATED,Z_DEFAULT_COMPRESSION
		);

	std::ifstream fp(fname, std::ios_base::binary);

	const int BUFSIZE = 4096;
	Bytef buf[BUFSIZE];
	do{
		fp.read(reinterpret_cast<char*>(buf), BUFSIZE);
		std::size_t size = static_cast<std::size_t>(fp.gcount());
		zipWriteInFileInZip(this->zf_, buf, size);
	} while (!fp.eof());

	zipCloseFileInZip(this->zf_);
}


/////
unz_file_info& operator>> (unz_file_info& info, time_t& tval){
	struct tm tdata;
	memset(&tdata, 0, sizeof(struct tm));
	tdata.tm_year = info.tmu_date.tm_year;
	tdata.tm_mon = info.tmu_date.tm_mon;
	tdata.tm_mday = info.tmu_date.tm_mday;
	tdata.tm_hour = info.tmu_date.tm_hour;
	tdata.tm_min = info.tmu_date.tm_min;
	tdata.tm_sec = info.tmu_date.tm_sec;
	tval = mktime(&tdata);
	return info;
}

UnzipCTL::UnzipCTL(std::string fname)
	:uf_{ unzOpen(fname.c_str()) }
{
}

UnzipCTL::~UnzipCTL(){
	if (!uf_) return;
	unzClose(uf_);
}

UnzipCTL::operator unzFile&(){
	return this->uf_;
}

bool 
UnzipCTL::operator!() {
	return !this->uf_;
}

void 
UnzipCTL::extract(std::tr2::sys::path root){
	UzContainer datas(this->uf_);
	for (auto& item : datas){
		item.save_file(root);
	}
}

//UzCurrentFile
UzCurrentFile::UzCurrentFile(unzFile& uf)
	:uf_{ uf }
{
	this->filename_[0] = 0;
	this->comment_[0] = 0;
	unzGetCurrentFileInfo(uf_, &info_,
		filename_, _MAX_PATH,
		nullptr, 0,
		comment_, _MAX_PATH
		);
}

bool
UzCurrentFile::save_file(const std::string targetdir){
	using namespace std::tr2::sys;

	// 폴더 생성하기
	path targetfile(targetdir);
	targetfile /= path(filename_);
	try{
		create_directories(targetfile.branch_path());
	}
	catch (...)	{
		return false;
	}

	/// 파일 생성하기
	unzOpenCurrentFile(uf_);
	{
		const int MAX_BUF = 4096;
		std::ofstream ofs(targetfile, std::ios_base::binary);
		char buf[MAX_BUF];
		for (;;){
			int len = unzReadCurrentFile(uf_, buf, MAX_BUF);
			if (0 == len) break;
			ofs.write(buf, len);
		}
	}
	unzCloseCurrentFile(uf_);

	// 생성 날짜 변경하기
	time_t tval;
	this->info_ >> tval;
	last_write_time(targetfile, tval);
	return true;
}


//UzIterator
static unzFile zfile = nullptr;
static UzIterator uzend{ zfile };

UzIterator::UzIterator(unzFile& uf)
	:uf_{ uf }
{
	if (!uf) return;
	ret_ = unzGoToFirstFile(uf_);
}

void
UzIterator::operator++(){
	ret_ = unzGoToNextFile(uf_);
}

void
UzIterator::operator++(int){
	ret_ = unzGoToNextFile(uf_);
}

UzCurrentFile
UzIterator::operator*(){
	return UzCurrentFile{ uf_ };
}

bool
UzIterator::operator !=(UzIterator& rhs){
	if (UNZ_OK == ret_){
		return &this->uf_ != &rhs.uf_;
	}
	return &uzend.uf_ != &rhs.uf_;
}

//UzContainer
UzContainer::UzContainer(unzFile& uf)
	:uf_{ uf }
{
}

UzContainer::UzContainer(UnzipCTL& ctl)
: uf_(ctl)
{
}

UzContainer::~UzContainer()
{
}

UzIterator
UzContainer::begin(){
	return UzIterator(uf_);
}

UzIterator
UzContainer::end(){
	return uzend;
}