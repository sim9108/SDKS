/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_mixdatob
#define _H_mixdatob

#ifndef _H_moastdif
	#include "moastdif.h"
#endif

#ifndef moa_in
	#define moa_in
#endif
#ifndef moa_inout
	#define moa_inout
#endif
#ifndef moa_out
	#define moa_out
#endif
#ifndef MAKE_MOASTATUS
	#define MAKE_MOASTATUS(code) ((MoaError)((MoaUlong)(0L << 31) | (MoaUlong)(4L << 16) | (MoaUlong)(code)))
#endif
#ifndef MoaDeclareMethod
	#define MoaDeclareMethod(macro_NAME, macro_ARGS) \
		STDMETHOD(macro_NAME) macro_ARGS PURE;
#endif
#ifndef MoaDeclareMethod_
	#define MoaDeclareMethod_(macro_RET, macro_NAME, macro_ARGS) \
		STDMETHOD_(macro_RET, macro_NAME) macro_ARGS PURE;
#endif
#ifndef EXTERN_DEFMETHOD
	#define EXTERN_DEFMETHOD(macro_NAME, macro_ARGS) \
		EXTERN_DEFINE_METHOD(MoaError, macro_NAME, macro_ARGS)
#endif
#ifndef EXTERN_DEFMETHOD_
	#define EXTERN_DEFMETHOD_(macro_RET, macro_NAME, macro_ARGS) \
		EXTERN_DEFINE_METHOD(macro_RET, macro_NAME, macro_ARGS)
#endif

/* -------------------------------------------------------------------------- */
/* -------------------------- DataObject Definitions ------------------------ */
/* -------------------------------------------------------------------------- */

/* Errors. */
#define kMoaErr_BadStorageMedium	((MoaError)0x80040069L) 	/* same as DV_E_TYMED */
#define kMoaErr_BadFormatEtc		((MoaError)0x80040064L) 	/* same as DV_E_FORMATETC */
#define kMoaStatus_SameFormat		((MoaError)0x00040130L) 	/* same as DATA_S_SAMEFORMATETC */
#define kMoaErr_NotImplemented		((MoaError)0x80004001L) 	/* same as E_NOTIMPL */
#define kMoaErr_AdviseNotSupported	((MoaError)0x80040003L) 	/* same as OLE_E_ADVISENOTSUPPORTED */
#define kMoaErr_InvalidEnumArg		((MoaError)0x80070057L) 	/* same as E_INVALIDARG */
#define kMoaStatus_DoDrop			((MoaError)0x00040100L) 	/* same as DRAGDROP_S_DROP */
#define kMoaStatus_CancelDrop		((MoaError)0x00040101L) 	/* same as DRAGDROP_S_CANCEL */
#define kMoaStatus_DoDefaultDrag	((MoaError)0x00040102L) 	/* same as DRAGDROP_S_USEDEFAULTCURSORS */

#if defined(WINDOWS) && !defined(WIN32)
	/* this must be binary-compatible with the MS BOOL parameter, which is
		a 16-bit int under Win16. Alas. */
	typedef int MoaMsBool;
	/* same deal with handles. */
	typedef unsigned int SysHandle;
	typedef unsigned int SysWindow;
#else
	typedef MoaBoolParam MoaMsBool;
	typedef PMoaVoid SysHandle;
	typedef PMoaVoid SysWindow;
#endif

/*
	Note that all of these definitions are binary-equivalent to the Windows
		standard interfaces and defines. We redefine them here, with
		slightly different names, so that the Windows headers aren't
		required (but can be included without conflicting).
*/

/* 
	these are some well-known clipboard formats. 
	note that text, tiff, unicode, and draggedfiles are all
	special-cased in IMoaFormatServices::RegisterFormat to match
	the Windows hard-coded values. 
	
	(Note: older versions of this header had kMoaCfFormat_Text,
	kMoaCfFormat_TIFF, kMoaCfFormat_Unicode, and kMoaCfFormat_DraggedFiles
	defined as straight numerical values; we removed those and now recommend
	you always use IMoaFormatServices::RegisterFormat to get the values.
	Don't worry, the values will always match those "special" values!)
*/

#define kMoaCfFormatName_Text			"kMoaCfFormat_Text" /* NULL-terminated ASCII (i.e., a C string) */
#define kMoaCfFormatName_BITMAP 		"kMoaCfFormat_BITMAP"
#define kMoaCfFormatName_MetafilePict	"kMoaCfFormat_MetafilePict"
#define kMoaCfFormatName_TIFF			"kMoaCfFormat_TIFF"
#define kMoaCfFormatName_DIB			"kMoaCfFormat_DIB"
#define kMoaCfFormatName_WAVE			"kMoaCfFormat_WAVE"
#define kMoaCfFormatName_Unicode		"kMoaCfFormat_Unicode"
#define kMoaCfFormatName_DraggedFiles	"kMoaCfFormat_DraggedFiles"

/*
	Some other well-known formats.
*/
#define kMoaCfFormatName_AIFF				"kMoaCfFormat_AIFF"
#define kMoaCfFormatName_snd				"kMoaCfFormat_snd"			/* Macintosh 'snd ' format */
#define kMoaCfFormatName_SWA				"kMoaCfFormat_SWA"
#define kMoaCfFormatName_MPEG3				"kMoaCfFormat_MPEG3"
#define kMoaCfFormatName_LRG				"kMoaCfFormat_LRG"
#define kMoaCfFormatName_PICT				"kMoaCfFormat_PICT"
#define kMoaCfFormatName_JPEG				"kMoaCfFormat_JPEG"
#define kMoaCfFormatName_GIF				"kMoaCfFormat_GIF"
#define kMoaCfFormatName_PNG				"kMoaCfFormat_PNG"
#define kMoaCfFormatName_MacDragRef 		"kMoaCfFormat_MacDragRef"	/* data is DragReference */
#define kMoaCfFormatName_Script 			"kMoaCfFormat_Script"	/* script cast member */
#define kMoaCfFormatName_Flash	 			"kMoaCfFormat_Flash"	/* flash cast member */

/* note: DragBounds is currently not always accurate. use the MacDragRef
	to get a more-accurate bounds rect. */
#define kMoaCfFormatName_DragBounds 		"kMoaCfFormat_DragBounds"	/* data is MoaRect (NOT Mac Rect) */
/*
	Note that this is NOT the same as CF_BITMAP or CF_DIB;
	this requests a format identical to a .BMP file (though
	note that a .BMP file is essentially a DIB with a short
	header on it).
*/
#define kMoaCfFormatName_BMP			"kMoaCfFormat_BMP"

enum {
	kMoaAspect_Content	= 1,
	kMoaAspect_Thumbnail= 2,
	kMoaAspect_Icon 	= 4,
	kMoaAspect_Print	= 8
};

#define kMoaStorageMediumType_NULL			0x00000000L /* undefined */
#define kMoaStorageMediumType_HGlobal		0x00000001L /* HGLOBAL/Mac Handle */
#define kMoaStorageMediumType_File			0x00000002L /* pathname */
#define kMoaStorageMediumType_IStream		0x00000004L /* IStream* */
#define kMoaStorageMediumType_IStorage		0x00000008L /* IStorage* */
#define kMoaStorageMediumType_GDI			0x00000010L /* HGDI */
#define kMoaStorageMediumType_Metafile		0x00000020L /* HMETAFILEPICT */
#define kMoaStorageMediumType_EnhMetafile	0x00000040L /* HENHMETAFILE */
#define kMoaStorageMediumType_AllTypes		0x7fffffffL /* all types. */

#if defined(WINDOWS) || defined(_WINDOWS)
	typedef MoaUshort MixFormat;
#else
	typedef MoaUlong MixFormat;
#endif
#if (defined(WINDOWS) || defined(_WINDOWS)) && defined(WIN32)
	#include <wchar.h>
	typedef MoaUnichar MoaPathnameChar;
	typedef PMoaUnichar PMoaPathnameChar;
	typedef ConstPMoaUnichar ConstPMoaPathnameChar;
	#define MoaPathnameLen wcslen
	#define MoaPathnameCpy wcscpy
	#define MoaPathnameNCpy wcsncpy
#else
	#include <string.h>
	typedef MoaChar MoaPathnameChar;
	typedef PMoaChar PMoaPathnameChar;
	typedef ConstPMoaChar ConstPMoaPathnameChar;
	#define MoaPathnameLen strlen
	#define MoaPathnameCpy strcpy
	#define MoaPathnameNCpy strncpy
#endif
typedef MoaUlong MixFormatParam;
typedef MixFormat * PMixFormat;
typedef const MixFormat * ConstPMixFormat;

/* 
	essentially identical to FORMATETC.
	(Note: apparently Mac OLE uses an unsigned long for the cfFormat field.)
*/
#if (defined(WINDOWS) || defined(_WINDOWS)) && defined(WIN32)
#pragma pack(push, 8)
#endif
typedef struct MoaFormatEtc {
	MixFormat			cfFormat;
	PMoaVoid			ptd;	/* unused in MOA; set to NULL */
	MoaLong 			dwAspect;
	MoaLong 			lindex;
	MoaLong 			tymed;
} MoaFormatEtc;
#if (defined(WINDOWS) || defined(_WINDOWS)) && defined(WIN32)
#pragma pack(pop)
#endif
typedef MoaFormatEtc * PMoaFormatEtc;
typedef const MoaFormatEtc * ConstPMoaFormatEtc;

#define SetMoaFormatEtc(fmt, cf, ty) \
	do { \
		(fmt)->cfFormat = (cf); \
		(fmt)->ptd = NULL; \
		(fmt)->dwAspect = kMoaAspect_Content; \
		(fmt)->lindex = -1; \
		(fmt)->tymed = (ty); \
	} while (0)

/* essentially identical to STGMEDIUM (but with extra union fields) */
#if (defined(WINDOWS) || defined(_WINDOWS)) && defined(WIN32)
#pragma pack(push, 8)
#endif
typedef struct MoaStorageMedium {
	MoaLong 					tymed;
	union {
		SysHandle				hBitmap;		/* HBITMAP */
		SysHandle				hMetaFilePict;	/* HMETAFILEPICT */
		SysHandle				hEnhMetaFile;	/* HENHMETAFILE */
		SysHandle				hGlobal;		/* HGLOBAL */
		PMoaPathnameChar		lpszFileName;	/* LPOLESTR */
		PIMoaUnknown			pstm;			/* LPSTREAM */
		PIMoaUnknown			pstg;			/* LPSTORAGE */
	} u;
	PIMoaUnknown				pUnkForRelease;
} MoaStorageMedium;
#if (defined(WINDOWS) || defined(_WINDOWS)) && defined(WIN32)
#pragma pack(pop)
#endif
typedef MoaStorageMedium * PMoaStorageMedium;
typedef const MoaStorageMedium * ConstPMoaStorageMedium;

#define kEnumDir_Get 1
#define kEnumDir_Set 2

#undef INTERFACE
#define INTERFACE IMoaEnumFormatEtc
DECLARE_INTERFACE_(IMoaEnumFormatEtc, IMoaUnknown)
/*	Description

*/
{
	STD_IUNKNOWN_METHODS

	MoaDeclareMethod(Next, (THIS_
		moa_in MoaUlong celt,
		moa_out PMoaFormatEtc pFormatEtc,
		moa_out MoaUlong * pCeltFetched
	))
/*	Description

*/

	MoaDeclareMethod(Skip, (THIS_
		moa_in MoaUlong celt
	))
/*	Description

*/

	MoaDeclareMethod(Reset, (THIS))
/*	Description

*/

	MoaDeclareMethod(Clone, (THIS_
		moa_out struct IMoaEnumFormatEtc * * ppEnum
	))
/*	Description

*/
};
typedef IMoaEnumFormatEtc * PIMoaEnumFormatEtc;
DEFINE_STD_GUID(IID_IMoaEnumFormatEtc, 0x00000103L, 0x0000, 0x0000, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46);

#undef INTERFACE
#define INTERFACE IMoaDataObject
DECLARE_INTERFACE_(IMoaDataObject, IMoaUnknown)
/*	Description

*/
{
	STD_IUNKNOWN_METHODS

	MoaDeclareMethod(GetData, (THIS_
		moa_in ConstPMoaFormatEtc pFormatEtc,
		moa_out PMoaStorageMedium pMedium
	))
/*	Description

*/

	MoaDeclareMethod(GetDataHere, (THIS_
		moa_in ConstPMoaFormatEtc pFormatEtc,
		moa_inout PMoaStorageMedium pMedium
	))
/*	Description

*/

	MoaDeclareMethod(QueryGetData, (THIS_
		moa_in ConstPMoaFormatEtc pFormatEtc
	))
/*	Description

*/

	MoaDeclareMethod(GetCanonicalFormatEtc, (THIS_
		moa_in ConstPMoaFormatEtc pFormatEtcIn,
		moa_in PMoaFormatEtc pFormatEtcOut
	))
/*	Description

*/

	MoaDeclareMethod(SetData, (THIS_
		moa_in ConstPMoaFormatEtc pFormatEtc,
		moa_in ConstPMoaStorageMedium pMedium,
		moa_in MoaMsBool release
	))
/*	Description

*/

	MoaDeclareMethod(EnumFormatEtc, (THIS_
		moa_in MoaLong direction,
		moa_out PIMoaEnumFormatEtc * ppEnum
	))
/*	Description

*/

	MoaDeclareMethod(DAdvise, (THIS_
		moa_in ConstPMoaFormatEtc pFormatEtc,
		moa_in MoaLong flags,
		moa_in PIMoaUnknown pSink,
		moa_out MoaLong * pConnection
	))
/*	Description

*/

	MoaDeclareMethod(DUnadvise, (THIS_
		moa_in MoaLong connection
	))
/*	Description

*/

	MoaDeclareMethod(EnumDAdvise, (THIS_
		moa_out PIMoaUnknown * ppEnum
	))
/*	Description

*/
};
typedef IMoaDataObject * PIMoaDataObject;
DEFINE_STD_GUID(IID_IMoaDataObject, 0x0000010EL, 0x0000, 0x0000, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46);

/* -------------------------------------------------------------------------- */
/* --------------------------- DragNDrop Definitions ------------------------ */
/* -------------------------------------------------------------------------- */

#define kMoaDropEffect_None 		0x00000000L
#define kMoaDropEffect_Copy 		0x00000001L
#define kMoaDropEffect_Move 		0x00000002L
#define kMoaDropEffect_Link 		0x00000004L
#define kMoaDropEffect_Autoscroll	0x80000000L

#define kMoaDropKey_LButton 		0x0001
#define kMoaDropKey_RButton 		0x0002
#define kMoaDropKey_Shift			0x0004
#define kMoaDropKey_Control 		0x0008
#define kMoaDropKey_MButton 		0x0010
#define kMoaDropKey_Alt 			0x0020
#define kMoaDropKey_Command 		0x0040

/* Note that this structure is equivalent to a POINTL,
	but not a MoaPoint; the fields are reversed. */
typedef struct {
	MoaLong x;
	MoaLong y;
} MoaGlobalMouseLoc;

/*
	Note: Macintosh systems are guaranteed that the drop window
	is the current GrafPort upon entry to each of these methods.
*/
#undef INTERFACE
#define INTERFACE IMoaDropTarget
DECLARE_INTERFACE_(IMoaDropTarget, IMoaUnknown)
/*	Description

*/
{
	STD_IUNKNOWN_METHODS

	MoaDeclareMethod(DragEnter, (THIS_
		moa_in PIMoaDataObject pData,
		moa_in MoaLong modifierKeys,
		moa_in MoaGlobalMouseLoc mouseLoc,	/* (note: by value, not reference) */
		moa_out MoaLong * pEffect
	))
/*	Description

*/

	MoaDeclareMethod(DragOver, (THIS_
		moa_in MoaLong modifierKeys,
		moa_in MoaGlobalMouseLoc mouseLoc,	/* (note: by value, not reference) */
		moa_out MoaLong * pEffect
	))
/*	Description

*/

	MoaDeclareMethod(DragLeave, (THIS))
/*	Description

*/

	MoaDeclareMethod(Drop, (THIS_
		moa_in PIMoaDataObject pData,
		moa_in MoaLong modifierKeys,
		moa_in MoaGlobalMouseLoc mouseLoc,	/* (note: by value, not reference) */
		moa_out MoaLong * pEffect
	))
/*	Description

*/
};
typedef IMoaDropTarget * PIMoaDropTarget;
DEFINE_STD_GUID(IID_IMoaDropTarget, 0x00000122L, 0x0000, 0x0000, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46);

/*
	This interface may optionally be implemented on the same object as IMoaDropTarget;
	if it is, then the Macintosh tracking code will use it to obtain a drag region to
	hilite. If the interface is not implemented, the entire window will be hilited.

	This is only implemented on the Macintosh.
*/
#undef INTERFACE
#define INTERFACE IMoaDropTargetMacHiliter
DECLARE_INTERFACE_(IMoaDropTargetMacHiliter, IMoaUnknown)
/*	Description

*/
{
	STD_IUNKNOWN_METHODS

	MoaDeclareMethod_(MoaBoolParam, HiliteRegionChanged, (THIS))
/*	Description

	This method should return T if the hilite rgn has
	changed since the last call to GetHiliteRegion.

	The caller may use this to avoid unnecessary calls
	to GetHiliteRegion.
*/

	MoaDeclareMethod(GetHiliteRegion, (THIS_
		moa_in SysWindow			macWindow,
		moa_in MoaLong				modifierKeys,
		moa_in MoaGlobalMouseLoc	mouseLoc,		/* (note: by value, not reference) */
		moa_out PMoaVoid *			phRgn,
		moa_out MoaBool *			pFreeRgn
	))
/*	Description

	macWindow is the Macintosh WindowPtr of the window.

	modifierKeys and mouseLoc are identical to those of IMoaDropTarget::DragOver.

	you should set *phRgn to the RgnHandle you want to be hilited.
	if *pFreeRgn is TRUE, the caller will free the region after using it;
		if *pFreeRgn is FALSE, the caller will NOT free the region.

	Note that the hilite region will not be shown unless IMoaDropTarget::DragEnter/
	IMoaDropTarget::DragOver returns a valid value for dropEffect.
*/
};
typedef IMoaDropTargetMacHiliter * PIMoaDropTargetMacHiliter;
DEFINE_GUID(IID_IMoaDropTargetMacHiliter, 0x063ED8C8L, 0x6525, 0x11CF, 0x84, 0x7B, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

#undef INTERFACE
#define INTERFACE IMoaDropSource
DECLARE_INTERFACE_(IMoaDropSource, IMoaUnknown)
/*	Description

*/
{
	STD_IUNKNOWN_METHODS

	MoaDeclareMethod(QueryContinueDrag, (THIS_
		moa_in MoaMsBool userCanceledDrag,
		moa_in MoaLong modifierKeys
	))
/*	Description

*/

	MoaDeclareMethod(GiveFeedback, (THIS_
		moa_in MoaLong effect
	))
/*	Description

*/
};
typedef IMoaDropSource * PIMoaDropSource;
DEFINE_STD_GUID(IID_IMoaDropSource, 0x00000121L, 0x0000, 0x0000, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46);

#undef INTERFACE

#endif
