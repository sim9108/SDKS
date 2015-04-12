/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_XMmPList
#define _H_XMmPList

/* ================================================================================

	XMmPList.h

	Purpose:
	
	Provides access to XSupport library. To use XSupport, you must link the
	XSupport.lib into your Xtra. Not that XSupport is not available for win16.
	PropHelp is designed to provide generic c++ wrapper objects for manipulating
	property list mmvalues more easily. In addition, this file declares specific 
	objects and methods for manipulating specific types of property lists used
	by MuiDialog via its C or Lingo interfaces.
	
=================================================================================== */

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

///////////////////////////////////////////////////////////////////////////////////
//
// Description
//
///////////////////////////////////////////////////////////////////////////////////

/*
	This file is used create and access MoaMmValues that are property lists.  
	The MmPropListHelper works exactly the same way as the MmValueHelper, see XMmValue.h
	
	For Example:
	MoaMmValue someValue = kVoidMoaMmValueInitializer;	// always initialize to void!
	MmPropListHelper listHelp( someValue, TRUE );		// TRUE tells the helper to 
														// call Release on the MoaMmValue,
														// it defaults to FALSE
	
	// etc. etc...

	See below for the specific PropList helpers and methods.

  Note:
	You can create your own subclasses of the MmPropListHelper. This has been done
	below for some of the standard lists types you deal with as attributes
	of items in MuiDialog, but you can do it for any particular MmList property
	list your dealing with. This will allow you to condense a dozen lines of code
	to add a property to your list to a single method call.

  For example, here is the declaration for a MmPropListHelper descendent which provides
  specific access to the lingo list passed to a call to MuiDialogs "Alert".

  class MuiLingoAlertPropListHelper : public MmPropListHelper
{
// --------------------------------------------------------------------------------
// Construction and Destruction
// --------------------------------------------------------------------------------
	public:
		MuiLingoAlertPropListHelper( MoaMmValue& list, MoaBool release = FALSE ) : 
			MmPropListHelper( list, release )
		{}
				
		// **** #buttons
		TMuiSymbol	GetButtons( void );
		MoaError 	SetButtons( TMuiSymbol buttons );

		// **** #default
		MoaError	GetDefault( MoaLong& value );
		MoaError 	SetDefault( const MoaLong& value );

		// **** #title
		MoaError	GetTitle( MoaMmValue& theValue );
		MoaError 	SetTitle( MoaMmValue& title );

		// **** #message
		MoaError	GetMessage( MoaMmValue& theValue );
		MoaError 	SetMessage( MoaMmValue& message );

		// **** #icon
		TMuiSymbol	GetIcon( void );
		MoaError 	SetIcon( TMuiSymbol icon );
	
		// **** #movable
		MoaError	GetMovable( MoaLong& movable );
		MoaError 	SetMovable( const MoaLong& movable );

};

	And here is the implementation for one of the methods, where
	kMuiAlertLingoMessage is a predefined MmValue with a property
	in it. (#message)

	// **** #message
	MoaError	MuiLingoAlertPropListHelper::GetMessage( MoaMmValue& theValue )		
	{
		MoaError	err = GetPropListItem( kMuiAlertLingoMessage, theValue );
		if( err )
			return err;
		
		return kMoaErr_NoErr;
	}

	MoaError	MuiLingoAlertPropListHelper::SetMessage( MoaMmValue& message )
	{
		return SetPropListItem( kMuiAlertLingoMessage, message );
	}

*/


///////////////////////////////////////////////////////////////////////////////////
//
// Forward declarations
//
///////////////////////////////////////////////////////////////////////////////////


class MmSymbolConverter;
typedef MmSymbolConverter * PMmSymbolConverter;


///////////////////////////////////////////////////////////////////////////////////
//
// MoaPropListHelper Class Declares
//
///////////////////////////////////////////////////////////////////////////////////

class MmPropListHelper
{
// --------------------------------------------------------------------------------
// Constructors / Destructors
// --------------------------------------------------------------------------------
	public:
		MmPropListHelper( MoaMmValue& list, MoaBoolParam release = FALSE );
		~MmPropListHelper( void );

// --------------------------------------------------------------------------------
// Methods
// --------------------------------------------------------------------------------
	public:
		// count elements
		MoaLong	 CountElements(void) const;
		
		// get/set elements

		MoaError GetProp( const MoaMmValue& prop, MoaMmValue& propertyValue ) const;
		MoaError SetProp( const MoaMmValue& prop, const MoaMmValue& propertyValue  );

		MoaError GetProp( TMuiSymbol muiSym, MoaMmValue& propertyValue ) const;
		MoaError SetProp( TMuiSymbol muiSym, const MoaMmValue& propertyValue  );

		MoaError GetPropLong( TMuiSymbol propMuiSym, MoaLong& value );
		MoaError SetPropLong( TMuiSymbol propMuiSym, const MoaLong value );
		
		MoaError GetPropBySymbol( MoaMmSymbol propSym, MoaMmValue& propertyValue ) const;
		MoaError SetPropBySymbol( MoaMmSymbol propSym, const MoaMmValue& propertyValue  );

		// get/set by index
		MoaError GetTupleByIndex( MoaLong propIndex, MoaMmValue& prop, MoaMmValue& propertyValue ) const;
		MoaError SetTupleByIndex( MoaLong propIndex, const MoaMmValue& prop, const MoaMmValue& propertyValue );

		// removal methods for different ways of specifying property name
		MoaError RemoveByPropName( const MoaMmValue& prop );
		MoaError RemoveByPropName( TMuiSymbol muiSym );
		MoaError RemoveByPropSymbol( MoaMmSymbol propSym );

		// removal method by index
		MoaError RemoveByIndex( MoaLong propIndex );

		// removal method by property value
		MoaError RemoveByPropValue( const MoaMmValue& propValue );
		
		// remove ALL
		MoaError RemoveAll ( void );

// --------------------------------------------------------------------------------
// Protected Methods
// --------------------------------------------------------------------------------
	protected:
		MoaError GetProp( TMuiSymbol prop, TMuiSymbol& propertyValue ) const;
		MoaError SetProp( TMuiSymbol prop, TMuiSymbol propertyValue  );

		MoaError GetPropListItem( TMuiSymbol property, MoaMmValue& value ) const;
		MoaError SetPropListItem( TMuiSymbol property, const MoaMmValue& value );
		
		MoaError GetPropListItem( const MoaMmValue& property, MoaMmValue& value ) const;
		MoaError SetPropListItem( const MoaMmValue& property, const MoaMmValue& value );
		void	 ValidateListExistence( void );
		
// --------------------------------------------------------------------------------
// Data
// --------------------------------------------------------------------------------
	protected:
		PIMoaMmListHelper	mListIP;	
		MoaMmValue& 		mPropertyList;
		PMmSymbolConverter	mSymConvert;
		MoaBoolParam		mRelease;
};

/*-------------------------------------------------------------------------*/

#ifdef MACINTOSH
#pragma options align=reset
#endif

#ifdef _WINDOWS
#pragma pack()
#endif


#endif	// _H_XMmPList

// EOF
