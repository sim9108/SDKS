// helloworld.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include <expat.h>
#include <fstream>
#include <sstream>

struct USER_DATA{
	int depth_;
};

void XMLCALL start(void *user_data, const char *name, const char **attr);
void XMLCALL end(void *user_data, const char *name);


int _tmain(int argc, _TCHAR* argv[]){

	XML_Parser ex = XML_ParserCreate(nullptr);
	if (!ex){
		std::cout << "xml parser error" << std::endl;
		return 0;
	}

	USER_DATA data;
	data.depth_ = 0;

	XML_SetUserData(ex, &data);
	XML_SetElementHandler(ex, start, end);

	std::ifstream rp("test.xml", std::ios_base::binary);
	const int BUF = 1024;
	char in[BUF];
	do{
		if (rp.eof()) break;
		rp.read(in, BUF);

		int len = static_cast<int>( rp.gcount() );
		auto ret = XML_Parse(ex, in, len, rp.eof());
		if (XML_STATUS_ERROR == ret){
			std::cout << "Error: line(" << XML_GetCurrentLineNumber(ex) << ") "
				<< "erro string:" << XML_ErrorString(XML_GetErrorCode(ex)) << std::endl;
			break;
		}
	} while (true);	

	XML_ParserFree(ex);

	return 0;
}

void XMLCALL
start(void *user_data, const char *name, const char **attr){
	USER_DATA* udata = static_cast<USER_DATA*>(user_data);
	std::stringstream ss;
	
	for (int i = 0; i < udata->depth_; i++)
		ss << "\t";

	ss << name<<" ";
	for (int i = 0; attr[i]; i += 2) {
		ss << attr[i] << "=" << attr[i + 1]<<" ";
	}
	ss << std::endl;

	std::cout << ss.str();
	udata->depth_++;
}

void XMLCALL
end(void *user_data, const char *name){
	USER_DATA* udata = static_cast<USER_DATA*>(user_data);
	udata->depth_--;
}
