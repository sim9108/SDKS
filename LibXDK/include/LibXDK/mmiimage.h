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
		Image object interfaces.
*/

#ifndef NO_PRAGMA_ONCE
#pragma once
#endif

#ifndef MMIIMAGE_H
#define MMIIMAGE_H

#include "moaxtra.h"		/* MOA shell */
#include "moastdif.h"		/* MOA standard interfaces */
#include "mmtypes.h"		/* multimedia types */
#include "mmiservc.h"


#ifdef	__cplusplus
extern "C" {
#endif


typedef struct MoaMmImageInfo
{
	MoaLong iWidth;
	MoaLong iHeight;
	MoaLong iRowBytes;
	MoaLong iTotalDepth;
	MoaLong iAlphaDepth;
	MoaBool bCartesian;

} MoaMmImageInfo, *PMoaMmImageInfo;

/* ----------------------------------------------------------------------------
/
/	IID_IMoaMmImage - MoaMmImage manipulation functions.
/
/ --------------------------------------------------------------------------- */

DEFINE_GUID(IID_IMoaMmImage, 0xf367FFFF, 0x11e7, 0x11d2, 0x9D, 0x18, 0x00, 
							 0x10, 0x4b, 0x27, 0x97, 0xf3);

#undef INTERFACE
#define INTERFACE IMoaMmImage

DECLARE_INTERFACE_(IMoaMmImage, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
	
/*-*-------------------------------------------------------------------------
/ Purpose
/	Does a blit from the source image to a destination image using either
/	a destination quad or rectangle, a source rectangle and various ink
/	parameters.
/
/ Notes
/	Lingo code syntax:
/		x.copyPixels (sourceImage, destRect, sourceRect)
/		x.copyPixels (sourceImage, destQuad, sourceRect)
/		x.copyPixels (sourceImage, destRect, sourceRect, [propList])
/		x.copyPixels (sourceImage, destQuad, sourceRect, [propList])
/		returns 1 on success, 0 if failure.
/
/	Pass NULL for a missing propList.
/
/	The prop list can contain the following items:
/
/		#color			- lingo color object
/		#bgColor		- lingo color object
/		#ink			- lingo ink value
/		#blendLevel 	- lingo blend value
/		#dither 		- non-zero value to dither, zero to not dither
/		#useFastQuads	- non-zero value to use fast quads in quad mode
/		#maskImage		- lingo mask image object for masking
/		#maskOffset 	- lingo point for mask offset.
/--------------------------------------------*/
	STDMETHOD(CopyPixels)		(THIS_ ConstPMoaMmValue pImage, 
		ConstPMoaMmValue pSourceImage, 
		ConstPMoaMmValue pDestRectOrQuad,
		ConstPMoaMmValue pSourceRect,
		ConstPMoaMmValue pPropList) PURE;

/*-*-------------------------------------------------------------------------
/ Purpose
/	Creates a copy of a lingo image object.
/
/ Notes
/	Does not work on mask items.
/--------------------------------------------*/
	STDMETHOD(Duplicate)		(THIS_ ConstPMoaMmValue pImage, 
		PMoaMmValue pResultImage)	PURE;

/*-*-------------------------------------------------------------------------
/ Purpose
/	Creates a cropped image based upon an image object and a rectangle.
/
/ Notes
/	Lingo code syntax:
/		x.crop (rect(a, b, c, d))
/		returns image object on success, 0 if failure.
/--------------------------------------------*/
	STDMETHOD(Crop) 			(THIS_ ConstPMoaMmValue pImage, 
		ConstPMoaMmValue pRect,
		PMoaMmValue pResultImage)	PURE;

/*-*-------------------------------------------------------------------------
/ Purpose
/	Returns a pixel value for a lingo image object as a
/	color object.
/
/ Notes
/	Lingo code syntax:
/		x.getPixel(x, y)
/		returns lingo color object on success, 0 if failure.
/--------------------------------------------*/
	STDMETHOD(GetPixel) 		(THIS_ ConstPMoaMmValue pImage, 
		MoaLong iX, 
		MoaLong iY, 
		PMoaMmValue pResultValue)	PURE;

/*-*-------------------------------------------------------------------------
/ Purpose
/	Sets a pixel of an image using a color object.
/
/ Notes
/	Lingo code syntax:
/		x.setPixel(x, y, colorObject)
/--------------------------------------------*/
	STDMETHOD(SetPixel) 		(THIS_ ConstPMoaMmValue pImage, 
		MoaLong iX, 
		MoaLong iY, 
		ConstPMoaMmValue pPixelValue)	PURE;

/*-*-------------------------------------------------------------------------
/ Purpose
/	Returns a pixel value for a lingo image object as a lingo integer.
/
/ Notes
/	Lingo code syntax:
/		x.getPixel(x, y, #integer)
/		returns lingo integer on success, 0 if failure.
/--------------------------------------------*/
	STDMETHOD(GetPixelInteger)	(THIS_ ConstPMoaMmValue pImage, 
		MoaLong iX, 
		MoaLong iY, 
		PMoaLong piResult)	PURE;

/*-*-------------------------------------------------------------------------
/ Purpose
/	Sets a pixel of an image using an integer value.
/
/ Notes
/	Lingo code syntax:
/		x.setPixel(x, y, rawInteger)
/--------------------------------------------*/
	STDMETHOD(SetPixelInteger)	(THIS_ ConstPMoaMmValue pImage, 
		MoaLong iX, 
		MoaLong iY,
		MoaLong iPixelValue)	PURE;

/*-*-------------------------------------------------------------------------
/ Purpose
/	Performs a drawing operation on a lingo image object.
/
/ Notes
/	Lingo code syntax:
/		x.draw (rect(l, t, r, b), <index or RGB color>, <[propList]>)
/		returns 1 on success, 0 if failure.
/
/	Pass NULL for missing propList.
/
/	The prop list can contain the following items:
/
/		#color			- lingo color object 
/		#lineSize		- line width 
/		#shapeType		- #rect, #roundRect, #oval, #line
/--------------------------------------------*/
	STDMETHOD(Draw) 			(THIS_ ConstPMoaMmValue pImage, 
		ConstPMoaMmValue pRect,
		ConstPMoaMmValue pPropList) PURE;

/*-*-------------------------------------------------------------------------
/ Purpose
/	Draws a filled shape on a lingo color object.
/
/ Notes
/	Lingo code syntax:
/		x.fill (rect(l, t, r, b), <index or RGB color>, <[propList]>)
/		x.fill (pt(a, b), pt(c, d), <index or RGB color>, <[propList]>)
/		x.fill (a, b, c, d, <index or RGB color>, <[propList]>)
/		returns 1 on success, 0 if failure.
/
/	Pass NULL for missing propList.
/
/	The prop list can contain the following items:
/
/		#color			- lingo color object 
/		#bgColor		- lingo color object 
/		#lineSize		- line width 
/		#shapeType		- #rect, #roundRect, #oval
/--------------------------------------------*/
	STDMETHOD(Fill) 			(THIS_ ConstPMoaMmValue pImage, 
		ConstPMoaMmValue pRect,
		ConstPMoaMmValue pPropList) PURE;

/*-*-------------------------------------------------------------------------
/ Purpose
/	Creates a matte image based upon an image object.
/
/ Notes
/	Lingo code syntax:
/		x.createMatte (<alpha threshold to use>)
/		returns image object on success, 0 if failure.
/--------------------------------------------*/
	STDMETHOD(CreateMatte)		(THIS_ ConstPMoaMmValue pImage, 
		PMoaMmValue pResultImage)	PURE;

/*-*-------------------------------------------------------------------------
/ Purpose
/	Creates a mask image based upon an image object.
/
/ Notes
/	Lingo code syntax:
/		x.createMask ()
/		returns image object on success, 0 if failure.
/--------------------------------------------*/
	STDMETHOD(CreateMask)		(THIS_ ConstPMoaMmValue pImage, 
		PMoaMmValue pResultImage)	PURE;

/*-*-------------------------------------------------------------------------
/ Purpose
/	Creates a new image with trimmed white edges based upon an
/	existing image object.
/
/ Notes
/	Lingo code syntax:
/		x.trimWhiteSpace ()
/		returns image object on success, 0 if failure.
/--------------------------------------------*/
	STDMETHOD(TrimWhiteSpace)	(THIS_ ConstPMoaMmValue pImage, 
		PMoaMmValue pResultImage)	PURE;

/*-*-------------------------------------------------------------------------
/ Purpose
/	For integer version, sets the alpha channel of an 32-bit image to a
/	constant value.  For image object version, inserts 8-bit image into
/	the alpha channel of a 32-bit image.
/
/ Notes
/	Lingo code syntax:
/		x.setAlpha (integer)
/		x.setAlpha (image object)
/		returns image object on success, 0 if failure.
/
/	Images must be the same size.
/	Destination image must be 32:0 or 32:8.
/	Alpha image must be 8 bit.
/	useAlpha flag is unaffected.
/--------------------------------------------*/
	STDMETHOD(SetAlpha) 		(THIS_ ConstPMoaMmValue pImage, 
		ConstPMoaMmValue pAlphaImageOrAlphaValue)	PURE;

/*-*-------------------------------------------------------------------------
/ Purpose
/	Creates a new image 8-bit grayscale image object that is only
/	the alpha channel plane from the existing image object, as long
/	as the existing image object is a 32:8 image.
/
/ Notes
/	Lingo code syntax:
/		x.extractAlpha ()
/		returns image object on success, 0 if failure.
/
/	Source image must be 32:0 or 32:8.
/--------------------------------------------*/
	STDMETHOD(ExtractAlpha) 	(THIS_ ConstPMoaMmValue pImage, 
		PMoaMmValue pResultImage)	PURE;

/*-*-------------------------------------------------------------------------
/ Purpose
/	Creates a lingo image object based upon the passed in parameters.
/
/ Notes
/	Lingo code syntax:
/		image(width, height, depth)
/		image(width, height, depth, alphaDepth)
/		image(width, height, depth, alphaDepth, palMember)
/
/	Pass NULL for missing pPaletteRef. Movie default palette is used.
/--------------------------------------------*/
	STDMETHOD(NewImage) 		(THIS_ 
		MoaLong iWidth, 
		MoaLong iHeight, 
		MoaLong iBitDepth, 
		MoaLong iAlphaDepth, 
		PMoaMmValue pPaletteRef,
		PMoaMmValue pResultImage)	PURE;

/*-*-------------------------------------------------------------------------
/ Purpose
/	Takes a MmValue containing an ImageRef, locks down an image 
/	handle and returns a pixel pointer.
/--------------------------------------------*/
	STDMETHOD(LockPixels)		(THIS_ ConstPMoaMmValue pImage, 
		PMoaVoid *ppPixels) PURE;

/*-*-------------------------------------------------------------------------
/ Purpose
/	Takes a MmValue containing an ImageRef and unlocks a locked
/	image handle.
/--------------------------------------------*/
	STDMETHOD(UnlockPixels) (THIS_ ConstPMoaMmValue pImage) PURE;

/*-*-------------------------------------------------------------------------
/ Purpose
/	Takes an open PIMoaStream and reads (and parses) an image from a file format.
/
/	Pass NULL for pProgressCookie if all data is available in the stream.
/	If pProgressCookie is non-NULL, a cookie is allocated and can be passed in
/	to UpdateImageFromStream. ReleaseProgressCookie must be called to deallocate
/	the cookie.
/--------------------------------------------*/
	STDMETHOD(NewImageFromStream) (THIS_ PIMoaStream pStream,
		PPMoaVoid pProgressCookie,
		PMoaMmValue pResultImage) PURE;
	STDMETHOD(UpdateImageFromStream) (THIS_ PIMoaStream pStream,
		PMoaVoid progressCookie,
		PMoaMmValue pUpdateImage) PURE;
	STDMETHOD(ReleaseProgressCookie) (THIS_ PPMoaVoid pProgressCookie) PURE;

/*-*-------------------------------------------------------------------------
/ Purpose
/	Takes a MmValue containing an ImageRef and return
/	a filled out MoaMmImageInfo structure.
/--------------------------------------------*/
	STDMETHOD(GetImageInfo) 	(THIS_ ConstPMoaMmValue pImage, 
		PMoaMmImageInfo pImageInfo) PURE;

/*-*-------------------------------------------------------------------------
/ Purpose
/	Takes an PIMoaPathName and opens and reads (and parses) an image from a file.
/--------------------------------------------*/
	STDMETHOD(NewImageFromPath) (THIS_ PIMoaPathName,
		PMoaMmValue pResultImage) PURE;
};

typedef IMoaMmImage * PIMoaMmImage;


/* ---------------------------------------------------------------------------- */

#undef INTERFACE

#ifdef	__cplusplus
}
#endif


#endif	/* MMIIMAGE_H */
