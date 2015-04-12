/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_muiview
#define _H_muiview

/* --------------------------------------------------------------------------------

	Purpose:
	
	MUI view interface definitions.
	
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
#include "muiservc.h"		/* MUI services */


/* ----------------------------------------------------------------------------
/
/	Constants
/
/ -------------------------------------------------------------------------- */

/* Default Size:  use as value for get size to specify defaults. */
#define	kMui_DefaultSize					(-1)


/* values for IMuiMouseTracker methods */
#define kMuiTracker_StopTracking		FALSE
#define kMuiTracker_ContinueTracking	TRUE


/* ----------------------------------------------------------------------------
/
/	IMuiView - Mui view interface
/
/ --------------------------------------------------------------------------- */

DEFINE_GUID(IID_IMuiView, 0x1CC95CECL, 0x2060, 0x11D1, 0xA1, 0xD4, 0x00, 
								0x05, 0x9A, 0x20, 0x6D, 0x9E);
								
#undef INTERFACE
#define INTERFACE IMuiView

DECLARE_INTERFACE_(IMuiView, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
	
	STDMETHOD(DrawSelf) 				(THIS_
			ConstPMoaRect				pUpdateRect) 
			PURE;
			
	STDMETHOD(OnSizeChanged)			(THIS_ 
			MoaCoord					cOldWidth,
			MoaCoord					cOldHeight,
			MoaCoord					cNewWidth,
			MoaCoord					cNewHeight) 
			PURE;
	
	STDMETHOD(OnLocationChanged)		(THIS_
			ConstPMoaPoint				pNewLocation)
			PURE;
	
	STDMETHOD(OnVisibilityChange)		(THIS_
			MoaBoolParam				bNewVisibility)
			PURE;
	
	STDMETHOD(GetPreferredSize)			(THIS_
			MoaCoord *					pPreferredWidth,
			MoaCoord *					pPreferredHeight)
			PURE;
	
	STDMETHOD(GetMinimumSize)			(THIS_
			MoaCoord *					pMinimumWidth,
			MoaCoord *					pMinimumHeight)
			PURE;
	
	STDMETHOD(GetMaximumSize)			(THIS_
			MoaCoord *					pMaximumWidth,
			MoaCoord *					pMaximumHeight)
			PURE;

	STDMETHOD(SetCallback)				(THIS_
			PIMuiCallback				pMuiCallback,
			PIMoaNotificationClient		pNotification,
			PMoaVoid					clientDataReference)
			PURE;
};

typedef IMuiView * PIMuiView;


/* ---------------------------------------------------------------------------- */

#undef INTERFACE


/* ----------------------------------------------------------------------------
/
/	IMuiKeyboardHandler - Mui keyboard handler interface
/
/ --------------------------------------------------------------------------- */

DEFINE_GUID(IID_IMuiKeyboardHandler, 0xEAB352A8L, 0x205F, 0x11D1, 0xA1, 0xD4, 0x00, 
								0x05, 0x9A, 0x20, 0x6D, 0x9E);								
#undef INTERFACE
#define INTERFACE IMuiKeyboardHandler

DECLARE_INTERFACE_(IMuiKeyboardHandler, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
	
	STDMETHOD(OnTakeFocus) 				(THIS) 
			PURE;
	
	STDMETHOD(OnLoseFocus) 				(THIS) 
			PURE;
			
	STDMETHOD(OnKeyDown)				(THIS_
			ConstPMoaMmKeyInfo			pKeyInfo,
			MoaBoolParam *				pbHandled) 
			PURE;
	
	STDMETHOD(DoInsertBuffer)			(THIS_
			ConstPMoaChar				pBuffer,
			MoaLong						nBufferSize,
			MoaBoolParam *				pbHandled)
			PURE;
};

typedef IMuiKeyboardHandler * PIMuiKeyboardHandler;


/* ---------------------------------------------------------------------------- */

#undef INTERFACE



/* ----------------------------------------------------------------------------
/
/	IMuiMouseHandler - Mui mouse handler interface
/
/ --------------------------------------------------------------------------- */

DEFINE_GUID(IID_IMuiMouseHandler, 0x0732E9FCL, 0x2060, 0x11D1, 0xA1, 0xD4, 0x00, 
								0x05, 0x9A, 0x20, 0x6D, 0x9E);		
														
#undef INTERFACE
#define INTERFACE IMuiMouseHandler

DECLARE_INTERFACE_(IMuiMouseHandler, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
			
	STDMETHOD(OnMouseDown)				(THIS_ 
			ConstPMoaPoint				pMousePt,
			MoaLong						modifiers,
			MoaBoolParam *				pbHandled) 
			PURE;
			
	STDMETHOD(OnMouseUp)				(THIS_ 
			ConstPMoaPoint				pMousePt,
			MoaLong						modifiers,
			MoaBoolParam *				pbHandled) 
			PURE;
			
	STDMETHOD(OnRightMouseDown)			(THIS_ 
			ConstPMoaPoint				pMousePt,
			MoaLong						modifiers,
			MoaBoolParam *				pbHandled) 
			PURE;
	
	STDMETHOD(OnRightMouseUp)			(THIS_
			ConstPMoaPoint				pMousePt,
			MoaLong						modifiers,
			MoaBoolParam *				pbHandled)
			PURE;
	
	STDMETHOD(OnMouseWheel)				(THIS_
			ConstPMoaPoint				pMousePt,
			MoaLong						modifiers,
			MoaLong						zDelta,
			MoaBoolParam *				pbHandled)
			PURE;
					
	STDMETHOD(OnDoubleClick)			(THIS_ 
			ConstPMoaPoint				pMousePt,
			MoaLong						modifiers,
			MoaBoolParam *				pbHandled) 
			PURE;
			
	STDMETHOD(OnMouseEnter)				(THIS_ 
			ConstPMoaPoint				pMousePt,
			MoaLong						modifiers,
			MoaBoolParam *				pbHandled) 
			PURE;
			
	STDMETHOD(OnMouseOver)				(THIS_ 
			ConstPMoaPoint				pMousePt,
			MoaLong						modifiers,
			MoaBoolParam *				pbHandled) 
			PURE;
			
	STDMETHOD(OnMouseLeave)				(THIS_ 
			ConstPMoaPoint				pMousePt,
			MoaLong						modifiers,
			MoaBoolParam *				pbHandled) 
			PURE;
};

typedef IMuiMouseHandler * PIMuiMouseHandler;


/* ---------------------------------------------------------------------------- */

#undef INTERFACE


/* ----------------------------------------------------------------------------
/
/	IMuiDragHandler - Drag handler interface
/
/ --------------------------------------------------------------------------- */

DEFINE_GUID(IID_IMuiDragHandler, 0x4A53AE06L, 0x2533, 0x11D1, 0xA1, 0xD1, 0x00, 
								0x05, 0x9A, 0x20, 0x6D, 0x9E);		
														
#undef INTERFACE
#define INTERFACE IMuiDragHandler

DECLARE_INTERFACE_(IMuiDragHandler, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
			
	STDMETHOD(OnDragOk)					(THIS_ 
			ConstPMoaPoint				pMousePt,
			MoaLong						modifiers,
			PIMuiDragInstance			pDragObject,
			MoaBoolParam *				pbHandled) 
			PURE;
			
	STDMETHOD(OnDragEnter)				(THIS_ 
			ConstPMoaPoint				pMousePt,
			MoaLong						modifiers,
			PIMuiDragInstance			pDragObject) 
			PURE;
			
	STDMETHOD(OnDragContinue)			(THIS_ 
			ConstPMoaPoint				pMousePt,
			MoaLong						modifiers,
			PIMuiDragInstance			pDragObject) 
			PURE;
			
	STDMETHOD(OnDragExit)				(THIS_ 
			PIMuiDragInstance			pDragObject) 
			PURE;
			
	STDMETHOD(OnDragDrop)				(THIS_ 
			ConstPMoaPoint				pMousePt,
			MoaLong						modifiers,
			PIMuiDragInstance			pDragObject) 
			PURE;
};

typedef IMuiDragHandler * PIMuiDragHandler;


/* ---------------------------------------------------------------------------- */

#undef INTERFACE


/* ----------------------------------------------------------------------------
/
/	IMuiMouseTracker - Mouse tracking interface
/
/ --------------------------------------------------------------------------- */

DEFINE_GUID(IID_IMuiMouseTracker, 0x1CE3F06EL, 0x336D, 0x11D1, 0x9E, 0x27, 0x00,
								0x05, 0x9A, 0x20, 0x19, 0x5A);

#undef INTERFACE
#define INTERFACE IMuiMouseTracker

DECLARE_INTERFACE_(IMuiMouseTracker, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
			
	STDMETHOD(OnTrackBegin)				(THIS_ 
			ConstPMoaPoint				pMousePt,
			MoaLong						modifiers,
			MoaBoolParam *				pbContinueTracking) 
			PURE;
			
	STDMETHOD(OnTrackConstrain)			(THIS_ 
			PMoaPoint					pMousePt,
			MoaLong						modifiers,
			MoaBoolParam *				pbContinueTracking) 
			PURE;
			
	STDMETHOD(OnTrackContinue)			(THIS_ 
			ConstPMoaPoint				pMousePt,
			MoaLong						modifiers,
			MoaBoolParam *				pbContinueTracking) 
			PURE;
			
	STDMETHOD(OnTrackEnd)				(THIS_ 
			ConstPMoaPoint				pMousePt,
			MoaLong						modifiers) 
			PURE;
			
};

typedef IMuiMouseTracker * PIMuiMouseTracker;


/* ---------------------------------------------------------------------------- */

#undef INTERFACE


/* ----------------------------------------------------------------------------
/
/	IMuiControl - Mui control interface
/
/ --------------------------------------------------------------------------- */

DEFINE_GUID(IID_IMuiControl, 0x7427924CL, 0x206A, 0x11D1, 0xA1, 0xD4, 0x00, 
								0x05, 0x9A, 0x20, 0x6D, 0x9E);		
														
#undef INTERFACE
#define INTERFACE IMuiControl

DECLARE_INTERFACE_(IMuiControl, IMuiView)
{
	/* IMoaUnknown methods */
	STD_IUNKNOWN_METHODS
	
	/* IMuiView methods */	
	STDMETHOD(DrawSelf) 				(THIS_
			ConstPMoaRect				pUpdateRect) 
			PURE;
			
	STDMETHOD(OnSizeChanged)			(THIS_ 
			MoaCoord					cOldWidth,
			MoaCoord					cOldHeight,
			MoaCoord					cNewWidth,
			MoaCoord					cNewHeight) 
			PURE;
	
	STDMETHOD(OnLocationChanged)		(THIS_
			ConstPMoaPoint				pNewLocation)
			PURE;
	
	STDMETHOD(OnVisibilityChange)		(THIS_
			MoaBoolParam				bNewVisibility)
			PURE;
	
	STDMETHOD(GetPreferredSize)			(THIS_
			MoaCoord *					pPreferredWidth,
			MoaCoord *					pPreferredHeight)
			PURE;
	
	STDMETHOD(GetMinimumSize)			(THIS_
			MoaCoord *					pMinimumWidth,
			MoaCoord *					pMinimumHeight)
			PURE;
	
	STDMETHOD(GetMaximumSize)			(THIS_
			MoaCoord *					pMaximumWidth,
			MoaCoord *					pMaximumHeight)
			PURE;
	
	STDMETHOD(SetCallback)				(THIS_
			PIMuiCallback				pMuiCallback,
			PIMoaNotificationClient		pNotification,
			PMoaVoid					clientDataReference)
			PURE;

	/* IMuiControl methods */	
	STDMETHOD(InitFromState)			(THIS_
			PIMuiControlState			pState)
			PURE;
				
	STDMETHOD(OnStateChange)			(THIS) 
			PURE;

	STDMETHOD(CallFunction)				(THIS_
			MoaMmSymbol					methodName,
			MoaLong						nArgs,
			ConstPMoaMmValue			pArgs,
			PMoaMmValue 				pResult)
			PURE;
};

typedef IMuiControl * PIMuiControl;


/* ---------------------------------------------------------------------------- */

#undef INTERFACE


#endif /* _H_muiview */
