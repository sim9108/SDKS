/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_mmixval
#define _H_mmixval

/* =========================================================================

	Purpose:
	
	Interface for xtra defined lingo object types.  Would have named then XObjects
	instead of XValues, but that name has historical significance.
	
   ========================================================================= */


#include "moaxtra.h"	 	/* MOA shell */
#include "moastdif.h"		/* MOA standard interfaces */
#include "mmtypes.h"		/* multimedia types */
#include "mmiservc.h"
#include "moafile2.h"

#ifndef	NO_PRAGMA_ONCE
#pragma	once
#endif

#ifdef	__cplusplus
extern "C" {
#endif



/* -------------------------------------------------------------------------

	Interface for  IID_IMoaMmXValue

   ------------------------------------------------------------------------- */
/*

// {D9484A62-D855-11d3-A49F-009027765A07}
DEFINE_GUID(<<name>>, 
0xd9484a62, 0xd855, 0x11d3, 0xa4, 0x9f, 0x0, 0x90, 0x27, 0x76, 0x5a, 0x7);

// {D9484A77-D855-11d3-A49F-009027765A07}
IMPLEMENT_OLECREATE(<<class>>, <<external_name>>, 
0xd9484a77, 0xd855, 0x11d3, 0xa4, 0x9f, 0x0, 0x90, 0x27, 0x76, 0x5a, 0x7);

*/

DEFINE_GUID(IID_IMoaMmXValue, 0xd9484a7a, 0xd855, 0x11d3, 0xa4, 0x9f, 0x0, 0x90, 0x27, 0x76, 0x5a, 0x7);

#undef INTERFACE
#define INTERFACE IMoaMmXValue



enum
{
	kMoaMmValueRange_None = 0,
	kMoaMmValueRange_Min,
	kMoaMmValueRange_Max,
	kMoaMmValueRange_MinMax,
	kMoaMmValueRange_List
};

typedef MoaLong	MoaMmValueRangeType;
#define kMaxPropDescription	128

typedef struct MoaMmValueDesc {
	MoaMmSymbol			name;
	MoaMmValueType		type;
	MoaMmValue			defaultVal;
	MoaChar				pDescription[ kMaxPropDescription ];
	MoaMmValueRangeType	range;
	MoaMmValue			rangeValue1;
	MoaMmValue			rangeValue2;
} MoaMmValueDescription, *PMoaMmValueDescription;


DECLARE_INTERFACE_(IMoaMmXValue, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
					
	/* ------------------ Inspection methods 
	/ These methods provide info on the actual properties of the object
	/ The idea is similiar to the getPropDescriptionList handler of a behavior,
	/  allowing inspection via lingo or possible an object inspector UI.
	*/

    /* GetPropCount returns the number of properties implemented by this interface.
	*/
	STDMETHOD(SetData) (THIS_
			PMoaVoid			pDatum)
			PURE;

	STDMETHOD(GetPropCount) (THIS_ 
			MoaLong						*pCount)
			PURE;
	
	/* GetPropDescriptionByIndex returns a description of the <index>'th property
	/   of this value.  Similiar to behavior's getPropertyDescritionList() 
	*/
	STDMETHOD(GetPropDescriptionByIndex) (THIS_ 
			MoaLong						index,
			PMoaMmValueDescription		pDescription)
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
	/ put x.prop1.prop2.prop3
	/ --> i.AccessProp(propName=#prop1,indexCount=0,pIndexValues={}
	/ --> i.AccessProp(propName=#prop2,indexCount=0,pIndexValues={}
	/ --> i.GetProp( propName=#prop3, indexCount=0 )
	/
	/ put x.sel.foo
	/ --> i.AccessProp(propName=#sel, indexCount=0, pIndexValues={} )
	/ --> i.GetProp( propName=foo, indexCount=0 )
	/ put x.sel[33].foo[10]
	/ --> i.AccessProp(propName=#sel, indexCount=1, pIndexValues={33} )
	/ --> i.GetProp( propName=#foo, indexCount=1, pIndexValues={10} )
	/ set x.sel[33].foo[9..11] = "Done!"
	/ --> i.AccessProp(propName=#sel, indexCount=1, pIndexValues={33} )
	/ --> i.SetProp( propName=#foo, indexCount=2, pIndexValues={9,11}, pNewValue="Done!" )
	/AccessProp
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
	/ !!@ Reserved for future use. Director 8.5 does not make these calls.
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

	STDMETHOD(Ilk)	(THIS_
			PMoaMmValue		pArgument,
			PMoaMmValue		pResult)
			PURE;
	
	STDMETHOD(StringRep)	(THIS_
			PMoaMmValue		pStringVal)
			PURE;

	STDMETHOD(SymbolRep)	(THIS_
			PMoaMmSymbol	pSymbol)
			PURE;

	STDMETHOD(IntegerRep) (THIS_
			PMoaLong		pIntVal)
			PURE;

	STDMETHOD(StreamOut) (THIS_
			PIMoaStream2	pStream)
			PURE;

	STDMETHOD(StreamIn)	(THIS_
			PIMoaStream2	pStream)
			PURE;
};


typedef IMoaMmXValue * PIMoaMmXValue;

#undef INTERFACE
/* -------------------------------------------------------------------------

	Interface for  IMoaMmPropReferenceValue

   ------------------------------------------------------------------------- */

DEFINE_GUID(IID_IMoaMmXValueUtils, 0xd9484a60, 0xd855, 0x11d3, 0xa4, 0x9f, 0x0, 0x90, 0x27, 0x76, 0x5a, 0x7);

#undef INTERFACE
#define INTERFACE IMoaMmXValueUtils

DECLARE_INTERFACE_(IMoaMmXValueUtils, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
	
	/* ValueToIXValue 
	/ Convert from a value containing a Lingo wrapper around an IMoaMmXValue, to
	/ the actual interface.
	*/
	STDMETHOD(ValueToIXValue) (THIS_ 
			ConstPMoaMmValue	pValue,
			PIMoaMmXValue		*ppPropRef)
			PURE;

	/* IXValueToValue
	/ Converts an IMoaMmXValue to a Lingo value, the caller
	/ must Release when done with it.
	*/
	STDMETHOD(IXValueToValue) (THIS_ 
			PIMoaMmXValue	pPropRef,
			PMoaMmValue		pValue)
			PURE;

	STDMETHOD_(MoaLong, valueIsIXValue)  (THIS_
			PMoaMmValue)
			PURE;
};


typedef IMoaMmXValueUtils * PIMoaMmXValueUtils;

#undef INTERFACE


#ifdef	__cplusplus
}
#endif

#endif	/* _H_mmipref */









