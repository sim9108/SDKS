#include "ZipCTL.h"
#include "zip_utils.h"
#include <filesystem>
#include <vector>
#include <string>

void folderlist(std::tr2::sys::path root, std::vector< std::tr2::sys::path>& filelist){
	using namespace std::tr2::sys;
	directory_iterator itr;
	directory_iterator path_end;

	for (itr = root; itr != path_end; ++itr){
		//std::cout << itr->path().string() << std::endl;
		if (is_directory(itr->path())){
			folderlist(*itr, filelist);
			continue;
		}
		filelist.push_back(itr->path());
	}
}

void substr_folder(std::tr2::sys::path root, std::vector< std::tr2::sys::path>& filelist,
	std::vector<std::string>& sublist
	)
{
	auto size = root.string().size() + 1;// strlen(c:\test)+1

	sublist.reserve(filelist.size());
	for (auto& item : filelist){
		std::string mpath = item.string().substr(size);
		//std::cout << mpath << std::endl;
		sublist.push_back(mpath);
	}
}

void operator <<(ZipCTL::NewCTL& f, std::ifstream& fp){
	const int BUFSIZE = 1024;
	Bytef buf[1024];
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

void 
ZipCTL::make_zip(std::tr2::sys::path mpath){
	std::vector< std::tr2::sys::path> filelist;
	folderlist(mpath, filelist);

	std::vector<std::string> sublist;
	substr_folder(mpath, filelist, sublist);
	
	auto size = filelist.size();
	for (std::size_t i = 0; i < size; ++i){
		auto mpath = filelist[i];
		auto submpath = sublist[i];
		NewCTL newadd(*this, mpath, submpath);
		std::ifstream fp(mpath.string());
		newadd << fp;
	}
}