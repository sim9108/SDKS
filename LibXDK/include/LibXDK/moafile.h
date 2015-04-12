/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_moafile
#define _H_moafile

#include "moastdif.h"


#ifdef MACINTOSH
	#define FILE_SPEC_LEN(pspec) ((pspec) ? (sizeof(short) + sizeof(long) + ((FSSpec*)(pspec))->name[0] + 1) : 0)
	typedef FSSpec MoaSystemFileSpec[1];
#else
	#define FILE_SPEC_LEN(pspec) ((pspec) ? (strlen((ConstPMoaChar)(pspec)) + 1) : 0)
	typedef MoaChar MoaSystemFileSpec[MOA_MAX_PATHNAME];
#endif

/* ----------------- */

#ifndef MOA_STD_METHOD_PROCPTR
	#define MOA_STD_METHOD_PROCPTR(a, b) STDPROCPTR(STDMETHODCALLTYPE, a, b)
#endif
typedef MOA_STD_METHOD_PROCPTR(MoaError, MoaFileEachProc)(
	ConstPMoaSystemFileSpec pSpec, 
	ConstPMoaChar pFileName,
	MoaBoolParam isDirectory, 
	MoaWide lastModDate,
	MoaUlong fileType,
	PMoaVoid refCon
);

/* values returned by GetAccess() */
enum {
	kMoaFileAccessPerm_Read = 1,
	kMoaFileAccessPerm_Write = 2
};

#define kMoaFileErr_IoError			MAKE_MOAERR(0x0384)
#define kMoaFileErr_BufferTooSmall	MAKE_MOAERR(0x0385)
#define kMoaFileErr_DuplicateSpec	MAKE_MOAERR(0x0386)
#define kMoaFileErr_DiskFull		MAKE_MOAERR(0x0387)
#define kMoaFileErr_FileBusy		MAKE_MOAERR(0x0388)
#define kMoaFileErr_BadFileSpec		MAKE_MOAERR(0x0389)

#undef INTERFACE
#define INTERFACE IMoaFile
DECLARE_INTERFACE_(IMoaFile, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
	STDMETHOD(SetSpec) (THIS_ struct IMoaFile * pDir, ConstPMoaChar pFileName) PURE;
	STDMETHOD(SetNewTempSpec) (THIS_ struct IMoaFile * pDir) PURE;
	STDMETHOD(GetSysSpec) (THIS_ PMoaSystemFileSpec pSpec, MoaLong specBufSize) PURE;
	STDMETHOD(SetSysSpec) (THIS_ ConstPMoaSystemFileSpec pSpec, MoaBoolParam resolveAlias) PURE;
	STDMETHOD(GetPathnameSpec) (THIS_ PMoaChar pNameBuf, MoaLong nameBufSize) PURE;
	STDMETHOD(SetPathnameSpec) (THIS_ ConstPMoaChar pNameBuf, MoaBoolParam resolveAlias) PURE;
	STDMETHOD(CreateFile) (THIS) PURE;
	STDMETHOD(CreateDirectory) (THIS) PURE;
	STDMETHOD(Delete) (THIS) PURE;
	STDMETHOD(SwapFile) (THIS_ struct IMoaFile * pOther) PURE;
	STDMETHOD(GetFilename) (THIS_ PMoaChar pNameBuf, MoaLong nameBufSize) PURE;
	STDMETHOD(SetFilename) (THIS_ ConstPMoaChar pNameBuf) PURE;
	STDMETHOD(GetSize) (THIS_ MoaLong * pFileLen) PURE;
	STDMETHOD(SetSize) (THIS_ MoaLong fileLen) PURE;
	STDMETHOD(GetAccess) (THIS_ MoaLong * pAccessPerm) PURE;
	STDMETHOD(GetDates) (THIS_ MoaWide * pCreate, MoaWide * pModify) PURE;
	STDMETHOD(GetDirectory) (THIS_ struct IMoaFile ** pDir) PURE;
	STDMETHOD(IsDirectory) (THIS) PURE;
	STDMETHOD(IsExisting) (THIS) PURE;
	STDMETHOD(IsSameFile) (THIS_ struct IMoaFile * pOther) PURE;
	STDMETHOD(EachFile) (THIS_ MoaBoolParam resolveAlias, MoaFileEachProc proc, PMoaVoid refCon) PURE;
	STDMETHOD(GetStream) (THIS_ MoaLong bufferSize, PIMoaStream * ppStream) PURE;
	STDMETHOD(Rename) (THIS_ ConstPMoaChar pNameBuf) PURE;
	STDMETHOD(Clone) (THIS_ struct IMoaFile ** ppFile) PURE;
};
typedef IMoaFile * PIMoaFile;

DEFINE_GUID(IID_IMoaFile, 0xAC6B6FA2L, 0x000E, 0x3DD0, 0x00, 0x00, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);
DEFINE_GUID(CLSID_CMoaFile, 0xAC721BD9L, 0x0015, 0x3F3E, 0x00, 0x00, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);


#endif
