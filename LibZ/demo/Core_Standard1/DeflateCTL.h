#pragma once
#include <zlib.h>
#include <vector>
#include <string>
#include <fstream>

class DeflateCTL
{
private:
	z_stream stream_;
	std::vector<Bytef> result_;

public:
	DeflateCTL();
	~DeflateCTL();

	DeflateCTL& operator <<(const std::vector<Bytef>& input);
	DeflateCTL& operator <<(const std::string& input);
	DeflateCTL& operator <<(std::ifstream& input);

	DeflateCTL&  operator << (DeflateCTL& (*fn)(DeflateCTL&)){
		return fn(*this);
	}
	
	static DeflateCTL&  endl(DeflateCTL&); // class scope
	void writefile(const std::string& fname); // instance scope
};

