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
		Macintosh window & event services callback interface definitions.
*/

#ifndef	NO_PRAGMA_ONCE
#pragma	once
#endif

#ifndef	MMIWNDM_H
#define	MMIWNDM_H


#include "moaxtra.h"	 	/* MOA shell */
#include "moastdif.h"		/* MOA standard interfaces */
#include "mmtypes.h"		/* multimedia types */
#include "mmiservc.h"		/* other mmi interfaces we rely on */


#ifdef	__cplusplus
extern "C" {
#endif


/* ----------------------------------------------------------------------------
/
/	IMoaMmWndMac - Macintosh window & event services
/
/ --------------------------------------------------------------------------- */

/* IID_IMoaMmWndMac: D9088160-32BF-11d0-8151-00AA005F3C08 */
DEFINE_GUID(IID_IMoaMmWndMac, 0xd9088160, 0x32bf, 0x11d0, 0x81, 0x51, 0x0, 
								0xaa, 0x0, 0x5f, 0x3c, 0x8);

#undef INTERFACE
#define INTERFACE IMoaMmWndMac

DECLARE_INTERFACE_(IMoaMmWndMac, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
	
	STDMETHOD(MacHandleEvent) 			(THIS_ 
			PMoaVoid 					pEventRecord) 
			PURE;
			
	STDMETHOD(MacPrepareModalDialog)	(THIS_ 
			PMoaMmDialogCookie 			pDialogCookie) 
			PURE;
			
	STDMETHOD(MacModalDialog)			(THIS_ 
			MoaMmDialogCookie 			dialogCookie, 
			PMoaVoid 					filterProcUPP, 
			MoaShort * 					itemHit) 
			PURE;
			
	STDMETHOD(MacUnprepareModalDialog)	(THIS_ 
			MoaMmDialogCookie 			dialogCookie) 
			PURE;
			
	STDMETHOD_(MoaLong, MacAlert)		(THIS_ 
			MoaLong 					alertID, 
			PMoaVoid 					filterProcUPP) 
			PURE;
			
	STDMETHOD(MacRegisterWindow)		(THIS_ 
			PIMoaMmMacEventHandler 		pEventHandler,
			PMoaMmMacWindow 			pWindow, 
			MoaMmRegWindowFlags 		flags, 
			PMoaVoid 					refCon) 
			PURE; 
			
	STDMETHOD(MacUnregisterWindow)		(THIS_
			 PIMoaMmMacEventHandler 	pEventHandler) 
			 PURE; 
			 
	STDMETHOD(MacDragWindow)			(THIS_ 
			PMoaMmMacWindow 			pWindow, 
			PMoaPoint 					pStartPoint, 
			PMoaRect 					pLimitRect) 
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
};

typedef IMoaMmWndMac * PIMoaMmWndMac;


/* ---------------------------------------------------------------------------- */

#undef INTERFACE


#ifdef	__cplusplus
}
#endif


#endif	/* MMIWNDM_H */
