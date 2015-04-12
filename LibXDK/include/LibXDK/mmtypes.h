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
		Common types for Director and Authorware MOA APIs.
*/

#ifndef NO_PRAGMA_ONCE
#pragma once
#endif


#ifndef _H_mmtypes
#define _H_mmtypes


/* This hack tells moatyedg.h no to pull in any Mac system headers */
#define _NO_MOA_MAC_INCLUDE_

#include "moatypes.h"


/* ----------------------------------------------------------------------------
/ -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
/  Make sure WINDOWS, WIN32, WIN16, and MACINTOSH are defined.
/  ------------------------------------------------------------------------ */

#ifdef _WINDOWS
	#ifndef WINDOWS
		#define WINDOWS
	#endif
	#ifdef _WIN32
		#ifndef WIN32
			#define WIN32
		#endif
	#else
		#ifndef WIN16
			#define WIN16
		#endif
	#endif
#else
	#ifndef MACINTOSH
		#define MACINTOSH			/* assume mac if not _WINDOWS */
	#endif
#endif



/* ----------------------------------------------------------------------------
								MMTYPES.H
/ -------------------------------------------------------------------------- */


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
/	Adaptive Types
/
/	These types can vary depending on the build platform.
/
/ -------------------------------------------------------------------------- */

typedef PMoaVoid MoaMmWinHdl32;

#ifdef WINDOWS
	/* Windows variants */
	#ifdef WIN16
		typedef unsigned int MoaMmWinHandle;
	#else
		typedef MoaMmWinHdl32 MoaMmWinHandle;
	#endif /* WIN16 : WIN32 */
	typedef PMoaVoid PMoaMmMacWindow;			
	typedef PMoaVoid PMoaMmMacGWorld;			
#endif /* WINDOWS */

#ifdef MACINTOSH
	/* Macintosh variants */
	typedef MoaMmWinHdl32 MoaMmWinHandle;
	typedef PMoaVoid PMoaMmMacWindow;			/* Macintosh WindowRef (used to be a WindowPtr) */
	typedef PMoaVoid PMoaMmMacGWorld;			/* Macintosh GWorldPtr */
#endif /* MACINTOSH */

typedef MoaMmWinHandle MoaMmHInst;				/* Windows HINST */
typedef MoaMmWinHandle MoaMmHWnd;				/* Windows HWND */


/* ----------------------------------------------------------------------------
/
/	Graphics Context Types
/
/ -------------------------------------------------------------------------- */

typedef MoaLong MoaMmGCType;

#define kMoaMmGCType_Null		0	
#define kMoaMmGCType_WinWindow	1
#define kMoaMmGCType_WinDIBDC	2
#define kMoaMmGCType_WinDDBDC	3
#define kMoaMmGCType_MacWindow	4
#define kMoaMmGCType_MacGWorld	5
#define kMoaMmGCType_Aux		6
#define kMoaMmGCType_ImlGC		7

/* WinDIB Flags (for win_dibdc variant of MoaMmNativeGCInfo) */

typedef MoaUlong MoaMmWinDIBDCFlags;

#define kMoaMmWinDIBDCFlags_None	0
#define kMoaMmWinDIBDCFlags_WinG	1

/* MoaMmNativeGCInfo: platform-specific graphics context info */

typedef struct
{
	MoaMmGCType type;
	
	union
	{
#ifdef WINDOWS
		/* Windows-only GC types */
		struct					
		{
			MoaMmWinHdl32		hDC;	/* Win16: only low 16-bits used */
			MoaMmWinHdl32		hWND;	/* Win16: only low 16-bits used */
		} win_window;
		
		struct
		{
			MoaMmWinHdl32		hDC;	/* Win16: only low 16-bits used */
			PMoaVoid			lpBitMapInfo;
			PMoaVoid			lpBits;
			MoaMmWinDIBDCFlags	flags;
		} win_dibdc;
		
		struct
		{
			MoaMmWinHdl32		hDC;	/* Win16: only low 16-bits used */
			PMoaVoid			hBITMAP;
		} win_ddbdc;
#endif /* WINDOWS */

#ifdef MACINTOSH
		/* Macintosh-only GC types */		
		struct
		{
			PMoaMmMacWindow 	windowPtr;		/* this is a WindowRef, not a WindowPtr */
			PMoaVoid			qdGlobalsPtr;	/* this is NULL for OS X and later */
		} mac_window;
		
		struct
		{
			PMoaMmMacGWorld 	gworldPtr;
			PMoaVoid			qdGlobalsPtr;	/* this is NULL for OS X and later */
		} mac_gworld;
#endif /* MACINTOSH */

		/* Generic GC types */		
		struct
		{
			PMoaVoid			idGC;
		} iml_gc;
		
		struct
		{
			PMoaVoid			data1;
			PMoaVoid			data2;
			PMoaVoid			data3;
			PMoaVoid			data4;
		} aux;
	} data;
	
} MoaMmNativeGCInfo;
typedef MoaMmNativeGCInfo * PMoaMmNativeGCInfo;
typedef const MoaMmNativeGCInfo * ConstPMoaMmNativeGCInfo;


/* ----------------------------------------------------------------------------
/
/	Color types (binary compatible with IML color spec)
/
/ -------------------------------------------------------------------------- */

typedef struct
{
	MoaByte   red;
	MoaByte   green;
	MoaByte   blue;
} MoaMmRGBTriple, *PMoaMmRGBTriple;
typedef const MoaMmRGBTriple * ConstPMoaMmRGBTriple;

typedef MoaByte MoaMmColorIndex, * PMoaMmColorIndex;

/* Copy between a MoaMmRGBTriple and a Mac RGBColor. Takes pointers. */

#define MoaToMacRGB(a,b)	((b)->red	= ((unsigned short)(a)->red << 8)	| (a)->red,   \
							 (b)->green = ((unsigned short)(a)->green << 8) | (a)->green, \
							 (b)->blue	= ((unsigned short)(a)->blue << 8)	| (a)->blue)
#define MacToMoaRGB(a,b)	((b)->red	= (MoaByte)((a)->red >> 8),   \
							 (b)->green = (MoaByte)((a)->green >> 8), \
							 (b)->blue	= (MoaByte)((a)->blue >> 8))

/* Copy between a MoaMmRGBTriple and a Windows COLORREF. Takes pointers.
   (RGB, GetRValue, GetGValue, and GetBValue are defined in windows.h)
*/

#define MoaToWinRGB(a,b)	((b) = RGB((a)->red, (a)->green, (a)->blue))
#define WinToMoaRGB(a,b)	((b)->red	= GetRValue(a), \
							 (b)->green = GetGValue(a), \
							 (b)->blue	= GetBValue(a))

/* Compare two MoaRGBTriples */
#define MoaEqualRGB(a,b)	((a)->red	== (b)->red &&	 \
							 (a)->green == (b)->green && \
							 (a)->blue	== (b)->blue )

/* Make a new index colorSpec */
#define ColorSpec_MakeFromIndex(pSpec, theIndex) \
  {  (pSpec)->type = kMoaMmColorSpec_Index; \
	(pSpec)->color.index = theIndex; }

/* Make a new RGB colorSpec from a Mac RGB value */
#define ColorSpec_MakeFromMacRGB(pSpec, pMacRGBColor) \
  {  (pSpec)->type = kMoaMmColorSpec_RGB; \
	MacToMoaRGB((pMacRGBColor), &(pSpec)->color.rgb); }

/* Make a new RGB colorSpec from a Windows COLORREF */
#define ColorSpec_MakeFromWinRGB(pSpec, winColorRef) \
  {  (pSpec)->type = kMoaMmColorSpec_RGB; \
	WinToMoaRGB((winColorRef), &(pSpec)->color.rgb); }


/* Values for type field of MoaMmColorSpec */
#define kMoaMmColorSpec_Index	0		/* use index field of color spec */
#define kMoaMmColorSpec_RGB 	1		/* use rgb field of color spec */

typedef struct
{
	union
	{
		MoaMmRGBTriple	rgb;		/* rgb color */
		MoaMmColorIndex index;		/* palette index */
	} color;

	MoaByte 			type;
	
} MoaMmColorSpec, *PMoaMmColorSpec;

typedef const MoaMmColorSpec *	ConstPMoaMmColorSpec;

/* ----------------------------------------------------------------------------
/
/	Events
/
/ -------------------------------------------------------------------------- */

typedef MoaLong 	MoaMmEventType;

#define kMoaMmEvent_Idle				0		/* idle (null) event */

/* Mouse events */
#define kMoaMmEvent_MouseDown			1		/* mouse down */
#define kMoaMmEvent_MouseDblClick		2		/* mouse double click */
#define kMoaMmEvent_MouseUp 			3		/* mouse up within clicked-on actor */
#define kMoaMmEvent_MouseUpOutside		4		/* mouse up outside clicked-on actor */ 
#define kMoaMmEvent_RightMouseDown		5		/* right mouse down */
#define kMoaMmEvent_RightMouseDblClick	6		/* right mouse double click */
#define kMoaMmEvent_RightMouseUp		7		/* right mouse up within clicked-on actor */
#define kMoaMmEvent_RightMouseUpOutside 8		/* right mouse up outside clicked-on actor */	

/* Key events */
#define kMoaMmEvent_KeyDown 			9		/* key down */
#define kMoaMmEvent_KeyUp				10		/* key up */
#define kMoaMmEvent_KeyFocusEnter		11		/* actor has just received keyboard focus */
#define kMoaMmEvent_KeyFocusExit		12		/* actor has just lost keyboard focus */

/* Play events */
#define kMoaMmEvent_Step				13		/* playback head has moved to next frame */
#define kMoaMmEvent_SpanBegin			14		/* a sprite span for actor has just started */
#define kMoaMmEvent_SpanEnd 			15		/* a sprite span for actor has just ended */
#define kMoaMmEvent_PlayBegin			16		/* movie has just started playing (author only) */
#define kMoaMmEvent_PlayEnd 			17		/* movie has just stopped playing (author only) */

/* Environment events */
#define kMoaMmEvent_StageWindowOpen 	18		/* stage window opened */
#define kMoaMmEvent_StageWindowClose	19		/* stage window hidden or destroyed */
#define kMoaMmEvent_StageWindowDepth	20		/* stage window and/or buffer has changed in depth */
#define kMoaMmEvent_SoundState			21		/* global sound on/off state has changed */
#define kMoaMmEvent_AssetModified		22		/* asset associated with actor has been modified */

/* 18oct96 JT Additional mouse events. */
#define kMoaMmEvent_MouseEnter		23
#define kMoaMmEvent_MouseExit		24
#define kMoaMmEvent_MouseWithin 	25

/* 20aug98 alx Additional scroll events. */
#define kMoaMmEvent_ScrollToTop			26
#define kMoaMmEvent_ScrollToEnd			27
#define kMoaMmEvent_ScrollToAbsolute	28
#define kMoaMmEvent_ScrollLineUp 		29
#define kMoaMmEvent_ScrollLineDown 		30
#define kMoaMmEvent_ScrollPageUp 		31
#define kMoaMmEvent_ScrollPageDown	 	32

/* 09sep99 map IME event */
#define kMoaMmEvent_IME					33

#define kMoaMmEvent_StageWindowChanged  34		/*explicit message when stage window has changed */

/*	Event masks
	Use these in the eventMask MoaMmXSpriteCaps field for sprite actors.
	To receive events from multiple event classes, "|" your masks together, ie.
	(kMoaMmEventMask_Mouse | kMoaMmEventMask_Key)
 */

typedef MoaUlong		MoaMmEventMask;

#define kMoaMmEventMask_Every			0xFFFFFFFF	/* receive all events */
#define kMoaMmEventMask_None			0x00000000	/* receive no events */
#define kMoaMmEventMask_Idle			0x00000001	/* receive idles (play-time only) */
#define kMoaMmEventMask_Mouse			0x00000002	/* receive all mouse events */
#define kMoaMmEventMask_Key 			0x00000004	/* receive all key and focus events */
#define kMoaMmEventMask_MouseRoll		0x00000008	/* receive the mouse rollover events
													/ kMoaMmEvent_MouseEnter,
													/ kMoaMmEvent_MouseExit,
													/ and kMoaMmEvent_MouseWithin
													*/
#define kMoaMmEventMask_Environment 	0x00000010	/* receive all environment change events */
#define kMoaMmEventMask_Play			0x00000020	/* receive play events */
#define kMoaMmEventMask_Span			0x00000040	/* receive span events */
#define kMoaMmEventMask_Step			0x00000080	/* receive step events */
#define kMoaMmEventMask_Scroll			0x00000100	/* receive scroll events */
#define kMoaMmEventMask_IME				0x00000200	/* receive IME events */

/* The following flags correspond to bits of the modifiers MoaMmKeyInfo field */

#define kMoaMmModifier_None 			0x00000000	/* no modifier keys pressed */
#define kMoaMmModifier_Shift			0x00000001	/* shift key pressed */
#define kMoaMmModifier_Command			0x00000002	/* command (mac) or control (win) pressed */
#define kMoaMmModifier_Option			0x00000004	/* option (mac) or alt (win) pressed */
#define kMoaMmModifier_Alt				0x00000004	/* option (mac) or alt (win) pressed */
#define kMoaMmModifier_Control			0x00000008	/* control key pressed */
	
/* Key info structure */

typedef struct
{
	MoaLong 	character;
	MoaLong 	modifiers;
	MoaLong 	nativeKeyCode;			/* platform-specific */
	MoaLong 	nativeModifiers;		/* platform-specific */
} MoaMmKeyInfo, * PMoaMmKeyInfo;
typedef const MoaMmKeyInfo *	ConstPMoaMmKeyInfo;

/* Event info structure */

typedef struct
{
	MoaMmEventType	type;			/* type of the event */
	MoaLong 		target; 		/* reserved */
	MoaPoint		where;			/* mouse location */
	MoaUlong		when;			/* timestamp (milliseconds) */
	
	union							/* extra data */
	{
		MoaMmKeyInfo	keyInfo;	/* key info for key events */
		
		struct					
		{
			MoaLong 	data1;		/* additional event-specific data */
			MoaLong 	data2;		/* additional event-specific data */
			MoaLong 	data3;		/* additional event-specific data */
			MoaLong 	data4;		/* additional event-specific data */
		} aux;
	} data;
} MoaMmEventInfo;
typedef MoaMmEventInfo *		PMoaMmEventInfo;
typedef const MoaMmEventInfo *	ConstPMoaMmEventInfo;


/* Return values for Sprite actor's Event() method */
#define kMoaMmSprEvent_Handled				TRUE
#define kMoaMmSprEvent_Pass 				FALSE

/* ----------------------------------------------------------------------------
/
/	Xtra asset types
/
/ -------------------------------------------------------------------------- */

/* Actor types */

typedef MoaLong 	MoaMmActorType;

#define kMoaMmActorType_Null				0
#define kMoaMmActorType_Sprite				1
#define kMoaMmActorType_Transition			2


/* PrepareNewContent flags */

typedef MoaLong MoaMmNewFlags;

#define kMoaMmNewFlags_None 			0
#define kMoaMmNewFlags_UiPermitted		1
#define kMoaMmNewFlags_AuthorInvoked	2


/* Asset capabilities */

typedef MoaUlong MoaMmAssetCaps, * PMoaMmAssetCaps;

#define kMoaMmAssetCaps_None					0x00000000
#define kMoaMmAssetCaps_HasMediaEditor			0x00000001
#define kMoaMmAssetCaps_HasPropsEditor			0x00000002
#define kMoaMmAssetCaps_PropsEditorNeedsMedia	0x00000004
#define kMoaMmAssetCaps_HasAboutBox 			0x00000008
#define kMoaMmAssetCaps_CanImageStaticThumb 	0x00000010
#define kMoaMmAssetCaps_StaticThumbNeedsMedia	0x00000020
#define kMoaMmAssetCaps_CanImageAnimThumb		0x00000040
#define kMoaMmAssetCaps_AnimThumbNeedsMedia 	0x00000080
#define kMoaMmAssetCaps_PreloadMedia			0x00000100
#define kMoaMmAssetCaps_LockSpriteToMemberSize	0x00000200

#define kMoaMmAssetCaps_CanLinkMedia			0x00001000
#define kMoaMmAssetCaps_MustLinkMedia			0x00002000
#define kMoaMmAssetCaps_CanStream				0x00004000
#define kMoaMmAssetCaps_StreamsOwnMedia			0x00008000
#define kMoaMmXAssetCapsFlags_SupportsBmFilters	0x00010000

/* Transition actor capabilities */

typedef MoaUlong	MoaMmTransCaps, * PMoaMmTransCaps;

/* If _DoesScreenAccess is set the application copies any windows in front of the
   transition area back into the offscreen buffer before doing the transition.
   Special handling on Mac Powerbooks with mirrored displays is also done.
*/
#define kMoaMmTransCaps_None					0
#define kMoaMmTransCaps_ChangeArea				1		/* supports change area option */
#define kMoaMmTransCaps_ChunkSize				2		/* supports chunk size control */
#define kMoaMmTransCaps_Duration				4		/* supports duration control */
#define kMoaMmTransCaps_CantPaletteInterleave	8		/* can't interleave with palette change */
#define kMoaMmTransCaps_DoesScreenAccess		16		/* draws direct to screen without using operating system */


/* Possible values for "defaultTransArea" registry entry.
	These determine the default state of the "transition area"
	control, assuming kMoaMmTransCaps_ChangeArea is set.
	If kMoaMmTransCaps_ChangeArea is not set, the default is
	automatically forced to kMoaMmTransArea_FullStage.
*/
#define kMoaMmTransArea_FullStage				0
#define kMoaMmTransArea_ChangeArea				1

/* Return values for transition actor's Continue() method */
#define kMoaMmTrans_IsComplete					TRUE
#define kMoaMmTrans_IsNotComplete				FALSE

/* Parameter block for transition actor's Cue(), Continue(), Finish() methods */
typedef struct
{
	MoaLong 	chunkSize;		/* chunkiness */
	MoaLong 	duration;		/* duration (milliseconds) */
	MoaLong 	realTime;		/* non-zero if running in real time,
									0 if frame-by-frame export */
	MoaLong 	elapsedTime;	/* elapsed time (milliseconds) */
	MoaLong 	aux;			/* reserved */
} MoaMmTransInfo;
typedef MoaMmTransInfo * PMoaMmTransInfo;
typedef const MoaMmTransInfo * ConstPMoaMmTransInfo;


/* Sprite actor capabilities */

typedef MoaUlong	MoaMmXSpriteCapsFlags;

#define kMoaMmXSpriteCapsFlags_None 				0
#define kMoaMmXSpriteCapsFlags_IsStatic 			1		/* sprite doesn't want CollectChgs() calls,
																use InvalSpriteRect() to request update */
#define kMoaMmXSpriteCapsFlags_AutoTabEnabled		2		/* app handles tab key */

#define kMoaMmXSpriteCapsFlags_SupportsRotation		0x04		
#define kMoaMmXSpriteCapsFlags_SupportsSkew			0x08		
#define kMoaMmXSpriteCapsFlags_SupportsFlipH 		0x10		
#define kMoaMmXSpriteCapsFlags_SupportsFlipV		0x20		
#define kMoaMmXSpriteCapsFlags_SupportsQuad  		0x40		
#define kMoaMmXSpriteCapsFlags_SupportsAutoBounds	0x80		

#define kMoaMmXSpriteCapsFlags_SupportsBlendInk		0x100	/* Explicitly supports blend ink.  If not set,
															/  blend will be mapped to copy */
#define kMoaMmXSpriteCapsFlags_AllowEventPreProcessing	0x200
															
typedef MoaLong 	MoaMmXSpriteImageMode;

#define kMoaMmXSpriteImageMode_Any					0
#define kMoaMmXSpriteImageMode_Offscreen			1
#define kMoaMmXSpriteImageMode_DirectToScreen		2

typedef struct
{
	MoaMmEventMask			eventMask;
	MoaMmXSpriteCapsFlags	flags;
	MoaMmXSpriteImageMode	requiredImageMode;
	MoaMmXSpriteImageMode	preferredImageMode;
} MoaMmXSpriteCaps, * PMoaMmXSpriteCaps;


/* ----------------------------------------------------------------------------
/
/	Sprite actor info - supplied as parameters to most XActor methods
/
/ -------------------------------------------------------------------------- */

/* Ink modes (raster ops) */
typedef MoaLong 	MoaMmInk, * PMoaMmInk;

#define kMoaMmInk_Copy					0
#define kMoaMmInk_Transparent			1
#define kMoaMmInk_Reverse				2
#define kMoaMmInk_Ghost 				3
#define kMoaMmInk_NotCopy				4
#define kMoaMmInk_NotTransparent		5
#define kMoaMmInk_NotReverse			6
#define kMoaMmInk_NotGhost				7
#define kMoaMmInk_Matte 				8
#define kMoaMmInk_Mask					9
#define kMoaMmInk_Blend 				32
#define kMoaMmInk_AddPin				33
#define kMoaMmInk_Add					34
#define kMoaMmInk_SubtractPin			35
#define kMoaMmInk_BackgroundTransparent 36
#define kMoaMmInk_Lightest				37
#define kMoaMmInk_Subtract				38
#define kMoaMmInk_Darkest				39
#define kMoaMmInk_Lighten 				40
#define kMoaMmInk_Darken 				41


typedef struct
{
	MoaMmColorSpec		fgColor;		/* ink foreground color */
	MoaMmColorSpec		bgColor;		/* ink background color */
	MoaLong 			blendValue; 	/* sprite blend value (v5.0: 0-255) */
	MoaBool 			obeyBlend;		/* if TRUE, use blendValue */
} MoaMmInkParams;
typedef MoaMmInkParams * PMoaMmInkParams;
typedef const MoaMmInkParams * ConstPMoaMmInkParams;
	
	
typedef MoaLong 	MoaMmXSpriteFlags;

#define kMoaMmXSpriteFlags_None 			0
#define kMoaMmXSpriteFlags_Moveable 		4
#define kMoaMmXSpriteFlags_Editable 		8
#define kMoaMmXSpriteFlags_Blend			16
#define kMoaMmXSpriteFlags_ExtendedInfo		32
#define kMoaMmXSpriteFlags_ImageRequired	64
#define kMoaMmXSpriteFlags_FiltersApplied	128

typedef MoaLong 	MoaMmPlayState;

#define kMoaMmPlayState_Stopped 			0
#define kMoaMmPlayState_Playing 			1


typedef MoaLong 	MoaMmSoundState;

#define kMoaMmSoundState_Disabled			0
#define kMoaMmSoundState_Enabled			1


typedef struct
{
	MoaMmPlayState			playState;		/* playing or stopped */
	MoaMmSoundState 		soundState; 	/* sound should be on/off */
} MoaMmPlayerStateInfo;
typedef MoaMmPlayerStateInfo *		 PMoaMmPlayerStateInfo;
typedef const MoaMmPlayerStateInfo * ConstPMoaMmPlayerStateInfo;


/* New parameters for Director */
typedef MoaLong 	MoaMmBlitState;

/* This is set if the author wants Director to use the quad 
/  parameter in the MoaMmBlitParams structure to do the Blit.  If
/  it's not set, the DstRect parameter to the function will be used.
/  Rotation/skew/flipH/flipV are OUTPUT values only, there are 
/  ignored for the Blit2 call. */
#define kMoaMmBlit_UseQuadForBlit		1

typedef struct
{
	MoaFloat				rotation;
	MoaFloat				skew;
	MoaBool					flipH;
	MoaBool					flipV;
	MoaFloat				quad[4][2];
	MoaMmBlitState			flags;

} MoaMmBlitParams;
typedef MoaMmBlitParams * PMoaMmBlitParams;
typedef const MoaMmBlitParams * ConstPMoaMmBlitParams;




typedef struct
{
	MoaRect 				spriteRect; 	/* rect on stage where sprite appears */
	MoaMmXSpriteImageMode	imageMode;		/* imaged direct or to an offscreen buffer */
	MoaMmXSpriteFlags		flags;			/* sprite FX flags (see above) */
	MoaMmInk				ink;			/* sprite ink mode */
	MoaMmInkParams			inkParams;		/* extra params for some inks */
	MoaMmBlitParams			blitParams;		/* extra params for Director blit functionality */
	MoaLong 				aux;			/* reserved */
} MoaMmXSpriteInfo;
typedef MoaMmXSpriteInfo *		PMoaMmXSpriteInfo;
typedef const MoaMmXSpriteInfo *	ConstPMoaMmXSpriteInfo;

typedef struct
 {
	MoaBool					bIsFilterApplied;
	MoaMmInk				ink;
	MoaRect					spriteRect;
	MoaRect					filterRect;
} MoaMmXFilterInfo;
   
typedef MoaMmXFilterInfo FAR*	PMoaMmXFilterInfo;
typedef const MoaMmXFilterInfo * ConstPMoaMmXFilterInfo;





/* ----------------------------------------------------------------------------
/
/	Symbol and Value defintions
/
/ -------------------------------------------------------------------------- */

/* MoaMmSymbols are unique run-time IDs used for properties, asset types, etc.
   They are guaranteed unique only for the current session, so they should never
   be stored on disk or otherwise transferred outside the running application.
   A unique symbol is obtained from a string using the IMoaMmUtils->StringToSymbol()
   method.	Symbols can likewise be converted back to a string using
   IMoaMmUtils->SymbolToString(). Symbol strings are case-insensitive.
 */

typedef MoaLong 			MoaMmSymbol;
typedef MoaMmSymbol *		PMoaMmSymbol;
typedef const MoaMmSymbol * ConstPMoaMmSymbol;


/* Do not access fields of MoaMmValue directly. Use the value conversion
   methods in IMoaMmUtils.
*/

typedef struct
{
	MoaLong 		ty; 				/* Type code */
	MoaLong 		it; 				/* Some datum */
} MoaMmValue;
typedef MoaMmValue *		PMoaMmValue;
typedef const MoaMmValue *	ConstPMoaMmValue;

typedef MoaLong 	MoaMmValueType;

/* These are the values returned by the IMoaMmUtils:ValueType() method.
   ** IMPORTANT: This is not the value for the MoaMmValue.ty **
*/

enum
{
	kMoaMmValueType_Void,
	kMoaMmValueType_Integer,
	kMoaMmValueType_Symbol,
	kMoaMmValueType_String,
	kMoaMmValueType_Object,
	kMoaMmValueType_Picture,
	kMoaMmValueType_Float,
	kMoaMmValueType_List,
	kMoaMmValueType_Point,
	kMoaMmValueType_Rect,
	kMoaMmValueType_PropList,
	kMoaMmValueType_Member,
	kMoaMmValueType_CastLib,
	kMoaMmValueType_Script,
	kMoaMmValueType_Instance,
	kMoaMmValueType_Xtra,
	kMoaMmValueType_Sprite,
	kMoaMmValueType_SoundSprite,
	kMoaMmValueType_Color,
	kMoaMmValueType_Date,
	kMoaMmValueType_Media,
	kMoaMmValueType_Other,
	kMoaMmValueType_3dVector,
	kMoaMmValueType_3dTransform,
	kMoaMmValueType_LAST,

	kMoaMmValueType_ANY = kMoaMmValueType_LAST
};

/* ----------------------------------------------------------------------------
/
/	RegisterWindow constants  (see IMoaMmUtils::MacRegisterWindow())
/
/ -------------------------------------------------------------------------- */

typedef MoaLong 					MoaMmRegWindowFlags;
typedef MoaMmRegWindowFlags *		PMoaMmRegWindowFlags;
typedef const MoaMmRegWindowFlags * ConstPMoaMmRegWindowFlags;

#define kMoaMmRegWindowFlags_None				0
#define kMoaMmRegWindowFlags_AuthorIdle 		1  /* receive idles at author-time */

#define kMoaMmMacEvent_CloseWindow				120 	
	/* Special event passed to your IMoaMmMacEventHandler() when
		the host app wants you to close your window.  Handle
		in the same way you would if the user clicked in the
		goAway box.
	*/

/* ----------------------------------------------------------------------------
/
/	Asset modification flags
/
/ -------------------------------------------------------------------------- */

typedef MoaLong 					MoaMmModifyFlags;
typedef MoaMmModifyFlags *			PMoaMmModifyFlags;
typedef const MoaMmModifyFlags *	ConstPMoaMmModifyFlags;

#define 	kMoaMmModifyFlags_None		0	/* Nothing was modified 		*/
#define 	kMoaMmModifyFlags_Media 	1	/* Media data was modified		*/
#define 	kMoaMmModifyFlags_Props 	2	/* Property data was modified	*/


/* ----------------------------------------------------------------------------
/
/	MacModalDialog- and WinDialogBox-related types
/
/ -------------------------------------------------------------------------- */

typedef MoaLong MoaMmDialogCookie, * PMoaMmDialogCookie;


/* ----------------------------------------------------------------------------
/
/	 Cue Points
/
/ -------------------------------------------------------------------------- */

#define kMoaMmCuePointMaxNameLen 32

typedef struct {
	MoaUlong	nMilliseconds;
	MoaChar 	sName[kMoaMmCuePointMaxNameLen];
} MoaMmCuePoint, * PMoaMmCuePoint;

typedef const MoaMmCuePoint * ConstPMoaMmCuePoint;

typedef struct {
	MoaMmValue	vChannel;
	MoaLong 	nIndex;
	MoaMmCuePoint cuePoint;
} MoaMmCuePointPassedInfo, * PMoaMmCuePointPassedInfo;

typedef const MoaMmCuePointPassedInfo * ConstPMoaMmCuePointPassedInfo;

#define CuePoint_GetChannel(pCuePoint) (&(pCuePoint)->vChannel)
#define CuePoint_GetIndex(pCuePoint) ((pCuePoint)->nIndex)
#define CuePoint_GetName(pCuePoint) ((pCuePoint)->sName)


/* ----------------------------------------------------------------------------
/
/	Scripting Xtra types
/
/ -------------------------------------------------------------------------- */

/* Call info structure - passed to IMoaMmXScript::Call() */

struct MoaMmCallInfo
{
	PMoaVoid				xinst;			/* MoaHandle to XtraInstRec */
	MoaLong 				methodFlags;	/* Reserved */
	MoaMmSymbol 			methodSelector; /* Method index or symbol */
	MoaMmValue				resultValue;	/* slot for result */
	MoaLong 				nargs;			/* Number of arguments passed */
	PMoaMmValue 			pArgs;			/* Pointer to array of args */
};

typedef struct MoaMmCallInfo MoaMmCallInfo;
typedef MoaMmCallInfo * PMoaMmCallInfo;

/* Old names for backward compatability with Director 5.0 XDK. */

#define MoaDrCallInfo	MoaMmCallInfo
#define PMoaDrCallInfo	PMoaMmCallInfo


#define pciGetArgByIndex(pCallInfo, argIndex, pValue) \
		(*(pValue) = (pCallInfo)->pArgs[(argIndex)-1])


/* ----------------------------------------------------------------------------
/
/	Tool Xtra types
/
/ -------------------------------------------------------------------------- */

typedef MoaLong 	MoaMmEnabledState,	* PMoaMmEnabledState;

#define kMoaMmEnabledState_Disabled 		0
#define kMoaMmEnabledState_Enabled			1

#define kMoaMmToolCategory_Unknown			""
	/* Xtra tools that don't specify a category end up with a default */
	

/* ----------------------------------------------------------------------------
/
/	Director-specific registry dictionary keys and types
/
/ -------------------------------------------------------------------------- */


/* For Tool Xtras */

#define kMoaMmDictKey_AlwaysEnabled 			"alwaysEnabled"
#define kMoaMmDictType_AlwaysEnabled			kMoaDictType_Long

/* Old names for backward compatability with Director 5.0 XDK. */
#define MoaDrEnabledState						MoaMmEnabledState
#define PMoaDrEnabledState						PMoaMmEnabledState
#define kMoaDrEnabledState_Disabled 			kMoaMmEnabledState_Disabled 	
#define kMoaDrEnabledState_Enabled				kMoaMmEnabledState_Enabled
#define kMoaDrToolCategory_Unknown				kMoaMmToolCategory_Unknown
#define kMoaDrDictKey_AlwaysEnabled 			kMoaMmDictKey_AlwaysEnabled
#define kMoaDrDictType_AlwaysEnabled			kMoaMmDictType_AlwaysEnabled


/* ----------------------------------------------------------------------------
/
/	Predefined Xtra asset categories
/
/ --------------------------------------------------------------------------- */

/* Although an asset Xtra can register any string as its "displayCategoryString"
   (kMoaMmDictKey_DisplayCategoryString), using one of these predefined categories
   is recommended.	Using the predefined categories helps the user locate asset
   Xtras, by grouping similar types together.  The predefined categories are also
   recognized by the application, and the correct translation is automatically
   substituted in internationalized versions.

   An Xtra should register the most-specific category applicable.  Some
   applications, however, may combine some of the categories into one group.
   For example, Director 5 combines Media Element, Sound, Video and ThreeD in
   the same submenu.
*/

/* Categories for sprite assets */

#define kMoaMmSpriteCategory_Unknown		""
	/* Xtras that don't specify a category end up with a default */

#define kMoaMmSpriteCategory_Media			"Media"
	/* Basic static media elements, such as bitmaps and static text */

#define kMoaMmSpriteCategory_Control		"Control"
	/* User-interface controls, such as pushbuttons and check boxes. */

#define kMoaMmSpriteCategory_Behavior		"Behavior"
	/* Canned behaviors, interactions, or scripts, such as "gravity" */

#define kMoaMmSpriteCategory_Database		"Database"
	/* Database management or connectivity */

#define kMoaMmSpriteCategory_Sound			"Sound"
	/* Any type of audio */

#define kMoaMmSpriteCategory_Video			"Video"
	/* Any type of video or animation */

#define kMoaMmSpriteCategory_ThreeD 		"ThreeD"
	/* Modeling, virtual worlds, etc. */

#define kMoaMmSpriteCategory_Network		"Network"
	/* Network communications and connectivity */


/* Categories for transition assets */


/* ----------------------------------------------------------------------------
/
/	Predefined registry dictionary keys and types
/
/ -------------------------------------------------------------------------- */

#define kMoaMmDictKey_SymbolString				"symbolString"
#define kMoaMmDictType_SymbolString 			kMoaDictType_CString

#define kMoaMmDictKey_DisplayNameString 		"displayNameString"
#define kMoaMmDictType_DisplayNameString		kMoaDictType_CString

#define kMoaMmDictKey_DisplayCategoryString 	"displayCategoryString"
#define kMoaMmDictType_DisplayCategoryString	kMoaDictType_CString

#define kMoaMmDictKey_ColorIcon 				"colorIcon"
#define kMoaMmDictType_ColorIcon				kMoaDictType_Bytes

#define kMoaMmDictKey_BWIcon					"BWIcon"
#define kMoaMmDictType_BWIcon					kMoaDictType_Bytes

#define kMoaMmDictKey_ActorType 				"actorType"
#define kMoaMmDictType_ActorType				kMoaDictType_Long

#define kMoaMmDictKey_AssetCaps 				"assetCaps"
#define kMoaMmDictType_AssetCaps				kMoaDictType_Bytes

#define kMoaMmDictKey_TransitionCaps			"transitionCaps"
#define kMoaMmDictType_TransitionCaps			kMoaDictType_Bytes

#define kMoaMmDictKey_TransDefaultArea			"transitionDefaultArea"
#define kMoaMmDictType_TransDefaultArea 		kMoaDictType_Long

#define kMoaMmDictKey_TransDefaultChunkSize 	"transitionDefaultChunkSize"
#define kMoaMmDictType_TransDefaultChunkSize	kMoaDictType_Long

#define kMoaMmDictKey_TransDefaultDuration		"transitionDefaultDuration"
#define kMoaMmDictType_TransDefaultDuration 	kMoaDictType_Long

#define kMoaMmDictKey_MessageTable				"msgTable"
#define kMoaMmDictType_MessageTable 			kMoaDictType_CString

#define kMoaMmDictKey_SafeForShockwave			"safeForShockwave"
#define kMoaMmDictType_SafeForShockwave			kMoaDictType_Bool

#define kMoaMmDictKey_FilterCaps 				"filterCaps"
#define kMoaMmDictType_FilterCaps				kMoaDictType_Bytes

/* Old names for backward compatability with Director 5.0 XDK. */

#define kMoaDrDictKey_MessageTable				kMoaMmDictKey_MessageTable
#define kMoaDrDictType_MessageTable 			kMoaMmDictType_MessageTable

/* ----------------------------------------------------------------------------
/
/	Error codes (extensions to standard MOA errors)
/
/ -------------------------------------------------------------------------- */

#define 	kMoaMmErr_Base						2000

#define 	kMoaMmErr_NotImplemented			MAKE_MOAERR(kMoaMmErr_Base)
#define 	kMoaMmErr_InterfaceNotFound 		MAKE_MOAERR(kMoaMmErr_Base + 1)
#define 	kMoaMmErr_InternalError 			MAKE_MOAERR(kMoaMmErr_Base + 2)
#define 	kMoaMmErr_XAssetTypeUnknown 		MAKE_MOAERR(kMoaMmErr_Base + 3)
#define 	kMoaMmErr_GCTypeNotSupported		MAKE_MOAERR(kMoaMmErr_Base + 4)

/* Property access errors */

#define 	kMoaMmErr_PropertyNotFound			MAKE_MOAERR(kMoaMmErr_Base + 5)
#define 	kMoaMmErr_CannotSetProperty 		MAKE_MOAERR(kMoaMmErr_Base + 6)
#define 	kMoaMmErr_NoMemForString			MAKE_MOAERR(kMoaMmErr_Base + 7)
#define 	kMoaMmErr_ArgOutOfRange 			MAKE_MOAERR(kMoaMmErr_Base + 8)
#define 	kMoaMmErr_IntegerExpected			MAKE_MOAERR(kMoaMmErr_Base + 9)
#define 	kMoaMmErr_SymbolExpected			MAKE_MOAERR(kMoaMmErr_Base + 10)
#define 	kMoaMmErr_FloatExpected 			MAKE_MOAERR(kMoaMmErr_Base + 11)
#define 	kMoaMmErr_StringExpected			MAKE_MOAERR(kMoaMmErr_Base + 12)
#define 	kMoaMmErr_PointExpected 			MAKE_MOAERR(kMoaMmErr_Base + 13)
#define 	kMoaMmErr_RectExpected				MAKE_MOAERR(kMoaMmErr_Base + 14)

/* CallHandler errors */

#define 	kMoaMmErr_FunctionNotFound			MAKE_MOAERR(kMoaMmErr_Base + 15)
#define 	kMoaMmErr_NoArgExpected 			MAKE_MOAERR(kMoaMmErr_Base + 16)
#define 	kMoaMmErr_OneArgExpected			MAKE_MOAERR(kMoaMmErr_Base + 17)
#define 	kMoaMmErr_TwoArgsExpected			MAKE_MOAERR(kMoaMmErr_Base + 18)
#define 	kMoaMmErr_ThreeArgsExpected 		MAKE_MOAERR(kMoaMmErr_Base + 19)
#define 	kMoaMmErr_FourArgsExpected			MAKE_MOAERR(kMoaMmErr_Base + 20)
#define 	kMoaMmErr_WrongNumberOfArgs 		MAKE_MOAERR(kMoaMmErr_Base + 21)

/* Misc */

#define 	kMoaMmErr_NotRegistered 			MAKE_MOAERR(kMoaMmErr_Base + 22)
#define 	kMoaMmErr_StreamInFailed			MAKE_MOAERR(kMoaMmErr_Base + 23)
#define 	kMoaMmErr_StreamOutFailed			MAKE_MOAERR(kMoaMmErr_Base + 24)
#define 	kMoaMmErr_FreeFailed				MAKE_MOAERR(kMoaMmErr_Base + 25)
#define 	kMoaMmErr_AssetCreationFailed		MAKE_MOAERR(kMoaMmErr_Base + 26)
#define 	kMoaMmErr_GCNotModifiable			MAKE_MOAERR(kMoaMmErr_Base + 27)
#define 	kMoaMmErr_GCNotAvailable			MAKE_MOAERR(kMoaMmErr_Base + 28)
#define 	kMoaMmErr_WinGNotEnabled			MAKE_MOAERR(kMoaMmErr_Base + 29)
#define 	kMoaMmErr_UnknownNotifyMsgCode		MAKE_MOAERR(kMoaMmErr_Base + 30)
#define 	kMoaMmErr_ValueTypeMismatch 		MAKE_MOAERR(kMoaMmErr_Base + 31)

#define 	kMoaMmErr_ColorExpected				MAKE_MOAERR(kMoaMmErr_Base + 32)

#define 	kMoaMmErr_AccessNotSupported		MAKE_MOAERR(kMoaMmErr_Base + 33)
#define 	kMoaMmErr_NotPropReference			MAKE_MOAERR(kMoaMmErr_Base + 34)
#define		kMoaMmErr_UnsafeforShockwave		MAKE_MOAERR(kMoaMmErr_Base + 35)
#define 	kMoaMmErr_NotIMoaMmXValue			MAKE_MOAERR(kMoaMmErr_Base + 36)
#define		kMoaMmErr_VectorValueExpected		MAKE_MOAERR(kMoaMmErr_Base + 37)
#define 	kMoaMmErr_MatrixValueExpected		MAKE_MOAERR(kMoaMmErr_Base + 38)
#define     kMoaMmErr_OperationNotAllowed       MAKE_MOAERR(kMoaMmErr_Base + 39)

/* For backward compatibility with Director 5.0 -- error code must match Director's old error code */
#define 	kMoaMmErr_ToolDisabled				MAKE_MOAERR(3011)
#define 	kMoaDrErr_ToolDisabled				kMoaMmErr_ToolDisabled


/* ----------------------------------------------------------------------------
/
/	Other constants
/
/ -------------------------------------------------------------------------- */

/* Maximum length of an xtra name symbol string in bytes */
#define kMoaMmMaxXtraName				32

/* Maximum length of a property name symbol string in bytes */
#define kMoaMmMaxPropName				32

/* Maximum length of an xtra display name in bytes */
#define kMoaMmMaxXtraDisplayName		64

/* Maximum length of an xtra display category in bytes */
#define kMoaMmMaxXtraDisplayCategory	64

/* Maximum of length of message table processed in authoring */
#define kMoaMmMaxXtraMessageTable		65536


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


#endif	/* !_H_mmtypes */
