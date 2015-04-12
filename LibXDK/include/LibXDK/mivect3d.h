/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_mivect3d
#define _H_mivect3d

/*

	Purpose:
	
	3d vector <---> Lingo value conversion
	
*/


#include "moaxtra.h"	 	/* MOA shell */
#include "moastdif.h"		/* MOA standard interfaces */
#include "mmtypes.h"		/* multimedia types */
#include "mmiservc.h"
#include "m3dtypes.h"


#ifndef	NO_PRAGMA_ONCE
#pragma	once
#endif

#ifdef	__cplusplus
extern "C" {
#endif


/* -------------------------------------------------------------------------

	Interface for  converting MoaMmValues to/from Moa3dVectors and Moa3dTransforms

   ------------------------------------------------------------------------- */

DEFINE_GUID(IID_IMoa3dVectorValueUtils, 0xd9484a78, 0xd855, 0x11d3, 0xa4, 0x9f, 0x0, 0x90, 0x27, 0x76, 0x5a, 0x7);

#undef INTERFACE
#define INTERFACE IMoa3dVectorValueUtils

DECLARE_INTERFACE_(IMoa3dVectorValueUtils, IMoaUnknown)
{ 
	STD_IUNKNOWN_METHODS
	
	/* ValueToMoa3dVector 
	/   Convert from a value into a Moa3dVector
	*/
	STDMETHOD(ValueToMoa3dVector) (THIS_ 
			ConstPMoaMmValue	pValue,
			Moa3dVector			pVector)
			PURE;

	/* Moa3dVectorToValue
	/ Converts a Moa3dVector into a Lingo value, the caller
	/ must Release when done with it.
	*/
	STDMETHOD(Moa3dVectorToValue) (THIS_ 
			Moa3dVector		pVector,
			PMoaMmValue		pValue)
			PURE;

	/* Like ValueToMoa3dVector, except that it returns a ptr to the 
	/  Moa3dVector this value represents, rather than a copy.  Locks any
	/  necessary handles.  Caller is responsible for calling ValueReleaseVectorPtr
	/  when they are done with it */
	STDMETHOD(ValueToMoa3dVectorPtr) (THIS_ 
			ConstPMoaMmValue pValue, 
			Moa3dVector **ppVector)
			PURE;

	/* Unlocks any handle associated with a previous call to ValueToMoa3dVectorPtr.
	/  Should be preceeded by a call to ValueToMoa3dVectorPtr */
	STDMETHOD(ValueReleaseVectorPtr) (THIS_ 
			ConstPMoaMmValue pValue, 
			Moa3dVector **ppVector)
			PURE;

	/* Returns 1 if the value represents a moa3dVector, 0 if not */
	STDMETHOD(ValueIsAVector) (THIS_ 
			ConstPMoaMmValue pValue )
			PURE;

	/* ValueToMoa3dMatrix 
	/   Convert from a value into a Moa3dMatrix
	*/
	STDMETHOD(ValueToMoa3dMatrix) (THIS_ 
			ConstPMoaMmValue	pValue,
			Moa3dMatrix			pMatrix)
			PURE;

	/* Moa3dMatrixToValue
	/ Converts a Moa3dVector into a Lingo value, the caller
	/ must Release when done with it.  First implemented in 8.5.1
	*/
	STDMETHOD(Moa3dMatrixToValue) (THIS_ 
			Moa3dMatrix		pMatrix,
			PMoaMmValue		pValue)
			PURE;

	/*  
	/  Not implemented */
	STDMETHOD(Method2) (THIS_ 
			PMoaVoid, 
			PMoaVoid)
			PURE;

	/*  
	/  Not implemented */
	STDMETHOD(Method3) (THIS_ 
			PMoaVoid, 
			PMoaVoid)
			PURE;

	/* Returns 1 if the value represents a moa3dMatrix, 0 if not */
	STDMETHOD(ValueIsATransform) (THIS_ 
			ConstPMoaMmValue pValue )
			PURE;
};


typedef IMoa3dVectorValueUtils * PIMoa3dVectorValueUtils;

#undef INTERFACE

#ifdef	__cplusplus
}
#endif

#endif	/* _H_mivect3d */









