#pragma once

#if defined(LZ4__NO_DLL)
#define LZ4_API

#elif defined(_MSC_VER)
#ifdef LZ4_API_EXPORTS
#define	LZ4_API __declspec(dllexport)
#else
#define LZ4_API __declspec(dllimport)
#endif

#elif defined(LZ4__USE_VISIBILITY_ATTR)
#define LZ4_API __attribute__ ((visibility ("default")))

#else
#define LZ4_API

#endif