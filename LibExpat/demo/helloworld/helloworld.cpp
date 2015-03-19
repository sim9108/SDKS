// helloworld.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <expat.h>
#include <fstream>
#include <sstream>
#include <cctype>
#include <string>
#include <deque>
#include <map>
#include <vector>

struct UNITDATA{
	std::string attr_name_;
	std::string data_;	
};

struct USER_DATA{
	std::deque<std::string> node_;
	std::map<std::string, std::vector<UNITDATA>> values_;
};

void XMLCALL start(void *user_data, const char *name, const char **attr);
void XMLCALL end(void *user_data, const char *name);
void XMLCALL char_data(void *user_data, const XML_Char *data, int len);

int _tmain(int argc, _TCHAR* argv[]){
	XML_Parser ex = XML_ParserCreate(nullptr);
	if (!ex){
		std::cout << "xml parser error" << std::endl;
		return 0;
	}

	USER_DATA result;
	XML_SetUserData(ex, &result);
	XML_SetElementHandler(ex, start, end);
	XML_SetCharacterDataHandler(ex, char_data);

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

	for (auto& item : result.values_){
		std::cout << item.first<<std::endl;
		for (auto& vitem : item.second){
			std::cout <<"\t"<< vitem.attr_name_ << "\t:" << vitem.data_ << std::endl;
		}

		std::cout << std::endl;
	}

	return 0;
}

void XMLCALL
start(void *user_data, const char *name, const char **attr){
	USER_DATA* udata = static_cast<USER_DATA*>(user_data);
	
	udata->node_.push_back(name);
	std::stringstream node_name;
	for (auto& item : udata->node_){
		node_name << item << "/";
	}	

	for (int i = 0; attr[i]; i += 2) {
		UNITDATA unit_data;
		unit_data.attr_name_ = attr[i];
		unit_data.data_ = attr[i + 1];
		udata->values_[node_name.str()].push_back(unit_data);
	}
}

void XMLCALL
end(void *user_data, const char *name){
	USER_DATA* udata = static_cast<USER_DATA*>(user_data);
	udata->node_.pop_back();
}

void XMLCALL 
char_data(void *user_data, const XML_Char *data, int len){
	if (1 == len && std::isspace(*data) ) return;
	USER_DATA* udata = static_cast<USER_DATA*>(user_data);

	std::stringstream node_name;
	for (auto& item : udata->node_){
		node_name << item << "/";
	}

	UNITDATA unit_data;
	unit_data.data_ = std::string(data, len);
	udata->values_[node_name.str()].push_back(unit_data);
}








