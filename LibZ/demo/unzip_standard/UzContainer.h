#pragma once
#include <unzip.h>
#include <iterator>
#include <string>

unz_file_info& operator>> (unz_file_info& info, time_t& tval);

class UzCurrentFile
{
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
	UzContainer(unzFile& uf);
	~UzContainer();

	UzIterator begin();
	UzIterator end();
};

