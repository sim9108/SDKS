/*
ADOBE SYSTEMS INCORPORATED
Copyright 2006 Ð 2007 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_filtercaps
#define _H_filtercaps

// Filter capabilities
typedef MoaUlong	MoaMmXFilterCapsFlags;

#define kMoaMmXFilterCapsFlags_None 				0
#define kMoaMmXFilterCapsFlags_AffectsAlpha 		1
#define kMoaMmXFilterCapsFlags_HasPropEditor		2

typedef struct
{
	MoaMmXFilterCapsFlags	flags;
} MoaBMFilterCaps, * PMoaBMFilterCaps;

/*Filter Rect and scale factor*/
/*This structure is used for storing Filter Rect and scale factor of the stage*/
typedef struct MoaFilterInfo
{
	MoaFloat scale;
	MoaRect rect;	
} MoaFilterInfo, *PMoaFilterInfo;


#endif //_H_filtercaps
