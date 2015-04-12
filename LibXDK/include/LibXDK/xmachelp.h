/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_XMacHelp
#define _H_XMacHelp

/* ================================================================================

	Purpose:
	
	Provides access to XSupport library. To use XSupport, you must link the
	XSupport.lib into your Xtra. Not that XSupport is not available for win16.
	Provides generally useful C++ classes for accessing common toolbox functionality.
	
=================================================================================== */

#ifdef MACINTOSH

///////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
///////////////////////////////////////////////////////////////////////////////////

#include "moaxtra.h"
#include "mmtypes.h"

// MacOS
#include <Events.h>		// for GetKeys
#include <QuickdrawText.h>
#include <Fonts.h>
#include <QDOffscreen.h>
#include <MacWindows.h>


///////////////////////////////////////////////////////////////////////////////////
//
// Defines
//
///////////////////////////////////////////////////////////////////////////////////


#define kMacAverageTextString "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
#define kMacAverageTextStringSize 52



///////////////////////////////////////////////////////////////////////////////////
//
// Drawing Utils
//
///////////////////////////////////////////////////////////////////////////////////

class VisRgnHelper 
{
	public:
		VisRgnHelper( CGrafPtr aPort ) : mRgn( NULL )
		{
			mRgn = NewRgn();
			GetPortVisibleRegion( aPort, mRgn );
		}
		
	VisRgnHelper( WindowRef aWindow ) : mRgn( NULL )
		{
			mRgn = NewRgn();
			GetPortVisibleRegion( GetWindowPort( aWindow ), mRgn );				
		}

		~VisRgnHelper( void )
		{
			DisposeRgn( mRgn );
		}
		
	operator RgnHandle() { return mRgn; }
		
		
	private:
		RgnHandle 	mRgn;
};

// This class can take no arguments which makes it useful for just restoring colors
class SetColorAndRestore 
{
	public:
		// This constructor deals with RGB values
		SetColorAndRestore( RGBColor* newFg = NULL, RGBColor* newBg = NULL )
		{
			// setup, store old colors
			::GetForeColor( &mFg );
			::GetBackColor( &mBg );
			
			// only set the color if they passed it in
			if( newFg )
				::RGBForeColor( newFg );
				
			if( newBg )
				::RGBBackColor( newBg );
		}
		
		// This constructor deals with old quickdraw colors		
		SetColorAndRestore( short indexFore = 0, short indexBack = 0 )
		{
			// setup, store old colors
			::GetForeColor( &mFg );
			::GetBackColor( &mBg );
			
			if( indexFore )
				::ForeColor( indexFore );
			
			if( indexBack )
				::BackColor( indexBack );
		}

		~SetColorAndRestore( void )
		{
			::RGBForeColor( &mFg );
			::RGBBackColor( &mBg );
		}
		
	private:
		Boolean		mSetFg;
		Boolean		mSetBg;
		RGBColor 	mFg;
		RGBColor 	mBg;
};

// these are if you are super lazy!
class SetForeHelper
{
	public:
		SetForeHelper( unsigned short r, unsigned short g, unsigned short b )
		{
			::GetForeColor( &mFg );
			RGBColor aColor = { r, g, b };
			::RGBForeColor( &aColor );		
		}

		~SetForeHelper( void )
		{
			::RGBForeColor( &mFg );
		}

	private:
		RGBColor 	mFg;	
};


class SetBackHelper
{
	public:
		SetBackHelper( unsigned short r, unsigned short g, unsigned short b )
		{
			::GetBackColor( &mBg );
			RGBColor aColor = { r, g, b };
			::RGBBackColor( &aColor );		
		}

		~SetBackHelper( void )
		{
			::RGBBackColor( &mBg );
		}

	private:
		RGBColor 	mBg;	
};



class SetPortAndRestore 
{
	public:
		SetPortAndRestore( WindowPtr newWindow = NULL ) : mRestorePort( NULL )
		{
			// setup
			::GetPort( &mRestorePort );
			
			// only set the port if they passed it in
			if( newWindow )
				::SetPortWindowPort( newWindow );
		}

		SetPortAndRestore( CGrafPtr newPort = NULL ) : mRestorePort( NULL )
		{
			// setup
			::GetPort( &mRestorePort );
			
			// only set the port if they passed it in
			if( newPort )
				::SetPort( (GrafPtr)newPort );
		}
		
		~SetPortAndRestore( void )
		{
			::SetPort( mRestorePort );
		}
		
	private:
		GrafPtr mRestorePort;
};

class SetClipAndRestore 
{
	public:
		SetClipAndRestore( Rect *newClipRectP = NULL ) : mSavedClipRgn( NULL )
		{
			// setup
			mSavedClipRgn = NewRgn();
			
			if (mSavedClipRgn)
				::GetClip(mSavedClipRgn);
			
			// only set the color if they passed it in
			if( newClipRectP )
				::ClipRect( newClipRectP );
				
		}
		
		~SetClipAndRestore( void )
		{
			if (mSavedClipRgn)
			{
				SetClip(mSavedClipRgn);
				DisposeRgn(mSavedClipRgn);
			}
		}
		
	private:
		RgnHandle mSavedClipRgn;
};

class SetOriginAndRestore 
{
	public:
		SetOriginAndRestore( int h, int v )
		{
			CGrafPtr currPortP;
			Rect	 portRect;
			
			GetPort( (GrafPtr*)&currPortP );
			GetPortBounds( currPortP, &portRect );
			// setup
			mSavedOrigin.v = portRect.top;
			mSavedOrigin.h = portRect.left;
			
			::SetOrigin( h, v );
		}
		
		~SetOriginAndRestore( void )
		{
			::SetOrigin( mSavedOrigin.h, mSavedOrigin.v );
		}
		
	private:
		Point mSavedOrigin;
};

class GWorldHelper
{
	public:
		GWorldHelper( GWorldPtr newPort = NULL, GDHandle device = NULL ) : mRestorePort( NULL), mRestoreDevice( NULL )
		{
			// setup
			::GetGWorld( &mRestorePort, &mRestoreDevice );
			
			if( newPort )
				::SetGWorld( newPort, device );	// okay if device is null
		}

		~GWorldHelper( void )
		{
			::SetGWorld( mRestorePort, mRestoreDevice );
		}
		
	private:
		GWorldPtr	mRestorePort;
		GDHandle	mRestoreDevice;
};

class PixelLocker
{
	public:
		PixelLocker( PixMapHandle pixels ) : mPixMap( pixels )
		{
			// lock them pixels
			::LockPixels( mPixMap );
		}

		~PixelLocker( void )
		{
			// unlock them pixels
			::UnlockPixels( mPixMap );
		}
		
		operator PixMapHandle ( void ) { return mPixMap; }
		
	private:
		PixMapHandle	mPixMap;
};



class PensizeHelper {
	public:
		PensizeHelper( short width = 0, short height = 0 )
		{
			// setup
			GetPenState( &mPenState );
			
			// set the pen size
			if( (width != 0) && (height != 0) )
				PenSize( width, height );
			else
				PenNormal();
		}
		
		~PensizeHelper( void )
		{
			SetPenState( &mPenState );
		}
		
	private:
		PenState mPenState;
};


// this guy sets the font and the size and then restores it.
// he can also give you information about that font which makes
// it useful for determining size info about the system font.

// This currently defaults to the system font and system font size
class FontHelper {
	public:
		FontHelper( short font = -1, short ptSize = -1, Style face = 0xFF, short mode = -1 ) :
			mOldFont( 0 ),
			mOldFace( 0 ),
			mOldSize( 0 ),
			mOldMode( 0 )
		{
			// grab the current setting from the active port
			GrafPtr aPort;

			// if there isn't a port your computer must be turned off.			
			::GetPort( &aPort );

			// setup			
			mOldFont = ::GetPortTextFont( aPort );
			mOldFace = ::GetPortTextFace( aPort );
			mOldSize = ::GetPortTextSize( aPort );
			mOldMode = ::GetPortTextMode( aPort );

			// only set the values if they are not the defaults
			if( font != -1 )
				::TextFont( font );
			if( face != 0xFF )
				::TextFace( face );
			if( ptSize != -1 )
				::TextSize( ptSize );
			if( mode != -1 )
				::TextMode( mode );
			
			// if the font is the system/application font set the 
			// size if they didn't specify it already
			if( (font == applFont || font == systemFont) && ptSize == -1 && face == 0xFF && mode == -1 )
			{
				::TextFace( normal );
				::TextSize( 0 );			// default size
			}
			
			// get font info for this font
			::GetFontInfo( &mInfo );
		}
		
		~FontHelper( void )
		{
			::TextFont( mOldFont );
			::TextSize( mOldSize );
			::TextFace( mOldFace );
			::TextMode( mOldMode );
		}
		
		MoaLong GetLineHeight( void )
		{
			return mInfo.ascent + mInfo.descent + mInfo.leading;
		}

		MoaLong GetFontBaseline( void )
		{
			return mInfo.ascent + mInfo.leading/2;	// ??? right formula?
		}
		
		MoaLong GetMaxCharWidth( void )
		{
			return mInfo.widMax;
		}
				
		MoaLong	GetAverageCharWidth( void )
		{
			char* range = kMacAverageTextString;
			short width = TextWidth( range, 0, kMacAverageTextStringSize );
			return (width / 26 + 1) / 2;
		}
		
		
		MoaLong GetDialogBaseUnits( void )
		{
			// calculate base unit for current font face and size
			// this operates like the windows version
			
			// as in the Windows version the HiWord contains the vertical and
			// the LoWord contains the horizontal
			// don't include the leading
			return 	((mInfo.ascent + mInfo.descent) << 16) | ((short)GetAverageCharWidth());
		}
		
		
	private:
		FontInfo	mInfo;
		short 		mOldFont;
		Style 		mOldFace;
		short 		mOldSize;
		short 		mOldMode;		
};


class HandleLocker
{
	public:
		HandleLocker( Handle handleToLock, MoaBool moveHi = FALSE ) :
			mHandle( handleToLock ),
			mState( 0 )
		{
			// grab handle state and lock handle
			if( mHandle )
			{
				mState = HGetState( mHandle );
				if( moveHi )
					HLockHi( mHandle );
				else
					HLock( mHandle );
			}
		}
			
		~HandleLocker( void )
		{
			if( mHandle )
				HSetState( mHandle, mState );
		}
		
	private:
		Handle 	mHandle;
		char	mState;
};

///////////////////////////////////////////////////////////////////////////////////
//
// CMoaMmGCSaver
//
///////////////////////////////////////////////////////////////////////////////////
class CMoaMmGCSaver
{
	public:
		CMoaMmGCSaver(ConstPMoaMmNativeGCInfo pNativeGC, ConstPMoaRect pMoaClipRect,
					   ConstPMoaPoint pMoaOrigin)
		{
			Rect	macClip;
			
			MoaToMacRect(pMoaClipRect, &macClip);

			switch ((pNativeGC)->type)
			{
				case kMoaMmGCType_MacWindow:
					mpPortSaver	  = new SetPortAndRestore(::GetWindowPort((WindowRef)(pNativeGC)->data.mac_window.windowPtr));
					break;
				case kMoaMmGCType_MacGWorld:
					mpPortSaver	  = new SetPortAndRestore( (CGrafPtr)(pNativeGC)->data.mac_window.windowPtr );
					break;
				default:
					mpPortSaver = NULL; //The type is not set. Alarm!..	
					
			}
		
			mpOriginSaver = new SetOriginAndRestore (-((pMoaOrigin)->x), -((pMoaOrigin)->y));
			mpClipSaver	  = new SetClipAndRestore	(&macClip);
			mpColorSaver  = new SetColorAndRestore	((RGBColorPtr) NULL, (RGBColorPtr) NULL);
			mpPenSaver	  = new PensizeHelper		();
			mpFontSaver	  = new FontHelper			();
		}

		~CMoaMmGCSaver()
		{
			delete mpFontSaver;
			delete mpPenSaver;
			delete mpColorSaver;
			delete mpClipSaver;
			delete mpOriginSaver;
			delete mpPortSaver;
		}

	private:
		SetPortAndRestore*		mpPortSaver;
		SetClipAndRestore*		mpClipSaver;
		SetOriginAndRestore*	mpOriginSaver;
		SetColorAndRestore*		mpColorSaver;
		PensizeHelper*			mpPenSaver;
		FontHelper*				mpFontSaver;
};

// straight from Think Reference [GetKeys] 
// compiler may not inline this because of function call...
inline MoaBool	MacIsKeyDown( unsigned short key )
{
	// this is one of those annoying packed pascal thingys
	KeyMap  keyMap;
	
	GetKeys( keyMap );
	return ((((unsigned char*)keyMap)[key >> 3] >> (key & 0x7)) & 0x1);
}


inline MoaBool	MouseMoved( Point oldPoint, Point newPoint, MoaShort threshold = 1 )
{
	if( (newPoint.v >= (oldPoint.v + threshold)) 
	 || (newPoint.v <= (oldPoint.v - threshold))
	 || (newPoint.h >= (oldPoint.h + threshold)) 
	 || (newPoint.h <= (oldPoint.h - threshold)) )
	 	return TRUE;
	 else
	 	return FALSE; 
}

inline MoaLong MacToMoaModifiers(MoaLong macModifiers)
{
	return (((macModifiers & shiftKey)	 ? kMoaMmModifier_Shift   : 0)	|
            ((macModifiers & controlKey) ? kMoaMmModifier_Control : 0)	|
            ((macModifiers & optionKey)	 ? kMoaMmModifier_Option  : 0)	|
            ((macModifiers & cmdKey)	 ? kMoaMmModifier_Command : 0));
}

inline MoaLong MoaToMacModifiers(MoaLong moaModifiers)
{
	return (((moaModifiers & kMoaMmModifier_Shift)	 ? shiftKey	  : 0)	|
            ((moaModifiers & kMoaMmModifier_Control) ? controlKey : 0)	|
            ((moaModifiers & kMoaMmModifier_Option)	 ? optionKey  : 0)	|
            ((moaModifiers & kMoaMmModifier_Command) ? cmdKey	  : 0));
}

const int		kCommandCode	= 55;
const int		kShiftCode		= 56;
const int		kOptionCode		= 58;
const int		kControlCode	= 59;

inline MoaLong MacGetModifierState()
{
	return ((MacIsKeyDown(kShiftCode)	  ?	shiftKey	: 0)	|
			(MacIsKeyDown(kControlCode)	  ?	controlKey	: 0)	|
			(MacIsKeyDown(kOptionCode)    ?	optionKey	: 0)	|
			(MacIsKeyDown(kCommandCode)	  ?	cmdKey		: 0));
}

#endif // MACINTOSH
#endif // _H_XMacHelp

// EOF
