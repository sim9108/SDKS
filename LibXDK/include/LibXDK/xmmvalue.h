/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_XMmValue
#define _H_XMmValue

/* ================================================================================
	
	Purpose:
	
	Provides access to XSupport library. To use XSupport, you must link the
	XSupport.lib into your Xtra. Not that XSupport is not available for win16.
	Provides C++ wrappers to create MmValue containers and access them easily.
	
=================================================================================== */

///////////////////////////////////////////////////////////////////////////////////
//
// Description
//
///////////////////////////////////////////////////////////////////////////////////

/*
	This file is used create and access MoaMmValues.  What makes the MmValueHelper
	useful is that you can request data from it in a type that is different
	than the MoaMmValue's native type and set different data without affecting the
	data type as well. (sounds confusing? check out the example)
	
	For Example:
	MoaMmValue someValue = kVoidMoaMmValueInitializer;	// always initialize to void!
	MmValueHelper valueHelp( someValue, TRUE );			// TRUE tells the helper to 
														// call Release on the MoaMmValue,
														// it defaults to FALSE
	
	// Set the value to a double, because the MoaMmValue is NULL the Helper
	// creates a new MoaMmValue that is a double. That means the internal type
	// is a double.
	MoaDouble someDouble = 120.121234;
	valueHelp.SetValue( someDouble );
	
	// get the value as a string and print it to the message window
	ConstPMoaChar strPtr = NULL;
	valueHelp.GetValue( strPtr );
	IMmUtilsHelper().Use()->PrintMessage1( "%s\n", (MoaLong)strPtr );
	
	// Get the value as a long and print it to message window
	MoaLong someLong = 0;
	valueHelp.GetValue( someLong );
	IMmUtilsHelper().Use()->PrintMessage1( "someLong: %d\n", someLong );

	// set it to a string (note: the type will still be a double)
	valueHelp.SetValue( "12334.2321" );

	// when we go out of scope this MoaMmValue will be invalid, to prevent
	// this leave out the TRUE parameter in the constructor for MmValueHelper.

  A Note on Ownership:
	Please be aware that a value helper will CREATE a value for you. This
	is often a memory allocation. One of two things can happen, if the release
	paramater on construction is TRUE, when the value helper object is destroyed
	it will release the MmValue it contains. If not, you are responsible for being
	aware of when it might have created a value, and releasing it when you don't
	need it anymore. A rule of thumb, if all you calls is "GetXXX" then you get off
	without having to release the value, but if you call "SetXXX" then your setup
	to have to release, if you didn't set Release = TRUE on initialization.

	Finally, MmList and MmPList helpers both descend from the MmValue helper.
	This means that the above warning applies to them as well, with the added
	caveat that leaking lists usually are bigger then single values.
 */


///////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
///////////////////////////////////////////////////////////////////////////////////

#include "MuiHelpr.h"

// Make sure we are using 8 byte alignment (regardless of Host App)
#ifdef _WINDOWS
#pragma pack(8)
#endif

///////////////////////////////////////////////////////////////////////////////////
//
// Defines
//
///////////////////////////////////////////////////////////////////////////////////

// ??? How does one clear a moa value?
#define kClearMoaMmValue( value )	do {(value).ty = 0L; (value).it = 0L;} while (0)
#define kVoidMoaMmValue( value )	do {(value).ty = 2L; (value).it = 0L;} while (0)
#define kNullMoaMmValueInitializer	{0,0}	// this is actually a NULL initializer
#define kVoidMoaMmValueInitializer	{2,0}	// this is how to get a void rather than null value in Director

typedef enum 
{
	kReleaseValue		= TRUE,
	kDontReleaseValue	= FALSE
} TValueReleaseOption;

typedef enum 
{
	kAllowConversion	= TRUE,
	kDontAllowConversion= FALSE
} TValueConversionOption;


///////////////////////////////////////////////////////////////////////////////////
//
// Fwd Declarations
//
///////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
//
// MmValueHelper Declaration
//
///////////////////////////////////////////////////////////////////////////////////

// This class does automatic coersion to strings with most types
class MmValueHelper
{
/*----------------------- Constructors / Destructor -----------------------*/
public:
		MmValueHelper(MoaBoolParam release = kDontReleaseValue);
		MmValueHelper( MoaMmValue& value, MoaBoolParam release = kDontReleaseValue );
		~MmValueHelper( void );

/*--------------------------- Accessor Methods ----------------------------*/
public:
		// whether or not to release MoaMmValue
		void 		 SetRelease( MoaBoolParam rel ) { mRelease = rel; }
		MoaBoolParam GetRelease( void ) const { return mRelease; }

		// use this to force a release and set the value to void
		void	ForceRelease();

		// use this to work on a different value
		void	SetNewValue( MoaMmValue& value );

		// use this get the underlying value's type
		MoaMmValueType GetValueType( void ) const;

/*--------------------------- Public Methods ------------------------------*/
public:
		MoaError SetValue( ConstPMoaChar stringP, MoaMmValueType forceType = kMoaMmValueType_Void );
		MoaError GetValue( ConstPMoaChar* stringPP );

		MoaError SetValue( MoaDouble newDouble, MoaMmValueType forceType = kMoaMmValueType_Void );
		MoaError GetValue( MoaDouble& doubleRepresentation );

		MoaError SetValue( MoaLong newInteger, MoaMmValueType forceType = kMoaMmValueType_Void );
		MoaError GetValue( MoaLong& intRepresentation, MoaBoolParam  conversionOK = kAllowConversion );

		// these two (MoaBitmap/Rect) don't convert to the other types..
		MoaError SetValue( MoaBitmap newBitmap, MoaMmValueType forceType = kMoaMmValueType_Void );
		MoaError GetValue( MoaBitmap& newBitmap );

		MoaError SetValue( const MoaRect& newRect, MoaMmValueType forceType = kMoaMmValueType_Void );
		MoaError GetValue( MoaRect& newRect );

		MoaError SetValue( const MoaPoint& newPoint, MoaMmValueType forceType = kMoaMmValueType_Void );
		MoaError GetValue( MoaPoint& newPoint );
		
		MoaError SetSymbol( MoaMmSymbol newSymbol, MoaMmValueType forceType = kMoaMmValueType_Void );
		MoaError GetSymbol( MoaMmSymbol& symbolRepresentation, MoaBoolParam  conversionOK = kAllowConversion);

/*--------------------------- Protected Methods ------------------------------*/
protected:
		MoaMmValue 	GetValueRef( void );

		MoaError 	CoerceValueToString( PMoaMmValue valueVP, PMoaMmValue stringVP );
		MoaError 	CoerceStringToValue( ConstPMoaChar stringP, PMoaMmValue valueVP,
										 MoaMmValueType forceType = kMoaMmValueType_Void);
		
		MoaError 	CoerceValueToFloat( PMoaMmValue valueVP, PMoaMmValue floatVP );
		MoaError 	CoerceValueToInteger( PMoaMmValue valueVP, PMoaMmValue intgerVP );
		MoaError 	CoerceValueToSymbol( PMoaMmValue valueVP, PMoaMmValue symbolVP );

		void	 	ReleaseValueStringPtr( void );

/*---------------------------  Data Members  ------------------------------*/
protected:
		PIMoaMmValueHelper	mValueIP;
		MoaMmValueType 		mValueType;
		MoaMmValue& 		mValueRef;
		MoaMmValue			mValue;
		PMoaChar			mValueStringP;
		MoaMmValue			mTempV;
		MoaBoolParam		mUseTempValue;
		MoaBoolParam		mRelease;
};
typedef MmValueHelper * PMmValueHelper;


/*-------------------------------------------------------------------------*/

#ifdef _WINDOWS
#pragma pack()
#endif

#endif	// _H_XMmValue
// EOF
