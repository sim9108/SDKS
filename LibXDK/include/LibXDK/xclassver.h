/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef	NO_PRAGMA_ONCE
#pragma	once
#endif


#ifndef	XCLASSVER_H
#define	XCLASSVER_H

#include "dversion.h"

/*****************************************************************************
 *  UTILITY MACROS
 *  --------------
 *	These macros calculate a precise version for your Xtra to be used in the
 *	Xtra definition below.  The first 4 #defines represent the release types
 *	you can choose when assembling your version.
 ****************************************************************************/ 
#define VERSION_DEVELOPMENT  0
#define VERSION_ALPHA        1
#define VERSION_BETA         2
#define VERSION_RELEASE      3

#define MAKE_VERSION(major, minor, sub, releaseVersion, releaseType) \
	                  (unsigned long)(((unsigned long)(major & 0x0ff) << 24) + \
	                  ((unsigned long)(minor & 0x0ff) << 16) + \
	                  ((unsigned long)(sub & 0x0ff) << 8) + \
	                  ((unsigned long)(releaseType & 0x003) << 6) + \
	                  (unsigned long)(releaseVersion & 0x03f) )

/*****************************************************************************
 *	VERSION DEFINITION
 *	------------------
 *	Fill in your version numbers below.
 ****************************************************************************/ 
#define XTRA_CLASS_MAJOR_VERSION    DVER_MAJORVERSION
#define XTRA_CLASS_MINOR_VERSION    DVER_MINORVERSION
#define XTRA_CLASS_SUB_VERSION      DVER_BUGFIXVERSION
#define XTRA_CLASS_RELEASE_TYPE     VERSION_RELEASE
#ifndef XTRA_CLASS_RELEASE_VERSION
#define XTRA_CLASS_RELEASE_VERSION  0
#endif


#define XTRA_CLASS_VERSION MAKE_VERSION(XTRA_CLASS_MAJOR_VERSION, \
                                         XTRA_CLASS_MINOR_VERSION, \
                                         XTRA_CLASS_SUB_VERSION, \
                                         XTRA_CLASS_RELEASE_VERSION, \
                                         XTRA_CLASS_RELEASE_TYPE)
                                    

#endif	/* XCLASSVER_H */
