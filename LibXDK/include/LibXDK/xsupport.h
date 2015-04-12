/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_XSupport
#define _H_XSupport

/* ================================================================================

	Purpose:
	
	Provides access to XSupport library. To use XSupport, you must link the
	XSupport.lib into your Xtra. Not that XSupport is not available for win16.
	
=================================================================================== */

///////////////////////////////////////////////////////////////////////////////////
//
// Description
//
///////////////////////////////////////////////////////////////////////////////////

/*
	This file is used to initialize and shutdown the XSupport library:
	try
	{
		XSupport::Initialize( pObj->pCallback, reserveMem, debugFlag );
	} ...
	
	Remember most of these routines THROW CMoaExceptions, you must catch these 
	otherwise you will terminate the host application with a fatal error.

	This file allows access to the pCallback anywhere in your code by calling:
	XSupport::GetCallback().
	
	There is also access to the Symbol Converter.  The Symbol Converter maintains
	the many symbols that MuiDialog and future Xtras use.  It is a static lookup
	table of tokens that can be switched on instead of MoaMmValues.  Almost all
	MuiDialog routines take a TMuiSymbol as their input. The Symbol Converter
	converts between tokens (TMuiSymbols) and MoaSymbols (MoaMmValues).
	See MSymCnvt.h for more info.  Use XSupport::GetSymbolConverter() anywhere
	to gain access to the Symbol Converter.
	
 */

///////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
///////////////////////////////////////////////////////////////////////////////////

#ifndef _H_moaxtra
#include "moaxtra.h"
#endif
#ifndef _H_XMoaExpt
#include "XMoaExpt.h"
#endif

#ifdef WINDOWS
#pragma warning( disable : 4290 )
#endif

///////////////////////////////////////////////////////////////////////////////////
//
// Checks
//
///////////////////////////////////////////////////////////////////////////////////

//#ifndef XDEBUG
//#error "You must define XDEBUG as either 1 for Debug or 0 for Release"
//#endif

#if XDEBUG
#define XDebugMessage(a) 		XSupport::XDebuggerMessage( (a) )
#else
#define XDebugMessage(a)
#endif


///////////////////////////////////////////////////////////////////////////////////
//
// Definitions
//
///////////////////////////////////////////////////////////////////////////////////

typedef enum {
	kXSupportDebugMode_Enable	= TRUE,
	kXSupportDebugMode_Disable	= FALSE
} TXSupportDebugMode;

typedef enum {
	kXSupportClientMode_Xtra	= TRUE,
	kXSupportClientMode_App		= FALSE
} TXSupportAppMode;

typedef class MmSymbolConverter * PMmSymbolConverter;

const size_t kXSupportDefaultReserve = 1024;	// grab 1k reserve memory for when memory is too tight to display exception

class XSupport
{
	public:
#ifdef WINDOWS
		// constructors/destructors for temporary or heap construction
		XSupport( PIMoaCallback pInterface,
				  size_t		reserveMem = kXSupportDefaultReserve,
				  MoaBoolParam	debugMode  = kXSupportDebugMode_Disable,
				  MoaBoolParam	clientMode = kXSupportClientMode_Xtra );
		~XSupport( void );
#endif
#ifdef MACINTOSH
		// constructors/destructors for temporary or heap construction
		XSupport( PIMoaCallback pInterface,
				  size_t		reserveMem	= kXSupportDefaultReserve,
				  MoaBoolParam	debugMode	= kXSupportDebugMode_Disable,
				  MoaBoolParam	clientMode	= kXSupportClientMode_Xtra ) throw( CMoaException );
		~XSupport( void ) throw( CMoaException );
#endif
		// Initializes the Exception Handler, Symbol Converter, and the Helpers
		static MoaError				Initialize( PIMoaCallback pInterface,
												size_t		  reserveMem = kXSupportDefaultReserve,
												MoaBoolParam  debugMode	 = kXSupportDebugMode_Disable,
												MoaBoolParam  clientMode = kXSupportClientMode_Xtra  ) throw();

		// Shuts down the library if the refcount is zero, also clears all statics for re-entry
		static void					Shutdown( void ) throw();
		
		static MoaBoolParam			Initialized() { return (mCallbackInterface != NULL); }

		static MoaBoolParam			GetClientMode() { return mClientMode; }

		static PMmSymbolConverter	GetSymbolConverter( void ) throw( CMoaException );

		static PIMoaCallback		GetCallback( void ) throw( CMoaException );
		static PIMoaCallback		UnSafeGetCallback( void ) throw();

#if XDEBUG
		static void					XDebuggerMessage( const char* format, ... );
#endif

	private:
		static PIMoaCallback		mCallbackInterface; 
		static PMmSymbolConverter	mSymbolConverter;
		static MoaLong				mRefCount;
		static MoaBoolParam			mClientMode;
};

#ifdef WINDOWS
#pragma warning( default : 4290 )
#endif

#endif	// _H_XSupport

// EOF
