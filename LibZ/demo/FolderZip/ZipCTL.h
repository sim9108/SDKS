#pragma once
#include <zip.h>
#include <string>
#include <fstream>
#include <filesystem>

class ZipCTL
{
private:
	zipFile zf_;

public:
	class NewCTL{
	private:
		ZipCTL& zipctl_;
		bool destory_;

	public:
		NewCTL(ZipCTL& zipctl, std::string in_name, std::string out_name);
		NewCTL(NewCTL&& rhs);
		~NewCTL();

		operator zipFile& () { return this->zipctl_; };
	};

public:
	explicit ZipCTL(std::string fname, int flags = APPEND_STATUS_CREATE);
	~ZipCTL();

	operator zipFile& ();

	void make_zip(std::tr2::sys::path mpath);

	NewCTL factory_new(std::string in_name, std::string out_name);
};

void operator <<(ZipCTL::NewCTL& f, std::ifstream& fp);


// void test(ZipCTL m);  test( ZipCTL("hello") );

