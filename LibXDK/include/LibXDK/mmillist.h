/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_mmillist
#define _H_mmillist

/*
	Purpose:
	
	MM List services callback interface definitions.	
*/


#include "moaxtra.h"	 	/* MOA shell */
#include "moastdif.h"		/* MOA standard interfaces */
#include "mmtypes.h"		/* multimedia types */

/* ----------------------------------------------------------------------------
/
/	IMoaMmLinearList - Linear list value functions
/
/ --------------------------------------------------------------------------- */

/* IID_IMoaMmLinearList: 1399C84C-43E6-11D1-95-19-00-05-9A-20-19-5A */
DEFINE_GUID(IID_IMoaMmLinearList, 0x1399C84CL, 0x43E6, 0x11D1, 0x95, 0x19, 0x00, 0x05, 0x9A, 0x20, 0x19, 0x5A);

#undef INTERFACE
#define INTERFACE IMoaMmLinearList

DECLARE_INTERFACE_(IMoaMmLinearList, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS

	STDMETHOD(NewListValue) 			(THIS_ 
			PMoaMmValue 				pListValue) 
			PURE;

	STDMETHOD_(MoaLong, CountElements) 	(THIS_ 
			ConstPMoaMmValue 			pListValue) 
			PURE;

	STDMETHOD(GetValueByIndex)			(THIS_ 
			ConstPMoaMmValue 			pListValue,
			MoaLong 					index, 
			PMoaMmValue 				pResultValue) 
			PURE;
			
	STDMETHOD(SetValueByIndex)			(THIS_ 
			PMoaMmValue 				pListValue, 
			MoaLong 					index, 
			PMoaMmValue 				pNewValue)
			PURE;

	STDMETHOD(AppendValueToList)		(THIS_ 
			PMoaMmValue 				pListValue, 
			PMoaMmValue 				pElementValue) 
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
			
/* InsertValueIntoListByIndex inserts a value into a linear list at the specified
   index (1-based).  Existing values are moved down the list.*/
	STDMETHOD(InsertValueIntoListByIndex)	(THIS_ 
			PMoaMmValue					pListValue,
			MoaLong 					index, 
			ConstPMoaMmValue			pNewValue) 
			PURE;
			
/* DeleteValueByIndex is the equivalent of the lingo function deleteAt.
   use it to delete values in linear or property lists given their
   index. Returns out of range error if index is greater then size of 
   list. Index is 1 based index into list.*/
	STDMETHOD(DeleteValueByIndex)		(THIS_ 
			PMoaMmValue 				pListValue, 
			MoaLong 					index)
			PURE;

};

typedef IMoaMmLinearList * PIMoaMmLinearList;

#undef INTERFACE

#endif /* _H_mmillist */
