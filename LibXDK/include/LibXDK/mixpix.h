/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_mixpix
#define _H_mixpix

#include "mixagent.h"

/* -------------------------------------------------------------------------- */
/*
	The following section describes the color table used in the ReceptorPixels
	interface. It was taken pretty directly from the Director Portable Player
	IML's implementation, but the types have been changed to be "Moa" types.
	Additionally, I have brought over only the 256 color table definition, since
	all of the others "can" be placed in a 256 color table, simply by adjusting
	the numEntries field.
*/

/*
	This is the type that describes a component in a color lookup table.
	Notice that this is not a 8 bit value (char), but a 16 bit value.
	For converting an 8 bit value into a 16 bit value, duplicate the
	8 bit value in both the upper and lower byte of the component.
	
	For example, a 24 bit RGB value of R 0x55 G 0x20 B 0xF7 would have a 16
	bit R 0x5555 G 0x2020 B 0xF7F7.
*/
typedef MoaUshort TMoaColorComponent;

/*
	This is the description of an entry in a color table. The color table will have
	an array of these entries in it.
*/
typedef struct TMoaCTableEntry {
	/*	
		Flags value is used to specify whether or not to lock out the color.
		Be sure to set the flags value to a valid setting for each table entry.
		This component is currently unused, but is left in for binary
		compatibility with the IML's image model.
	*/
	MoaUshort Flags;
	
	/* Red, green, blue color values */
	TMoaColorComponent Red;
	TMoaColorComponent Gre;
	TMoaColorComponent Blu;
} TMoaCTableEntry;

/*
	This is the description of an the number of entries in the color table.
	"Just In Case" somebody wants to have a color table that has only a few
	entries, it can be accomplished by adjusting iNumEntries..
*/
typedef struct TMoaCTableHeader {
	/* Number of entries in use (typically 256 or fewer) and number of
	/ entries allocated (typically exactly 256). */
	MoaLong iNumEntries;
	MoaLong iNumEntriesAlloc;
} TMoaCTableHeader;

/*
	This is "The" color table definition for a 256 color lookup table.
	Presumably, this does not preclude the existence of larger color tables,
	nor smaller ones, but 256 should suffice for our needs, and if it becomes
	a problem, we can augment the interface later.
*/
typedef struct TMoaCTable256 {
	TMoaCTableHeader Header;
	TMoaCTableEntry Entry[256];
} TMoaCTable256;

/* -------------------------------------------------------------------------- */
/*
	The following is a description of several of the color formats available
	in the IMoaReceptorPixels interface. Note, that not all applications will
	support all of the formats, and in fact most will support only a few.
	
	Each application and Xtra are REQUIRED to support RPCS_RGB as a minumum,
	which is 24 bit, 3 samples per pixel, 8 bits per sample, RGB image data.
	
	Beyond that, it seems wise to specify several more common formats here, so
	that it won't need to be done piecemeal later. This is not to say that this
	list is by any means exhaustive, because if another format is desired, then
	there just needs to be agreement on what the type identifier for that format
	is. A good general rule for that is that you can generate a long value for
	a "new" type by the following mechanism:

		XYZ data may be 'xyza' to create a 32 bit value equaling 0x78797A61.
		Hexacrhome may be 'HEXA', or 0x48455841.

	As long as all users of IMoaReceptorPixels all agree on those identifiers, then
	all will be well...

	In general, for each of these formats:

	each line is padded at the end to a rowbytes size specified by the provider
		of data.

	Here is a description of the currently enumerated image types:
	RPCS_RGB:
		24 bit chunky RGB (RGBRGBRGB)
		each component should be 8 bits, making a 24 bit triple.
		the triples are packed together on 24 bit boundaries
		There must always be 3 samples per pixel

	RPCS_RGBA:
		32 bit (or more) chunky RGBA (RGBARGBARGBA)
		each component should be 8 bits, making a 32 bit quadruple for RGBA.
		This format supports more than 4 samples per pixel as well...
		tuples are packed to 8 bit boundaries.

	RPCS_INDEXED_RGB:
		1, 2, 4, or 8 bit color indexed into RGB
		the size of each index in the image is 1, 2, 4, or 8 bits
		the indices are packed together based on the bits per sample
		There should always be 1 sample per pixel, the index value.

		To specify a monochrome bitmap, create a 1 bit indexed image, and
		put "White" and "Black" in the appropriate indices.
		
		To specify a GrayScale bitmap, create an 8 bit indexed image, and
		put the gray ramp in the appropriate indices.

	RPCS_INDEXED_RGBA:
		1, 2, 4, or 8 bit color indexed into RGB
		the size of each index in the image is 1, 2, 4, or 8 bits
		the indices are packed together based on the bits per sample
		There may also be multiple samples per pixel, allowing alpha

		To specify a monochrome bitmap, create a 1 bit indexed image, and
		put "White" and "Black" in the appropriate indices.
		
		To specify a GrayScale bitmap, create an 8 bit indexed image, and
		put the gray ramp in the appropriate indices.

	RPCS_CMYK:
		32 bit (or more) chunky CMYK (CMYKCMYKCMYK)
		each component should be 8 bits, making a 32 bit quadruple.
		the quadruple are packed together on 32 bit boundaries

	RPCS_RGB16:
		16 bit chunky RGB (RGBRGBRGB)
		each component should be 5 bits, making a 16 bit triple, with 1 bit of
		pad at the end.

		the triples are packed together on 16 bit boundaries.
		the 1 bit pad "P" (PRRRRRGGGGGBBBBB) is generally unused, but may be
		interpreted as transparency.
		Bit 15 of the 16 bit value is the "unused" or "transparent" bit.

	RPCS_MONO:
			Monochrome bitmap where 1 is Black and 0 is White.

----------------------------------------------------------------------------
the following definition is very vague, and needs further specification by
the parties interested in using/producing this data.
	RPCS_WIDE_RGBA
		>24/>32 bit chunky RGB/RGBA
		the component size is specified in bits per sample
		each component starts on a byte boundary, No packing of components bit-wise
		each triple/quad packed based on samples/pixel and bits/sample
		
*/
enum {
	RPCS_RGB,					/* R, G and B data (24 bit RGB) */

		/* the following formats are "possible", however unrequired. */
	RPCS_RGBA,					/* R, G and B data, with Alpha. (32 bit RGB) */
								/* May be more than 4 samples per pixel */
	RPCS_INDEXED_RGB,			/* One value per pixel, index into color table (1,2,4,8 bit) */
	RPCS_INDEXED_RGBA,			/* Two values per pixel, index into color table (1,2,4,8 bit), */
								/* and Alpha (8 bit). May be more if the universe desires */
								/* NOTE: for the above two indexed formats, it is likely that */
								/* some applications may be unable to do 1, 2, or 4 bit indexed */
								/* image data, and may have to fail on BeginPixels because */
								/* of that. Therefore, convention states that 8 bit is the */
								/* preferred format, but 1, 2, and 4 are not strictly */
								/* forbidden. */
	
		/* the following formats are "possible", however unlikely to be supported by */
		/* many apps or file formats. They are include for the potential that they become */
		/* more interesting as interchange formats. */
	RPCS_CMYK,					/* C, M, Y, K data. (32 bit) */
	RPCS_RGB16,					/* handy common 16 bit RGB...has 5 R, 5G, 5B, and 1 mask. */
	RPCS_WIDE_RGBA,				/* RGBA data, but wider than 8 bits. Stored Red, Green, Blue */
								/* and optionally Alpha. All components start on a byte */
								/* boundary. No packing of components bit-wise. */
	RPCS_MONO					/* Monochrome bitmap where 1 is Black and 0 is White */
};


/* The following are bitfields which can be placed into 
	the "flags" member of the ColorSpace. */
#define RPCSFLAGS_NONE			0x00000000
#define RPCSFLAGS_TRANSPARENT	0x00000001

enum {
	TOP_DOWN,
	BOTTOM_UP
};

/*
	This structure contains the "dimension" information passed into BeginPixels.
	the members of the structure are:

		pixels:		describes the size, in pixels, of the image.

		resolution:	describes the pixels/inch of the image.
			This value should be the resolution contained in the image file, or
			from the application sending the data. If there is no "natural" or
			"apparent" resolution, some heuristics come to mind. The Mac has a
			a default of 72 dpi screen resolution, and Windows is between 72 and 96.

		rowBytes:	describes the padded size of a row of the image.
*/
typedef struct RenderPixelDimensions {
	MoaPoint pixels;
	MoaPoint resolution;
	MoaLong rowBytes;
} RenderPixelDimensions;

/*
	This structure contains the "color space" information passed into BeginPixels.
	the members of the structure are:

		colorSpace:	describes the color model of the image (RPCS_RGB, RPCS_CMYK).
			This must be one of the RPCS_ color spaces defined above, or an extended
			space agreed upon by at least one application and one Xtra developer.

		samplesPerPixel: describes the number of samples per pixel. Some of the
			pre-defined RPCS_ color spaces have "required" values for this, but
			each has an allegory that allows any number of samples per pixel.

		bitsPerSample: describes the number of bits per sample in the image. Some
			of the pre-defined RPCS_ color spaces have "required" values for this,
			but each has an allegory that allows any number of bits per sample.

		transparentIdx: For indexed images, contains the index that is "transparent".
			If there is no transparent index, this value should be -1 (0xFFFF).

		colorTable: For indexed images, describes the color table for the image.
			Can be 2, 4, 16, or 256 colors
*/
typedef struct RenderPixelColorSpace {
	MoaLong colorSpace;
	MoaShort samplesPerPixel;	/* 3 for RGB, 3+ for RGBA */
	MoaShort bitsPerSample;		/* Set to 8 for RGBA and INDEXED_COLOR, may be different */
								/* for currently "unsupported" formats */
	MoaShort transparentIdx;	/* the transparent color in the color table, if any */
	MoaShort flags;				/* Special properties of this color space... RPCSFLAGS_* */
	TMoaCTable256 colorTable;	/* 8 bit Color table, filled in if required */
} RenderPixelColorSpace;

/*
	This structure contains the "pixel format" information passed into BeginPixels.
	it contains both the dimension and color space data structures.
*/
typedef struct MoaPixelFormat {
	RenderPixelDimensions dim;
	RenderPixelColorSpace cs;
} MoaPixelFormat;
typedef MoaPixelFormat * PMoaPixelFormat;
typedef const MoaPixelFormat * ConstPMoaPixelFormat;

/* Standard dictionary keys: */

/* Take careful note...some of these keys are used only in the Get/SetOptions methods. */
/* Some of these keys are used only in the Register method. */
/* Some of these keys are used both places! */

/* For indexed color images, the following dictionary keys can assign special */
/* meaning to certain index numbers. */
#define kPixelFormatRegKey_MatteIndex 	"PixelFormat_MatteIndex"

/* For images supporting various encoding qualities, this defines the quality. */
/* 0 is least quality. 100 is top quality. This does not necessarily mean */
/* completely lossless however! Certain color transformations introduce integer */
/* rounding errors, so a Q-Factor of 100% does not mean that the RGBA binary */
/* representation of the image will NOT change as the file is written and read back. */
#define kPixelFormatRegKey_QFactor 		"PixelFormat_QFactor"

/* Here's a weird key which can occur in two places... */
/* For writers, this will be part of the Registry for the Writer interface. It indicates */
/* whether the writer is a progressive type or not. Writing and interlace file usually */
/* requires a different data flow than that possible when writing a non-interlaced file. */
/* For readers...this will be an incoming property of the file being read. It is */
/* available via the Reader's GetReaderOptions method. */
/* This organization is not that weird...for JPEG, we want two Writer interfaces...one */
/* for progressive JPEG and one for standard JPEG. However, one Readerclass which */
/* can deal with any JPEG file will do. It would be confusing to have two JPEG reader classes. */
#define kPixelFormatRegKey_Interlaced 	"PixelFormat_Interlaced"

/* Set to TRUE or FALSE */
#define kPixelFormatRegKey_Compression "PixelFormat_Compression"

/* Set to 8, 24, 32, whatever */
#define kPixelFormatRegKey_BitsPerPixel "PixelFormat_BitsPerPixel"
/* This allows image format selection to occur in the Writer UI and also allows */
/* the application strict control of the options. It also enables distributed */
/* rendering to function...as you know, all options in the Writer and Reader UI */
/* need to travel in a dictionary and not in any local variables. */

/* Here is the basic IMoaReceptorPixels interface...this is what most Agents will implement to */
/* receive pixel data. A Reader (or a IMoaPixelConvert) calls InquireFormat to determine */
/* what pixel types can be passed into SetPixels. */

#undef INTERFACE
#define INTERFACE IMoaReceptorPixels
DECLARE_INTERFACE_(IMoaReceptorPixels, IMoaReceptor)
/*	Description

	??? need a good general description here
*/
{
	STD_IUNKNOWN_METHODS
	
	STDMETHOD(SetRefCon)(THIS_
		moa_in PMoaVoid refCon
	) PURE;
/*	Description

*/

	STDMETHOD(SetStatus)(THIS_
		moa_in MoaError status
	) PURE;
/*	Description

*/

	STDMETHOD(InquireFormat)(THIS_
		moa_in MoaLong colorSpaceCount,
		moa_out MoaLong * pColorSpace,
		moa_in MoaLong dimensionCount,
		moa_out PMoaPoint pDimension,
		moa_in MoaBoolParam dimensionRange,
		moa_in MoaLong directionCount,
		moa_out MoaLong * pDirection
	) PURE;
/*	Description

	InquireFormat is used to negotiate with the receptor 
	to determine what format will be transmitted to the 
	receptor. 

	The parameters are:

	colorSpaceCount:	the number of color spaces in the array
	pColorSpace:		a pointer to the array of color spaces
	dimensionCount:		the number of image dimensions in the array
	pDimension:			a pointer to the array of image	dimensions
	dimensionRange:		a boolean specifying whether the dimensions are a range
	directionCount:		the number of directions in the array
	pDirection:			a pointer to the array of directions

	The data provider must fill in the the color space 
	array with all of the color spaces that it can provide 
	to the Receptor from the data it is to provide. The 
	first item in the array must be the "Native" format of 
	the data, and the array must contain RPCS_RGB.

	The data provider should not be compelled to convert 
	the data into any "non-native" format other than 
	RPCS_RGB, but those conversions are not forbidden.

	If the native format is 8 bit indexed, the list should 
	be:

		RPCS_INDEXED_COLOR
		RPCS_RGB

	If the data provider is especially overeager, then the 
	list MIGHT be:

		RPCS_INDEXED_COLOR
		RPCS_RGB
		RPCS_RGB16

	(This would allow an application to have the provider 
	"downsample" the data to 16 bit RGB if it genuinely 
	was not interested in 24 bit color, and wanted an 
	extra-smart Xtra to do it...)

	The list should NOT be:

		RPCS_INDEXED_COLOR
		RPCS_RGB16
		RPCS_RGB

	(This would imply to the application that the native 
	image data "prefers" 16 bit RGB over 24 bit RGB, even 
	though that is a loss of resolution. If there is to be 
	any loss of resolution, the receptor should be the one 
	to determine that preference.)

	The Receptor must choose one of the color spaces in 
	the array and place that color space as the first in 
	the list. This is the color space that the data 
	provider must use.

	The data provider must fill in the the image 
	dimensions array with all of the image dimensions that 
	it can provide to the Receptor from the data it is to 
	provide. The first item in the array must be the 
	"Native" format of the data, or a preferred size. For 
	images that have a single, fixed dimension, the 
	provider must have only that dimension in the list. 
	For images that have multiple, discrete dimensions 
	available, the provider should list them all, with the 
	"best" as the one first in the list. For images that 
	have "variable" dimensions (ie, splines that will be 
	rendered into bitmaps), the provider should put the 
	"best" dimension first, the minimum dimension second, 
	and the maximum dimension third. The Receptor must 
	choose one of the dimensions in the array and place 
	that dimension as the first in the list. This is the 
	dimension that the data provider must use.

	The data provider must fill in the direction array 
	with the image flow directions that it can provide. 
	Currently, only "top down" and "bottom up" are 
	supported, but if that changes, the provider will have 
	to support at least one of those. The Receptor must 
	choose one of the directions in the array and place 
	that direction as the first in the list. This is the 
	direction that the data provider must use.
*/

	STDMETHOD(BeginPixels)(THIS_
		moa_in ConstPMoaPixelFormat pPixelFormat
	) PURE;
/*	Description

	After InquireFormat has been called, BeginPixels 
	should be called to tell the Receptor exactly what the 
	format of the data will be. Inside of BeginPixels, the 
	Receptor must either accept the data format, and 
	prepare to receive it, or refuse and return 
	kMoaErr_BadParam.

	The data provider must fill in the MoaPixelFormat 
	structure fully, based on the return codes from the 
	InquireFormat negotiation and its own internal 
	abilities.

	The Receptor must either accept or reject this 
	MoaPixelFormat. If, for some reason, the results are 
	not acceptable to the Receptor, it must return 
	kMoaErr_BadParam to abort processing.
*/

	STDMETHOD(SetPixels)(THIS_
		moa_in MoaLong scanline,
		moa_in PMoaVoid pPixelBuffer,
		moa_in MoaLong bufferSize
	) PURE;
/*	Description

	The parameters are:	

	scanline:		describing which scanline, or row is being passed
	pPixelBuffer:	a pointer to the pixel data, owned by the provider
	bufferSize:		the size of the data in the buffer.

	The Receptor is responsible for remembering (from 
	BeginPixels) what format the data is coming in. The 
	data provider will send one row of the image in at a 
	time, with "scanline" indicating which row. Row 0 is 
	always the "visually topmost" row of the image, so 
	that a provider passing data in "top down" would pass 
	row 0, then 1, etc., while one passing data in "bottom 
	up" would pass 799, then 798, etc.

	The bufferSize parameter is intended only to provide 
	the Receptor with the size of the buffer passed in. 
	The rowbytes field of the MoaPixelFormat structure is 
	the specification of the "correct" size of the data, 
	plus padding.
*/
	STDMETHOD(EndPixels)(THIS) PURE;
/*	Description

	After the image has been transferred, as a final 
	handshake that the processing is genuinely completed, 
	EndPixels is called to let the Receptor know that no 
	more image data will be coming. The Receptor should 
	finish any processing needed at this point.
*/
};
typedef IMoaReceptorPixels * PIMoaReceptorPixels;
DEFINE_GUID(IID_IMoaReceptorPixels, 0xBCC00E6AL, 0xB303, 0x11CF, 0x94, 0x32, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

#endif



