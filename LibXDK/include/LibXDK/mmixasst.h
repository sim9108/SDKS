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
		Common media asset interfaces.
*/

#ifndef	NO_PRAGMA_ONCE
#pragma	once
#endif

#ifndef	MMIXASST_H
#define	MMIXASST_H


#include "moaxtra.h"
#include "moastdif.h"
#include "moastr2.h"
#include "mmiservc.h"


#ifdef	__cplusplus
extern "C" {
#endif

/* ----------------------------------------------------------------------------
/
/	IMoaRegister
/
/ --------------------------------------------------------------------------- */

/* This interface is defined in moastdif.h.  A class must be implemented
   which supports this interface; the Register() method should register
   information for each cast member type supported by this xtra.  
   Register() must add dictionary entries for:
   
   typeSymbolString	
   displayNameString
   displayCategoryString
   actorType
   caps
   
   [ optional: 
   
   colorIcon 
   BWIcon
   
   ]

*/


/* ----------------------------------------------------------------------------
/
/	IMoaMmXAsset - External media asset interface
/
/ --------------------------------------------------------------------------- */

/* IID_IMoaMmXAsset: AC401E5C0001A85500000800072C6326 */
DEFINE_GUID(IID_IMoaMmXAsset, 0xAC401E5CL, 0x0001, 0xA855, 0x00, 0x00, 0x08,
				0x00, 0x07, 0x2C, 0x63, 0x26);

#undef INTERFACE
#define INTERFACE IMoaMmXAsset

DECLARE_INTERFACE_(IMoaMmXAsset, IMoaMmPropOwner)
{
	STD_IUNKNOWN_METHODS

	/* IMoaMmPropOwner methods */
	
	STDMETHOD(GetProp)(THIS_ 						
			MoaMmSymbol 				symbol,
			PMoaMmValue					pPropValue)
			PURE;
			
	STDMETHOD(SetProp)(THIS_ 
			MoaMmSymbol 				symbol,
			ConstPMoaMmValue			pPropValue)
			PURE;

	/* IMoaMmXAsset methods */

	STDMETHOD(CallFunction) 			(THIS_ 
			MoaMmSymbol 				methodName, 
			MoaLong 	   				nArgs, 
			ConstPMoaMmValue 			pArgs, 
			PMoaMmValue					pResult)
			PURE;
	
	STDMETHOD(SetCallback) 				(THIS_
			PIMoaMmAssetCallback		 pAssetCallback)
			PURE;

	STDMETHOD(Notify) 					(THIS_
			MoaLong						msgCode,
			PMoaVoid					refCon)
			PURE;
			
	STDMETHOD(GetDefaultRect)			(THIS_ 
			PMoaRect					pRect)
			PURE;
			
	STDMETHOD(PrepareNewContent)		(THIS_
			MoaMmNewFlags 				newFlags)
			PURE;
			
	STDMETHOD(StreamOutMedia)			(THIS_
			PIMoaStream 				pOutStream)
			PURE;
			
	STDMETHOD(StreamInMedia)			(THIS_
			PIMoaStream 				pInStream)
			PURE;
			
	STDMETHOD(FreeMedia)				(THIS) 
			PURE;
			
	STDMETHOD(GetInMemMediaSize)		(THIS_ 
			MoaUlong *	 				pSizeBytes)
			PURE;
			
	STDMETHOD(GetStreamOutMediaSize)	(THIS_ 
			MoaUlong *	 				pSizeBytes)
			PURE;
						
	STDMETHOD(StreamOutProps)			(THIS_
			PIMoaStream 				pOutStream)
			PURE;
			
	STDMETHOD(StreamInProps) 			(THIS_
			PIMoaStream 				pInStream)
			PURE;
			
	STDMETHOD(FreeProps) 				(THIS) 
			PURE;
			
	STDMETHOD(GetInMemPropsSize) 		(THIS_ 
			MoaUlong *	 				pSizeBytes)
			PURE;
			
	STDMETHOD(GetStreamOutPropsSize) 	(THIS_ 
			MoaUlong *	 				pSizeBytes)
			PURE;
						
	STDMETHOD(NewXActor) 				(THIS_ 
			PMoaVoid *	 				ppXActor)  	
			PURE;
			
	/* Authoring methods */
	STDMETHOD(InvokeMediaEditor) 		(THIS_						
			PMoaMmModifyFlags			pModifyFlags)					
			PURE;
			
	STDMETHOD(InvokePropsEditor) 		(THIS)						
			PURE;
			
	STDMETHOD(FinishPropsEditor) 		(THIS_	
			MoaBoolParam				commitChanges,					
			PMoaMmModifyFlags			pModifyFlags)						
			PURE;
			
	STDMETHOD(InvokeAboutBox) 			(THIS)						
			PURE;
			
	STDMETHOD(ImageStaticThumb) 		(THIS_						
			PIMoaMmGC					pGC,
			ConstPMoaRect				pImageRect)
			PURE;
			
	STDMETHOD(BeginImageAnimThumb) 		(THIS_					
			PIMoaMmGC					pGC,
			ConstPMoaRect				pImageRect)
			PURE;
			
	STDMETHOD(ServiceImageAnimThumb) 	(THIS_
			PIMoaMmGC					pGC,
			ConstPMoaRect				pImageRect)
			PURE;
			
	STDMETHOD(EndImageAnimThumb) 		(THIS_
			PIMoaMmGC					pGC,
			ConstPMoaRect				pImageRect)
			PURE;
};

typedef IMoaMmXAsset * PIMoaMmXAsset;


/* IID_IMoaMmXImport: 234e2a70-1ad7-11d2-a718-00a0c98b9ff5 */
DEFINE_GUID(IID_IMoaMmXImport, 0x234e2a70, 0x1ad7, 0x11d2, 0xa7, 0x18, 0x00, 0xa0, 0xc9, 0x8b, 0x9f, 0xf5);

#undef INTERFACE
#define INTERFACE IMoaMmXImport

DECLARE_INTERFACE_(IMoaMmXImport, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS

	STDMETHOD(ReadFromStream) (THIS_
		/*IN*/	PIMoaStream2	pStream,
		/*IN*/	PIMoaDict		pOptionsDict,
		/*OUT*/	PIMoaDict *		ppOptionsDict
	) PURE;

	STDMETHOD(ReadFromFile) (THIS_
		/*IN*/	PIMoaPathName	pPathToRead,
		/*IN*/	PIMoaDict		pOptionsDict,
		/*OUT*/	PIMoaDict *		ppOptionsDict
	) PURE;
};

typedef IMoaMmXImport * PIMoaMmXImport;


/* ----------------------------------------------------------------------------
/
/	IMoaMmXSpriteActor - sprite actor interface
/
/ --------------------------------------------------------------------------- */

/* IID_IMoaMmXSpriteActor: AC401E760001AEA700000800072C6326 */
DEFINE_GUID(IID_IMoaMmXSpriteActor, 0xAC401E76L, 0x0001, 0xAEA7, 0x00, 0x00, 0x08, 
				0x00, 0x07, 0x2C, 0x63, 0x26);

#undef INTERFACE
#define INTERFACE IMoaMmXSpriteActor

DECLARE_INTERFACE_(IMoaMmXSpriteActor, IMoaMmPropOwner)
{
	STD_IUNKNOWN_METHODS

	/* IMoaMmPropOwner methods */
	
	STDMETHOD(GetProp) 					(THIS_ 						
			MoaMmSymbol 				symbol,
			PMoaMmValue					pPropValue)
			PURE;
			
	STDMETHOD(SetProp) 					(THIS_ 
			MoaMmSymbol 				symbol,
			ConstPMoaMmValue			pPropValue)
			PURE;
			
	/* Sprite Actor methods */
	
	STDMETHOD( CallFunction ) 			(THIS_ 
			MoaMmSymbol 				methodName, 
			MoaLong 					nArgs, 
			ConstPMoaMmValue			pArgs, 
			PMoaMmValue 				pResult)
			PURE;

	STDMETHOD(SetCallback) 				(THIS_ 
			PIMoaMmSpriteCallback 		pSpriteCallback)
			PURE;
			
	STDMETHOD(Notify) 					(THIS_
			MoaLong						msgCode,
			PMoaVoid					refCon)
			PURE;

	STDMETHOD(GetCaps) 					(THIS_
			PMoaMmXSpriteCaps			pActorCaps)
			PURE;
			
	STDMETHOD(CollectChanges) 			(THIS_ 
			ConstPMoaMmXSpriteInfo 		pActorInfo,
			PIMoaMmChangeCollector 		pChangeCollector)
			PURE;
			
	STDMETHOD(ContainsPoint) 			(THIS_
			ConstPMoaMmXSpriteInfo		pActorInfo,
			ConstPMoaPoint				pTestPoint,
			MoaBoolParam *				pTestResult)
			PURE;

	STDMETHOD(Image) 					(THIS_ 
			ConstPMoaMmXSpriteInfo 		pActorInfo,
			ConstPMoaMmPlayerStateInfo	pPlayerStateInfo,
			PIMoaMmGC			  		pGC)
			PURE;

	STDMETHOD(Event) 					(THIS_ 
			ConstPMoaMmEventInfo		pEventInfo,
			ConstPMoaMmPlayerStateInfo	pPlayerStateInfo,
			MoaBoolParam *	 		  	pHandled)
			PURE;

};

typedef IMoaMmXSpriteActor * PIMoaMmXSpriteActor;


/* ----------------------------------------------------------------------------
/
/	IMoaMmXTransitionActor - transition actor interface
/
/ --------------------------------------------------------------------------- */

/* IID_IMoaMmXTransitionActor: AC401E9C0001B76800000800072C6326 */
DEFINE_GUID(IID_IMoaMmXTransitionActor, 0xAC401E9CL, 0x0001, 0xB768, 0x00, 0x00, 
				0x08, 0x00, 0x07, 0x2C, 0x63, 0x26);

#undef INTERFACE
#define INTERFACE IMoaMmXTransitionActor

DECLARE_INTERFACE_(IMoaMmXTransitionActor, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
				
	STDMETHOD(Notify) (THIS_
			MoaLong						msgCode,
			PMoaVoid					refCon)
			PURE;
						
	STDMETHOD(Cue) (THIS_
			PIMoaMmGC 					pDestGC,
			PIMoaMmGC 					pSrcGC,
			ConstPMoaRect				pRect,
			ConstPMoaMmTransInfo		pTransInfo)
			PURE;
			
	STDMETHOD(Continue) (THIS_
			PIMoaMmGC 					pDestGC,
			PIMoaMmGC 					pSrcGC,
			ConstPMoaRect				pRect,
			ConstPMoaMmTransInfo		pTransInfo,
			MoaBoolParam *	 			pFinished)
			PURE;
			
	STDMETHOD(Finish) (THIS_
			PIMoaMmGC 					pDestGC,
			PIMoaMmGC 					pSrcGC,
			ConstPMoaRect 				pRect,
			ConstPMoaMmTransInfo		pTransInfo)
			PURE;
};

typedef IMoaMmXTransitionActor * PIMoaMmXTransitionActor;


#ifdef	__cplusplus
}
#endif


#endif	/* MMIXASST_H */
