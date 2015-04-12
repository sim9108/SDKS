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
	Abstract:
		Public header for MoaFile2.
*/

#ifndef _H_MoaFile2_
#define _H_MoaFile2_

/* ------------------------------------------------------------------------
** Includes
*/


#ifndef _H_moatypes
	#include "moatypes.h"
#endif

#ifndef _H_moastdif
	#include "moastdif.h"
#endif

#ifndef _H_MoaPathName_
	#include "moapath.h"
#endif

#ifndef _H_MoaIOAttr_
	#include "mioattr.h"
#endif

#ifndef _H_MoaStream2_
	#include "moastr2.h"
#endif

/* ------------------------------------------------------------------------
** Macros
*/
#ifndef _H_moafile
	#define kMoaFileErr_IoError			MAKE_MOAERR(900)
	#define kMoaFileErr_BufferTooSmall	MAKE_MOAERR(901)
	#define kMoaFileErr_DuplicateSpec	MAKE_MOAERR(902)
	#define kMoaFileErr_DiskFull		MAKE_MOAERR(903)
	#define kMoaFileErr_FileBusy		MAKE_MOAERR(904)
	#define kMoaFileErr_BadFileSpec		MAKE_MOAERR(905)
#endif /* _H_moafile */

/*
** MoaFile2's error code range is 0x12D0 to 12DF
*/
#define kMoaFile2Err_FileLocked			MAKE_MOAERR(0x12D0)
#define kMoaFile2Err_NotEnoughHandles	MAKE_MOAERR(0x12D1)
#define kMoaFile2Err_NoStreamInterface	MAKE_MOAERR(0x12D2)
#define kMoaFile2Err_UnSupportedKey		MAKE_MOAERR(0x12D3)

/* ------------------------------------------------------------------------
** Types
*/

#ifndef _H_moafile
	/* values returned by GetAccess() */
	enum {
		kMoaFileAccessPerm_Read	 = 1,
		kMoaFileAccessPerm_Write = 2
	};
#endif

/* EnumFiles callback function declaration */
typedef STDPROCPTR(STDMETHODCALLTYPE, MoaError, MoaFileEnumProc)(
	PIMoaPathName	pIMoaPathName,
	MoaBoolParam	isDirectory, 
	MoaWide *		pLastModDate,
	PMoaChar		pFileType,
	PMoaVoid		pClientData
);


#ifndef _H_moafile
	/* MoaSystemFileSpec declaration */
	#ifdef _MACINTOSH
		typedef FSSpec MoaSystemFileSpec[1];
	#else
		typedef MoaChar MoaSystemFileSpec[MOA_MAX_PATHNAME];
	#endif
#endif

/* forward declare stream interface ptr */
typedef struct IMoaFile2 * PIMoaFile2;

/* ------------------------------------------------------------------------
** MoaFile2 Interface
*/

#undef INTERFACE

#define INTERFACE IMoaFile2

DECLARE_INTERFACE_(IMoaFile2, IMoaUnknown)

/* Description

The IMoaFile2 interface provides high level cross platform access to the 
filesystem via MOA. It is functionally a superset of the original IMoaFile interface,
but not a direct superset of IMoaFile.

*/
{
	STD_IUNKNOWN_METHODS

	/* Methods to set the MoaFile object's underlying pathName */
	STDMETHOD(SetSpec)			(THIS_ 
		ConstPMoaChar pFullPathName)			/* pointer to a pathName string */
		PURE;
	
	/* returns
	
	MoaError
	
	kMoaErr_NoErr								- Success	
	
	*/
	
	/* description
	
	Used to set the underlying pathName.
	
	*/
	
	STDMETHOD(SetPathName)		(THIS_ 
		const PIMoaPathName pIMoaPathName)		/* pointer to an instance of IMoaPathname */
		PURE;
	
	/* returns
	
	MoaError
	
	kMoaErr_NoErr								- Success
	kMoaFileErr_BadFileSpec						- @@@
	
	*/
	
	/* description
	
	Sets the MoaFile object's underlying pathName using a previously resolved IMoaPathname
	object.

	The given PathName interface is cloned so the client doesn't have a reference to the same
	interface as the MoaFile2 implementation.
	
	The client should release this interface after calling SetPathName()
	*/
	
	/* see also
	
	IMoaFile2::GetPathName()
	IMoaPathname
	
	*/
	   
	/* Methods to get the underlying pathName object */
	STDMETHOD(GetPathName)		(THIS_
		PIMoaPathName * ppIMoaPathName)		/* on return contains a pointer to an IMoaPathName instance */
		PURE;
		
	/* returns
	
	MoaError
	
	kMoaErr_NoErr							- Success
	or standard MOA QueryInterface errors
  
	*/
	
	/* description
	
	Returns a pointer to an instance of the IMoaPathName interface. This interface is
	a clone of the underlying PathName interface. It can be used to set the filename, path and other details
	of the target file using the SetPathName() method. The client is responsible for releasing this interface when done.
	
	*/
	
	/* see also
	
	IMoaFile2::SetPathName()
	IMoaPathName
	
	*/

	/* Get a MOA Stream on the current path */
	STDMETHOD(GetStream)		(THIS_ 
		MoaUlong				bufferSize, 	/* buffer size of the stream */ 
		PIMoaStream2	*		ppStream)		/* on return contains a pointer to an IMoaStream2 interface */
		PURE;
	
	/* returns
	
	MoaError
	
	kMoaErr_NoErr							- Success
	or standard MOA QueryInterface errors
	
	*/
	
	/* description
	
	Used to acquire an IMoaStream2 interface to the current file. The client is responsible
	for releasing this interface when done. 
	
	*/
	
	/* see also
	
	IMoaStream2
	
	*/

	/* Functions to control various file attributes */
	STDMETHOD(GetSize)			(THIS_ 
		MoaUlong * pFileLen)				/* on return contains file size */ 
		PURE;
	
	/* returns
	
	MoaError
	
	kMoaErr_NoErr							- Success
	kMoaErr_FileNotFound					- Could not find the file
	kMoaFileErr_BadFileSpec					- underlying pathname is invalid
	kMoaFileErr_IoError						- Unknown I/O error
	
	*/
	
	/* description
	
	Returns the size of the current file in *pFileLen.
	
	*/
	
	STDMETHOD(SetSize)			(THIS_ 
		MoaUlong fileLen)						/* new size of file */
		PURE;
		
	/* returns
	
	MoaError

	kMoaErr_NoErr							- Success
	kMoaErr_FileNotFound					- Could not find the file
	kMoaFileErr_BadFileSpec					- underlying pathname is invalid
	kMoaFileErr_DiskFull					- Create failed, disk is full	
	kMoaFileErr_IoError						- Unknown I/O error
			
	*/
	
	/* description
	
	Sets the current file's length to fileLen.
	
	*/
	
	STDMETHOD(GetAccess)		(THIS_ 
		MoaUlong * pAccessPerm)				/* on return contains access perm for the current file */
		PURE;
		
	/* returns
	
	MoaError

	kMoaErr_NoErr							- Success
	kMoaErr_FileNotFound					- Could not find the file
	kMoaFileErr_BadFileSpec					- underlying pathname is invalid
	kMoaFileErr_IoError						- Unknown I/O error
			
	*/
	
	/* description
	
	Returns the access permissions for the current file in pAccessPerm.
	
	*/
	
	STDMETHOD(SetAccess)		(THIS_ 
		MoaUlong accessPerm)					/* new access permissions for current file */ 
		PURE;
	
	/* returns
	
	MoaError

	kMoaErr_NoErr							- Success
	kMoaErr_FileNotFound					- Could not find the file
	kMoaFileErr_BadFileSpec					- underlying pathname is invalid
	kMoaFileErr_IoError						- Unknown I/O error
			
	*/
	
	/* description
	
	Used to set the access perms of the current file.
	
	*/
	
	STDMETHOD(GetDates)			(THIS_ 	
		MoaWide * pCreate,					/* on return contains creation date */ 
		MoaWide * pModify,					/* on return contains last modified date */
		MoaWide * pAccess)					/* on return contains last access date */
		PURE;
	
	/* returns
	
	MoaError
	kMoaErr_NoErr							- Success
	kMoaErr_FileNotFound					- Could not find the file
	kMoaFileErr_BadFileSpec					- underlying pathname is invalid
	kMoaFileErr_IoError						- Unknown I/O error
	
	*/
	
	/* description
	
	Returns the various date attributes of the current file.
	
	*/
	
	STDMETHOD(SetDates)			(THIS_
		const MoaWide * pCreate,			/* creation date */ 
		const MoaWide * pModify,			/* modified date */
		const MoaWide * pAccess)			/* access date */
		PURE;
		
	/* returns
	
	MoaError

	kMoaErr_NoErr							- Success
	kMoaErr_FileNotFound					- Could not find the file
	kMoaFileErr_BadFileSpec					- underlying pathname is invalid
	kMoaFileErr_IoError						- Unknown I/O error
		
	*/
	
	/* description
	
	Sets the various date attributes of the current file.
	
	*/
	
	STDMETHOD_(MoaCachePriority, GetCachePriority)	(THIS) PURE; 
	
	/* returns
	
	MoaCachePriority
	
	*/
	
	/* description
		
	Gets the cache priority of the current file. Cache priorities include:

	kMoaCachePriority_Always		 
 	kMoaCachePriority_IfYouCan	
  	kMoaCachePriority_Never		
	
	These are only useful if the underlying path object references a remote
	file.

	*/
	
	STDMETHOD(SetCachePriority)	(THIS_ 
		MoaCachePriority cacheVal)				/* new cache priority constant */
		PURE;
	
	/* returns
	
	MoaError
	
	kMoaErr_NoErr							- Success
	kMoaErr_BadParam						- You passed me a bogus argument

	*/
	
	/* description
		
	Sets the cache priority of the current file. Cache priorities include:
	
	kMoaCachePriority_Always		 
 	kMoaCachePriority_IfYouCan	
  	kMoaCachePriority_Never		
	
	These are only useful if the underlying path object references a remote
	file.
	
	All implementations are required to support get/set cache priority even
	if the implementation doesn't deal with remote files.
	
	*/
	
	STDMETHOD_(MoaBoolParam, IsDirectory)		(THIS) PURE;
		
	/* returns 
	
	MoaBoolParam
	
	*/
	
	/* description
	
	Returns TRUE if the underlying pathname points to a directory.
	
	*/
	
	STDMETHOD_(MoaBoolParam, IsExisting)		(THIS) PURE;

	/* returns
	
	MoaBoolParam
	
	*/

	/* description
	
	Returns TRUE if the object pointed to by the underlying pathname exists.
	
	*/

	
	/* Method to get IMoaIOAttributeDict interface */
	STDMETHOD(GetMoaIOAttributeDict)	(THIS_ 
		PIMoaIOAttributeDict *ppIDict)			/* on return contains pointer to an IMoaDict interface */
		PURE;

	/* returns
	
	MoaError
	
	kMoaErr_NoErr							- Success
	or standard MOA QueryInterface errors
	
	*/
	
	/* description
	
	Returns an interface the an IMoaIOAttribute interface. This interface is used to
	access the various file attributes of the object pointed to by the underlying
	pathname. This interface is identical to IMoaDict. The client is responsible for
	releasing this interface when done.
	
	The list of predefined keys include:

			kMoaIOKey_StreamFlags
			kMoaIOKey_StreamAccessMode
			kMoaIOKey_StreamBufferSize
			kMoaIOKey_StreamDataRate
			kMoaIOKey_StreamEnd
			kMoaIOKey_StreamCurrentLength
			kMoaIOKey_StreamPosition
			kMoaIOKey_StreamPositionType

			kMoaIOKey_FileFlags				
			kMoaIOKey_FileAccessPerm
			kMoaIOKey_FileLength
			kMoaIOKey_FileType
			kMoaIOKey_FileEncoding
			kMoaIOKey_FileCreator
			kMoaIOKey_FileCreateDate
			kMoaIOKey_FileModifyDate
			kMoaIOKey_FileAccessDate
			kMoaIOKey_FileCachePriority	
	
			kMoaIOKey_VolumeFlags
			kMoaIOKey_VolumeBlockSize
			kMoaIOKey_VolumeSectorSize
			kMoaIOKey_VolumeFreeSpace

			kMoaIOKey_LASTPREDEFINED

	Attributes are designed to be accessed thru the Get/Put methods of the IMoaDict 
	interface. The type of an attribute's value pointer is specific to the attribute:

			Stream Attribute	Type of valuePtr	Length
			----------------	----------------	------
			Flags				PMoaLong			4
			AccessMode			PMoaLong			4
			BufferSize			PMoaLong			4
			DataRate			PMoaLong			4
			End					PMoaLong			4
			CurrentLength		PMoaLong			4
			Position			PMoaLong			4
			PositionType		PMoaLong			4

			File Attribute		Type of valuePtr	Length
			--------------		----------------	------
			Flags				PMoaLong			4
			AccessPerm			PMoaLong			4
			Length				PMoaLong			4
			Type				PMoaChar			variable
			Encoding			PMoaChar			variable
			Creator				PMoaChar			variable
			CreateDate			PMoaWide			8
			ModifyDate			PMoaWide			8
			AccessDate			PMoaWide			8
			CachePriority		PMoaLong			4

			Volume Attribute	Type of valuePtr	Length
			----------------	----------------	------
			Flags				PMoaLong			4
			BlockSize			PMoaLong			4
			SectorSize			PMoaLong			4
			FreeSpace			PMoaLong			4

	*/
	
	/* see also
	
	IMoaDict
	IMoaStream2::GetMoaIOAttributeDict()
	
	*/

	/* Misc file functions */
	STDMETHOD(CreateFile)		(THIS) PURE;
	
	/* returns
	
	MoaError

	kMoaErr_NoErr									- Success
	kMoaFileErr_DiskFull							- Create failed, disk is full	
	kMoaFileErr_IoError								- Create failed, unknown I/O error
	kMoaErr_FileNotFound							- File not found
	
	*/
	
	/* description
	
	Creates a new file.
	
	*/
	
	STDMETHOD(CreateDirectory)	(THIS) PURE;
		
	/* returns
	
	MoaError

	kMoaErr_NoErr									- Success
	kMoaErr_FileNotFound							- File not found
	kMoaFileErr_IoError								- Create failed, unknown
	kMoaFileErr_DiskFull							- Create failed, disk is full
	kMoaFileErr_DuplicateSpec						- Directory already exists
		
	*/
	
	/* description
	
	Creates a new directory.
	
	*/
	
	STDMETHOD(Delete)			(THIS) PURE;
	
	/* returns
	
	MoaError

	kMoaErr_NoErr									- Success
	kMoaFileErr_IoError								- Delete failed, unknown
	kMoaErr_FileNotFound							- File not found
	kMoaFileErr_DirectoryNotEmpty					- Directory specified for deletion was not empty
		
	*/
	
	/* description
	
	Deletes the file or directory specifed by the underlying pathname

	*/
	
	STDMETHOD(SwapFile)			(THIS_ 
		PIMoaFile2 pOther)							/* pointer to an IMoaFile2 instance */
		PURE;
		
	/* returns
	
	MoaError
	
	kMoaErr_NoErr									- Success
	kMoaErr_FileNotFound							- File not found
	kMoaFileErr_BadFileSpec							- One of files is a directory
	
	*/
	
	/* description
	
	Exchanges the files specified by the two IMoaFile2 instances (this and pOther).

	If pOther doesn't exist, the file specified by "this" is moved to the pathname
	specified by pOther.
	
	*/
	
	STDMETHOD_(MoaBoolParam, IsSameFile)		(THIS_ 
		PIMoaFile2 pOther)							/* pointer to an IMoaFile2 interface */
		PURE;
	
	/* returns
	
	MoaBoolParam
	
	*/
	
	/* description
	
	Returns TRUE if the current IMoaFile instance and the instance pointer passed
	in via pOther point to the same file or directory.
	
	*/
	
	STDMETHOD(EnumFiles)		(THIS_ 
		MoaBoolParam		resolveAlias,			/* resolve flag */
		MoaFileEnumProc		proc,					/* proc pointer to be called for each file/directory */
		PMoaVoid			pClientData)			/* client data */
		PURE;
	
	/* returns
	
	MoaError
	
	kMoaErr_NoErr									- Success
	kMoaErr_BadParam								- You passed me a bogus argument
	
	*/
	
	/* description
	
	Enumerates all entries in the current directory. For each entry in the current directory,
	the procedure provided via proc is called. The following data are passed to the
	callback procedure: 
	
	pIMoaPathName - IMoaPathName interface to the current file
	isDirectory   - TRUE if the file is a directory
	pLastModDate  - The last modified date of the file
	pFileType	  - The file type as a "C" string (implementation dependent)
					e.g. for MAC files: "TEXT"
						 for WIN files: "txt"
						 for remote files: "text/html"
	pClientData   - The client data passed in to EnumFiles
	
	*/
	
	STDMETHOD(Rename)			(THIS_ 
		ConstPMoaChar pNameBuf)						/*  point to string containing new name */
		PURE;
		
	/* returns
	
	MoaError
	
	kMoaErr_NoErr									- Success
	kMoaErr_FileNotFound							- File not found
	kMoaFileErr_FileBusy							- File in use
	kMoaFileErr_IoError								- Unknown I/O error
	
	*/
	
	/* description
	
	Renames the file or directory currently pointed to by the underlying pathname.
	
	*/
	
	STDMETHOD(Clone)			(THIS_ 
		PIMoaFile2 * ppFile)					/* on return contains a pointer to an IMoaFile2 instance */
		PURE;

	/* returns
	
	MoaError
	
	kMoaErr_NoErr									- Success
	or standard MOA QueryInterface errors
	
	*/
	
	/* description
	
	Creates a new IMoaFile2 object identical to the current instance. Client is responsible
	for releasing this interface when done. 
	
	*/
};
/* typedef IMoaFile2 * PIMoaFile2; */

/*
** Original MoaFile GUID for reference purposes:
** IID_IMoaFile: AC6B6FA2000E3DD00000080007160DC3
** DEFINE_GUID(IID_IMoaFile, 0xAC6B6FA2L, 0x000E, 0x3DD0, 0x00, 0x00, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);
*/

/* IID_IMoaFile2: A7EBAA8A-78EC-11d0-9037-00059A20195A */
DEFINE_GUID(IID(IMoaFile2), 0xA7EBAA8AL, 0x78EC, 0x11D0, 0x90, 0x37, 0x00, 0x05, 0x9A, 0x20, 0x19, 0x5A);

/* CLSID_CMoaFile2: D394EB1A-78EC-11d0-9037-00059A20195A */
/* Instantiate a CLSID_CMoaFile2:IID_IMoaFile2 for files known to be local */
DEFINE_GUID(CLSID(CMoaFile2), 0xD394EB1AL, 0x78EC, 0x11D0, 0x90, 0x37, 0x00, 0x05, 0x9A, 0x20, 0x19, 0x5A);

/* CLSID_CNetFile: 41E3BA47-212E-11d0-BE97-00AA00B4D34D} */
/* Instantiate a CLSID_CNetFile:IID_IMoaFile2 for files which might be URLs */
DEFINE_GUID(CLSID_CNetFile, 
	0x41e3ba47, 0x212e, 0x11d0, 0xbe, 0x97, 0x0, 0xaa, 0x0, 0xb4, 0xd3, 0x4d);

#endif /* _H_MoaFile2_ */
