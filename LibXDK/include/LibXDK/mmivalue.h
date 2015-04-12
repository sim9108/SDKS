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
		MoaMmValue services interface definitions.
*/

#ifndef	NO_PRAGMA_ONCE
#pragma	once
#endif

#ifndef	MMIVALUE_H
#define	MMIVALUE_H


#include "moaxtra.h"	 	/* MOA shell */
#include "moastdif.h"		/* MOA standard interfaces */
#include "mmtypes.h"		/* multimedia types */


#ifdef	__cplusplus
extern "C" {
#endif


/* ----------------------------------------------------------------------------
/
/	IMoaMmValue - MoaMmValue services interface
/
/ --------------------------------------------------------------------------- */

/* IID_IMoaMmValue: 151A5780-32BD-11d0-8151-00AA005F3C08 */
DEFINE_GUID(IID_IMoaMmValue, 0x151a5780, 0x32bd, 0x11d0, 0x81, 0x51, 0x0, 
								0xaa, 0x0, 0x5f, 0x3c, 0x8);

#undef INTERFACE
#define INTERFACE IMoaMmValue

DECLARE_INTERFACE_(IMoaMmValue, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
		
	/* Symbol dictionary access */
	
	STDMETHOD(StringToSymbol)			(THIS_ 
			ConstPMoaChar 				pString, 
			PMoaMmSymbol 				pSymbol) 
			PURE;
			
	STDMETHOD(SymbolToString)			(THIS_ 
			MoaMmSymbol 				symbol, 
			PMoaChar 					pStringBuf, 
			MoaLong 					bufLen) 
			PURE;
	
	/* Value access */
	
	STDMETHOD(ValueType)				(THIS_ 
			ConstPMoaMmValue 			pValue, 
			MoaMmValueType *			pResult) 
			PURE;
	STDMETHOD(ValueAddRef)				(THIS_ 
			PMoaMmValue 				pValue) 
			PURE;
	STDMETHOD(ValueRelease)				(THIS_ 
			PMoaMmValue 				pValue) 
			PURE;
	
	/* Convert from value */
	
	STDMETHOD(ValueToInteger)			(THIS_ 
			ConstPMoaMmValue 			pValue, 
			MoaLong *					pResult) 
			PURE;
			
	STDMETHOD(ValueToSymbol)			(THIS_ 
			ConstPMoaMmValue 			pValue, 
			PMoaMmSymbol	 			pResult) 
			PURE;
			
	STDMETHOD(ValueToFloat) 			(THIS_ 
			ConstPMoaMmValue 			pValue, 
			MoaDouble *					pResult) 
			PURE;
			
	STDMETHOD(ValueStringLength)		(THIS_ 
			ConstPMoaMmValue 			pValue, 
			MoaLong *					pResult) 
			PURE;
			
	STDMETHOD(ValueToString) 			(THIS_ 
			ConstPMoaMmValue 			pValue, 
			PMoaChar 					pStringBuf, 
			MoaLong 					bufLen) 
			PURE;
			
	STDMETHOD(ValueToStringPtr)			(THIS_ 
			ConstPMoaMmValue 			pValue, 
			ConstPMoaChar *				pResult)
			PURE;
			
	STDMETHOD(ValueReleaseStringPtr)	(THIS_ 
			ConstPMoaMmValue			pValue) 
			PURE;
			
	STDMETHOD(ValueToPoint) 			(THIS_
			 ConstPMoaMmValue 			pValue, 
			 PMoaPoint 					pResult) 
			 PURE;
			 
	STDMETHOD(ValueToRect) 				(THIS_ 
			ConstPMoaMmValue 			pValue, 
			PMoaRect 					pResult) 
			PURE;

	/* Convert to value */
	
	STDMETHOD(IntegerToValue) 			(THIS_ 
			MoaLong 					num, 
			PMoaMmValue 				pValue) 
			PURE;
			
	STDMETHOD(SymbolToValue) 			(THIS_ 
			MoaMmSymbol 				symbol, 
			PMoaMmValue 				pValue) 
			PURE;
			
	STDMETHOD(FloatToValue) 			(THIS_ 
			MoaDouble 					fnum, 
			PMoaMmValue 				pValue) 
			PURE;
			
	STDMETHOD(StringToValue) 			(THIS_ 
			ConstPMoaChar 				pString, 
			PMoaMmValue 				pValue) 
			PURE;
			
	STDMETHOD(StringHandleToValue)		(THIS_ 
			MoaHandle 					hString, 
			PMoaMmValue 				pValue) 
			PURE;
			
	STDMETHOD(PointToValue) 			(THIS_ 
			ConstPMoaPoint 				pPoint, 
			PMoaMmValue 				pValue) 
			PURE;
			
	STDMETHOD(RectToValue) 				(THIS_ 
			ConstPMoaRect 				pRect, 
			PMoaMmValue 				pValue) 
			PURE;
};

typedef IMoaMmValue * PIMoaMmValue;

/* ---------------------------------------------------------------------------- */

#undef INTERFACE


#ifdef	__cplusplus
}
#endif


#endif	/* MMIVALUE_H */
