/*
ADOBE SYSTEMS INCORPORATED
Copyright 2006 Ð 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/
#ifndef _H_bmfilters
#define _H_bmfilters

#include "moaxtra.h"
#include "mmixasst.h"
#include "filtercaps.h"


/* ----------------------------------------------------------------------------
/
/	IMoaMmFilterCallback - callback interface for Bitmap Filters
/
/ --------------------------------------------------------------------------- */

#ifdef INTERFACE
#undef INTERFACE
#endif

#define INTERFACE IMoaBitmapFilterCallback

DECLARE_INTERFACE_(IMoaBitmapFilterCallback, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS

	STDMETHOD (NotifySprites) (THIS_ MoaLong	msgCode,PMoaVoid refCon) PURE;	

};
typedef IMoaBitmapFilterCallback * PIMoaBitmapFilterCallback;

/* IID_IMoaBitmapFilterCallback - {B80FFDAF-6D02-425c-8FDC-5EDEF5718F27}*/
DEFINE_STD_GUID(IID_IMoaBitmapFilterCallback, 
0xb80ffdaf, 0x6d02, 0x425c, 0x8f, 0xdc, 0x5e, 0xde, 0xf5, 0x71, 0x8f, 0x27);

#undef INTERFACE

#define INTERFACE IMoaBitmapFilter

//Declare it to be inheriting from IMoaUnknown
DECLARE_INTERFACE_(IMoaBitmapFilter, IMoaUnknown)
{
	//Standard Moa Methods
    STD_IUNKNOWN_METHODS
	//APIs exposed by our filter interface

	//Method to get the value of a property as defined by the MoaMmSymbol parameter
	STDMETHOD (GetProp) (THIS_ MoaMmSymbol, PMoaMmValue) PURE;

	//Method to set the value of a property as defined by the MoaMmSymbol parameter 
	//As passed in the PMoaMmValue param
	STDMETHOD (SetProp) (THIS_ MoaMmSymbol, ConstPMoaMmValue) PURE;

	//Method to apply the filter effect
	//PIMoaMmGc - Pointer to the GC containing the image information
	//const PMoaFilterInfo - Defines the rect where to apply the filter.  It also specifies the scale factor of stage.
    STDMETHOD (RunFilter) (THIS_ PIMoaMmGC, const PMoaFilterInfo) PURE;

	//Method for streaming out properties of the filter, if any
	//The stream interface allows to write the data to an opaque storage object,
	//usually a disk file or a memory block
	STDMETHOD (StreamOutProps) (THIS_ PIMoaStream) PURE;

	//Method for streaming in the properties of the filter, if any
	//The stream interface allows to read the data from an opaque storage object,
	//usually a disk file or a memory block
	STDMETHOD (StreamInProps) (THIS_ PIMoaStream) PURE;

	//informs the host application of the total amount of properties data, in bytes, 
	//streamed out when the application calls your StreamOutProps() method. 
	//host application uses this value to determine the optimal file layout when saving a file 
	//and to test for available memory when streaming out to memory
	STDMETHOD (GetStreamOutPropsSize) (THIS_ PMoaLong) PURE;

	//Optionally implement this method to display the property editor user interface for the filter. 
	//It is assumed that this is a modal dialog, and that this method returns when the user 
	//presses the OK or CANCEL buttons. 
	//However, changes to properties should not be committed until the FinishPropsEditor() method is called
	STDMETHOD (InvokePropsEditor) (THIS ) PURE;

	/*
	//This method is called after the filter's InvokePropsEditor() method has returned 
	//to complete any changes made through that method. 
	//This method informs the that the host application properties editor has been closed 
	//by the user. The boolean parameter (commitChanges) reflects whether the user accepted 
	//or canceled modifications to the Filter's properties. 
	//If accepted,the changes to its properties are finalized
	//if canceled, the properties are retained same as before InvokePropsEditor() was called
	//The PMoaMmModifyFlags represents the property modification state of the filter
	//kMoaMmModifyFlags_None - No properties have changed  
	//kMoaMmModifyFlags_Props - Properties data has been modified  
	*/

	STDMETHOD (FinishPropsEditor) (THIS_ MoaBoolParam, PMoaMmModifyFlags) PURE;

	//This method returns the output rectagle after the filter is applied to any image
	//The first FilterScaleRect param is the pointer to the input FilterScaleRect 
	//the output FilterScaleRect is returned in the second FilterScaleRect param 
	STDMETHOD (GetOutputRect) (THIS_ const PMoaFilterInfo, PMoaFilterInfo) PURE;

	//This method provides scripting support for the filters
	//This method is called by the host application to call the filter functions
	//This allows you to export some arbitrary functions to use the host applications scripting
	//or expression language
	STDMETHOD (CallFunction) (THIS_ MoaMmSymbol, MoaLong, ConstPMoaMmValue, PMoaMmValue) PURE;

	//This allows the filters to publish their capabilities
	STDMETHOD (GetFilterCaps) (THIS_ PMoaBMFilterCaps) PURE;

	//This method is to callback the player whenever a filter is applied.
	STDMETHOD(SetCallback) 				(THIS_
			PIMoaBitmapFilterCallback		 pBitmapFilterCallback)
			PURE;

	//This metod provides communication between filter and player and also between filter and some other Xtras.
	STDMETHOD(Notify) 					(THIS_
			MoaLong						msgCode,
			PMoaVoid					refCon)
			PURE;
};

typedef IMoaBitmapFilter * PIMoaBitmapFilter;
//GUID
// {77985BA1-009D-41c7-A3F8-3202797807C1}
DEFINE_STD_GUID(IID_IMoaBitmapFilter, 
0x77985ba1, 0x9d, 0x41c7, 0xa3, 0xf8, 0x32, 0x2, 0x79, 0x78, 0x7, 0xc1);

#undef INTERFACE

/*Helper calss ColorRatio*/
/*This structure is used for storing color ratio value for GradientControl*/
typedef struct TColorRatio
{
	MoaLong lColor;
	MoaLong lRatio;
} TColorRatio, *PTColorRatio;

#endif //define _H_bmfilters
