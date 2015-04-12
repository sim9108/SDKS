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
		Macromedia Open Architecture - Property Page Interfaces
*/


/*/////////////////////////////////////////////////////////////////////////////////
//
// Description
//
/////////////////////////////////////////////////////////////////////////////////*/



#ifndef	MUIPROPPAGE_H
#define	MUIPROPPAGE_H

/*----------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------*/
#include "moaxtra.h"
#include "moatypes.h"
#include "moastdif.h"
#include "mmipref.h"
#include "mmiservc.h"


#ifdef	__cplusplus
extern "C" {
#endif


/*----------------------------------------------------------------------------
// types
//--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
// Registry settings
//--------------------------------------------------------------------------*/
#define kMuiDictKey_PropPageEntry "VDLPath"
#define kMuiDictKey_PropPageList "VDL"
/*----------------------------------------------------------------------------
// Interfaces
//--------------------------------------------------------------------------*/





// {35F34993-6FB8-11d3-B85E-00902754C68C}
DEFINE_GUID(IID_IMuiPropertyPage,
0x35f34993, 0x6fb8, 0x11d3, 0xb8, 0x5e, 0x0, 0x90, 0x27, 0x54, 0xc6, 0x8c);
// {35F34995-6FB8-11d3-B85E-00902754C68C}
DEFINE_GUID(IID_IMuiPropertyPageTarget,
0x35f34995, 0x6fb8, 0x11d3, 0xb8, 0x5e, 0x0, 0x90, 0x27, 0x54, 0xc6, 0x8c);
// {35F34996-6FB8-11d3-B85E-00902754C68C}
DEFINE_GUID(IID_IMuiPropertyPageInstance,
0x35f34996, 0x6fb8, 0x11d3, 0xb8, 0x5e, 0x0, 0x90, 0x27, 0x54, 0xc6, 0x8c);


#undef INTERFACE
#define INTERFACE IMuiPropertyPageTarget

DECLARE_INTERFACE_(IMuiPropertyPageTarget, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
	
	STDMETHOD(GetTargetCount)			(THIS_
			PMoaLong					pCount)
			PURE;
		
	STDMETHOD(GetNthTarget)				(THIS_
			MoaLong						index,
			PIMoaUnknown*				ppTarget)
			PURE;
	

	STDMETHOD(GetProp)			(THIS_
			MoaMmSymbol					property,
			PMoaMmValue					pResult)
			PURE;

	STDMETHOD(SetProp)			(THIS_
			MoaMmSymbol					property,
			ConstPMoaMmValue			pValue)
			PURE;

};

typedef IMuiPropertyPageTarget * PIMuiPropertyPageTarget;


DECLARE_INTERFACE_(IMuiPropertyPageInstance, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
	
	STDMETHOD(GetControlProp)			(THIS_
			MoaMmSymbol					controlID,
			MoaMmSymbol					property,
			PMoaMmValue					pResult)
			PURE;

	STDMETHOD(SetControlProp)			(THIS_
			MoaMmSymbol					controlID,
			MoaMmSymbol					property,
			ConstPMoaMmValue			pValue)
			PURE;

};

typedef IMuiPropertyPageInstance * PIMuiPropertyPageInstance;




#undef INTERFACE
#define INTERFACE IMuiPropertyPage

DECLARE_INTERFACE_(IMuiPropertyPage, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
	
	
	STDMETHOD(GetDescriptor)			(THIS_
			MoaMmValue*					pResult)
			PURE;

	STDMETHOD_(void,SetTarget)			(THIS_
			PIMuiPropertyPageInstance 	pPageInstance,
			PIMuiPropertyPageTarget 	pTarget)
			PURE;

	STDMETHOD_(MoaBool,ItemClicked)		(THIS_
			MoaMmSymbol 				itemID,
			ConstPMoaMmValue			pItemValue,
			PIMuiPropertyPageInstance 	pPageInstance,
			PIMuiPropertyPageTarget 	pTarget)
			PURE;

	STDMETHOD_(void,UpdateItems)			(THIS_
			PIMuiPropertyPageInstance 	pPageInstance,
			PIMuiPropertyPageTarget 	pTarget)
			PURE;

	STDMETHOD_(void,Idle)			(THIS_
			PIMuiPropertyPageInstance 	pPageInstance,
			PIMuiPropertyPageTarget 	pTarget)
			PURE;
};

typedef IMuiPropertyPage * PIMuiPropertyPage;


#ifdef	__cplusplus
}
#endif

#endif
