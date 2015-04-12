/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_mixpal
#define _H_mixpal

#include "mixagent.h"
#include "mixpix.h"

typedef	RenderPixelColorSpace *PRenderPixelColorSpace;
typedef	const RenderPixelColorSpace *ConstPRenderPixelColorSpace;

#undef INTERFACE
#define INTERFACE IMoaReceptorPalette
DECLARE_INTERFACE_(IMoaReceptorPalette, IMoaReceptor)
{
	STD_IUNKNOWN_METHODS
	
	STDMETHOD(SetRefCon)(THIS_
		moa_in PMoaVoid refCon
	) PURE;

	STDMETHOD(SetStatus)(THIS_
		moa_in MoaError status
	) PURE;

	STDMETHOD(SetPalette)(THIS_
		moa_in ConstPRenderPixelColorSpace pRenderPixelColorSpace
	) PURE;

	STDMETHOD(EndPalette)(THIS) PURE;
};
typedef IMoaReceptorPalette * PIMoaReceptorPalette;
DEFINE_GUID(IID_IMoaReceptorPalette, 0xbedde900, 0x3cac, 0x11d0, 0xa1, 0x83, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00);

#endif

