/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_XMmList
#define _H_XMmList

/* ================================================================================

	XMmList.h    

	Purpose:
	
	Provides access to XSupport library. To use XSupport, you must link the
	XSupport.lib into your Xtra. Not that XSupport is not available for win16.
	Provides C++ wrappers around generic linear list MmValue manipulation, in addition
	to MuiDialog specific list accessors for attributes lists used by MuiDialog.
	
=================================================================================== */

///////////////////////////////////////////////////////////////////////////////////
//
// Description
//
///////////////////////////////////////////////////////////////////////////////////

/*
	This file is used create and access MoaMmValues that are linear lists.  
	The MmListHelper works exactly the same way as the MmValueHelper, see XMmValue.h
	
	For Example:
	MoaMmValue someValue = kVoidMoaMmValueInitializer;	// always initialize to void!
	MmListHelper listHelp( someValue, TRUE );			// TRUE tells the helper to 
														// call Release on the MoaMmValue,
														// it defaults to FALSE
	
	listHelp.AppendValue( "SomeString" );
	listHelp.AppendValue( (MoaDouble)123.2324 );
	listHelp.AppendValue( (MoaLong)456 );
	
	// extract values
	MoaDouble sd = 0.0f;
	listHelp.GetValue( sd, 2 );
	// sd = 123.2324
	
	// set that double using a string
	listHelp.SetValue( "1009.9991", 2 );
	// sd = 1009.9991 (notice it is still a double)


	See below for the specific List helpers.

  Note:
	You can create your own subclasses of the MmListHelper. This has been done
	below for some of the standard lists types you deal with as attributes
	of items in MuiDialog, but you can do it for any particular MmList your
	dealing with.
*/

///////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
///////////////////////////////////////////////////////////////////////////////////


#include "XMmValue.h"

// Win: Make sure we are using 8 byte alignment (regardless of Host App)
#ifdef _WINDOWS
#pragma pack(8)
#endif

// Mac: Make sure we are using 4 byte alignment (regardless of Host App)
#ifdef MACINTOSH
#pragma options align=power
#endif

// Index is 1 based
class MmListHelper : public MmValueHelper
{
// --------------------------------------------------------------------------------
// Construction & Destruction
// --------------------------------------------------------------------------------
	public:
		MmListHelper( MoaMmValue& list, MoaBoolParam release = FALSE );
		~MmListHelper( void );

/*----------------------------- List Methods -------------------------------*/
	public:
		MoaLong	 CountElements(void) const;
		
		MoaError AppendValue( ConstPMoaChar );
		MoaError SetValue( ConstPMoaChar,	   MoaLong index );
		MoaError GetValue( ConstPMoaChar*,	   MoaLong index );

		MoaError AppendValue( MoaDouble );
		MoaError SetValue( MoaDouble,		   MoaLong index);
		MoaError GetValue( MoaDouble&, 		   MoaLong index );

		MoaError AppendValue( MoaLong );
		MoaError SetValue( MoaLong,			   MoaLong index );
		MoaError GetValue( MoaLong&, 		   MoaLong index );

		MoaError AppendValue( const MoaMmValue& );
		MoaError SetValue( const MoaMmValue& , MoaLong index );
		MoaError GetValue( MoaMmValue&, 	   MoaLong index );

		MoaError AppendValue( MoaBitmap );
		MoaError SetValue( MoaBitmap newBitmap,  MoaLong index );
		MoaError GetValue( MoaBitmap& newBitmap, MoaLong index );
		
		MoaError AppendSymbolValue( MoaMmSymbol );
		MoaError SetSymbolValue( MoaMmSymbol,	MoaLong index );
		MoaError GetSymbolValue( MoaMmSymbol&, 	MoaLong index );

		MoaError RemoveValue( MoaLong index );
		MoaError RemoveAll ( void);
		
/*----------------------------- Misc methods -----------------------------*/
	protected:
		MoaError ValidateListExistence();

/*----------------------------- Data -------------------------------*/
	protected:
		PIMoaMmListHelper	mListIP;	
};


///////////////////////////////////////////////////////////////////////////////////
//
// MoaLinearListHelper Class Implementation
//
///////////////////////////////////////////////////////////////////////////////////

class MoaLinearListHelper : public MmListHelper
{
// --------------------------------------------------------------------------------
// Construction
// --------------------------------------------------------------------------------
	public:
		MoaLinearListHelper( MoaMmValue& list, MoaBoolParam release = FALSE );

/*----------------------------- List Methods, for names of symbols -------------------------------*/
		// add a str's sym to the list, will add to end regardless of whether there's already on there
		MoaError	AppendListSymStr( ConstPMoaChar symbolStr );
		
		// find position of str's sym in the list
		MoaLong 	GetPosListSymStr( ConstPMoaChar symbolStr );

		// determine whether the linearlist contains the str's sym equivalent
		MoaBoolParam ContainsListSymStr( ConstPMoaChar symbolStr );

		// remove the str's sym from the linearlist 
		MoaError	RemoveListSymStr( ConstPMoaChar symbolStr );

		// adds the str's sym to the list if on, else removes it from the list if its there.
		MoaError	SetListSymStr( ConstPMoaChar symbolStr, MoaBoolParam on );
		
/*----------------------------- List Methods, for strings -------------------------------*/
		// add a string to the list, will add to end regardless of whether there's already on there
		MoaError	AppendList( ConstPMoaChar symbolStr );
		
		// find position of string in the list
		MoaLong 	GetPosList( ConstPMoaChar symbolStr );

		// determine whether the linearlist contains the string equivalent
		MoaBoolParam ContainsList( ConstPMoaChar symbolStr );

		// remove the string from the linearlist 
		MoaError	RemoveList( ConstPMoaChar symbolStr );

		// adds the string to the list if on, else removes it from the list if its there.
		MoaError	SetList( ConstPMoaChar symbolStr, MoaBoolParam on );
/*----------------------------- List Methods, for values -------------------------------*/
		// add a value to the list, will add to end regardless of whether there's already on there
		MoaError	AppendList( PMoaMmValue pTheValue );
		
		// find position of value in the list
		MoaLong 	GetPosList( PMoaMmValue pTheValue );

		// determine whether the linearlist contains the value equivalent
		MoaBoolParam ContainsList( PMoaMmValue pTheValue );

		// remove the value from the linearlist 
		MoaError	RemoveList( PMoaMmValue pTheValue );

		// adds the value to the list if on, else removes it from the list if its there.
		MoaError	SetList( PMoaMmValue pTheValue, MoaBoolParam on );
};

///////////////////////////////////////////////////////////////////////////////////
//
// MuiLinearListHelper Class Implementation
//
///////////////////////////////////////////////////////////////////////////////////

class MuiLinearListHelper : public MmListHelper
{
// --------------------------------------------------------------------------------
// Construction
// --------------------------------------------------------------------------------
	public:
		MuiLinearListHelper( MoaMmValue& list, MoaBoolParam release = FALSE );

/*----------------------------- List Methods -------------------------------*/
		// add a muisym to the list, will add to end regardless of whether there's already on there
		MoaError	AppendListMuiSym( const TMuiSymbol symbol );
		
		// find position of muiSymbol in the list
		MoaLong 	GetPosListMuiSym( const TMuiSymbol symbol );

		// determine whether the linearlist contains the muiSym equivalent
		MoaBoolParam ContainsListMuiSym( const TMuiSymbol symbol );

		// remove the muiSym equivalent from the list
		MoaError	RemoveListMuiSym( const TMuiSymbol symbol );

		// adds the muisym to the list if on, else removes it from the list if its there.
		MoaError	SetListMuiSym( const TMuiSymbol symbol, MoaBoolParam on );

};

///////////////////////////////////////////////////////////////////////////////////
//
// MuiLayoutStyleListHelper Class Declares / Implementation
//
///////////////////////////////////////////////////////////////////////////////////

class MuiLayoutStyleListHelper : public MuiLinearListHelper
{
// --------------------------------------------------------------------------------
// Construction
// --------------------------------------------------------------------------------
	public:
		MuiLayoutStyleListHelper( MoaMmValue& list, MoaBoolParam release = FALSE );

		// **** #lockPosition
		MoaBoolParam	GetLockPosition( void );
		MoaError 		SetLockPosition( MoaBoolParam on );

		// **** #lockSize
		MoaBoolParam	GetLockSize( void );
		MoaError 		SetLockSize( MoaBoolParam on );

		// **** #centerH
		MoaBoolParam	GetCenterH( void );
		MoaError 		SetCenterH( MoaBoolParam on );
					
		// **** #right
		MoaBoolParam	GetRight( void );
		MoaError 		SetRight( MoaBoolParam on );
					
		// **** #left
		MoaBoolParam	GetLeft( void );
		MoaError 		SetLeft( MoaBoolParam on );
		
		// **** #centerV
		MoaBoolParam	GetCenterV( void );
		MoaError 		SetCenterV( MoaBoolParam on );
					
		// **** #top
		MoaBoolParam	GetTop( void );
		MoaError 		SetTop( MoaBoolParam on );

		// **** #bottom
		MoaBoolParam	GetBottom( void );
		MoaError 		SetBottom( MoaBoolParam on );

		// **** #minimize
		MoaBoolParam	GetMinimize( void );
		MoaError 		SetMinimize( MoaBoolParam on );
		
		// **** #maxSize
		MoaBoolParam	GetMaxSize( void );
		MoaError 		SetMaxSize( MoaBoolParam on );
};




/*

	The following two helpers, MuiSliderPropHelper, MuiTextStylePropHelper both
	allow you to check and set parameters for the slider and text widgets.
	Specificying FALSE in the Set parameter turns that flag off if found in the list.

*/


class MuiSliderPropHelper : public MuiLinearListHelper
{
// --------------------------------------------------------------------------------
// Construction and Destruction
// --------------------------------------------------------------------------------
	public:
		MuiSliderPropHelper( MoaMmValue& list, MoaBoolParam release = FALSE );

		// #showTicks
		MoaBoolParam	GetShowTicks( void );
		MoaError		SetShowTicks( MoaBoolParam on );

		// #showValue
		MoaBoolParam	GetShowValue( void );
		MoaError		SetShowValue( MoaBoolParam on );
};


class MuiTextStylePropHelper : public MuiLinearListHelper
{
// --------------------------------------------------------------------------------
// Construction and Destruction
// --------------------------------------------------------------------------------
	public:
		MuiTextStylePropHelper( MoaMmValue& list, MoaBoolParam release = FALSE );

		// #normal
		MoaBoolParam	GetNormal( void );
		MoaError		SetNormal( MoaBoolParam on );

		// #bold
		MoaBoolParam	GetBold( void );
		MoaError		SetBold( MoaBoolParam on );
				
		// #italic
		MoaBoolParam	GetItalic( void );
		MoaError		SetItalic( MoaBoolParam on );

		// #underline
		MoaBoolParam	GetUnderline( void );
		MoaError		SetUnderline( MoaBoolParam on );
};

/*-------------------------------------------------------------------------*/

#ifdef MACINTOSH
#pragma options align=reset
#endif

#ifdef _WINDOWS
#pragma pack()
#endif


#endif	// _H_XMmList

// EOF
