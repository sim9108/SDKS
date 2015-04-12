/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_XMuiUI
#define _H_XMuiUI

/* ================================================================================

	Purpose:
	
	Provides some useful utilities for drawing mui compliant ui.
	
=================================================================================== */

///////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
///////////////////////////////////////////////////////////////////////////////////

#include "mmtypes.h"
#include "Mui.h"

///////////////////////////////////////////////////////////////////////////////////
//
// CMuiUI Constants
//
///////////////////////////////////////////////////////////////////////////////////

const short k54PercentGrey		  = (short)0x8888;
const short	k27PercentGrey		  = (short)0xBABA;
const short	k14PercentGrey		  = (short)0xDCDC;	

const short	kMuiWindowBackground  = (short)0xDDDD;
const short	kWhiteColor			  = (short)0xFFFF;
const short	kBlackColor			  = (short)0x0000;

const short	kShadowDark			  = k54PercentGrey;
const short	kShadowLight		  = k27PercentGrey;

const short	kDisabledGrey		  = k54PercentGrey;

///////////////////////////////////////////////////////////////////////////////////
//
// CMuiUI Declares
//
///////////////////////////////////////////////////////////////////////////////////

// DO NOT INSTATIATE THIS CLASS -- contains some drawing routines
class CMuiUI
{
// --------------------------------------------------------------------------------
// Construction and Destruction
// --------------------------------------------------------------------------------
	private:
		CMuiUI( void )	{}
		~CMuiUI( void ) {}

// --------------------------------------------------------------------------------
// Drawing routines
// --------------------------------------------------------------------------------
	public:
		enum FrameStyle {kFlatFrame = 0, kSunkenBevel, kFlatFrameAndSunkenBevel};
		enum FrameOptions {
			kFrameTop			= 1, 
			kFrameLeft			= 2,
			kFrameBottom		= 4,
			kFrameRight			= 8,
			kFrameTopLeft		= (kFrameTop | kFrameLeft),
			kFrameBottomRight	= (kFrameBottom | kFrameRight),
			kFrameAll			= (kFrameTop | kFrameLeft | kFrameBottom | kFrameRight)
		};

		static void 	DrawFrame( ConstPMoaMmNativeGCInfo pNativeGC,
								   const MoaRect& frameRect,
								   CMuiUI::FrameStyle frameStyle, CMuiUI::FrameOptions frameOptions = kFrameAll);
								   
		
		static MoaLong DoMuiAlert( ConstPMoaChar alertText, ConstPMoaChar alertTitle, TMuiAlertButtonType buttons = kMuiAlertButtons_Ok, MoaLong defaultButton = 1, TMuiAlertIcon icon = kMuiAlertIcon_Note );
};


#endif	// _H_XMuiUI

// EOF
