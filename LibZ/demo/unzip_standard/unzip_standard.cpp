// unzip_standard.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "UzContainer.h"
#include <string>
#include <filesystem>

std::string
wchar_to_ansi(const std::wstring& wstr, const std::locale& loc){
	typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
	codecvt_t const& codecvt = std::use_facet<codecvt_t>(loc);

	std::string temp((wstr.size()) * codecvt.max_length(), (char)0);

	wchar_t const* in_next = wstr.c_str();
	char* out_next = &temp[0];

	std::mbstate_t state = std::mbstate_t();
	std::codecvt_base::result r = codecvt.out(state,
		wstr.c_str(), wstr.c_str() + wstr.size(), in_next,
		&temp[0], &temp[0] + temp.size(), out_next);
	if (r == std::codecvt_base::error) return "";

	std::size_t len = out_next - &temp[0];
	temp.resize(len);
	return temp;
}

int _tmain(int argc, _TCHAR* argv[])
{
	using namespace std::tr2::sys;

	if (3 != argc) return 0;
	
	std::locale loc("");
	std::string zipfilename = wchar_to_ansi(argv[1], loc);
	std::string targetdir = wchar_to_ansi(argv[2], loc);

	unzFile uf = unzOpen(zipfilename.c_str());
	UzContainer uzdatas{ uf };
	for (auto& m : uzdatas){
		m.save_file(targetdir);
	}	
	unzClose(uf);
	return 0;
}

