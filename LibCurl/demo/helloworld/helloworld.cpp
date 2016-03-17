// helloworld.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <curl/curl.h>
#include <string>

size_t 
write_data(void *ptr, size_t size, size_t nmemb, void *stream){
	std::string* mdata =  static_cast<std::string*>(stream);

	size_t written = size*nmemb;
	mdata->reserve(mdata->size() + written);
	
	char* temp = static_cast<char*>(ptr);
	for (size_t i = 0; i<written; ++i){
		mdata->push_back(temp[i]);
	}
	return written;
}

int 
_tmain(int argc, _TCHAR* argv[]){
	curl_global_init(CURL_GLOBAL_ALL);
	
	CURL* curl = curl_easy_init();
	if (!curl){
		std::cout << "error" << std::endl;
		return 0;
	}

	curl_easy_setopt(curl, CURLOPT_URL, "http://wttr.in/suwon");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

	std::string datas;
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &datas);

	CURLcode rc = curl_easy_perform(curl);
	if (CURLE_OK != rc){
		std::cout << "result:" << curl_easy_strerror(rc) << std::endl;
	}
	else{
		std::cout << datas << std::endl;
	}	

	curl_easy_cleanup(curl);

	return 0;
}

