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
if you want to set up different version information for different xtras in the XDK,
make a copy of this file in the source folder for the example project. The local copy will
be found before the copy in the include folder and so you can customize it as you wish.
*/
					
#ifndef DVERSION_H										
#define DVERSION_H										

#define DVER_BUILDNUM 1							
#define DVER_BUILDNUM_STRING "1"					

#define DVER_COUNTRY VER_COUNTRY_US			

#define DVER_MAJORVERSION 11				    
#define DVER_MAJORVERSION_STRING "11"

#define DVER_MINORVERSION 0
#define DVER_MINORVERSION_STRING "1"     
														
#define DVER_BUGFIXVERSION 0
#define DVER_BUGFIXVERSION_STRING "0"

#if DVER_BUGFIXVERSION > 0
	#define	DVER_VERSION_STRING	DVER_MAJORVERSION_STRING "." DVER_MINORVERSION_STRING "." DVER_BUGFIXVERSION_STRING 
#else
	#define	DVER_VERSION_STRING	DVER_MAJORVERSION_STRING "." DVER_MINORVERSION_STRING 
#endif

// following is used for the build development in the apple 'vers' resource
#define DVER_MACBUILDSTAGE	development
//#define VER_MACBUILDSTAGE	alpha
//#define VER_MACBUILDSTAGE	beta
//#define VER_MACBUILDSTAGE	final

// following are used in version info for the windows resource 
#define DVER_ORIGINALFILENAME	"ExampleXtra.x32"
#define DVER_COMPANY			"Unknown Company, Inc."
#define DVER_FILEDESCRIPTION	"Director Xtra"
#define DVER_PRODUCTNAME		"XDK Example Xtra"
#define DVER_INTERNALNAME		"Xtra"

#endif													
