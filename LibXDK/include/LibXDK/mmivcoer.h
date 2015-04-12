/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_mmivcoer
#define _H_mmivcoer

/* ================================================================================

	Purpose:
	
	MM Value coercion services callback interface definitions.
	
	This interface allows clients to coerce values from one type to another.
	The range of supported types and coercions between them will vary among Moa host apps.

	However any implementation of IMoaMmValueCoercion is guaranteed to support the following
	types and conversions between them:
		kMoaMmValueType_Integer
		kMoaMmValueType_String
		kMoaMmValueType_Float
		kMoaMmValueType_Any
		
	kMoaMmValueType_Any is a special destination type for value conversions from strings.
	
=================================================================================== */



#include "moaxtra.h"	 	/* MOA shell */
#include "moastdif.h"		/* MOA standard interfaces */
#include "mmtypes.h"		/* multimedia types */

/* ----------------------------------------------------------------------------
/
/	IMoaMmValueCoercion - value coercion functions
/
/ --------------------------------------------------------------------------- */

/* IID_IMoaMmValueCoercion:  */
DEFINE_GUID(IID_IMoaMmValueCoercion, 0x638A3D90L, 0xF6DF, 0x11D0, 0xB4, 0xFA, 0x00, 0x05, 0x9A, 0x20, 0x19, 0x5A);

#undef INTERFACE
#define INTERFACE IMoaMmValueCoercion

DECLARE_INTERFACE_(IMoaMmValueCoercion, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS

/* CoerceValue:
   IN:  ConstPMoaMmValue 	pSrcValue
   IN:  MoaMmValueType 	 	desiredType
   OUT:	PMoaMmValue			pCoercedValue
   
   Attempts to convert the given value to a value of the given type.
   Clients must release the coerced value.
*/		
	STDMETHOD(CoerceValue)				(THIS_ 
			ConstPMoaMmValue			pSrcValue,
			MoaMmValueType				desiredType,
			PMoaMmValue	 				pCoercedValue) 
			PURE;

};

typedef IMoaMmValueCoercion * PIMoaMmValueCoercion;

#undef INTERFACE


#endif /* _H_mmivcoer */
