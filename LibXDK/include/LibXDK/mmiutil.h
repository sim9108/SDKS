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
		Multimedia utilities callback interface definitions.
*/

#ifndef	NO_PRAGMA_ONCE
#pragma	once
#endif

#ifndef	MMIUTIL_H
#define	MMIUTIL_H


#include "moaxtra.h"	 	/* MOA shell */
#include "moastdif.h"		/* MOA standard interfaces */
#include "mmtypes.h"		/* multimedia types */
#include "mmiservc.h"


#ifdef	__cplusplus
extern "C" {
#endif

/* ----------------------------------------------------------------------------
/
/	IMoaMmUtils2 - shared utility functions
/
/ --------------------------------------------------------------------------- */

/* IID_IMoaMmUtils2: 3F698780-32BE-11d0-8151-00AA005F3C08 */
DEFINE_GUID(IID_IMoaMmUtils2, 0x3f698780, 0x32be, 0x11d0, 0x81, 0x51, 0x0, 
								0xaa, 0x0, 0x5f, 0x3c, 0x8);

#undef INTERFACE
#define INTERFACE IMoaMmUtils2

DECLARE_INTERFACE_(IMoaMmUtils2, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
	
	/* Print to debug (message) window */
	
	STDMETHOD(PrintMessage)				(THIS_ 
			ConstPMoaChar 				pMsg) 
			PURE;
			
	STDMETHOD(PrintMessage1)			(THIS_ 
			ConstPMoaChar 				pMsg, 
			MoaLong 					arg1) 
			PURE;
			
	STDMETHOD(PrintMessage2)			(THIS_ 
			ConstPMoaChar 				pMsg, 
			MoaLong 					arg1, 
			MoaLong 					arg2) 
			PURE;
			
	STDMETHOD(PrintMessage3)			(THIS_ 
			ConstPMoaChar 				pMsg, 
			MoaLong 					arg1, 
			MoaLong 					arg2, 
			MoaLong 					arg3) 
			PURE;
			
	STDMETHOD(PrintMessage4)			(THIS_ 
			ConstPMoaChar 				pMsg, 
			MoaLong 					arg1, 
			MoaLong 					arg2, 
			MoaLong 					arg3, 
			MoaLong 					arg4) 
			PURE;
			

	/* Graphics context */
	
	STDMETHOD(NewGC)					(THIS_ 
			ConstPMoaMmNativeGCInfo 	pNativeGCInfo, 
			PIMoaMmGC * 	 			ppGC) 
			PURE;
	
	/* Misc */
	
	STDMETHOD(RGBToIndex)				(THIS_ 
			ConstPMoaMmRGBTriple 		pRGBColor, 
			PMoaMmColorIndex 			pIndex) 
			PURE; 
			
	STDMETHOD(IndexToRGB)				(THIS_ 
			MoaMmColorIndex 			index, 
			PMoaMmRGBTriple 			pRGBColor) 
			PURE; 
};

typedef IMoaMmUtils2 * PIMoaMmUtils2;


/* ---------------------------------------------------------------------------- */

#undef INTERFACE


#ifdef	__cplusplus
}
#endif


#endif	/* MMIUTIL_H */
