#include "UzContainer.h"
#include <fstream>

/////////////////////////////////
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

UzCurrentFile::~UzCurrentFile(){
}

const char*
UzCurrentFile::filename(){
	return this->filename_;
}

bool 
UzCurrentFile::save_file(const std::string targetdir){
	unzOpenCurrentFile(uf_);/////---@@@@
	std::ofstream ofs(targetdir, std::ios_base::binary);
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

/////////////////////////////////
static unzFile uzend = nullptr;
static UzIterator itrend(uzend);

UzIterator::UzIterator(unzFile& uf)
	:uf_{ uf }, ret_{0}
{
	if (!uf_) return;
	ret_ = unzGoToFirstFile(uf_);
}

void 
UzIterator::operator++(){
	ret_ = unzGoToNextFile(uf_);
}

bool 
UzIterator::operator !=(UzIterator& rhs){
	if (UNZ_OK == ret_) return (rhs.uf_ != this->uf_);
	return itrend.uf_ != rhs.uf_;
}

UzCurrentFile
UzIterator::operator*(){
	return UzCurrentFile(this->uf_);
}
/////////////////////////////////
UzContainer::UzContainer(unzFile& uf)
	: uf_{ uf }
{
}

UzContainer::~UzContainer()
{
}

UzIterator
UzContainer::begin(){
	return  UzIterator(uf_);
}

UzIterator
UzContainer::end(){
	return itrend;
}

/*
bool extractZip(const std::string zippath, const std::string targetdir){
unzFile uf = unzOpen(zippath.c_str());

UzContainer datas(uf);
for (auto& m : datas){
m.save_file(targetdir + m.filename());
}

unzClose(uf);
return true;
}


int _tmain(int argc, _TCHAR* argv[])
{
extractZip("c.zip", ".\\test\\");
system("pause");

return 0;
}
*/