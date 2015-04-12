/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_XWinUI
#define _H_XWinUI

/* ================================================================================

	Purpose:
	
	Provides some useful utilities for drawing mui compliant windows ui.
	
=================================================================================== */

///////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
///////////////////////////////////////////////////////////////////////////////////

#ifdef _WINDOWS

#include "XMuiUI.h"

///////////////////////////////////////////////////////////////////////////////////
//
// CWinUI Declaration
//
///////////////////////////////////////////////////////////////////////////////////

// DO NOT INSTATIATE THIS CLASS -- contains some drawing routines
class CWinUI
{
// --------------------------------------------------------------------------------
// Construction and Destruction
// --------------------------------------------------------------------------------
	private:
		CWinUI( void )	{}
		~CWinUI( void ) {}

// --------------------------------------------------------------------------------
// Drawing routines
// --------------------------------------------------------------------------------
	public:
		static void 	DrawFrame(HDC hDC, const RECT&, CMuiUI::FrameStyle frameStyle,
									CMuiUI::FrameOptions frameOptions);
};


#endif  // _WINDOWS
#endif	// _H_XWinUI

// EOF
