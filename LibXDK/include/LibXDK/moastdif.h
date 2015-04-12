/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_moastdif
#define _H_moastdif

/*
	File: moastdif.h (core)

	Abstract:

	This file contains various basic interfaces used in conjunction with
	Moa (e.g., MoaHandle, MoaDict, MoaCache, etc.).
*/


#include "moaxtra.h"

#ifdef MOA_pointers_in_D0
#pragma push
#pragma pointers_in_D0
#endif

/* --------------------------------------------------------------- IMoaHandle */

typedef struct _MoaHandle * MoaHandle;

/* avoid gnarly internal FH problems */
#ifdef Free
	#undef Free
#endif

#undef INTERFACE
#define INTERFACE IMoaHandle
DECLARE_INTERFACE_(IMoaHandle, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS

	STDMETHOD_(MoaHandle, Alloc) (THIS_ MoaLong cb) PURE;
	STDMETHOD_(MoaHandle, ZeroAlloc) (THIS_ MoaLong cb) PURE;
	STDMETHOD_(void, Free) (THIS_ MoaHandle h) PURE;
	STDMETHOD_(MoaLong, GetSize) (THIS_ MoaHandle h) PURE;
	STDMETHOD_(MoaBoolParam, Resize) (THIS_ MoaHandle h, MoaLong newSize) PURE;
	STDMETHOD_(PMoaVoid, Lock) (THIS_ MoaHandle h) PURE;
	STDMETHOD_(void, Unlock) (THIS_ MoaHandle h) PURE;
	STDMETHOD_(MoaLong, GetLargestFreeBlock) (THIS) PURE;
};
typedef IMoaHandle * PIMoaHandle;

DEFINE_STD_GUID(IID_IMoaHandle, 0xAB9D9921L, 0x7F02, 0x2F99, 0x48, 0x7F, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);


/* ----------------------------------------------------------------- IMoaDict */

#define	kMoaDictErr_NotFound		MAKE_MOAERR(42) /* Lookup of a nonexistent key. */
#define	kMoaDictErr_WrongType		MAKE_MOAERR(43) /* Key exists but holds a value of a different type than requested. */
#define	kMoaDictErr_CantWrite		MAKE_MOAERR(44) /* Attempt to put or remove in a dictionary where that is not allowed (e.g. structure dicts). */
#define	kMoaDictErr_BufferTooSmall	MAKE_MOAERR(45) /* Value won't fit into the buffer you passed. */

enum {
	kMoaDictType_Bogus = 0,		/* illegal. */
	kMoaDictType_Dict = 1,		/* MoaDict. Put does AddRef; Remove does Release. */
	kMoaDictType_PIMoaUnknown,	/* Returns a PIMoaUnknown. Put does AddRef; Remove does Release. */
	kMoaDictType_Long,			/* Returns a MoaLong. */
	kMoaDictType_Float,			/* Returns a 32 bit IEEE single precision floating point number. */
	kMoaDictType_Wide,			/* MoaWide */
	kMoaDictType_Bool,			/* MoaBool */
	kMoaDictType_MoaID,			/* MoaID */
	kMoaDictType_Bytes,			/* Arbitrary bytes */
	kMoaDictType_CString,		/* Null-terminated C String */
	kMoaDictType_Double,		/* MoaDouble */
	kMoaDictType_MoaPoint,		/* MoaPoint */
	kMoaDictType_MoaRect,		/* MoaRect */
	_kMoaDictType_Last_
};

/* Type IDs are 32 bit numbers. The first 2^16 of them are reserved to be defined by Moa Dict.
 * (The so called "well known" types. See below.) Type IDs above that are dynamically assigned
 * at runtime and map to an object exporting the IMoaDictType interface. These can be looked up
 * either using the well known types dictionary, or by enumerating Moa interfaces.
 */
typedef MoaUlong MoaDictTypeID;

/*
	By convention, all keys beginning with these characters
	will never be written to persistent storage; they
	are runtime-only keys.
*/
#define MOADICT_RUNTIME_KEY_PREFIX	"_rt_"

#undef INTERFACE
#define INTERFACE IMoaDict
DECLARE_INTERFACE_(IMoaDict, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS

	/* This can be called to pre-allocate space in a dictionary.
	 * This makes it more efficient if the size is more-or-less known before hand.
	 */
	STDMETHOD(SetSize) (THIS_ MoaUlong numEntries) PURE;

	/* Makes a dictionary and places it inside another under a given key.
		Optionally return the new dict (if pNewDict is not NULL).
		The dict returned is "owned" by This and does not need to be
		Released by the caller.
	*/
	STDMETHOD(MakeDict) (THIS_
		struct IMoaDict ** pNewDict,
		ConstPMoaChar key
	) PURE;

	/* Returns a value from a dictionary given its key.
	 */
	STDMETHOD(Get) (THIS_
		MoaDictTypeID type,
		PMoaVoid pValueBuffer,
		MoaLong bufferSize,
		ConstPMoaChar key
	) PURE;
	
	/* Inserts a value into a dictionary.
	 */
	STDMETHOD(Put) (THIS_
		MoaDictTypeID type,
		ConstPMoaVoid pValueBuffer,
		MoaLong bufferSize,
		ConstPMoaChar key
	) PURE;

	/* Removes a value from a dictionary. For reference counted types, count will simply be decremented. */
	STDMETHOD(Remove) (THIS_
		ConstPMoaChar key
	) PURE;
	
	/* Tells how many (key, type, value) tuples are in a dictionary. */
	STDMETHOD(Count) (THIS_
		MoaUlong * pCount
	) PURE;


	/* Returns info on the nth entry in a dictionary.
		Any of the values may be NULL if that info isn't desired. */
	STDMETHOD(GetNth) (THIS_
		MoaUlong index,					/* in */
		MoaDictTypeID * pType,			/* out */
		MoaLong * pValueSize,			/* out */
		ConstPMoaChar * pKey			/* out */
	) PURE;

	/* Returns info on the entry with the given key in a dictionary.
		Any of the values may be NULL if that info isn't desired. */
	STDMETHOD(GetInfo) (THIS_
		MoaDictTypeID * pType,			/* out */
		MoaLong * pValueSize,			/* out */
		ConstPMoaChar key				/* in */
	) PURE;

	/* Returns the "magic" key used for fast access.
		Use of this routine is optional but can provide
		much faster dictionary access. NULL will only be
		returned when out of memory. */
	STDMETHOD_(ConstPMoaChar, FindKey) (THIS_
		ConstPMoaChar key				/* in */
	) PURE;
};
typedef IMoaDict * PIMoaDict;

DEFINE_STD_GUID(IID_IMoaDict, 0xAC41A018L, 0x0013, 0x4760, 0x00, 0x00, 0x08, 0x00, 0x07, 0x37, 0xAF, 0x92);

/* ---------------------------------------------------------------- IMoaCache */

typedef PIMoaDict PIMoaFileEntryDict;
typedef PIMoaDict PIMoaXtraEntryDict;
typedef PIMoaDict PIMoaRegistryEntryDict;

typedef STDPROCPTR(STDMETHODCALLTYPE, MoaError, MoaCacheFileEntryEnumProc)(
	PIMoaFileEntryDict pFileDict,
	ConstPMoaSystemFileSpec pFileSpec,
	MoaWide fileDate,
	MoaUlong fileType,	/* Mac OS type */
	PMoaVoid refCon
);

typedef STDPROCPTR(STDMETHODCALLTYPE, MoaError, MoaCacheXtraEntryEnumProc)(
	PIMoaXtraEntryDict pXtraDict,
	PMoaVoid refCon
);

typedef STDPROCPTR(STDMETHODCALLTYPE, MoaError, MoaCacheRegistryEntryEnumProc)(
	PIMoaRegistryEntryDict pRegistryDict,
	ConstPMoaClassID pClassID,
	ConstPMoaInterfaceID pInterfaceID,
	PMoaVoid refCon
);

typedef STDPROCPTR(STDMETHODCALLTYPE, MoaError, MoaCacheRegistryEntryEnumFromFileProc)(
	PIMoaFileEntryDict pFileDict,
	ConstPMoaSystemFileSpec pFileSpec,
	MoaWide fileDate,
	MoaUlong fileType,	/* Mac OS type */
	PIMoaXtraEntryDict pXtraDict,
	PIMoaRegistryEntryDict pRegistryDict,
	ConstPMoaClassID pClassID,
	ConstPMoaInterfaceID pInterfaceID,
	PMoaVoid refCon
);

#undef INTERFACE
#define INTERFACE IMoaCache
DECLARE_INTERFACE_(IMoaCache, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS

	STDMETHOD(EnumerateFileEntries) (THIS_
		MoaCacheFileEntryEnumProc pEnumProc,
		PMoaVoid refCon
	) PURE;

	STDMETHOD(EnumerateXtraEntries) (THIS_
		MoaCacheXtraEntryEnumProc pEnumProc,
		PMoaVoid refCon
	) PURE;

	STDMETHOD(EnumerateRegistryEntries) (THIS_
		MoaCacheRegistryEntryEnumProc pEnumProc,
		PMoaVoid refCon
	) PURE;

	STDMETHOD(SetXtraEntryNoncacheable) (THIS_
		PIMoaXtraEntryDict pXtraDict
	) PURE;

	/*
		Note: the registry dict returned by AddRegistryEntry
		is owned by the cache, and you MUST NOT call
		Release() on it (unless you also call AddRef()
		on it).
	*/
	STDMETHOD(AddRegistryEntry) (THIS_
		PIMoaXtraEntryDict pXtraDict,
		ConstPMoaClassID pClassID,
		ConstPMoaInterfaceID pInterfaceID,
		PIMoaRegistryEntryDict * ppRegistryDict
	) PURE;

	STDMETHOD(GetInstanceFromRegistry) (THIS_
		PIMoaRegistryEntryDict pRegistryDict,
		ConstPMoaInterfaceID pInterfaceID,
		PPMoaVoid ppvObj
	) PURE;

	STDMETHOD(CreateNewInstanceFromRegistry) (THIS_
		PIMoaRegistryEntryDict pRegistryDict,
		ConstPMoaInterfaceID pInterfaceID,
		PPMoaVoid ppvObj
	) PURE;

	STDMETHOD(EnumerateRegistryEntriesFromFile) (THIS_
		ConstPMoaSystemFileSpec pFileSpec,
		MoaLong maxFolderLevels,
		MoaCacheRegistryEntryEnumFromFileProc pEnumProc,
		PMoaVoid refCon
	) PURE;

	/*
		returns:
			-- kMoaDictErr_BufferTooSmall if buffer is too small
			-- kMoaDictErr_NotFound if dict contains no file info
			-- kMoaErr_NoErr otherwise
	*/
	STDMETHOD(GetFileSpecFromFileDict) (THIS_
		PIMoaFileEntryDict pFileDict,
		PMoaSystemFileSpec pFileSpec,
		MoaLong fileSpecLen
	) PURE;
};
typedef IMoaCache * PIMoaCache;

DEFINE_STD_GUID(IID_IMoaCache, 0xAC3E7803L, 0x0002, 0x8FE3, 0x00, 0x00, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

/* ------------------------------------------------------------- IMoaRegister */

#undef INTERFACE
#define INTERFACE IMoaRegister
DECLARE_INTERFACE_(IMoaRegister, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS

	STDMETHOD(Register) (THIS_
		PIMoaCache pCache,
		PIMoaXtraEntryDict pXtraDict
	) PURE;
};
typedef IMoaRegister * PIMoaRegister;

DEFINE_STD_GUID(IID_IMoaRegister, 0xAC3E7803L, 0x0002, 0x8FE5, 0x00, 0x00, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

/* --------------------------------------------------------- IMoaInitFromDict */

#undef INTERFACE
#define INTERFACE IMoaInitFromDict
DECLARE_INTERFACE_(IMoaInitFromDict, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS

	STDMETHOD(InitFromDict) (THIS_
		PIMoaRegistryEntryDict pRegistryDict
	) PURE;
};
typedef IMoaInitFromDict * PIMoaInitFromDict;

DEFINE_STD_GUID(IID_IMoaInitFromDict, 0xAC416431L, 0x000D, 0x7D16, 0x00, 0x00, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

/* --------------------------------------------------------------- IMoaStream */

typedef MoaUlong MoaStreamPosition;
typedef MoaUlong MoaStreamCount;

/*
	max number of lookahead bytes that ReadAhead() is guaranteed to support
*/
#define kMoaStreamReadAheadLimit	32

/* constants for Open::accessMode */
enum {
	kMoaStreamOpenAccess_ReadOnly,
	kMoaStreamOpenAccess_ReadWrite,
	kMoaStreamOpenAccess_WriteOnly
};

/* constants for Open::setPositionType */
enum {
	/* I'll never call SetPosition */
	kMoaStreamSetPositionType_None,
		
	/* I'll rarely call SetPosition, so it can be expensive */
	kMoaStreamSetPositionType_Expensive,	

	/* I'll call SetPosition often, so it better be cheap */
	kMoaStreamSetPositionType_Cheap	
};

/* errors */
#define	kMoaStreamErr_StreamNotOpen 		MAKE_MOAERR(80)		/* returned by everything but Open */
#define	kMoaStreamErr_StreamAlreadyOpen 	MAKE_MOAERR(81)		/* returned by Open */
#define	kMoaStreamErr_BadParameter 			MAKE_MOAERR(82)		/* you passed me bad parm */
#define	kMoaStreamErr_IoError				MAKE_MOAERR(83)		/* catch-all r/w/pos error */
#define	kMoaStreamErr_ReadPastEnd 			MAKE_MOAERR(84)		/* attempted to read past end */
#define	kMoaStreamErr_BadAccessMode			MAKE_MOAERR(85)		/* not an allowable r/w perm  */
#define	kMoaStreamErr_BadSetPositionMode	MAKE_MOAERR(86)		/* not an allowable seek perm */
#define kMoaStreamErr_ReadAheadTooFar		MAKE_MOAERR(87)		/* you asked for too much ReadAhead */
#define	kMoaStreamErr_WrotePastEnd 			MAKE_MOAERR(88)		/* attempted to write too much: disk full, etc. */
/* system-specific error messages are possible as well. */

#undef INTERFACE
#define INTERFACE IMoaStream
DECLARE_INTERFACE_(IMoaStream, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS

	STDMETHOD(Open) (THIS_
		MoaLong accessMode,
		MoaLong setPositionType
	) PURE;

	STDMETHOD(Close) (THIS) PURE;

	STDMETHOD(Read) (THIS_
		PMoaVoid pBuf,
		MoaStreamCount numToRead,
		MoaStreamCount * pNumActuallyRead
	) PURE;

	STDMETHOD(ReadAhead) (THIS_
		PMoaVoid pBuf,
		MoaStreamCount numToRead,
		MoaStreamCount * pNumActuallyRead
	) PURE;

	STDMETHOD(Write) (THIS_
		PMoaVoid pBuf,
		MoaStreamCount numToWrite,
		MoaStreamCount * pNumActuallyWritten
	) PURE;

	STDMETHOD(GetPosition) (THIS_
		MoaStreamPosition * pPos
	) PURE;

	STDMETHOD(SetPosition) (THIS_
		MoaStreamPosition pos
	) PURE;

	STDMETHOD(GetEnd) (THIS_
		MoaStreamPosition * pPos
	) PURE;

	STDMETHOD(Flush) (THIS) PURE;

	STDMETHOD(GetModes) (THIS_
		MoaLong * pAccessMode,
		MoaLong * pSetPositionType
	) PURE;
};
typedef IMoaStream * PIMoaStream;

DEFINE_GUID(IID_IMoaStream, 0xAC734D52L, 0x005D, 0x042A, 0x00, 0x00, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

/* ---------------------------------------------------------- IMoaProgressBox */

#define kMoaErr_ProgressUserCancel			MAKE_MOAERR(21000)
#define kMoaErr_ProgressBoxNotOpen			MAKE_MOAERR(21001)

#undef INTERFACE
#define INTERFACE IMoaProgressBox
DECLARE_INTERFACE_(IMoaProgressBox, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS

	STDMETHOD(OpenProgress) (THIS_ MoaBoolParam showStopButton) PURE;
	STDMETHOD(SetCaption) (THIS_ ConstPMoaChar s) PURE;
	STDMETHOD(SetUserMessage) (THIS_ ConstPMoaChar s) PURE;
	STDMETHOD(SetUserProgress) (THIS_ MoaUlong done, MoaUlong total) PURE;
	STDMETHOD_(MoaBoolParam, TestUserCanceled) (THIS) PURE;
	STDMETHOD(CloseProgress) (THIS) PURE;
};
typedef IMoaProgressBox * PIMoaProgressBox;

DEFINE_STD_GUID(IID_IMoaProgressBox, 0xABB2E822L, 0x0000, 0x74CD, 0x00, 0x00, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

/* -------------------------------------------------------------------------- */

/*
	IMoaAppInfo abstract:
	
	Describes a standard callback interface used to obtain App Info
	by an Xtra (e.g., app serial number, etc.) This may be used
	for any purpose the Xtra wishes. Most likely, the Xtra will want
	to use this to provide for copy protection of some sort (e.g.,
	serializing the Xtra to run only with an app of a given serial
	number, or only to run at authoring time, etc.)
*/

/*
	possible values for desiredInfo.

	Note that not all apps will support all these; you must be prepared
	for an app to return kMoaErr_NoSuchKey for any or all of these.
	
	additional values may be added in the future.
*/
enum {
	/*
		the application's serial number is returned as a string.
		the format of a serial number is unspecified, except that
		it is guaranteed to be less than 256 bytes in length.
		An empty string is a legal return value.
	*/
	kMoaAppInfo_SerialNumber,

	/*
		the RunMode of the app is return as a string. Currently
		defined modes are:

			"Author"	-- app is running in authoring mode
			"Projector"	-- app is running as a projector-only app
			"Plugin"	-- app is running as a plugin
			"Exporter"	-- app is running in an exporter

		These values are NEVER localized.		
	*/
	kMoaAppInfo_RunMode,
	
	/*
		the user name entered at application customization time.
	*/
	kMoaAppInfo_UserName,

	/*
		the company/organization name entered at application customization time.
	*/
	kMoaAppInfo_OrganizationName,
	
	/*
		a MoaSystemFileSpec to the application/projector executable.
		This is a MoaSystemFileSpec, not a character string, thus
		on the Macintosh you will receive an FSSpec.
	*/
	kMoaAppInfo_AppFileSpec,
	
	/*
		The name of the product, minus any version information.
		This is NEVER localized.
		A by-no-means exhaustive list of possibilities:
			"Director"
			"SoundEdit"
			"FreeHand"
			(etc.)
	*/
	kMoaAppInfo_ProductName,
	
	/*
		The version of the product. This is generally a character
		string representation of a number, though there may be
		letters as well, e.g.,
			"5.0"
			"5.0a"
			"5.5"
	*/
	kMoaAppInfo_ProductVersion,

	/*
		The build number version of the product. This is generally a character
		string representation of a number.
	*/
	kMoaAppInfo_ProductBuildVersion,

	/*
		The language of the authoring app, if present, or the
		language of the currently running OS. 
		This can be:
		
			"English"
			"Japanese"
			"French"
			"German"
			"Other"
	*/
	kMoaAppInfo_UILanguage,
	
	/*
		The language of the currently running OS.
		This can be:
			
			"English"
			"Japanese"
			"French"
			"German"
			"Portuguese"
			"Italian"
			"Spanish"
			"Swedish"
			"Chinese"
			"Korean"
			"Arabic"
			"Hebrew"
			"Other"
	*/
	kMoaAppInfo_OSLanguage,	

	/*
		This key is reserved for internal use only.  Use this key at your own risk.
		This key is purposely not documented because we don't want you to use it.
		It may not be implemented depending on what platform you are on. You have been warned! 
	*/
	kMoaAppInfo_AppFileRef,

	/*
		Find the user-writable config folder location and return it in a MoaFileSpec.
    */
	kMoaAppInfo_ConfigFolder
};

#undef INTERFACE
#define INTERFACE IMoaAppInfo
DECLARE_INTERFACE_(IMoaAppInfo, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS

	/*
		Possible return values:
			
			kMoaErr_BadParam		buffer passed is too small to contain information
			kMoaErr_NoSuchKey		this app doesn't support that value for desiredInfo
			kMoaErr_NoErr			everything is cool
	*/
	STDMETHOD(GetInfo) (THIS_
		MoaLong 	desiredInfo,
		PMoaChar	pInfoBuffer,
		MoaLong		infoBufferLen
	) PURE;
};
typedef IMoaAppInfo * PIMoaAppInfo;

DEFINE_GUID(IID_IMoaAppInfo, 0xACC7E2B8L, 0x000A, 0xB4D0, 0x00, 0x00, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

/* -------------------------------------------------------------------------- */

/*
	IMoaXtraLoadControl abstract:
	
	Describes an interface that an Xtra can use to control
	its load/unload status. This is core code, but really only
	useful on the Macintosh.
	
	An Xtra should register this interface using the normal
	technique (in IMoaRegister). It can also register an
	additional long in its registry dict to request that it
	be preloaded at launch, and/or that it never be unloaded
	once it is loaded. (Note that Macintosh Xtras that install
	callback routines with the MacOS should set both of these bits.)
		
	Note also that this interface was only implemented as of Moa 1.43,
	thus earlier apps using MOA (namely SoundEdit 16 2.0 and FreeHand 5.5)
	don't implement this.
*/

enum {
	kMoaXlc_PreloadAtLaunch = 0x0001,
	kMoaXlc_NeverUnload = 0x0002
};

#undef INTERFACE
#define INTERFACE IMoaXtraLoadControl
DECLARE_INTERFACE_(IMoaXtraLoadControl, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
};
typedef IMoaXtraLoadControl * PIMoaXtraLoadControl;
DEFINE_GUID(IID_IMoaXtraLoadControl, 0x70D7CCF4L, 0x615F, 0x11CF, 0xB4, 0x8D, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

#define kMoaRegKey_XtraLoadControlOptions 	"XtraLoadControlOptions"
#define kMoaRegType_XtraLoadControlOptions	kMoaDictType_Long

/* -------------------------------------------------------------------------- */

#undef INTERFACE
#define INTERFACE IMoaStringRefUtil
DECLARE_INTERFACE_(IMoaStringRefUtil, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS

	STDMETHOD(AllocStringUnicode) (THIS_
		ConstPMoaUnichar uchars,
		MoaUlong len,	/* note: number of uchars, not number of bytes */
		MoaStringRef * pRef
	) PURE;

	STDMETHOD(AllocStringAnsi) (THIS_
		ConstPMoaChar chars,
		MoaUlong len,
		MoaStringRef * pRef
	) PURE;

	STDMETHOD(FreeString) (THIS_
		MoaStringRef ref
	) PURE;

	STDMETHOD(StringLen) (THIS_
		MoaStringRef ref,
		MoaUlong * pLen	/* note: returns number of chars, not number of bytes */
	) PURE;

	STDMETHOD(GetUnicodeChars) (THIS_
		MoaStringRef ref,
		MoaUlong bufLen,	/* note: number of uchars, not number of bytes */
		PMoaUnichar uchars
	) PURE;

	STDMETHOD(GetAnsiChars) (THIS_
		MoaStringRef ref,
		MoaUlong bufLen,
		PMoaChar chars
	) PURE;
};
typedef IMoaStringRefUtil * PIMoaStringRefUtil;
DEFINE_GUID(IID_IMoaStringRefUtil, 0x61A83308L, 0x800E, 0x11D0, 0x9E, 0x30, 0x00, 0x05, 0x9A, 0x80, 0xE4, 0xFD);

/* -------------------------------------------------------------------------- */

#ifdef MOA_pointers_in_D0
#pragma pop
#endif

#undef INTERFACE

#endif
