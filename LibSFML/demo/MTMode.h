#pragma once
#define SFML_STATIC
#define GLEW_STATIC

#include <SFML/Graphics.hpp>

#ifdef _DEBUG
#pragma comment(lib,"LibSFMLSystemSMTd.lib")
#pragma comment(lib,"LibSFMLGraphicsSMTd.lib")
#pragma comment(lib,"LibSFMLWindowSMTd.lib")
#pragma comment(lib,"LibSFMLNetworkSMTd.lib")
#pragma comment(lib,"LibSFMLAudioSMTd.lib")
#pragma comment(lib,"LibJpegTurboSMTd.lib")
#pragma comment(lib,"LibGLewSMTd.lib")
#else
#pragma comment(lib,"LibSFMLSystemSMT.lib")
#pragma comment(lib,"LibSFMLGraphicsSMT.lib")
#pragma comment(lib,"LibSFMLWindowSMT.lib")
#pragma comment(lib,"LibSFMLNetworkSMT.lib")
#pragma comment(lib,"LibSFMLAudioSMT.lib")
#pragma comment(lib,"LibJpegTurboSMT.lib")
#pragma comment(lib,"LibGLewSMT.lib")
#endif

#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"Opengl32.lib")