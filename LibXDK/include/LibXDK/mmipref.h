/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_mmipref
#define _H_mmipref

/*

	Purpose:
	
	Property Reference Interface.  
	Used to access indexed properties.

*/


#include "mmiservc.h"		/* multimedia types */


/* -------------------------------------------------------------------------

	Interface for  IMoaMmPropReference

   ------------------------------------------------------------------------- */


DEFINE_GUID(IID_IMoaMmPropertyReference, 0x52514f40, 0x1755, 0x11d2, 0x91, 0x2b, 0x0, 0xa0, 0xc9, 0x5, 0x87, 0xdb);

#undef INTERFACE
#define INTERFACE IMoaMmPropertyReference

DECLARE_INTERFACE_(IMoaMmPropertyReference, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
					
	/* ------------------ Inspection methods 
	/ These methods provide info on the actual properties, as opposed to the values
	/ of the properties, implemented by the
	/ interface. The idea is to support an object inspector that can
	/ query the object for the property supported.
	/ !!@ Reserved for future use. Director does not make these calls.
	*/
	
	/* GetPropCount returns the number of properties implemented by this interface.
	/ !!@ Reserved for future use. Director does not make these calls.
	/ Return: kMoaMmErr_AccessNotSupported
	*/
	STDMETHOD(GetPropCount) (THIS_ 
			MoaLong						*pCount)
			PURE;
	
	/* GetPropNameByIndex returns the symbol name of a given property. 
	/ Index is 1-based.
	/ !!@ Reserved for future use. Director does not make these calls.
	/ Return: kMoaMmErr_AccessNotSupported
	*/
	STDMETHOD(GetPropNameByIndex) (THIS_ 
			MoaLong						index,
			PMoaMmSymbol				pSymbol)
			PURE;

	/* GetPropTypeByIndex returns the type symbol for a property.
	/ !!@ Reserved for future use. Director does not make these calls.
	/ Return: kMoaMmErr_AccessNotSupported
	*/
	STDMETHOD(GetPropTypeByIndex) (THIS_ 
			MoaLong						index,
			PMoaMmValue					pType)
			PURE;
	
	/*  ------------------  Basic Property Access Methods 
	/ Basic property access, getting and setting properties and
	/ index properties.
	*/
	
	/* GetCount returns the number "chunks" for the given propName
	/
	/ Example Calling Sequence:
	/ put x.word.count
	/ --> i.GetCount(propName=#word).
	/
	/ Parameters:
	/	propName = the name of the property to lookup
	/	pCount = return count
	/
	/ Error Returns:
	/	kMoaMmErr_PropertyNotFound - the property is not found.
	/	kMoaMmErr_AccessNotSupported - the property is not countable
	*/
	STDMETHOD(GetCount) (THIS_ 
			MoaMmSymbol					propName,
			MoaLong						*pCount)
			PURE;

	/* GetProp 
	/ GetProp retrieves the value of a property.
	/ A property name, and optionally, index values may be specified.
	/ 
	/ Examples:
	/ put x.foo
	/ --> i.GetProp( propName=foo, indexCount=0 )
	/ put x.foo[10]
	/ --> i.GetProp( propName=#foo, indexCount=1, pIndexValues={10} )
	/ put x.foo[9..11]
	/ --> i.GetProp( propName=#foo, indexCount=2, pIndexValues={9,11} )
	/
	/ Parameters:
	/ selfRef = The property reference object, as a lingo value. Assign to pResult
	/	and AddRef to return the same property reference. Use to support properties that are further
	/	references as opposed to final values.
	/ propName = the property name to lookup
	/ indexCount = number of indices: zero, one, or two
	/ pIndexValues = array of indices
	/ pResult = return value
	/
	/ Error Returns:
	/	kMoaMmErr_PropertyNotFound - the property is not found.
	/	kMoaMmErr_AccessNotSupported - specified style of property access not support
	/ return this if you dont support indexCount > 0, for example
	*/
	STDMETHOD(GetProp) (THIS_ 				
			ConstPMoaMmValue			selfRef,
			MoaMmSymbol 				propName,
			MoaLong						indexCount,
			ConstPMoaMmValue			pIndexValues,
			PMoaMmValue					pResult)
			PURE;
			
	/* SetProp 
	/ SetProp assigns a value to a property, or range with in a property.
	/ A property name, and optionally index values may be specified.
	/ 
	/ Examples:
	/ set x.foo = 99
	/ --> i.SetProp( propName=foo, indexCount=0, pNewValue=99 )
	/ set x.foo[10] = 99
	/ --> i.GetProp( propName=#foo, indexCount=1, pIndexValues={10}, pNewValue=99 )
	/ set x.foo[9..11] = 123
	/ --> i.GetProp( propName=#foo, indexCount=2, pIndexValues={9,11}, pNewValue=123 )
	/
	/ Parameters:
	/ propName = the property name to lookup
	/ indexCount = number of indices: zero, one, or two
	/ pIndexValues = array of indices
	/ pNewValue = new value to assign
	/
	/ Error Returns:
	/	kMoaMmErr_PropertyNotFound - the property is not found.
	/	kMoaMmErr_AccessNotSupported - specified style of property access not support
	/ return this if you dont support indexCount > 0, for example
	/	kMoaMmErr_CannotSetProperty - set not allowed on specified property
	*/
	STDMETHOD(SetProp) (THIS_ 
			MoaMmSymbol 				propName,
			MoaLong						indexCount,
			ConstPMoaMmValue			pIndexValues,
			ConstPMoaMmValue			pNewValue)
			PURE;

	/*  ------------------  Extended Property Access Methods 
	/ The following methods are provided for extended property access.
	/ They allow for cascaded property references and put-into/before/after
	/ functionality.
	/ Return kMoaMmErr_AccessNotSupported if you do not wish to support
	/ these capabilities.
	*/

	/* AccessPropRef 
	/ AccessPropRef sets up a cascaded property reference.
	/ A property name, and optionally index values may be specified.
	/ A cascaded index property reference series will begin and continue
	/ with AccessPropRef calls, up to the last access which will be
	/ either a GetProp, SetProp, or SetContents/Before/After
	/ 
	/ Examples:
	/ put x.sel[33].foo
	/ --> i.AccessProp(propName=#sel, indexCount=1, pIndexValues={33} )
	/ --> i.GetProp( propName=foo, indexCount=0 )
	/ put x.sel[33].foo[10]
	/ --> i.AccessProp(propName=#sel, indexCount=1, pIndexValues={33} )
	/ --> i.GetProp( propName=#foo, indexCount=1, pIndexValues={10} )
	/ set x.sel[33].foo[9..11] = "Done!"
	/ --> i.AccessProp(propName=#sel, indexCount=1, pIndexValues={33} )
	/ --> i.SetProp( propName=#foo, indexCount=2, pIndexValues={9,11}, pNewValue="Done!" )
	/
	/ Parameters:
	/ selfRef = The property reference object, as a lingo value. Assign to pResult
	/	and AddRef to return self.
	/ propName = the property name to lookup
	/ indexCount = number of indices: zero, one, or two
	/ pIndexValues = array of indices
	/ pResult = Optional return value. To Return a new property reference Lingo object
	/ 	use IMoaMmPropReferenceValue::PropReferenceToValue
	/
	/ Error Returns:
	/	kMoaMmErr_PropertyNotFound - the property is not found.
	/	kMoaMmErr_AccessNotSupported - specified style of property access not support
	/ return this if you dont support cascaded references for example
	*/
	STDMETHOD(AccessPropRef) (THIS_ 
			ConstPMoaMmValue			selfRef,
			MoaMmSymbol					propName,
			MoaLong						indexCount,
			ConstPMoaMmValue			pIndexValues,
			PMoaMmValue					pResult)
			PURE;

	/* GetContents
	/ !!@ Reserved for future use. Director does not make these calls.
	/ Return: kMoaMmErr_AccessNotSupported
	*/
	STDMETHOD(GetContents) (THIS_ 				
			ConstPMoaMmValue			selfRef,
			PMoaMmValue					pResult)
			PURE;

	/* SetContents 
	/ Issued when index property reference used in with the put-into command.
	/
	/ Examples:
	/ put "Banana" into x.sel[33].foo
	/ --> i.AccessProp(propName=#sel, indexCount=1, pIndexValues={33} )
	/ --> i.AccessProp(propName=#foo, indexCount=0, pIndexValues={} )
	/ --> i.SetContents( pNewValue="Banana" )
	/
	/ Parameters:
	/ pNewValue = new value to stuff into target.
	/
	/ Error Returns:
	*/
	STDMETHOD(SetContents) (THIS_ 				
			PMoaMmValue			pNewValue)
			PURE;

	/* SetContentsBefore 
	/ Issued when index property reference used in with the put-before command.
	/
	/ Examples:
	/ put "Banana" before x.sel[33].foo
	/ --> i.AccessProp(propName=#sel, indexCount=1, pIndexValues={33} )
	/ --> i.AccessProp(propName=#foo, indexCount=0, pIndexValues={} )
	/ --> i.SetContents( pNewValue="Banana" )
	/
	/ Parameters:
	/ pNewValue = new value to stuff into target.
	/
	/ Error Returns:
	*/
	STDMETHOD(SetContentsBefore) (THIS_ 				
			PMoaMmValue			pNewValue)
			PURE;

	/* SetContentsAfter 
	/ Issued when index property reference used in with the put-after command.
	/
	/ Examples:
	/ put "Banana" before x.sel[33].foo
	/ --> i.AccessProp(propName=#sel, indexCount=1, pIndexValues={33} )
	/ --> i.AccessProp(propName=#foo, indexCount=0, pIndexValues={} )
	/ --> i.SetContents( pNewValue="Banana" )
	/
	/ Parameters:
	/ pNewValue = new value to stuff into target.
	/
	/ Error Returns:
	*/
	STDMETHOD(SetContentsAfter) (THIS_ 				
			PMoaMmValue			pNewValue)
			PURE;
	
	/* CallHandler
	/ CallHandler dispatch a handler call on an property reference.
	/
	/ Examples:
	/ x.sel[1].goDown(99)
	/ --> AccesProp( propName=#sel, indexCount=1, pIndexValues={1})
	/ --> CallHandlers( callPtr->nargs=2, args={instance, 99} )
	/
	/ Error Returns:
	/	kMoaMmErr_FunctionNotFound - if you do not support the handler call.
	*/
	STDMETHOD(CallHandler) (THIS_ 
			PMoaMmCallInfo 				callPtr)
			PURE;
};


typedef IMoaMmPropertyReference * PIMoaMmPropertyReference;


/* -------------------------------------------------------------------------

	Interface for  IMoaMmPropReferenceValue

   ------------------------------------------------------------------------- */


DEFINE_GUID(IID_IMoaMmPropReferenceValue, 
0x7a002b52, 0x1bfe, 0x11d2, 0x91, 0x2d, 0x0, 0xa0, 0xc9, 0x5, 0x87, 0xdb);

#undef INTERFACE
#define INTERFACE IMoaMmPropReferenceValue

DECLARE_INTERFACE_(IMoaMmPropReferenceValue, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
	
	/* ValueToPropReference 
	/ Convert from a value containing a Lingo wrapper around a property reference, to
	/ the actual interface.
	*/
	STDMETHOD(ValueToPropReference) (THIS_ 
			ConstPMoaMmValue			pValue,
			PIMoaMmPropertyReference	*ppPropRef)
			PURE;

	/* PropReferenceToValue
	/ Converts an property reference to a Lingo value, the caller
	/ must Release when done with it.
	*/
	STDMETHOD(PropReferenceToValue) (THIS_ 
			PIMoaMmPropertyReference	pPropRef,
			PMoaMmValue					pValue)
			PURE;
};


typedef IMoaMmPropReferenceValue * PIMoaMmPropReferenceValue;


#endif	/* _H_mmipref */









