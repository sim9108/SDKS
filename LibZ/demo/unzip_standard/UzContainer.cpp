#include "UzContainer.h"
#include <fstream>
#include <filesystem>

unz_file_info& operator>> (unz_file_info& info, time_t& tval){
	    struct tm tdata;
		memset(&tdata, 0, sizeof(struct tm));
		tdata.tm_year = info.tmu_date.tm_year;
		tdata.tm_mon = info.tmu_date.tm_mon;
		tdata.tm_mday = info.tmu_date.tm_mday;
		tdata.tm_hour = info.tmu_date.tm_hour;
		tdata.tm_min= info.tmu_date.tm_min  ;
		tdata.tm_sec= info.tmu_date.tm_sec;
		tval=mktime(&tdata);
		return info;
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
	targetfile /=path(filename_);
	try{
		 create_directories(targetfile.branch_path());
	}
	catch (...)	{
		return false;
	}
	/// 파일 생성하기
	unzOpenCurrentFile(uf_);
	{
		const int MAX_BUF= 4096;
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
:uf_{uf}
{
	if (!uf) return;
	ret_=unzGoToFirstFile(uf_);
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
	return UzCurrentFile{uf_};
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
:uf_{uf}
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