#pragma once

#ifdef _DEBUG
#define DEBUG_FIX "d"
#else
#define DEBUG_FIX ""
#endif

#if defined(_MT) && defined(_DLL)
#define RT_FIX ""
#else
#define RT_FIX "MT"
#endif

#if defined(DYNAIC_MODE)
#define STATIC_FIX ""
#else
#define STATIC_FIX "S"
#define SFML_STATIC
#define GLEW_STATIC
#endif

#pragma comment(lib,"LibSFMLSystem" STATIC_FIX  RT_FIX  DEBUG_FIX".lib")
#pragma comment(lib,"LibSFMLGraphics" STATIC_FIX  RT_FIX  DEBUG_FIX".lib")
#pragma comment(lib,"LibSFMLWindow" STATIC_FIX  RT_FIX  DEBUG_FIX".lib")
#pragma comment(lib,"LibSFMLNetwork" STATIC_FIX  RT_FIX  DEBUG_FIX".lib")
#pragma comment(lib,"LibSFMLAudio" STATIC_FIX  RT_FIX  DEBUG_FIX".lib")
#pragma comment(lib,"LibJpegTurbo" STATIC_FIX  RT_FIX  DEBUG_FIX".lib")
#pragma comment(lib,"LibGLew" STATIC_FIX  RT_FIX  DEBUG_FIX".lib")
#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"Opengl32.lib")