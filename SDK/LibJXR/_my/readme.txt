src/imge/sys/strcodec.h

//#ifdef WIN32
#if (defined(WIN32) || defined(_WIN64)) && !defined(UNDER_CE) 


src/jxrgluelib/JXRMeta.h

#if !defined(WIN32) && !defined(_WIN64) 
#include <wmspecstring.h>
#endif

#if  defined(WIN32) || defined(_WIN64)
#define __in_win    __in
#define __out_win   __out
#endif