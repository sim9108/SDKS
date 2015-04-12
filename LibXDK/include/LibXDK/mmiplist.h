/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_mmiplist
#define _H_mmiplist

/*
	Purpose:
	
	MM Property List services callback interface definitions.
*/


#include "moaxtra.h"	 	/* MOA shell */
#include "moastdif.h"		/* MOA standard interfaces */
#include "mmtypes.h"		/* multimedia types */

/* ----------------------------------------------------------------------------
/
/	IMoaMmPropList - list value functions
/
/ --------------------------------------------------------------------------- */

/* IMoaMmPropList: D9864BEE-43E5-11D1-95-19-00-05-9A-20-19-5A */
DEFINE_GUID(IID_IMoaMmPropList, 0xD9864BEEL, 0x43E5, 0x11D1, 0x95, 0x19, 0x00, 0x05, 0x9A, 0x20, 0x19, 0x5A);

#undef INTERFACE
#define INTERFACE IMoaMmPropList

DECLARE_INTERFACE_(IMoaMmPropList, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS

	STDMETHOD(NewPropListValue)			(THIS_ 
			PMoaMmValue 				pListValue) 
			PURE;
			
	STDMETHOD_(MoaLong, CountElements) 	(THIS_ 
			ConstPMoaMmValue 			pListValue) 
			PURE;

	STDMETHOD(GetTupleByIndex)			(THIS_ 
			ConstPMoaMmValue 			pListValue,
			MoaLong 					index, 
			PMoaMmValue 				pPropNameValue,
			PMoaMmValue 				pPropValue) 
			PURE;
			
	STDMETHOD(SetTupleByIndex)			(THIS_ 
			PMoaMmValue 				pListValue, 
			MoaLong 					index, 
			ConstPMoaMmValue			pPropNameValue,
			ConstPMoaMmValue			pNewValue)
			PURE;

	STDMETHOD(AppendValueToPropList) 	(THIS_ 
			PMoaMmValue 				pListValue, 
			ConstPMoaMmValue			pPropNameValue, 
			ConstPMoaMmValue			pPropValue) 
			PURE;
			
	STDMETHOD(GetValueByProperty) 		(THIS_ 
			ConstPMoaMmValue 			pListValue, 
			ConstPMoaMmValue			pPropNameValue,
			PMoaMmValue 				pPropValue) 
			PURE;
			
	STDMETHOD(SetValueByProperty) 		(THIS_ 
			PMoaMmValue 				pListValue, 
			ConstPMoaMmValue			pPropNameValue, 
			ConstPMoaMmValue			pNewValue) 
			PURE;
			
/* GetIndexByValue is the equivalent of the lingo function getPos. Walks
   the list, sets pResultIndex to a 1 based index into the first list
   position which contains an equivalent value. Sets to 0 if value isn’t
   found in list. */		
	STDMETHOD(GetIndexByValue)			(THIS_ 
			ConstPMoaMmValue 			pListValue,
			ConstPMoaMmValue 			pSearchValue,
			MoaLong *	 				pResultIndex) 
			PURE;
			
/* InsertTupleByIndex inserts a value into a prop list at the specified
   index (1-based).  Existing values are moved down the list.*/
	STDMETHOD(InsertTupleByIndex)		(THIS_ 
			PMoaMmValue					pListValue,
			MoaLong 					index, 
			ConstPMoaMmValue			pPropNameValue, 
			ConstPMoaMmValue			pPropValue) 
			PURE;
			
/* DeleteValueByIndex is the equivalent of the lingo function deleteAt.
   use it to delete values in linear or property lists given their
   index. Returns out of range error if index is greater then size of 
   list. Index is 1 based index into list.*/
	STDMETHOD(DeleteValueByIndex)		(THIS_ 
			PMoaMmValue 				pListValue, 
			MoaLong 					index)
			PURE;

/* DeleteValueByProperty is the equivalent of the lingo function
   deleteProp. use it to delete values in property lists given their
   property name. Returns out of range error if property is not found in
   the list. Property is a symbol value.*/
	STDMETHOD(DeleteValueByProperty ) 	(THIS_ 
			PMoaMmValue 				pListValue, 
			ConstPMoaMmValue			pPropNameValue) 
			PURE;

};

typedef IMoaMmPropList * PIMoaMmPropList;

#undef INTERFACE

#endif /* _H_mmiplist */
