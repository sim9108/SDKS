/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

/*
	Purpose
		Include appropriate moatyedg file for the platform.
*/

#ifndef	NO_PRAGMA_ONCE
#pragma	once
#endif

#ifndef _H_moaplat
#define	_H_moaplat



/*--------------------------------------------------------------------------*/
#if (defined(WINDOWS) || defined(_WINDOWS)|| defined(WINVER))
	#if (defined(WIN32) || defined(_WIN32))
		#include "win32/moatyedg.h"
	#else
		#error unknown windows development environment
	#endif
#endif /* WINDOWS || _WINDOWS */

#if (defined(MACINTOSH) || defined(_MACINTOSH) || defined(macintosh))		
	#if defined(__MACH__)
		#include "macmach/moatyedg.h"
	#else 
		#error unknown macintosh development environment
	#endif
#endif



/*-------------------------------------------------------------------------*/

#endif	/* _H_moaplat */
