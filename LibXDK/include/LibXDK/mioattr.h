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

File: mioattr.h

Abstract:
Public header that defines stream/file/volume attributes.

*/

#ifndef _H_MoaIOAttr_
#define _H_MoaIOAttr_


/* ------------------------------------------------------------------------
** Includes
*/

#include "moatypes.h"
#include "moastdif.h"

/* ------------------------------------------------------------------------
** Macros
*/

#define kMoaStreamFlag_HasAsyncIO				0x0001L
#define kMoaStreamFlag_HasBufferredIO			0x0002L
#define kMoaStreamFlag_HasDirectBufferAccess	0x0004L

#define kMoaFileFlag_IsExistant					0x0001L
#define kMoaFileFlag_IsDirectory				0x0002L

#define kMoaVolFlag_IsRemovable					0x0001L
#define kMoaVolFlag_IsCDROM						0x0002L
#define kMoaVolFlag_IsHardDisk					0x0004L
#define kMoaVolFlag_IsNetworkDrive				0x0008L


/* ------------------------------------------------------------------------
** Types
*/


/*
** MoaCachePriority:
*/
enum {
  kMoaCachePriority_Always,		/* If the cache doesn't like it, use a temp file */
  kMoaCachePriority_IfYouCan,	/* if there's room in the cache (is this useful?) */
  kMoaCachePriority_Never,		/* don't cache this */
  kMoaCachePriority_Preload,	/* force into the cache even if cgi or similar.  Subsequent Open
								/  will remove it from the cache if it shouldn't be there. */
  kMoaCachePriority_WhileObject	/* Will be forced into the cache while stream object exists.
								/  When stream object is destroyed, url will be removed from
								/  the cache if it shouldn't be there. */
};
typedef MoaLong MoaCachePriority, *PMoaCachePriority;


/*
** MoaIOKeys: list of pre-defined keys for I/O attributes
*/
#define kMoaIOKey_StreamFlags			"MIOKey_StreamFlags"
#define kMoaIOKey_StreamAccessMode		"MIOKey_StreamAccessMode"
#define kMoaIOKey_StreamBufferSize		"MIOKey_StreamBufferSize"
#define kMoaIOKey_StreamDataRate		"MIOKey_StreamDataRate"
#define kMoaIOKey_StreamEnd				"MIOKey_StreamEnd"
#define kMoaIOKey_StreamCurrentLength	"MIOKey_StreamCurrentLength"
#define kMoaIOKey_StreamPosition		"MIOKey_StreamPosition"
#define kMoaIOKey_StreamPositionType	"MIOKey_StreamPositionType"

#define kMoaIOKey_FileFlags				"MIOKey_FileFlags"
#define kMoaIOKey_FileAccessPerm		"MIOKey_FileAccessPerm"
#define kMoaIOKey_FileLength			"MIOKey_FileLength"
#define kMoaIOKey_FileType				"MIOKey_FileType"
#define kMoaIOKey_FileEncoding			"MIOKey_FileEncoding"
#define kMoaIOKey_FileCreator			"MIOKey_FileCreator"
#define kMoaIOKey_FileCreateDate		"MIOKey_FileCreateDate"
#define kMoaIOKey_FileModifyDate		"MIOKey_FileModifyDate"
#define kMoaIOKey_FileAccessDate		"MIOKey_FileAccessDate"
#define kMoaIOKey_FileCachePriority		"MIOKey_FileCachePriority"

#define kMoaIOKey_VolumeFlags			"MIOKey_VolumeFlags"
#define kMoaIOKey_VolumeBlockSize		"MIOKey_VolumeBlockSize"
#define kMoaIOKey_VolumeSectorSize		"MIOKey_VolumeSectorSize"
#define kMoaIOKey_VolumeFreeSpace		"MIOKey_VolumeFreeSpace"

enum {
kMoaIOKeyIndex_StreamFlags,
kMoaIOKeyIndex_StreamAccessMode,
kMoaIOKeyIndex_StreamBufferSize,
kMoaIOKeyIndex_StreamDataRate,
kMoaIOKeyIndex_StreamEnd,
kMoaIOKeyIndex_StreamCurrentLength,
kMoaIOKeyIndex_StreamPosition,
kMoaIOKeyIndex_StreamPositionType,

kMoaIOKeyIndex_FileFlags,
kMoaIOKeyIndex_FileAccessPerm,
kMoaIOKeyIndex_FileLength,
kMoaIOKeyIndex_FileType,
kMoaIOKeyIndex_FileEncoding,
kMoaIOKeyIndex_FileCreator,
kMoaIOKeyIndex_FileCreateDate,
kMoaIOKeyIndex_FileModifyDate,
kMoaIOKeyIndex_FileAccessDate,
kMoaIOKeyIndex_FileCachePriority,

kMoaIOKeyIndex_VolumeFlags,
kMoaIOKeyIndex_VolumeBlockSize,
kMoaIOKeyIndex_VolumeSectorSize,
kMoaIOKeyIndex_VolumeFreeSpace,

kMoaIOKeyIndex_LastPredefined
};

/* ------------------------------------------------------------------------
** IMoaIOAttributeDict is simply an IMoaDict
*/

typedef IMoaDict			IMoaIOAttributeDict;
typedef IMoaIOAttributeDict	*PIMoaIOAttributeDict;

/* ------------------------------------------------------------------------
** Notes

** I/O attributes are designed to be accessed thru
** the Get/Put methods of the IMoaDict interface.

** The type of an attribute's value pointer is specific to the attribute:
**
**		Stream Attribute	Type of valuePtr	Length
**		Flags				PMoaLong			4
**		AccessMode			PMoaLong			4
**		BufferSize			PMoaLong			4
**		DataRate			PMoaLong			4
**		End					PMoaLong			4
**		CurrentLength		PMoaLong			4
**		Position			PMoaLong			4
**		PositionType		PMoaLong			4
**
**
**
**		File Attribute		Type of valuePtr	Length
**		Flags				PMoaLong			4
**		AccessPerm			PMoaLong			4
**		Length				PMoaLong			4
**		Type				PMoaChar			variable
**		Encoding			PMoaChar			variable
**		Creator				PMoaChar			variable
**		CreateDate			PMoaWide			8
**		ModifyDate			PMoaWide			8
**		AccessDate			PMoaWide			8
**		CachePriority		PMoaLong			4
**
**
**
**		Volume Attribute	Type of valuePtr	Length
**		Flags				PMoaLong			4
**		BlockSize			PMoaLong			4
**		SectorSize			PMoaLong			4
**		FreeSpace			PMoaLong			4
**
**
*/

#endif /* _H_MoaIOAttr_ */
