#pragma once
#include <unzip.h>
#include <iostream>
#include <string>
#include <iterator>

//CurrentFile
class UzCurrentFile{
	unz_file_info info_;
	char filename_[_MAX_PATH];
	char comment_[_MAX_PATH];

	unzFile& uf_;
public:
	UzCurrentFile(unzFile& uf);
	~UzCurrentFile();

	const char* filename();
	bool save_file(const std::string targetdir);
};

//UzIterator
class UzIterator :public std::iterator<std::forward_iterator_tag, unzFile>
{
	unzFile& uf_;
	int ret_;

public:
	UzIterator(unzFile& uf);

	void operator ++();
	bool operator !=(UzIterator& rhs);
	UzCurrentFile operator* ();
};

// UzContainer
class UzContainer{
private:
	unzFile& uf_;

public:
	UzContainer(unzFile& uf);
	~UzContainer();

	UzIterator begin();
	UzIterator end();
};

