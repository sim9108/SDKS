// helloworld.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <dshow.h>
#include <atlbase.h>
#include <iostream>
#pragma comment(lib, "strmiids.lib")

int _tmain(int argc, _TCHAR* argv[])
{   
	// COM 기반 설계
	auto hr = CoInitialize(nullptr);
	{
		CComPtr<IGraphBuilder> pGraph;
		CoCreateInstance(
			CLSID_FilterGraph,
			nullptr,
			CLSCTX_INPROC_SERVER,
			__uuidof(IGraphBuilder),
			reinterpret_cast<void**>(&pGraph)
			);

		pGraph->RenderFile(LR"(C:\Users\test.mpeg)", nullptr);

		CComQIPtr<IMediaControl> pControl(pGraph);
		pControl->Run();

		std::cin.get();
	}
	CoUninitialize();
	return 0;
}

