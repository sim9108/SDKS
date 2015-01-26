#pragma once
#include <unzip.h>
#include <zip.h>
#include <filesystem>
#include <vector>

zip_fileinfo& operator<< (zip_fileinfo& info, std::tr2::sys::path& mpath);
void folderlist(std::tr2::sys::path root, std::vector< std::tr2::sys::path>& filelist);

class ZipCTL
{
public:
	enum class ZipFlag{
		ZF_NEW = APPEND_STATUS_CREATE,
		ZF_AFTER = APPEND_STATUS_CREATEAFTER,
		ZF_ADD = APPEND_STATUS_ADDINZIP
	};
private:
	zipFile zf_;
public:
	explicit ZipCTL(std::string fname, ZipFlag flags = ZipFlag::ZF_NEW);
	~ZipCTL();

	operator zipFile& ();

	void append(std::tr2::sys::path fname);

	bool operator!();

	void append_file(std::tr2::sys::path fname, std::tr2::sys::path root = ".");
};

//unzip
unz_file_info& operator>> (unz_file_info& info, time_t& tval);

class UnzipCTL{
private:
	unzFile uf_;
public:
	explicit UnzipCTL(std::string fname);
	~UnzipCTL();

	operator unzFile& ();

	bool operator!();

	void extract(std::tr2::sys::path root);
};

class UzCurrentFile
{
	friend class UnzipCTL;
private:
	unz_file_info info_;
	char filename_[_MAX_PATH];
	char comment_[_MAX_PATH];

	unzFile& uf_;
public:
	UzCurrentFile(unzFile& uf);
	bool save_file(const std::string targetdir);
};


class UzIterator
	:public std::iterator<std::input_iterator_tag, UzCurrentFile>
{
private:
	unzFile& uf_;
	int ret_;
public:
	UzIterator(unzFile& uf);

	void operator++();
	void operator++(int);

	UzCurrentFile operator*();
	bool operator !=(UzIterator& rhs);
};

class UzContainer
{
private:
	unzFile& uf_;

public:
	explicit UzContainer(unzFile& uf);
	explicit UzContainer(UnzipCTL& ctl);
	~UzContainer();

	UzIterator begin();
	UzIterator end();
};