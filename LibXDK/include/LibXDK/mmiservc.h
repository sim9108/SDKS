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
 Filename: mmiservc.h
 
 Purpose
	Multimedia services callback interface definitions.
	Multimedia common interface defintions.	
*/

#ifndef NO_PRAGMA_ONCE
#pragma once
#endif

#ifndef MMISERVC_H
#define MMISERVC_H


#include "moaxtra.h"		/* MOA shell */
#include "moastdif.h"		/* MOA standard interfaces */
#include "mmtypes.h"		/* multimedia types */
#include "moapath.h"


#ifdef	__cplusplus
extern "C" {
#endif


/* ----------------------------------------------------------------------------
/
/	Notification types
/
/ -------------------------------------------------------------------------- */




/* ----------------------------------------------------------------------------
/
/	IMoaMmGC - graphics context interface
/
/ --------------------------------------------------------------------------- */


/* IID_IMoaMmGC: AC96CF3E0045474800000040105023FB */
DEFINE_GUID(IID_IMoaMmGC, 0xAC96CF3EL, 0x0045, 0x4748, 0x00, 0x00, 0x00,
				0x40, 0x10, 0x50, 0x23, 0xFB);

#undef INTERFACE
#define INTERFACE IMoaMmGC

DECLARE_INTERFACE_(IMoaMmGC, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
	
	STDMETHOD(GetContentRect)			(THIS_
			PMoaRect					pContentRect)
			PURE;
			
	STDMETHOD(GetPixelDepth)			(THIS_
			MoaLong *					pPixelDepth)
			PURE;
			
	STDMETHOD(GetNativeGCInfo)			(THIS_
			PMoaMmNativeGCInfo			pNativeGCInfo)
			PURE;
			
	STDMETHOD(SetNativeGCInfo)			(THIS_
			ConstPMoaMmNativeGCInfo 	pNativeGCInfo)
			PURE;
			
	STDMETHOD(Blit) 					(THIS_
			ConstPMoaRect				pDestRect,
			struct IMoaMmGC *			pSrcGC,
			ConstPMoaRect				pSrcRect,
			MoaMmInk					ink,
			ConstPMoaMmInkParams		pInkParams,
			PMoaVoid					nativeClipRegion)
			PURE;

	
};

typedef IMoaMmGC * PIMoaMmGC;


/* IID_IMoaMmGC2: AC96CF3F0045474800000040105023FB */
DEFINE_GUID(IID_IMoaMmGC2, 0xAC96CF3FL, 0x0045, 0x4748, 0x00, 0x00, 0x00,
				0x40, 0x10, 0x50, 0x23, 0xFB);

#undef INTERFACE
#define INTERFACE IMoaMmGC2

DECLARE_INTERFACE_(IMoaMmGC2, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS

	STDMETHOD(Blit2) 					(THIS_
			ConstPMoaRect				pDestRect,
			struct IMoaMmGC *			pSrcGC,
			ConstPMoaRect				pSrcRect,
			MoaMmInk					ink,
			ConstPMoaMmInkParams		pInkParams,
			ConstPMoaMmBlitParams		pBlitParams,
			struct IMoaMmGC *			pAlphaGC,
			PMoaVoid					nativeClipRegion)
			PURE;
};

typedef IMoaMmGC2 * PIMoaMmGC2;

/* ----------------------------------------------------------------------------
/
/	IMoaMmMacEventHandler - interface for handling Macintosh events
/							use this with MacRegisterWindow(),
/							MacUnregisterWindow().
/
/								MAC ONLY
/
/ --------------------------------------------------------------------------- */

/* IID_IMoaMmMacEventHandler: ACCE6D8900A40C6B0000080007FC20C1 */
DEFINE_GUID(IID_IMoaMmMacEventHandler, 0xACCE6D89L, 0x00A4, 0x0C6B, 0x00,
				0x00, 0x08, 0x00, 0x07, 0xFC, 0x20, 0xC1);

#undef INTERFACE
#define INTERFACE IMoaMmMacEventHandler

DECLARE_INTERFACE_(IMoaMmMacEventHandler, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
				
	STDMETHOD(SetRegistered)			(THIS_
			MoaBoolParam				registered,
			PMoaVoid					refCon)
			PURE;
			
	STDMETHOD(HandleEvent)				(THIS_	
			PMoaVoid					pWindow,			
			PMoaVoid					pEventRecord,
			PMoaVoid					refCon)
			PURE;
};

typedef IMoaMmMacEventHandler * PIMoaMmMacEventHandler;


/* ----------------------------------------------------------------------------
/
/	IMoaMmPropOwner - property interface
/
/ --------------------------------------------------------------------------- */

/* IID_IMoaMmPropOwner: AC96CF670045508500000040105023FB */
DEFINE_GUID(IID_IMoaMmPropOwner, 0xAC96CF67L, 0x0045, 0x5085, 0x00, 0x00,
				0x00, 0x40, 0x10, 0x50, 0x23, 0xFB);

#undef INTERFACE
#define INTERFACE IMoaMmPropOwner

DECLARE_INTERFACE_(IMoaMmPropOwner, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
				
	STDMETHOD(GetProp) (THIS_				
			MoaMmSymbol 				symbol,
			PMoaMmValue 				pPropValue)
			PURE;
			
	STDMETHOD(SetProp) (THIS_
			MoaMmSymbol 				symbol,
			ConstPMoaMmValue			pPropValue)
			PURE;
	
};

typedef IMoaMmPropOwner * PIMoaMmPropOwner;

/* ----------------------------------------------------------------------------
/
/	IMoaMmChangeCollector - callback interface for collecting sprite actor
/							change rectangles.
/
/ --------------------------------------------------------------------------- */

/* IID_IMoaMmChangeCollector: AC96CF790045551B00000040105023FB */
DEFINE_GUID(IID_IMoaMmChangeCollector, 0xAC96CF79L, 0x0045, 0x551B, 0x00,
				0x00, 0x00, 0x40, 0x10, 0x50, 0x23, 0xFB);

#undef INTERFACE
#define INTERFACE IMoaMmChangeCollector

DECLARE_INTERFACE_(IMoaMmChangeCollector, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
	
	STDMETHOD(AddChangeRect)			(THIS_
			ConstPMoaRect				pChangeRect)
			PURE;	
			
	STDMETHOD(AddChangeRectAbove)		(THIS_
			ConstPMoaRect				pChangeRect)
			PURE;	
};

typedef IMoaMmChangeCollector * PIMoaMmChangeCollector;


/* ----------------------------------------------------------------------------
/
/	IMoaMmAssetCallback - callback interface for asset xtras.
/
/ --------------------------------------------------------------------------- */

/* IID_IMoaMmAssetCallback: ACC33D0301368B390000080007FC20C1 */
DEFINE_GUID(IID_IMoaMmAssetCallback, 0xACC33D03L, 0x0136, 0x8B39, 0x00, 0x00,
				0x08, 0x00, 0x07, 0xFC, 0x20, 0xC1);

#undef INTERFACE
#define INTERFACE IMoaMmAssetCallback

DECLARE_INTERFACE_(IMoaMmAssetCallback, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS

	STDMETHOD(GetModified)				(THIS_
			PMoaMmModifyFlags			pModifyFlags)
			PURE;	

	STDMETHOD(SetModified)				(THIS_
			MoaMmModifyFlags			modifyFlags)
			PURE;	

	STDMETHOD(NotifySprites)			(THIS_
			MoaLong 					msgCode,
			PMoaVoid					refCon)
			PURE;	

	STDMETHOD(InvalDefaultRectValue)	(THIS)
			PURE;	

};

typedef IMoaMmAssetCallback * PIMoaMmAssetCallback;


/* ----------------------------------------------------------------------------
/
/	IMoaMmAssetCallback2 - callback interface for asset xtras, Dir 6.0 and AWare 4.0.
/
/ --------------------------------------------------------------------------- */

/* IID_IMoaMmAssetCallback2: 5B0D82A6-3257-11d0-A222-00A02453444C */
DEFINE_GUID(IID_IMoaMmAssetCallback2, 0x5b0d82a6, 0x3257, 0x11d0, 0xa2, 0x22,
				0x0, 0xa0, 0x24, 0x53, 0x44, 0x4c);

#undef INTERFACE
#define INTERFACE IMoaMmAssetCallback2

DECLARE_INTERFACE_(IMoaMmAssetCallback2, IMoaMmAssetCallback)
{
	STD_IUNKNOWN_METHODS

	STDMETHOD(GetModified)				(THIS_
			PMoaMmModifyFlags			pModifyFlags)
			PURE;	

	STDMETHOD(SetModified)				(THIS_
			MoaMmModifyFlags			modifyFlags)
			PURE;	

	STDMETHOD(NotifySprites)			(THIS_
			MoaLong 					msgCode,
			PMoaVoid					refCon)
			PURE;	

	STDMETHOD(InvalDefaultRectValue)	(THIS)
			PURE;	

	STDMETHOD(ForceMediaLoad)			(THIS)
			PURE;
/* Category
Managing Media
*/

/* Description
Forces the application to call the asset xtra's <tt>StreamInMedia</tt> Call, Regardless of
whether or not the media is already loaded.  This is no way insures the application will keep
the asset loaded;  it may at any time after call the asset's <tt>FreeMedia</tt>method.
*/

	STDMETHOD(ForceMediaUnload) 		(THIS)
			PURE;
/* Category
Managing Media
*/

/* Description
Forces the application to call the asset xtra's <tt>FreeMedia</tt> Call, Regardless of
whether or not the media is already loaded.  This is no way insures the application will keep
the asset unloaded;  it may at any time after call the asset's <tt>StreamInMedia</tt>method, and
often wlll call it immediately.
*/

};

typedef IMoaMmAssetCallback2 * PIMoaMmAssetCallback2;

/* ----------------------------------------------------------------------------
/
/	IMoaMmSpriteCallback - callback interface for sprite asset xtras.
/
/ --------------------------------------------------------------------------- */

/* IID_IMoaMmSpriteCallback: ACC33D4401369A830000080007FC20C1 */
DEFINE_GUID(IID_IMoaMmSpriteCallback, 0xACC33D44L, 0x0136, 0x9A83, 0x00,
				0x00, 0x08, 0x00, 0x07, 0xFC, 0x20, 0xC1);

#undef INTERFACE
#define INTERFACE IMoaMmSpriteCallback

DECLARE_INTERFACE_(IMoaMmSpriteCallback, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
	
	STDMETHOD(InvalSpriteRect)			(THIS)
			PURE;
				
	STDMETHOD(InvalSpriteCaps)			(THIS)
			PURE;	
			
	STDMETHOD(NotifyAsset)				(THIS_
			MoaLong 					msgCode,
			PMoaVoid					refCon)
			PURE;	
};

typedef IMoaMmSpriteCallback * PIMoaMmSpriteCallback;



DEFINE_GUID(IID_IMoaMmCuePointCallback, 0x6C711BF4L, 0x16E8, 0x11D0, 0xA0,
			0x46, 0x00, 0x05, 0x9A, 0x80, 0xE8, 0x2F);

#undef INTERFACE
#define INTERFACE IMoaMmCuePointCallback

DECLARE_INTERFACE_(IMoaMmCuePointCallback, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
	
	STDMETHOD(GenerateCuePoint) 		(THIS_
			ConstPMoaMmCuePoint 		pCuePoint,
			MoaUlong					nIndex)
			PURE;	


};

typedef IMoaMmCuePointCallback * PIMoaMmCuePointCallback;

/* ----------------------------------------------------------------------------
/
/	IMoaMmUtils - shared utility functions
/
/ --------------------------------------------------------------------------- */

/* IID_IMoaMmUtils: AC96CF890045587900000040105023FB */
DEFINE_GUID(IID_IMoaMmUtils, 0xAC96CF89L, 0x0045, 0x5879, 0x00, 0x00, 0x00,
				0x40, 0x10, 0x50, 0x23, 0xFB);

#undef INTERFACE
#define INTERFACE IMoaMmUtils

DECLARE_INTERFACE_(IMoaMmUtils, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
	
	/* Print to debug (message) window */
	
	STDMETHOD(PrintMessage) 			(THIS_
			ConstPMoaChar				pMsg)
			PURE;
			
	STDMETHOD(PrintMessage1)			(THIS_
			ConstPMoaChar				pMsg,
			MoaLong 					arg1)
			PURE;
			
	STDMETHOD(PrintMessage2)			(THIS_
			ConstPMoaChar				pMsg,
			MoaLong 					arg1,
			MoaLong 					arg2)
			PURE;
			
	STDMETHOD(PrintMessage3)			(THIS_
			ConstPMoaChar				pMsg,
			MoaLong 					arg1,
			MoaLong 					arg2,
			MoaLong 					arg3)
			PURE;
			
	STDMETHOD(PrintMessage4)			(THIS_
			ConstPMoaChar				pMsg,
			MoaLong 					arg1,
			MoaLong 					arg2,
			MoaLong 					arg3,
			MoaLong 					arg4)
			PURE;
	
	/* Symbol dictionary access */
	
	STDMETHOD(StringToSymbol)			(THIS_
			ConstPMoaChar				pString,
			PMoaMmSymbol				pSymbol)
			PURE;
			
	STDMETHOD(SymbolToString)			(THIS_
			MoaMmSymbol 				symbol,
			PMoaChar					pStringBuf,
			MoaLong 					bufLen)
			PURE;
	
	/* Value access */
	
	STDMETHOD(ValueType)				(THIS_
			ConstPMoaMmValue			pValue,
			MoaMmValueType *			pResult)
			PURE;
	STDMETHOD(ValueAddRef)				(THIS_
			PMoaMmValue 				pValue)
			PURE;
	STDMETHOD(ValueRelease) 			(THIS_
			PMoaMmValue 				pValue)
			PURE;
	
	/* Convert from value */
	
	STDMETHOD(ValueToInteger)			(THIS_
			ConstPMoaMmValue			pValue,
			MoaLong *					pResult)
			PURE;
			
	STDMETHOD(ValueToSymbol)			(THIS_
			ConstPMoaMmValue			pValue,
			PMoaMmSymbol				pResult)
			PURE;
			
	STDMETHOD(ValueToFloat) 			(THIS_
			ConstPMoaMmValue			pValue,
			MoaDouble *		 			pResult)
			PURE;
			
	STDMETHOD(ValueStringLength)		(THIS_
			ConstPMoaMmValue			pValue,
			MoaLong *					pResult)
			PURE;
			
	STDMETHOD(ValueToString)			(THIS_
			ConstPMoaMmValue			pValue,
			PMoaChar					pStringBuf,
			MoaLong 					bufLen)
			PURE;
			
	STDMETHOD(ValueToStringPtr) 		(THIS_
			ConstPMoaMmValue			pValue,
			ConstPMoaChar *		 		pResult)
			PURE;
			
	STDMETHOD(ValueReleaseStringPtr)	(THIS_
			ConstPMoaMmValue			pValue)
			PURE;
			
	STDMETHOD(ValueToPoint) 			(THIS_
			 ConstPMoaMmValue			pValue,
			 PMoaPoint					pResult)
			 PURE;
			
	STDMETHOD(ValueToRect)				(THIS_
			ConstPMoaMmValue			pValue,
			PMoaRect					pResult)
			PURE;

	/* Convert to value */
	
	STDMETHOD(IntegerToValue)			(THIS_
			MoaLong 					num,
			PMoaMmValue 				pValue)
			PURE;
			
	STDMETHOD(SymbolToValue)			(THIS_
			MoaMmSymbol 				symbol,
			PMoaMmValue 				pValue)
			PURE;
			
	STDMETHOD(FloatToValue) 			(THIS_
			MoaDouble					fnum,
			PMoaMmValue 				pValue)
			PURE;
			
	STDMETHOD(StringToValue)			(THIS_
			ConstPMoaChar				pString,
			PMoaMmValue 				pValue)
			PURE;
			
	STDMETHOD(StringHandleToValue)		(THIS_
			MoaHandle					hString,
			PMoaMmValue 				pValue)
			PURE;
			
	STDMETHOD(PointToValue) 			(THIS_
			ConstPMoaPoint				pPoint,
			PMoaMmValue 				pValue)
			PURE;
			
	STDMETHOD(RectToValue)				(THIS_
			ConstPMoaRect				pRect,
			PMoaMmValue 				pValue)
			PURE;

	/* Graphics context */
	
	STDMETHOD(NewGC)					(THIS_
			ConstPMoaMmNativeGCInfo 	pNativeGCInfo,
			PIMoaMmGC *		 			ppGC)
			PURE;

	/* Mac-specific callbacks - some are Mac toolbox replacements */	
	
	STDMETHOD(MacHandleEvent)			(THIS_
			PMoaVoid					pEventRecord)
			PURE;
			
	STDMETHOD(MacPrepareModalDialog)	(THIS_
			PMoaMmDialogCookie			pDialogCookie)
			PURE;
			
	STDMETHOD(MacModalDialog)			(THIS_
			MoaMmDialogCookie			dialogCookie,
			PMoaVoid					filterProcUPP,
			MoaShort *					itemHit)
			PURE;
			
	STDMETHOD(MacUnprepareModalDialog)	(THIS_
			MoaMmDialogCookie			dialogCookie)
			PURE;
			
	STDMETHOD_(MoaLong, MacAlert)		(THIS_
			MoaLong 					alertID,
			PMoaVoid					filterProcUPP)
			PURE;
			
	STDMETHOD(MacRegisterWindow)		(THIS_
			PIMoaMmMacEventHandler		pEventHandler,
			PMoaMmMacWindow 			pWindow,
			MoaMmRegWindowFlags 		flags,
			PMoaVoid					refCon)
			PURE;
			
	STDMETHOD(MacUnregisterWindow)		(THIS_
			 PIMoaMmMacEventHandler 	pEventHandler)
			 PURE;
			
	STDMETHOD(MacDragWindow)			(THIS_
			PMoaMmMacWindow 			pWindow,
			PMoaPoint					pStartPoint,
			PMoaRect					pLimitRect)
			PURE;
			
	STDMETHOD(MacSelectWindow)			(THIS_
			PMoaMmMacWindow 			pWindow)
			PURE;
			
	STDMETHOD(MacXShowWindow)			(THIS_
			PMoaMmMacWindow 			pWindow)
			PURE;
			
	STDMETHOD(MacHideWindow)			(THIS_
			PMoaMmMacWindow 			pWindow)
			PURE;

	/* Win-specific callbacks - no-ops on Mac - Use this instead of Win API */
	
	STDMETHOD_(MoaLong, WinDialogBox)	(THIS_
			XtraResourceCookie			resCookie,
			 ConstPMoaChar				lpszTemplate,
			 PMoaVoid					dlgprc)
			 PURE;

	STDMETHOD(WinGetParent) 			(THIS_
			MoaMmHInst *				phInst,
			MoaMmHWnd *		 			phWnd)
			PURE;
	
	/* Misc */
	
	STDMETHOD(RGBToIndex)				(THIS_
			ConstPMoaMmRGBTriple		pRGBColor,
			PMoaMmColorIndex			pIndex)
			PURE;
			
	STDMETHOD(IndexToRGB)				(THIS_
			MoaMmColorIndex 			index,
			PMoaMmRGBTriple 			pRGBColor)
			PURE;
	
	/* More win-specific */
	
	STDMETHOD_(MoaLong, WinDialogBoxParam)	(THIS_
			XtraResourceCookie			resCookie,
			ConstPMoaChar				lpszTemplate,
			PMoaVoid					dlgprc,
			MoaLong 					lparamInit)
			PURE;

	/* Win: Use these to surround other Windows API calls
		invoking modal dialogs.  Not needed if using
		WinDialogBox() or WinDialogBoxParam().
	*/
	
	STDMETHOD(WinPrepareDialogBox)		(THIS_
			PMoaMmDialogCookie			pDialogCookie)
			PURE;
			
	STDMETHOD(WinUnprepareDialogBox)	(THIS_
			MoaMmDialogCookie			dialogCookie)
			PURE;

};

typedef IMoaMmUtils * PIMoaMmUtils;




/* ----------------------------------------------------------------------------
/
/	IMoaMmList - list value functions
/
/ --------------------------------------------------------------------------- */

/* IID_IMoaMmList: AC96CF9800455C0B00000040105023FB */
DEFINE_GUID(IID_IMoaMmList, 0xAC96CF98L, 0x0045, 0x5C0B, 0x00, 0x00, 0x00, 0x40, 0x10, 0x50, 0x23, 0xFB);

#undef INTERFACE
#define INTERFACE IMoaMmList

DECLARE_INTERFACE_(IMoaMmList, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS

	STDMETHOD(NewListValue) 			(THIS_
			PMoaMmValue 				pListValue)
			PURE;
			
	STDMETHOD(AppendValueToList)		(THIS_
			PMoaMmValue 				pListValue,
			PMoaMmValue 				pElementValue)
			PURE;
			
	STDMETHOD_(MoaLong, CountElements)	(THIS_
			ConstPMoaMmValue			pListValue)
			PURE;
			
	STDMETHOD(GetValueByIndex)			(THIS_
			ConstPMoaMmValue			pListValue,
			MoaLong 					index,
			PMoaMmValue 				pResultValue)
			PURE;
			
	STDMETHOD(SetValueByIndex)			(THIS_
			PMoaMmValue 				pListValue,
			MoaLong 					index,
			PMoaMmValue 				pNewValue)
			PURE;

	STDMETHOD(NewPropListValue) 		(THIS_
			PMoaMmValue 				pListValue)
			PURE;
			
	STDMETHOD(AppendValueToPropList)	(THIS_
			PMoaMmValue 				pListValue,
			PMoaMmValue 				pElementProp,
			PMoaMmValue 				pElementValue)
			PURE;
			
	STDMETHOD(GetValueByProperty)		(THIS_
			ConstPMoaMmValue			pListValue,
			PMoaMmValue 				pPropNameValue,
			PMoaMmValue 				pResultValue)
			PURE;
			
	STDMETHOD(SetValueByProperty)		(THIS_
			PMoaMmValue 				pListValue,
			PMoaMmValue 				pPropNameValue,
			PMoaMmValue 				pNewValue)
			PURE;
			
	STDMETHOD(GetPropertyNameByIndex)	(THIS_
			ConstPMoaMmValue			pListValue,
			MoaLong 					index,
			PMoaMmValue 				pResultValue)
			PURE;
};

typedef IMoaMmList * PIMoaMmList;


/* ----------------------------------------------------------------------------
/
/	IMoaMmInterrogator - Classes supporting this interface are instantiated
/							at start-up after all xtras have been registered
/							and called immediately.  They are released at
/							sometime later (app quit at the latest).  Use
/							IMoaMmTerminator, below, for quit notification.
/
/							You should ensure you are running in the correct
/							application before doing anything, as all Macromedia
/							apps supporting the Multimedia APIs will call
/							these at start-up.
/
/						Note: This interface is the same as IMoaFhInterrogator
/							(used by FreeHand Xtras) but has a different IID.
/							This prevents it from being called by FreeHand.
/							In the future MOA may supply a generic
/							Interrogator interface.
/
/ --------------------------------------------------------------------------- */

/* IID_IMoaMmInterrogator: AA3E3B1E477311CF824E00009403C70F */
DEFINE_GUID(IID_IMoaMmInterrogator, 0xAA3E3B1EL, 0x4773, 0x11CF, 0x82, 0x4E,
					0x00, 0x00, 0x94, 0x03, 0xC7, 0x0F);
					
#undef INTERFACE
#define INTERFACE IMoaMmInterrogator
DECLARE_INTERFACE_(IMoaMmInterrogator, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
	STDMETHOD_(void, Interrogate) (THIS) PURE;
};
typedef IMoaMmInterrogator * PIMoaMmInterrogator;


/*
	Keys to enter in the registry:
	None.
*/

/* ----------------------------------------------------------------------------
/
/	IMoaMmTerminator - Classes supporting this interface are instantiated
/						at shut-down, called immediately, and then released.
/
/						You should ensure you are running in the correct
/						application before doing anything, as all Macromedia
/						apps supporting the Multimedia APIs will call
/						these at shut-down.
/
/						Note: This interface is the same as IMoaFhTerminator
/							(used by FreeHand Xtras) but has a different IID.
/							This prevents it from being called by FreeHand.
/							In the future MOA may supply a generic
/							Interrogator interface.
/
/ --------------------------------------------------------------------------- */

/* IID_IMoaMmTerminator: D74E09F4477311CF824E00009403C70F */
DEFINE_GUID(IID_IMoaMmTerminator, 0xD74E09F4L, 0x4773, 0x11CF, 0x82, 0x4E,
					0x00, 0x00, 0x94, 0x03, 0xC7, 0x0F);

#undef INTERFACE
#define INTERFACE IMoaMmTerminator
DECLARE_INTERFACE_(IMoaMmTerminator, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
	STDMETHOD_(void, Terminate) (THIS) PURE;
};
typedef IMoaMmTerminator * PIMoaMmTerminator;


/*
	Keys to enter in the registry:
	None.
*/


/* ---------------------------------------------------------------------------- */

#undef INTERFACE


/*
**
** FilePath Services interface
**
*/


/* IID_IMoaMmFilePathServices:	1c292400-1a7c-11d2-a718-00a0c98b9ff5 */
DEFINE_GUID(IID_IMoaMmFilePathServices, 0x1c292400, 0x1a7c, 0x11d2, 0xa7, 0x18, 0x00, 0xa0, 0xc9, 0x8b, 0x9f, 0xf5);

#undef	INTERFACE
#define INTERFACE	IMoaMmFilePathServices

DECLARE_INTERFACE_(IMoaMmFilePathServices, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS

	STDMETHOD(ResolveFilePath) (THIS_
		/*IN*/	PIMoaPathName	pPathToResolve,
		/*IN*/	MoaBoolParam	bBrowseForFile,
		/*OUT*/ PIMoaPathName	pResolvedPath
	) PURE;

	STDMETHOD(SetFilePath) (THIS_
		/*IN*/	PIMoaPathName	pNewPath
	) PURE;

	STDMETHOD(GetFilePath) (THIS_
		/*OUT*/ PIMoaPathName	pPathOfMember
	) PURE;

	STDMETHOD(GetLocalFilePath) (THIS_
		/*OUT*/	PIMoaPathName	pLocalPathOfMember
	) PURE;

	STDMETHOD(SetLinked) (THIS_
		/*IN*/	MoaBoolParam	bLinked
	) PURE;

	STDMETHOD_(MoaBoolParam, IsLinked) (THIS) PURE;

	STDMETHOD(BeginDownload) (THIS_
		/*OUT*/ PIMoaStream *ppStream
	) PURE;

	STDMETHOD(BrowseForFile) (THIS_
		/*IN*/	PIMoaPathName	pPathToBeginBrowsingIn,
		/*IN*/	ConstPMoaLong	pFileTypeList,
		/*IN*/	ConstPMoaChar	pFileExtensions,
		/*OUT*/ PIMoaPathName	pSelectedFile
	) PURE;

	STDMETHOD(SetStreamable) (THIS_	
		/*IN*/	MoaBoolParam	bStreamable
	) PURE;

	STDMETHOD_(MoaBoolParam, IsStreamable) (THIS) PURE;
};
typedef IMoaMmFilePathServices *PIMoaMmFilePathServices;

/* ---------------------------------------------------------------------------- */

#ifdef	__cplusplus
}
#endif


#include "mmiutil.h"
#include "mmivalue.h"
#include "mmiwndm.h"
#include "mmiwndw.h"
#include "moanotif.h"

#endif	/* MMISERVC_H */
