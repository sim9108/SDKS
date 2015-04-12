/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef	NO_PRAGMA_ONCE
#pragma	once
#endif

#ifndef	DRTYPES_H
#define	DRTYPES_H


#include "mmtypes.h"


#ifdef	__cplusplus
extern "C" {
#endif


/* ----------------------------------------------------------------------------
/
/	Set Alignment/Packing
/
/ -------------------------------------------------------------------------- */

#ifdef MACINTOSH
#pragma pack(2)
#endif /* MACINTOSH */
#ifdef WINDOWS
#pragma pack(2)
#endif /* WINDOWS */


/* ----------------------------------------------------------------------------
/
/	Director Cast Member Reference
/
/ -------------------------------------------------------------------------- */

typedef MoaLong	MoaDrMemberIndex, * PMoaDrMemberIndex;
typedef const MoaDrMemberIndex * ConstPMoaDrMemberIndex;

typedef MoaLong	MoaDrCastIndex, * PMoaDrCastIndex;
typedef const MoaDrCastIndex * ConstPMoaDrCastIndex;


typedef struct
{
	MoaDrCastIndex		movieCastIndex;	
	MoaDrMemberIndex	memberIndex;	
} MoaDrCMRef, * PMoaDrCMRef;
typedef const MoaDrCMRef * ConstPMoaDrCMRef;


/* MoaDrCMRef accessors: use these rather than directly touching fields
*/

#define	CMRef_GetMemberIndex(pCMRef) ((pCMRef)->memberIndex)

#define CMRef_GetMovieCastIndex(pCMRef) ((pCMRef)->movieCastIndex)

#define CMRef_Make(pCMRef, castInd, memberInd) 		\
			{ (pCMRef)->movieCastIndex = castInd; 	\
			  (pCMRef)->memberIndex = memberInd; 	\
			}
			
#define	CMRef_MakeNull(pCMRef) 						\
			{ (pCMRef)->movieCastIndex = 0; 		\
				(pCMRef)->memberIndex = 0; 			\
			}

/* ----------------------------------------------------------------------------
/
/	Director Unique Cast Member IDs
/
/ -------------------------------------------------------------------------- */

typedef struct {
	unsigned char id[16];
} MoaDrCMId, * PMoaDrCMId;
typedef const MoaDrCMId * ConstPMoaDrCMId;

/* ----------------------------------------------------------------------------
/
/	Director Sound Reference
/
/ -------------------------------------------------------------------------- */

typedef MoaShort MoaDrSoundChannelIndex, * PMoaDrSoundChannelIndex;
typedef const MoaDrSoundChannelIndex * ConstPMoaDrSoundChannelIndex;

typedef struct {
	MoaDrSoundChannelIndex soundChannelIndex;
} MoaDrSoundSpriteRef, * PMoaDrSoundSpriteRef;
typedef const MoaDrSoundSpriteRef * ConstPMoaDrSoundSpriteRef;

#define SoundSpriteRef_GetSoundChannelIndex(pSoundSpriteRef) ((pSoundSpriteRef)->soundChannelIndex)

#define SoundSpriteRef_Make(pSoundSpriteRef, soundChanInd) (pSoundSpriteRef)->soundChannelIndex = (soundChanInd)

/* ----------------------------------------------------------------------------
/
/	Director Sprite Reference
/
/ -------------------------------------------------------------------------- */

typedef MoaShort MoaDrSpriteIndex, * PMoaDrSpriteIndex;
typedef const MoaDrSpriteIndex * ConstPMoaDrSpriteIndex;

typedef struct {
	MoaDrSpriteIndex spriteIndex;
} MoaDrSpriteRef, * PMoaDrSpriteRef;
typedef const MoaDrSpriteRef * ConstPMoaDrSpriteRef;

#define SpriteRef_GetSpriteIndex(pSpriteRef) ((pSpriteRef)->spriteIndex)

#define SpriteRef_Make(pSpriteRef, spriteInd) (pSpriteRef)->spriteIndex = (spriteInd)

/* ----------------------------------------------------------------------------
/
/	Director Cast Lib Reference
/
/ -------------------------------------------------------------------------- */

typedef MoaShort MoaDrCastLibIndex, * PMoaDrCastLibIndex;
typedef const MoaDrCastLibIndex * ConstPMoaDrCastLibIndex;

typedef struct {
	MoaDrCastLibIndex castLibIndex;
} MoaDrCastLibRef, * PMoaDrCastLibRef;
typedef const MoaDrCastLibRef * ConstPMoaDrCastLibRef;

#define CastLibRef_GetCastLibIndex(pCastLibRef) ((pCastLibRef)->castLibIndex)

#define CastLibRef_Make(pCastLibRef, castLibInd) (pCastLibRef)->castLibIndex = (castLibInd)

/* ----------------------------------------------------------------------------
/
/	Score-related data types
/
/ -------------------------------------------------------------------------- */

typedef MoaLong		MoaDrSpriteChanIndex,  * PMoaDrSpriteChanIndex;
typedef MoaLong		MoaDrSoundChanIndex,  * PMoaDrSoundChanIndex;
typedef MoaLong		MoaDrFrameIndex,	 * PMoaDrFrameIndex;


/* ----------------------------------------------------------------------------
/
/	Cast Member Media Access
/
/ -------------------------------------------------------------------------- */

/* Media Access options - these depend on the media label being set/get.
						- ignored by labels that don't allow set/get options
*/

typedef	MoaLong			MoaDrMediaOpts;

#define kMoaDrMediaOpts_None						0

/* Image media "set" options
	- These are only valid when setting "image" label media
*/
/* the lower 16 bytes are for the Image options, the upper 16
are bit flags */
#define kMoaDrImgMediaOpts_AuthorDefined			0
#define kMoaDrImgMediaOpts_ImageDepthAndPal			1
#define kMoaDrImgMediaOpts_CastMemDepthAndPal		2
#define kMoaDrImgMediaOpts_ScreenDepthAndPal		3
#define kMoaDrImgMediaOpts_Dialog					4 /* author app only */
#define kMoaDrImgMediaOpts_AuxInfo					5

#define kMoaDrImgMediaFlags_DontTrimWhiteEdges		0x10000

/* When specifying kMoaDrImgMediaOpts_AuxInfo option for setting image
	media, the "aux" field of your MoaDrMediaInfo structure must
	be a pointer to a caller-owned MoaDrImageAuxInfo
	struct.  This permits explicit definition of the pixel depth,
	associated palette cast member, and resampling option (dither
	or remap) to be used.
*/

typedef struct
{
	MoaLong			pixelDepth;		/* Pixel depth: 1, 2, 4, 8, 16, 32 */
	MoaMmSymbol		paletteSymbol;	/* sym of built-in pal to use, 0 to use paletteCMRef */
	MoaDrCMRef		paletteCMRef;	/* MoaDrCMRef of cast-based palette to use */
	MoaBoolParam 	dither;			/* TRUE: dither, FALSE: remap */
} MoaDrImageAuxInfo, * PMoaDrImageAuxInfo;
typedef const MoaDrImageAuxInfo * ConstPMoaDrImageAuxInfo;


/* Information used by cast member and movie GetMedia() and SetMedia()
   methods.
*/

typedef struct
{
	MoaMmSymbol				labelSymbol;	/* Label of media to set/get */
	MoaMmSymbol				formatSymbol;	/* Format of media to set/get */
	PMoaVoid				mediaData;		/* Media data */
	MoaDrMediaOpts			options;		/* Get/Set options (depend on label) */
	PMoaVoid				aux;			/* Aux data */
	PMoaVoid				refCon;			/* Reserved */
} MoaDrMediaInfo, * PMoaDrMediaInfo;
typedef const MoaDrMediaInfo * ConstPMoaDrMediaInfo;


/* Information used by MediaAccessors.
	MediaAccessors, such as the ScoreAccessor, handle communication
	with the media owner object to provide a task-specific interface for
	accessing its media.
*/

typedef struct
{
	struct IMoaDrMediaOwner * 			pOwner;			/* obj owning media */
	MoaMmSymbol							labelSymbol;	/* label of media */
	MoaMmSymbol							formatSymbol;	/* format of media */
} MoaDrAccessInfo, * PMoaDrAccessInfo;
typedef const MoaDrAccessInfo * ConstPMoaDrAccessInfo;

/* ----------------------------------------------------------------------------
/
/	Notification Client
/
/ -------------------------------------------------------------------------- */

/* Notification types.
	These are used in conjunction with IMoaDrUtils::RegisterNotificationClient()
	to obtain callbacks when certain events occur in the host application.
*/

typedef MoaLong	MoaDrNotifyType;

#define	kMoaDrNotifyType_DocFileIO					1



/* Notification message codes.
	These message codes are passed to your IMoaDrNotificationClient::Notify()
	method when events occur that you've registered to be notified about.
	
	The host app reserves message code space >= 65536
	
*/

/* --- Document File I/O (kMoaDrNotifType_DocFileIO) message codes --- */

#define	kMoaDrMsgCode_DocFileIO					65536	/* ## WINDOWS VERSION ONLY ##
														ConstPMoaChar for file pathname
														passed in refCon argument to
														Notify() */



/* ----------------------------------------------------------------------------
/
/	Director sound channel status info.
/
/ -------------------------------------------------------------------------- */

enum {
	kMoaDrSoundChannelStatus_Playing,
	kMoaDrSoundChannelStatus_Stopped
};

typedef MoaUlong MoaDrSoundChannelPlayStatus;


/* ----------------------------------------------------------------------------
/
/	Director cursor IDs.
/
/ -------------------------------------------------------------------------- */

/* Director built-in Cursors*/
#define kMoaDrCursor_Arrow  		-1
#define kMoaDrCursor_Default  		0
#define kMoaDrCursor_IBeam  		1
#define kMoaDrCursor_Crosshair  	2
#define kMoaDrCursor_Crossbar  		3
#define kMoaDrCursor_Watch  		4
#define kMoaDrCursor_Finger  		280


typedef MoaLong MoaDrCursorID;


/* ----------------------------------------------------------------------------
/
/	Director-specific error codes (extensions to shared Mm errors)
/
/ -------------------------------------------------------------------------- */

#define		kMoaDrErr_Base						3000


/* Property access errors */

#define		kMoaDrErr_CastMemberExpected		MAKE_MOAERR(kMoaDrErr_Base)
#define		kMoaDrErr_MediaExpected				MAKE_MOAERR(kMoaDrErr_Base + 1)
#define		kMoaDrErr_PictureExpected			MAKE_MOAERR(kMoaDrErr_Base + 2)

/* Media access errors */

#define		kMoaDrErr_MediaLabelNotSupported	MAKE_MOAERR(kMoaDrErr_Base + 3)
#define		kMoaDrErr_MediaFormatNotSupported	MAKE_MOAERR(kMoaDrErr_Base + 4)
#define		kMoaDrErr_MediaInUse				MAKE_MOAERR(kMoaDrErr_Base + 5)
#define		kMoaDrErr_MediaDataInvalid			MAKE_MOAERR(kMoaDrErr_Base + 6)
#define		kMoaDrErr_MediaDataNonexistant		MAKE_MOAERR(kMoaDrErr_Base + 7)
#define		kMoaDrErr_LabelNotFound				MAKE_MOAERR(kMoaDrErr_Base + 8)

/* Score access errors */

#define		kMoaDrErr_NotInUpdateMode			MAKE_MOAERR(kMoaDrErr_Base + 9)
#define		kMoaDrErr_OwnerNotSet				MAKE_MOAERR(kMoaDrErr_Base + 10)

/* For backward compatibility with Director 5.0, error code kMoaDrErr_Base +11 is in mmtypes,
   as kMoaMmErr_ToolDisabled.   */


/* Misc */

#define		kMoaDrErr_CastNotFound				MAKE_MOAERR(kMoaDrErr_Base + 12)
#define		kMoaDrErr_CastMemNotFound			MAKE_MOAERR(kMoaDrErr_Base + 13)
#define		kMoaDrErr_DiskIO					MAKE_MOAERR(kMoaMmErr_Base + 14)
#define		kMoaDrErr_FrameNotFound				MAKE_MOAERR(kMoaDrErr_Base + 15)
#define		kMoaDrErr_NotifyTypeUnknown			MAKE_MOAERR(kMoaDrErr_Base + 16)
#define		kMoaDrErr_HandlerNotDefined			MAKE_MOAERR(kMoaDrErr_Base + 17)
#define		kMoaDrErr_MovieNotOpen				MAKE_MOAERR(kMoaDrErr_Base + 18)


/* Director error codes */

/* Property access errors */

#define		kMoaDrErr_SpriteExpected			MAKE_MOAERR(kMoaDrErr_Base + 19)
#define		kMoaDrErr_CastLibExpected			MAKE_MOAERR(kMoaDrErr_Base + 20)
#define		kMoaDrErr_SoundSpriteExpected		MAKE_MOAERR(kMoaDrErr_Base + 21)

/* Misc */

#define 	kMoaDrErr_SoundChannelOutOfRange	MAKE_MOAERR(kMoaDrErr_Base + 22)
#define 	kMoaDrErr_AuthorTimeOnly			MAKE_MOAERR(kMoaDrErr_Base + 28)

#define		kMoaDrErr_ImageExpected				MAKE_MOAERR(kMoaDrErr_Base + 29)
// used when media not fully downloaded or streamed in
#define		kMoaDrErr_MediaNotReady				MAKE_MOAERR(kMoaDrErr_Base + 30)
/* ----------------------------------------------------------------------------
/
/	Other constants
/
/ -------------------------------------------------------------------------- */

/* Maximum length of a cast name in bytes */

#define kMoaDrMaxCastName				255



/* ----------------------------------------------------------------------------
/
/	Reset Alignment/Packing
/
/ -------------------------------------------------------------------------- */

#ifdef MACINTOSH
#pragma pack()
#endif /* MACINTOSH */
#ifdef WINDOWS
#pragma pack()
#endif /* WINDOWS */


#ifdef	__cplusplus
}
#endif


#endif	/* DRTYPES_H */
