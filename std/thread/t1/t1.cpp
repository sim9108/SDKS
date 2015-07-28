// t1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <thread>

void hello(){
	std::cout << "Hello Concurrent World" << std::endl;
	std::cout << "thread id:" << std::this_thread::get_id() << std::endl;
}


int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "thread id:" << std::this_thread::get_id()<<std::endl;
	std::thread t(hello);	
	t.join();
	return 0;
}

