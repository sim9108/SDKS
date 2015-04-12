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
		Windows window & event services callback interface definitions.	
*/

#ifndef	NO_PRAGMA_ONCE
#pragma	once
#endif

#ifndef	MMIWNDW_H
#define	MMIWNDW_H


#include "moaxtra.h"	 	/* MOA shell */
#include "moastdif.h"		/* MOA standard interfaces */
#include "mmtypes.h"		/* multimedia types */


#ifdef	__cplusplus
extern "C" {
#endif


/* ----------------------------------------------------------------------------
/
/	IMoaMmWndWin - Windows window & event services
/
/ --------------------------------------------------------------------------- */

/* IID_IMoaMmWndWin: D9088161-32BF-11d0-8151-00AA005F3C08 */
DEFINE_GUID(IID_IMoaMmWndWin, 0xd9088161, 0x32bf, 0x11d0, 0x81, 0x51, 0x0, 
								0xaa, 0x0, 0x5f, 0x3c, 0x8);

#undef INTERFACE
#define INTERFACE IMoaMmWndWin

DECLARE_INTERFACE_(IMoaMmWndWin, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
	
	STDMETHOD_(MoaLong, WinDialogBox)	(THIS_ 
			XtraResourceCookie 			resCookie,
			 ConstPMoaChar 				lpszTemplate, 
			 PMoaVoid 					dlgprc) 
			 PURE;

	STDMETHOD(WinGetParent)				(THIS_ 
			MoaMmHInst * 				phInst, 
			MoaMmHWnd * 				phWnd) 
			PURE; 
		
	STDMETHOD_(MoaLong, WinDialogBoxParam)	(THIS_ 
			XtraResourceCookie 			resCookie, 
			ConstPMoaChar 				lpszTemplate, 
			PMoaVoid 					dlgprc, 
			MoaLong 					lparamInit) 
			PURE;

	/* Win: Use these to surround other Windows API calls
		invoking modal dialogs.  Not needed if using
		WinDialogBox() or WinDialogBoxParam().
	*/
	
	STDMETHOD(WinPrepareDialogBox)		(THIS_ 
			PMoaMmDialogCookie 			pDialogCookie) 
			PURE;
			
	STDMETHOD(WinUnprepareDialogBox)	(THIS_ 
			MoaMmDialogCookie 			dialogCookie) 
			PURE;

};

typedef IMoaMmWndWin * PIMoaMmWndWin;

/* ---------------------------------------------------------------------------- */

#undef INTERFACE


#ifdef	__cplusplus
}
#endif


#endif	/* MMIWNDW_H */
