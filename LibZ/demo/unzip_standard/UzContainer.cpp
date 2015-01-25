#include "UzContainer.h"
#include <fstream>

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
	unzOpenCurrentFile(uf_);/////---@@@@
	std::ofstream ofs(targetdir + filename_, std::ios_base::binary);
	// 기록 일자 변경...

	char buf[4096];
	for (;;){
		int len = unzReadCurrentFile(uf_, buf, sizeof(buf));///--^^^^^^
		if (0 == len) break;
		ofs.write(buf, len);
	}
	unzCloseCurrentFile(uf_);/////---@@@@

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