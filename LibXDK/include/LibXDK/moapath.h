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
	Abstract: Public header for IMoaPathName
*/

#ifndef _H_MoaPathName_
#define _H_MoaPathName_

/* ------------------------------------------------------------------------
** Includes
*/

#include "moastdif.h"

/* ------------------------------------------------------------------------
** Macros
*/

#define kMoaPath_DontResolveOnInit		FALSE
#define kMoaPath_ResolveOnInit			TRUE

#define kMoaPath_DontResolveWithDialog	FALSE
#define kMoaPath_ResolveWithDialog		TRUE

#define kMoaPath_NotForMenu 			FALSE
#define kMoaPath_ForMenu				TRUE

/* ------------------------------------------------------------------------
** Types
*/

enum {
	kMoaPathDialect_LOCAL		= -1,
	kMoaPathDialect_CANON_STYLE = 0,	/* Canonical style relative pathname. */
	kMoaPathDialect_MAC_STYLE	= 1,	/* Macintosh style pathname. */
	kMoaPathDialect_DOS_STYLE	= 2,	/* MSDOS style pathname. */
	kMoaPathDialect_UNIX_STYLE	= 3,	/* Unix style pathname. */
	kMoaPathDialect_URL_STYLE	= 4 	/* URL(RFC 1738) style pathname. */
};

typedef MoaLong MoaPathDialect;

/* forward declare interface pointer */
typedef struct IMoaPathName * PIMoaPathName;

/* ------------------------------------------------------------------------
** MoaPathName Interface
*/
#undef INTERFACE
#define INTERFACE IMoaPathName
DECLARE_INTERFACE_(IMoaPathName, IMoaUnknown)

/* Description

The IMoaPathName interface provides various methods for the manipulation of pathnames. 

*/
{
	STD_IUNKNOWN_METHODS

	/* path initialization methods */

	STDMETHOD(InitFromString)	(THIS_ 
		ConstPMoaChar	pFileName,				/* pointer to file name string */
		MoaPathDialect	pathDialect,			/* path dialect */
		MoaBoolParam	bResolve,				/* resolve flag */
		MoaBoolParam	bUseDialog) 			/* use dialog flag */
		PURE;

	/* returns
	MoaError
	kMoaErr_NoErr								- Success
	*/
	
	/* description
	
	InitFromString() intializes the current IMoaPathName object from a 
	path string. The pathDialect flag tells the object what the style of the
	supplied path is. Valid dialects are:
	
	kMoaPathDialect_LOCAL			- Style is based on current platform,
									  but checks for CANON and URL also.
	kMoaPathDialect_CANON_STYLE 	- Canonical style relative pathname
	kMoaPathDialect_MAC_STYLE		- Macintosh style pathname.
	kMoaPathDialect_DOS_STYLE		- MSDOS style pathname.
	kMoaPathDialect_UNIX_STYLE		- Unix style pathname.
	kMoaPathDialect_URL_STYLE		- URL(RFC 1738) style pathname.
	
	If LOCAL dialect is specified, InitFromString() will check if the supplied	path 
	is a URL or canonical pathname, if so the dialect will changed as such.
	The resolve flag determines whether or not a relative path or leaf should be resolved into
	a fully qualified pathname. The use dialog flag determines whether or not the host
	app should use a dialog to resolve a path/file that cannot be located.
	
	bUseDialog is not likely to be supported outside Director's implementation.

	*/

	STDMETHOD(SetTempPath)		(THIS_	
		PIMoaPathName pIDir)	
		PURE;

	/* returns
	MoaError
	kMoaErr_NoErr								- Success
	kMoaErr_FileNotFound						- File not found
	*/
	
	/* description
	
	Changes the path to reference a temporary path instance with a unique name. If you want the temporary
	file to be created in a particular folder, then pass a reference to this folder in pIDir. Pass in NULL
	to have the temporary path located in the system's temporary folder.
	*/
	
	/* path manipulation methods */
	
	STDMETHOD(Resolve)			(THIS_ 
		MoaBoolParam bUseDialog)				/* use dialog flag */
		PURE;
	
	/* returns
	MoaError
	kMoaErr_NoErr								- Success
	kMoaErr_NotEnoughSSW						- Environment does not support UseDialog
	kMoaErr_BadParam							- You passed me a bogus argument
	*/
	
	/* description
	
	Resolves the current instance into a fully qualifed path. If the bUseDialog
	flag is set, the host app provides a dialog when the target cannot be found.
	
	
	bUseDialog is not likely to be supported outside Director's implementation.
	*/
	
	STDMETHOD(RemoveFinal)		(THIS) PURE;
	
	/* returns
	
	MoaError
	
	kMoaErr_NoErr								- Success
	kMoaErr_BadParam							- You passed me a bogus argument
	
	*/
	
	/* description
	
	Removes the final component of the current pathname. 
	
	*/
	
	STDMETHOD(AddFinal) 		(THIS_ 
		ConstPMoaChar pFinalComponent)			/* new path component */
		PURE;
	
	/* returns
	
	MoaError
	
	kMoaErr_NoErr								- Success
	
	*/
	
	/* description
	
	Adds the string passed in to the current pathname. The string can be either a
	directory or filename.
	
	*/
	
	STDMETHOD(Concat)			(THIS_ 
		PIMoaPathName pIMoaPathName)			/* pointer to an IMoaPathName instance */
		PURE;

	/* returns
	
	MoaError
	
	kMoaErr_NoErr								- Success
	kMoaErr_BadParam							- You passed me a bogus argument
	
	*/
	
	/* description
	
	Concatenates the pathname of the current instance with the instance passed in
	via pIMoaPathName.
	
	*/

	STDMETHOD(SetVolumeName)	(THIS_ 
		ConstPMoaChar pNameBuf) 				/* string containing the new volume name */
		PURE;

	/* returns
	
	MoaError
	
	kMoaErr_NoErr								- Success
	kMoaErr_BadParam							- You passed me a bogus argument
	kMoaPathErr_UnsupportedForCurrentDialect	- Not supported by current dialect
	
	*/
	
	/* description
	
	Used to set the volume name of the current pathname. For the MAC_STYLE dialect,
	this is the name of the target volume. For the MSDOS_STYLE dialect it is the drive
	letter of the target volume. This method has no effect with UNIX_STYLE or URL_STYLE
	pathnames.
	
	*/

	STDMETHOD(SetExtension) 	(THIS_ 
		ConstPMoaChar pNameBuf) 				/* buffer containing the extension */
		PURE;

	/* returns
	
	MoaError
	
	kMoaErr_NoErr								- Success
	kMoaErr_BadParam							- You passed me a bogus argument
	
	*/
	
	/* description
	
	Sets the extension of the current filename. 
	
	*/

	STDMETHOD(SetScheme)		(THIS_ 
		ConstPMoaChar pNameBuf) 				/* buffer containingg the URL scheme */
		PURE;

	/* returns
	
	MoaError
	
	kMoaErr_NoErr								- Success
	kMoaErr_BadParam							- You passed me a bogus argument
	kMoaPathErr_UnsupportedForCurrentDialect	- Not supported by current dialect
	
	*/
	
	/* description
	
	Sets the scheme component of the current URL_STYLE pathname. This method has no effect 
	on non URL_STYLE pathnames.
	
	*/

	STDMETHOD(SetHostName)		(THIS_ 
		ConstPMoaChar pNameBuf) 				/* buffer containing the hostname */
		PURE;

	/* returns
	
	MoaError
	
	kMoaErr_NoErr								- Success
	kMoaErr_BadParam							- You passed me a bogus argument
	kMoaPathErr_UnsupportedForCurrentDialect	- Not supported by current dialect
	
	*/
	
	/* description
	
	Sets the hostname component of the current URL_STYLE pathname. This method has no
	effect on non URL_STYLE pathnames.
	
	*/
	
	STDMETHOD(SetPort)			(THIS_ 
		MoaLong portNumber) 					/* MoaLong specifying the port number */
		PURE;

	/* returns
	
	MoaError
	
	kMoaErr_NoErr								- Success
	kMoaErr_BadParam							- You passed me a bogus argument
	kMoaPathErr_UnsupportedForCurrentDialect	- Not supported by current dialect
	
	*/
	
	/* description
	
	Sets the port component of the current URL_STYLE pathname. This method has no
	effect on non URL_STYLE pathnames.
	
	*/

	STDMETHOD(SetUserID)		(THIS_ 
		ConstPMoaChar pNameBuf) 				/* buffer containing the userID */ 
		PURE;

	/* returns
	
	MoaError
	
	kMoaErr_NoErr								- Success
	kMoaErr_BadParam							- You passed me a bogus argument
	kMoaPathErr_UnsupportedForCurrentDialect	- Not supported by current dialect
	
	*/
	
	/* description
	
	Sets the user ID component of the current URL_STYLE pathname. This method has no
	effect on non URL_STYLE pathnames.
	
	*/

	STDMETHOD(SetPassword)		(THIS_ 
		ConstPMoaChar pNameBuf) 				/* buffer containing the password */
		PURE;

	/* returns
	
	MoaError
	
	kMoaErr_NoErr								- Success
	kMoaErr_BadParam							- You passed me a bogus argument
	kMoaPathErr_UnsupportedForCurrentDialect	- Not supported by current dialect
	
	*/
	
	/* description
	
	Sets the password component of the current URL_STYLE pathname. This method has no
	effect on non URL_STYLE pathnames.
	
	*/

	STDMETHOD(SetParameters)	(THIS_ 
		ConstPMoaChar pNameBuf) 				/* buffer containing user defined parameters */ 
		PURE;

	/* returns
	
	MoaError
	
	kMoaErr_NoErr								- Success
	kMoaErr_BadParam							- You passed me a bogus argument
	kMoaPathErr_UnsupportedForCurrentDialect	- Not supported by current dialect
	
	*/
	
	/* description
	
	Sets any miscelleaneous components of the current URL_STYLE pathname. This method 
	has no effect on non URL_STYLE pathnames.
	
	*/

	STDMETHOD(SetAnchor)		(THIS_ 
		ConstPMoaChar pNameBuf) 				/* buffer containing anchor */ 
		PURE;

	/* returns
	
	MoaError
	
	kMoaErr_NoErr								- Success
	kMoaErr_BadParam							- You passed me a bogus argument
	kMoaPathErr_UnsupportedForCurrentDialect	- Not supported by current dialect
	
	*/
	
	/* description
	
	Sets the anchor component of the current URL_STYLE pathname. This method has no
	effect on non URL_STYLE pathnames.
	
	*/
	
	/* path query methods */
	
	/* Getting the size of the path string */
	STDMETHOD(GetPathSize)		(THIS_
		 MoaLong * pPathSize)				/* pointer to long to receive the path size */
		 PURE;
	
	/* returns
		
	MoaError
	
	kMoaErr_NoErr								- Success
	
	*/
	
	/* description
	
	Returns a pointer to a string containing the current pathname. 
	
	*/

	STDMETHOD(GetPath)			(THIS_ 
		PMoaChar	pNameBuf,					/* pointer to buffer for returned path */
		MoaLong 	nameBufSize)				/* size of buffer */
		PURE;
	
	/* returns
		
	MoaError
	
	kMoaErr_NoErr								- Success
	
	*/
	
	/* description
	
	Returns a pointer to a string containing the current pathname. 
	
	*/
		
	STDMETHOD(GetDisplayPath)	(THIS_ 
		PMoaChar		pNameBuf,				/* pointer to buffer for returned path */ 
		MoaLong 		nameBufSize,			/* size of buffer */
		MoaBoolParam	bForMenu)				/* for menu flag */
		PURE;
	
	/* returns
		
	MoaError
	
	kMoaErr_NoErr								- Success
	
	*/
	
	/* description
	
	Returns the display path for the current pathName. The display path is
	formatted to properly fit within the buffer supplied. If the entire
	path cannot be fit in to the buffer, it is re-formatted.
	
	Example: 
	
	The for menu boolean tells the method whether or not the path is going
	to be used within a menu, in which case, the formatting is altered.
	
	Example:
		
	*/
	
	STDMETHOD(GetDisplayFileName)	(THIS_ 
		PMoaChar		pNameBuf,				/* buffer for the returned filename */ 
		MoaLong 		nameBufSize,			/* length of buffer */
		MoaBoolParam	bFullName)				/* full name flag */
		PURE;

	/* returns
		
	MoaError
	
	kMoaErr_NoErr								- Success
	kMoaErr_BadParam							- You passed me a bogus argument
	
	*/
	
	/* description
	
	Returns the display filename for the current pathName. The display filename 
	is formatted to properly fit within the buffer supplied. If the entire
	fileName cannot be fit in to the buffer, it is re-formatted.
	
	Example: foo.txt becomes foo 
	
	The full name boolean tells the method whether or not the full filename is
	being requested. If FALSE, the formatting is done and any extension may be
	stripped. If TRUE, then the entire filename (or as much as will fit in the
	buffer) is returned with no formatting done on it.
	
	Example: 
		
	*/

	STDMETHOD(GetVolumeName)	(THIS_ 
		PMoaChar	pNameBuf,					/* buffer for the returned volume name */
		MoaLong 	nameBufSize)				/* size of the buffer */
		PURE;
	
	/* returns
		
	MoaError
	
	kMoaErr_NoErr								- Success
	
	*/
	
	/* description
	
	Returns the volume name of the current pathname. For MAC_STYLE paths, this is the
	name of the volume. For MSDOS_STYLE paths, it is the drive letter of volume. This
	method has no effect on UNIX_STYLE or URL_STYLE paths.
		
	*/
	
	STDMETHOD(GetExtension) 	(THIS_ 
		PMoaChar	pNameBuf,					/* buffer for the returned extension */
		MoaLong 	nameBufSize)				/* size of the buffer */
		PURE;

	/* returns
		
	MoaError
	
	kMoaErr_NoErr								- Success
	kMoaErr_BadParam							- You passed me a bogus argument
	
	*/
	
	/* description
	
	Returns the extension of the current file. 
		
	*/
	
	STDMETHOD(GetScheme)		(THIS_ 
		PMoaChar	pNameBuf,					/* buffer for the returned scheme */
		MoaLong 	nameBufSize)				/* size of buffer */
		PURE;
	
	/* returns
	
	MoaError
	
	kMoaErr_NoErr								- Success
	kMoaPathErr_UnsupportedForCurrentDialect	- Not supported by current dialect
	
	*/
	
	/* description
	
	Returns the scheme component of the current pathname. This method has no effect
	on non-URL_STYLE paths.
		
	*/
	
	STDMETHOD(GetHostName)		(THIS_ 
		PMoaChar	pNameBuf,					/* buffer for the returned hostname */
		MoaLong 	nameBufSize)				/* size of buffer */
		PURE;

	/* returns
	
	MoaError
	
	kMoaErr_NoErr								- Success
	kMoaPathErr_UnsupportedForCurrentDialect	- Not supported for current dialect
	
	*/
	
	/* description
	
	Returns the hostname component of the current pathname. This method has no effect
	on non-URL_STYLE paths.
		
	*/
	
	STDMETHOD(GetPort)			(THIS_ 
		MoaLong * pPortNumber)				/* on return contains port number */
		PURE;
	
	/* returns
	
	MoaError
	
	kMoaErr_NoErr								- Success
	kMoaPathErr_UnsupportedForCurrentDialect	- Not supported for current dialect
	
	*/
	
	/* description
	
	Returns the port component of the current pathname. This method has no effect
	on non-URL_STYLE paths.
		
	*/
	
	STDMETHOD(GetUserID)		(THIS_ 
		PMoaChar	pNameBuf,					/* buffer for returned UserID */
		MoaLong 	nameBufSize)				/* size of buffer */
		PURE;
		
	/* returns
	
	MoaError
	
	kMoaErr_NoErr								- Success
	kMoaPathErr_UnsupportedForCurrentDialect	- Not supported for current dialect
	
	*/
	
	/* description
	
	Returns the userID component of the current pathname. This method has no effect
	on non-URL_STYLE paths.
		
	*/	
	
	STDMETHOD(GetPassword)		(THIS_ 
		PMoaChar	pNameBuf,					/* buffer for returned password */ 
		MoaLong 	nameBufSize)				/* size of buffer */
		PURE;

	/* returns
	
	MoaError
	
	kMoaErr_NoErr								- Success
	kMoaPathErr_UnsupportedForCurrentDialect	- Not supported for current dialect
	
	*/
	
	/* description
	
	Returns the password component of the current pathname. This method has no effect
	on non-URL_STYLE paths.
		
	*/

	STDMETHOD(GetParameters)	(THIS_ 
		PMoaChar	pNameBuf,					/* buffer for returned parameters */
		MoaLong 	nameBufSize)				/* size of buffer */
		PURE;
	
	/* returns
	
	MoaError
	
	kMoaErr_NoErr								- Success
	kMoaPathErr_UnsupportedForCurrentDialect	- Not supported for current dialect
	
	*/
	
	/* description
	
	Returns user defined components of the current pathname. This method has no effect
	on non-URL_STYLE paths.
		
	*/
	
	STDMETHOD(GetAnchor)		(THIS_ 
		PMoaChar	pNameBuf,					/* buffer for returned anchor */
		MoaLong 	nameBufSize)				/* size of buffer */
		PURE;

	/* returns
	
	MoaError
	
	kMoaErr_NoErr								- Success
	kMoaPathErr_UnsupportedForCurrentDialect	- Not supported for current dialect
	
	*/
	
	/* description
	
	Returns the anchor component of the current pathname. This method has no effect
	on non-URL_STYLE paths.
		
	*/

	STDMETHOD_(MoaBoolParam, IsValid)		(THIS) PURE;
		
	/* returns
	
	MoaBoolParam
	
	*/
	
	/* description
	
	Returns TRUE if the current path instance has been properly initialized. This is
	used internally when the object is created and HAS NOTHING TO DO WITH WHETHER OR
	NOT THE PATH IS VALID. THIS SHOULD NOT BE EXPOSED!

	Should be implemented as real validity check on the syntax of the pathname.
	e.g. nothing but "\" in DOS style pathnames
		
	*/	
	
	STDMETHOD_(MoaBoolParam, IsEmpty)		(THIS) PURE;
		
	/* returns
	
	MoaBoolParam
	
	*/
	
	/* description
	
	Returns TRUE if the current path is empty.
		
	*/
	
	STDMETHOD_(MoaBoolParam, IsAbsolute)	(THIS) PURE;
	
	/* returns
	
	MoaBoolParam
	
	*/
	
	/* description
	
	Returns TRUE if the current path is absolute.
		
	*/
	
	STDMETHOD_(MoaBoolParam, IsRelative)	(THIS) PURE;

	/* returns
	
	MoaBoolParam
	
	*/
	
	/* description
	
	Returns TRUE if the current path is relative.
		
	*/

	STDMETHOD_(MoaBoolParam, IsCanonical)	(THIS) PURE;
		
	/* returns
	
	MoaBoolParam
	
	*/
	
	/* description
	
	Returns TRUE if the current path is canonical.
		
	*/

	STDMETHOD_(MoaBoolParam, IsRemote)	(THIS) PURE;
		
	/* returns
	
	MoaBoolParam
	
	*/
	
	/* description
	
	Returns TRUE if the current path is a URL and not a file: based URL.
		
	*/

	STDMETHOD_(MoaBoolParam, AreEqual)		(THIS_ 
		PIMoaPathName pIMoaPathName)				/* pointer to an IMoaPathName instance */
		PURE;
		
	/* returns
	
	MoaBoolParam
	
	*/
	
	/* description
	
	Returns TRUE if the provided IMoaPathName instance is the same as the current
	instance.
		
	*/		

	/* methods for getting related paths */
	
	STDMETHOD(GetParentDirectory)	(THIS_	
		PIMoaPathName * ppIMoaPathName) 		/* on return contains a pointer to an IMoaPathName */ 
		PURE;

	/* returns
	
	MoaError
	
	kMoaErr_NoErr								- Success
	or standard MOA QueryInterface errors
	
	*/
	
	/* description
	
	Returns a PIMoaPathName that references the current instances parent directory. The
	client is responsible for releasing this interface when done.
		
	*/	
		
	STDMETHOD(GetWorkingDirectory)	(THIS_	
		PIMoaPathName * ppIMoaPathName) 	/* on return contains a pointer to an IMoaPathName */ 
		PURE;
	
	/* returns
	
	MoaError
	
	kMoaErr_NoErr								- Success
	or standard MOA QueryInterface errors

	*/
	
	/* description
	
	Returns a PIMoaPathName that references the current working directory. The client
	is responsible for releasing this interface when done.
		
	*/
	
	STDMETHOD(GetRelativePath)		(THIS_	
		PIMoaPathName		pToPath,			/* pointer to the desination path */ 
		PIMoaPathName * ppRelPath)			/* on return contains a pointer to an IMoaPathName */
		PURE;

	/* returns
	MoaError
	kMoaErr_NoErr								- Success
	or standard MOA QueryInterface errors
	*/
	
	/* description
	
	Returns a PIMoaPathName that contains the relative path from the current instance
	to the destination path in the pToPath instance. The client is responsible for releasing
	this interface when done.
		
	*/

	STDMETHOD(Clone)				(THIS_	
		PIMoaPathName * ppIMoaPathName) 	/* on return contains a pointer to an IMoaPathName */
		PURE;

	/* returns
	MoaError
	kMoaErr_NoErr								- Success
	or standard MOA QueryInterface errors
	*/
	
	/* description
	
	Creates an IMoaPathName instance identical to the current instance. The client is
	responsible for releasing this interface when done.
		
	*/
	
	/* MACINTOSH specific methods */
	STDMETHOD(InitFromFSSpec)	(THIS_ 
		ConstPMoaVoid pFSSpec)					/* pointer to Macintosh FSSpec */ 
		PURE;
	
	/* returns
	MoaError
	kMoaErr_NoErr								- Success
	*/
	
	/* description
	
	Initializes the current instance from a Macintosh FSSpec.
		
	*/
	
	STDMETHOD(GetFSSpec)		(THIS_ 
		PMoaVoid pFSSpec)						/* pointer to an Macintosh FSSpec */ 
		PURE;

	/* returns
	MoaError
	kMoaErr_NoErr								- Success
	*/
	
	/* description
	
	on return fills in the given Macintosh FSSpec from the current instance.
		
	*/
};


/* IID_IMoaPathName: ea60b640-0b45-11d0-9faf-00a0c9008a48 */
DEFINE_GUID(IID(IMoaPathName), 0xEA60B640L, 0x0b45, 0x11D0, 0x9F, 0xAF, 0x00, 0xa0, 0xc9, 0x00, 0x8a, 0x48);

/* CLSID_CMoaPath: d6d9f6a0-1639-11d0-9faf-00a0c9008a48 */
DEFINE_GUID(CLSID(CMoaPath), 0xD6D9F6A0L, 0x1639, 0x11D0, 0x9F, 0xAF, 0x00, 0xa0, 0xc9, 0x00, 0x8a, 0x48);

#endif	/* _H_MoaPathName_ */
