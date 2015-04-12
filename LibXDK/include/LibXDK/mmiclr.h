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
		Interface definition for converting between MoaMmValues of
		ValueType() == kMoaMmValueType_Color and MoaMmColorSpec values 
*/

#ifndef	NO_PRAGMA_ONCE
#pragma	once
#endif

#ifndef	MMICLR_H
#define	MMICLR_H


#include "moaxtra.h"	 	/* MOA shell */
#include "moastdif.h"		/* MOA standard interfaces */
#include "mmtypes.h"		/* multimedia types */
#include "mmiservc.h"


#ifdef	__cplusplus
extern "C" {
#endif


/* ----------------------------------------------------------------------------
/
/	IID_IMoaMmColor - MoaMmColorSpec manipulation functions.
/
/ --------------------------------------------------------------------------- */
/* 999
	   f67eacf0-11e7-11d2-9D18-00104b2797f3 
   999
*/

/* IID_IMoaMmUtils2: f36721d0-11e7-11d2-9D18-00104b2797f3 */
DEFINE_GUID(IID_IMoaMmColor, 0xf36721d0, 0x11e7, 0x11d2, 0x9D, 0x18, 0x00, 
							 0x10, 0x4b, 0x27, 0x97, 0xf3);

#undef INTERFACE
#define INTERFACE IMoaMmColor

DECLARE_INTERFACE_(IMoaMmColor, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
	
	STDMETHOD(ValueToColor)				(THIS_
			ConstPMoaMmValue 			pValue,
			PMoaMmColorSpec				pColor)
			PURE;

	STDMETHOD(ColorToValue) 			(THIS_
			ConstPMoaMmColorSpec 		pColor,
			PMoaMmValue 				pValue)
			PURE;

	STDMETHOD(RGBToIndex)				(THIS_ 
			ConstPMoaMmRGBTriple 		pRGBColor, 
			PMoaMmColorIndex 			pIndex) 
			PURE; 
			
	STDMETHOD(IndexToRGB)				(THIS_ 
			MoaMmColorIndex 			index, 
			PMoaMmRGBTriple 			pRGBColor) 
			PURE; 
};

typedef IMoaMmColor * PIMoaMmColor;


/* ---------------------------------------------------------------------------- */

#undef INTERFACE


#ifdef	__cplusplus
}
#endif


#endif	/* MMICLR_H */
