/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_muiservc
#define _H_muiservc

/* --------------------------------------------------------------------------------

	Purpose:
	
	MUI services interface definitions.
	
----------------------------------------------------------------------------------- */


/* on WINDOWS, include windows.h before any moa files */
#ifdef _WINDOWS
	#ifndef _WINDOWS_
	#include <windows.h>
	#endif
#endif

#include "moaxtra.h"	 	/* MOA shell */
#include "moastdif.h"		/* MOA standard interfaces */
#include "mmtypes.h"		/* multimedia types */
#include "mmiservc.h"


/* ----------------------------------------------------------------------------
/
/	IMuiDragInstance - Mui drag instance
/
/ --------------------------------------------------------------------------- */

DEFINE_GUID(IID_IMuiDragInstance, 0xAB05B018L, 0x2534, 0x11D1, 0xA1, 0xD1, 0x00, 
								0x05, 0x9A, 0x20, 0x6D, 0x9E);
																
#undef INTERFACE
#define INTERFACE IMuiDragInstance

DECLARE_INTERFACE_(IMuiDragInstance, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
	
	STDMETHOD_(MoaBool, HasType) 		(THIS_
			MoaMmSymbol					itemType) 
			PURE;
			
	STDMETHOD(GetItemCount)				(THIS_
			MoaLong *					pItemCountOut) 
			PURE;
	
	STDMETHOD(DoesItemHaveType)			(THIS_
			MoaLong						nItemIndex,
			MoaMmSymbol					itemType,
			MoaBool *					pHasTypeOut)
			PURE;
	
	STDMETHOD(GetItemTypeCount)			(THIS_
			MoaLong						itemIndex,
			MoaLong *					pTypeCountOut)
			PURE;
	
	STDMETHOD(GetItemType)				(THIS_
			MoaLong						itemIndex,
			MoaLong						typeIndex,
			PMoaMmSymbol				pTypeOut)
			PURE;
	
	STDMETHOD(GetItemSize)				(THIS_
			MoaLong						itemIndex,
			MoaMmSymbol					itemType,
			MoaLong *					pItemSizeOut)
			PURE;
	
	STDMETHOD(GetItemData)				(THIS_
			MoaLong						itemIndex,
			MoaMmSymbol					itemType,
			PMoaVoid					pDataOut,
			MoaLong *					pSizeInOut)
			PURE;
	
	STDMETHOD(AddItem)					(THIS_
			MoaMmSymbol					itemType,
			ConstPMoaVoid				pItemDataIn,
			MoaLong						itemSize)
			PURE;
};

typedef IMuiDragInstance * PIMuiDragInstance;

/* ---------------------------------------------------------------------------- */

#undef INTERFACE

/* ----------------------------------------------------------------------------
/
/	IMuiControlState - Mui control state
/
/ --------------------------------------------------------------------------- */

/* IID_IMuiControlState: 6A06A3FC-247A-11d1-B989-00059A20195A */
DEFINE_GUID(IID_IMuiControlState, 0x6A06A3FCL, 0x247A, 0x11D1, 0xB9, 0x89,
			0x00, 0x05, 0x9A, 0x20, 0x19, 0x5A);

#undef	INTERFACE
#define INTERFACE IMuiControlState

DECLARE_INTERFACE_(IMuiControlState, IMoaMmPropOwner)
{
	STD_IUNKNOWN_METHODS


	/*
	** Property owner methods
	*/
	STDMETHOD(GetProp)		(THIS_ 				
							MoaMmSymbol 		symbol,
							PMoaMmValue			pPropValue
							)
							PURE;
	
	STDMETHOD(SetProp)		(THIS_ 
							MoaMmSymbol 		symbol,
							ConstPMoaMmValue	pPropValue
							)
							PURE;
	
	
	/*
	** Control State methods
	*/
	STDMETHOD(MergePropList)
							(THIS_
							PMoaMmValue			pAttributeList
							)
							PURE;
	
	STDMETHOD(GetPropList)
							(THIS_
							PMoaMmValue			pAttributeList
							)
							PURE;
	
	STDMETHOD(SetPropList)
							(THIS_
							ConstPMoaMmValue	pAttributeList
							)
							PURE;
	
};

typedef IMuiControlState * PIMuiControlState;

/* ---------------------------------------------------------------------------- */

#undef INTERFACE


/* ----------------------------------------------------------------------------
/
/	IMuiCallback - Mui callback services
/
/ --------------------------------------------------------------------------- */

/* IID_IMuiCallback: 85FCA054-286A-11d1-8D42-00059A20195A */
DEFINE_GUID(IID_IMuiCallback, 0x85FCA054L, 0x286A, 0x11D1, 0x8D, 0x42,
			0x00, 0x05, 0x9A, 0x20, 0x19, 0x5A);

#undef	INTERFACE
#define INTERFACE IMuiCallback

DECLARE_INTERFACE_(IMuiCallback, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS


	STDMETHOD(GetNativeGC) (THIS_
							PMoaMmNativeGCInfo	pGCInfo
							)
							PURE;
			
	STDMETHOD(Invalidate)	(THIS_
							ConstPMoaRect pInvalRect
							)
							PURE;
	
	STDMETHOD(GetBounds)	(THIS_
							PMoaRect	pBoundsRect
							)
							PURE;
	
	STDMETHOD(CreateDragInstance)	(THIS)
									PURE;
									
	STDMETHOD(InstallMouseTracker)	(THIS)
									PURE;
									
};

typedef IMuiCallback * PIMuiCallback;

/* ---------------------------------------------------------------------------- */

#undef INTERFACE


#endif /* _H_muiservc */
