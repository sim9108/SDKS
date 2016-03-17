#pragma once
#if defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__)
#if defined(FREELECTWITCURL_STATIC)
#define FREELECTWITCURL_API // statically linked library
#elif defined(FREELECTWITCURL_EXPORT)
#define FREELECTWITCURL_API __declspec(dllexport)
#else
#define FREELECTWITCURL_API __declspec(dllimport)
#endif 
#elif defined(__GNUC__)
#if defined(FREELECTWITCURL_STATIC)
#define FREELECTWITCURL_API
#elif defined(FREELECTWITCURL_EXPORT)
#define FREELECTWITCURL_API __attribute__((visibility ("default")))
#else
#define FREELECTWITCURL_API
#endif

#endif