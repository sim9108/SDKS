/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef __T_MUIINIT__
#define  __T_MUIINIT__

/* ================================================================================

	Purpose:
	
	Primary MuiDialog constants and types declaractions.
	
=================================================================================== */

/*---------------------------------------------------------------------------
//
// Includes
//
---------------------------------------------------------------------------*/

#ifndef _H_mmtypes 
#include "MMTYPES.H"               // Moa types defined.
#endif


// Make sure we are using 4 byte alignment (regardless of Host App)
#ifdef _WINDOWS
#pragma pack(4)
#endif

#ifdef MACINTOSH
#pragma options align=power
#endif


/*---------------------------------------------------------------------------
//
// Enumerations
//
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
//
// IMui supported widget/formatting flags enumerated
// 
//	NOTE
//      If you add/change/reorder this list, make sure to 
//		fix up CMuiLingo::symToEnumWidgetType.
//
//	<textSize>		= #large, #tiny, #normal(default) 
//	<textStyle>		= [#bold, #italic, #underline, #plain (default), #inverse (v2)]
//	<justification>	= #left, #right, #center (defaults to system language standard)
//	<popupStyle>	= #tiny, #cramped, #normal (default)
//	<valueList>		= ["one", #two, 3, 4.0] (list of mmvalues, all coerced to strings)
//	<valueRange>	= [#min:0.0, #max:1000.0, #increment:1.0, #jump:10.0, #acceleration:0.5]
//	<sliderStyle>	= [#ticks, #value]
//	<editStyle>		= #VScrollBar, #HScrollBar, #BothScrollBars
//	<bitmapStyle>	= [#bitmapIcon: #stop/#note/#caution/#question/#error]
//	<layoutStyle>	= [#lockPosition, #lockSize, #minimize, #center, #right, #left, (#top, #bottom ) (v1.1)]	
//
//	UT	= Uses Title
//  All items obey layout styles except group items
//
---------------------------------------------------------------------------*/
enum {
	// widget constant								// #   |    vs  | UT | vAttribute to use
	kMuiWidgetType_None = 0,						// 0   |	1.0	| No | none

	kMuiWidgetType_Void_VDivider,					// 1   |	1.0	| No | none
	kMuiWidgetType_Void_VDividerExpander,			// 2   |		|    | not supported yet
	kMuiWidgetType_Void_HDivider,					// 3   |	1.0	| No | none
	kMuiWidgetType_Void_HDividerExpander,			// 4   |		|    | not supported yet
	
	kMuiWidgetType_Bitmap,							// 5   |	1.0 | No | <bitmapStyle>
	kMuiWidgetType_Actor,							// 6   |	    |    | 

	kMuiWidgetType_Boolean_Checkbox = 10,			// 10  |	1.0	| Yes| <textSize>
	kMuiWidgetType_Boolean_RadioButton,				// 11  |	1.0	| Yes| <textSize>

	kMuiWidgetType_Enum_TriStateCheckbox = 20,		// 20  |		|    | not supported yet
	kMuiWidgetType_Enum_TriStateRadioButton,		// 21  |		|    | not supported yet

	kMuiWidgetType_Enum_PopupList = 30,				// 30  |	1.0	| No | <popupStyle><valueList>
	kMuiWidgetType_Enum_EditablePopupList,			// 31  |	3.0	| No | <popupStyle><valueList>
	kMuiWidgetType_Enum_PopupMenu,					// 32  |	   	|    | not supported yet
	kMuiWidgetType_Enum_ListBox,					// 33  |		|    | supported from D11 onwards

	kMuiWidgetType_Char_EditText = 40,				// 40  |	1.0	| No | <textSize><justification><editStyle>
	kMuiWidgetType_Char_EditTextVScroll,			// 41  |	3.0	| No | <textSize><justification><editStyle>
	kMuiWidgetType_Char_EditTextHScroll,			// 42  |	3.0	| No | <textSize><justification><editStyle>
	kMuiWidgetType_Char_EditTextVHScroll,			// 43  |	3.0	| No | <textSize><justification><editStyle>

	kMuiWidgetType_Group_WindowBegin = 50,			// 50  |	1.0	| No | none
	kMuiWidgetType_Group_WindowEnd,					// 51  |	1.0	| No | none
	kMuiWidgetType_Group_HBegin,					// 52  |	1.0	| No | none
	kMuiWidgetType_Group_HEnd,						// 53  |	1.0	| No | none
	kMuiWidgetType_Group_VBegin,					// 54  |	1.0	| No | none
	kMuiWidgetType_Group_VEnd,						// 55  |	1.0	| No | none
	kMuiWidgetType_Group_TabBegin,					// 56  |		|    | not supported yet
	kMuiWidgetType_Group_TabEnd,					// 57  |		|    | not supported yet
	kMuiWidgetType_Group_HScrollbarBegin,			// 58  |		|    | not supported yet
	kMuiWidgetType_Group_HScrollbarEnd,				// 59  |		|    | not supported yet
	kMuiWidgetType_Group_VScrollbarBegin,			// 60  |		|    | not supported yet
	kMuiWidgetType_Group_VScrollbarEnd,				// 61  |		|    | not supported yet

	kMuiWidgetType_Label_Normal = 70,				// 70  |	1.0	| No | <textSize><justification><editStyle>

	kMuiWidgetType_DragWell_Blank = 80,				// 80  |		|    | not supported yet
	kMuiWidgetType_DragWell_Text,					// 81  |		|    | not supported yet
	kMuiWidgetType_DragWell_Graphic,				// 82  |		|    | not supported yet
	kMuiWidgetType_DragWell_Color,					// 83  |		|    | not supported yet

	kMuiWidgetType_Long_HSlider = 90,				// 90  |	1.0 | No | <sliderStyle><valueRange>
	kMuiWidgetType_Long_VSlider,					// 91  |		|    | not supported yet <sliderStyle><valueRange>
	kMuiWidgetType_Long_Dial,						// 92  |		|    | not supported yet <valueRange>

	kMuiWidgetType_Float_HSlider = 100,				// 100 |	1.0 | No | <sliderStyle><valueRange>
	kMuiWidgetType_Float_VSlider,					// 101 |		|    | not supported yet <sliderStyle><valueRange>
	kMuiWidgetType_Float_Dial,						// 102 |		|    | not supported yet <valueRange>

	kMuiWidgetType_Point2d_Picker = 110,			// 110 |		|    | not supported yet
	kMuiWidgetType_Point2d_Vector,					// 111 |		|    | not supported yet
	kMuiWidgetType_Point3d_Picker,					// 112 |		|    | not supported yet
	kMuiWidgetType_Point3d_Vector,					// 113 |		|    | not supported yet, (trackball)
	
	kMuiWidgetType_Rect_Sizer = 120,				// 120 |		|    | not supported yet

	kMuiWidgetType_Button_DefaultPushButton = 130,	// 130 |	1.0	| Yes| <textSize>
	kMuiWidgetType_Button_PushButton,				// 131 |	1.0	| Yes| <textSize>
	kMuiWidgetType_Button_CancelButton,				// 132 |	1.0	| Yes| <textSize>
	kMuiWidgetType_Button_Toggle,					// 133 |	1.0	| Yes| <textSize>
	kMuiWidgetType_Button_EllipseButton,			// 134 |		|    | not supported yet
	kMuiWidgetType_Button_Push_Up,					// 135 |		|    | not supported yet
	kMuiWidgetType_Button_Push_Down,				// 136 |		|    | not supported yet

	kMuiWidgetType_Button_Nav_Up = 140,				// 140 |		|    | not supported yet
	kMuiWidgetType_Button_Nav_Down,					// 141 |		|    | not supported yet
	kMuiWidgetType_Button_Nav_Left,					// 142 |		|    | not supported yet
	kMuiWidgetType_Button_Nav_Right,				// 143 |		|    | not supported yet
	kMuiWidgetType_Button_Nav_Info,					// 144 |		|    | not supported yet
	kMuiWidgetType_Button_Nav_Script,				// 145 |		|    | not supported yet
	kMuiWidgetType_Button_Nav_Plus,					// 146 |		|    | not supported yet
	kMuiWidgetType_Button_Nav_PlusPopMenu,			// 147 |		|    | not supported yet
	kMuiWidgetType_Button_Nav_Minus,				// 148 |		|    | not supported yet
	kMuiWidgetType_Button_Nav_MinusPopMenu,			// 149 |		|    | not supported yet
	kMuiWidgetType_Button_Nav_Trash,				// 150 |		|    | not supported yet

	kMuiWidgetType_RGB_ColorPicker = 160,			// 160 |	   	|    | none
	kMuiWidgetType_Index_ColorPicker,				// 161 |	   	|    | none

	kMuiWidgetType_CountOfWidgetTypes

};
typedef MoaLong TMuiWidgetType;


/*---------------------------------------------------------------------------
// Layout types
---------------------------------------------------------------------------*/
enum {
	kMuiLayout_Dynamic = 0,
	kMuiLayout_DialogUnit,
	kMuiLayout_Pixel,
	kMuiLayout_Resource,

	kMuiLayout_MAX

}; 
typedef MoaLong TMuiLayoutType;


/*---------------------------------------------------------------------------
// Window types
---------------------------------------------------------------------------*/
enum 
{
	kMuiWindowType_None = 0,	// not implemented	
	kMuiWindowType_Palette,		// not implemented	
	kMuiWindowType_Standard,
	kMuiWindowType_NonMovable	// same as kMuiWindowType_Standard on Windows
};
typedef MoaLong TMuiWindowType;


/*---------------------------------------------------------------------------
// Alert button layouts
---------------------------------------------------------------------------*/
enum 
{
	kMuiAlertButtons_Ok				= 1,		
	kMuiAlertButtons_OkCancel,
	kMuiAlertButtons_AbortRetryIgnore,
	kMuiAlertButtons_YesNoCancel,
	kMuiAlertButtons_YesNo,
	kMuiAlertButtons_RetryCancel,

	kMuiAlertButtons_MAX	// always keep this last
};
typedef MoaLong TMuiAlertButtonType;

/*---------------------------------------------------------------------------
// Alert icons
---------------------------------------------------------------------------*/
enum 
{												// Macintosh	Windows
	kMuiAlertIcon_None				= 0,
	kMuiAlertIcon_Stop				= 1,		// Stop			MB_ICONSTOP, MB_ICONHAND	
	kMuiAlertIcon_Note,							// Note			MB_ICONINFORMATION, MB_ICONASTERISK
	kMuiAlertIcon_Caution,						// Caution		MB_ICONWARNING, MB_ICONEXCLAMATION
	kMuiAlertIcon_Question,						// Note			MB_ICONQUESTION
	kMuiAlertIcon_Error,						// Stop			MB_ICONERROR, MB_ICONHAND

	kMuiAlertIcon_MAX							// always keep this last
};
typedef MoaLong TMuiAlertIcon;

/*---------------------------------------------------------------------------
// Window positions
---------------------------------------------------------------------------*/
enum 
{
	kMuiWindowPosition_None = 0,
	kMuiWindowPosition_Alert,
	kMuiWindowPosition_Centered,
	kMuiWindowPosition_CenterX,		// not implemented
	kMuiWindowPosition_CenterY,		// not implemented
	kMuiWindowPosition_Minimized,	// not implemented
	kMuiWindowPosition_Maximized	// not implemented
};
typedef MoaLong TMuiWindowPosition;


/*---------------------------------------------------------------------------
//
// 	IMui environment events.
//
---------------------------------------------------------------------------*/
typedef enum {
	kMui_WindowOp_Hide = 0,					
	kMui_WindowOp_Show,						
	kMui_WindowOp_Center,					
	kMui_WindowOp_Zoom,						// not supported yet
	kMui_WindowOp_ToolTipsOn,				// not supported yet
	kMui_WindowOp_ToolTipsOff,				// not supported yet

	kMui_WindowOp_CountOfWindowOps			

} TIMui_WindowOp;


/*---------------------------------------------------------------------------
//
// 	MuiDialog NotificationClient events
//
---------------------------------------------------------------------------*/
enum
{
	kMuiDialogItemChanged = 1,		// item = itemChanged,	data = NULL
	kMuiDialogItemClicked,			// item = itemClicked,	data = NULL

	kMuiDialogWindowOpening,		// item = 0,			data = NULL
	kMuiDialogWindowClosed,			// item = 0,			data = NULL
	kMuiDialogWindowResize,			// item = 0,			data = PMoaRect
	kMuiDialogWindowZoom,			// item = 0,			data = PMoaRect
	
	kMuiDialogItemEnteringFocus,	// item = item getting focus, data = NULL
	kMuiDialogItemLosingFocus,		// item = item about to lose focus, data = NULL
	kMuiDialogListSelChanged,		// item = item whose selection changed, data = int* of selected items
	kMuiDialogListDoubleClicked		// item = item which was double clicked, data = int* of selected items
};
typedef MoaLong TMuiDialogEvent;


/*---------------------------------------------------------------------------
//
// 	IMui state flags, stored on object after initialization to indicate
//	whether input and init strucs were valid enough to allow dialog to run.
//
---------------------------------------------------------------------------*/
typedef enum {
	kMui_State_Inited = 0,					// baseline, what the state is set to on object creation
	kMui_State_InitError,					// init structure had one or more errors
	kMui_State_InputError,					// input structure had one or more errors
	kMui_State_OK,							// ok to run

	kMui_State_CountOfStates				// not supported yet

} TIMui_State;


/*---------------------------------------------------------------------------
// File/Url dialog strings
---------------------------------------------------------------------------*/
const MoaLong kMuiFile_MaxStringLength	= 1024;


/*---------------------------------------------------------------------------
// Fwd declarations
---------------------------------------------------------------------------*/
/* forward declare MuiDialog interface pointer	*/
typedef struct IMuiDialog  * PIMuiDialog;

/* forward declare MuiDlgPersist interface ptr	*/
typedef struct IMuiDlgPersist *PIMuiDlgPersist;

/* forward declare MuiUrl interface pointer		*/
typedef struct IMuiUrl * PIMuiUrl;

/* forward declare MuiAlert interface pointer	*/
typedef struct IMuiAlert * PIMuiAlert;

/* forward declare MuiUrl interface pointer		*/
typedef struct IMuiFile * PIMuiFile;



/*---------------------------------------------------------------------------
//
// Structures used in communicating with IMuiDialog
//
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
// MuiEventRecord structure
---------------------------------------------------------------------------*/
typedef struct
{
	MoaLong			itemHit;
	TMuiDialogEvent event;
	PIMuiDialog		pDialog;
	MoaLong			reserved;
} TMuiEventRecord;

typedef TMuiEventRecord * PTMuiEventRecord;

typedef const TMuiEventRecord *ConstPTMuiEventRecord;

/*---------------------------------------------------------------------------
//	Mui Window structure
---------------------------------------------------------------------------*/

typedef struct
{
	MoaLong				iStructSize;	// always keep first, use to determine vs. of struct, = sizeof(TMuiWindow)	
	/* 8-byte stuff goes first (doubles, etc.)

	/* 4-byte stuff next (longs, ptrs) */
  	TMuiWindowType		iType;			// window type
  	ConstPMoaChar		pTitle;			// name of window, set to NULL for none
	TMuiLayoutType		iLayout;		// flags method to use when laying out dlog, TIMui_LayoutType
	TMuiWindowPosition	iPosition;		// if this is non zero iXPosition and iYPosition may be ignored
	MoaLong				iXPosition;		// position of upper left of window, from upper left of dialog, NULL = Center
	MoaLong				iYPosition;		// position of upper left of window, from upper left of dialog, NULL = Center
	MoaLong				iWidth;			// pixel width of window, NULL = automatic
	MoaLong				iHeight;		// pixel height of window, NULL = automatic

	/* 2-byte stuff now (short, unichars) */

	/* single-byte stuff here: chars, bools, bytes go last. */
 	MoaBool				bModal;				// whether or not dialog is modal
	MoaBool				bToolTipsOn;		// flag for whether or not to INITIALLY use tooltips	
	MoaBool				bHasCloseBox;		// flag for whether or not dialog has close box
	MoaBool				bCanZoom;			// whether or not window zooms
} TMuiWindow, * PTMuiWindow;

typedef const TMuiWindow *ConstPTMuiWindow;

/*---------------------------------------------------------------------------
//
//	Mui Item  structure
//		Contains the information on how to display/use the value provided
//		to draw/service UI
//
---------------------------------------------------------------------------*/
typedef struct
{
	MoaLong			iStructSize;	// always keep first, use to determine vs. of struct, = sizeof(TMuiWindow)	
	/* 8-byte stuff goes first (doubles, etc.)

	/* 4-byte stuff next (longs, ptrs) */
	MoaMmValue		vValue;			// initial value/data to be modified
	TMuiWidgetType	iType;			//	contextual, if data driven, a control TIMui_WidgetType
									//				if DU driven, a control TIMui_WidgetType
									//				if Pixel driven, a control TIMui_WidgetType
									//				if resource driven, a resource ID
	MoaMmValue		vAttributeList;	// 	contextual, if enum list of valid values
									// 				if int/float list with min 1st, and optionally, max 2nd
									// 				if a label, a list with #center, #right, and #left for justification.
	ConstPMoaChar 	pTitle;			//	widget title, NULL no title
	ConstPMoaChar 	pToolTip;		//	string with tool tip to display when floating over, NULL = not tip
	MoaLong			iXPosition;		// position of upper left of item, from upper left of dialog
	MoaLong			iYPosition;		// position of upper left of item, from upper left of dialog
	MoaLong			iWidth;			// pixel width of item, NULL = automatic
	MoaLong			iHeight;		// pixel height of item, NULL = automatic

	/* 2-byte stuff now (short, unichars) */

	/* single-byte stuff here: chars, bools, bytes go last. */
	MoaBool			bEnabled;		//	the UI information related to value
						
} TMuiItem, * PTMuiItem;

typedef const TMuiItem *ConstPTMuiItem;

typedef MoaLong TMuiSymbol;


/* ----------------------------------------------------------------------------
/
/	Mui-specific error codes (extensions to shared Mm errors)
/	These will only be returned from Mui interfaces.
/
/ -------------------------------------------------------------------------- */

#define		kMuiErr_Base						0x0390

/*
** MUI DIALOG Xtra has been allocated 16 errors by Steven Johnson
** so don't go over kMuiErr_Base + 15 
*/

#define		kMuiErr_DialogCanceledWithCloseBox	MAKE_MOAERR(kMuiErr_Base	+ 00)	
#define		kMuiErr_DialogCanceledWithESC		MAKE_MOAERR(kMuiErr_Base	+ 01)
#define		kMuiErr_DialogWrongModality			MAKE_MOAERR(kMuiErr_Base	+ 02)
#define		kMuiErr_FileDialogCancelled			MAKE_MOAERR(kMuiErr_Base	+ 03)


/*-------------------------------------------------------------------------*/


#ifdef _WINDOWS
#pragma pack()
#endif

#ifdef MACINTOSH
#pragma options align=reset
#endif


/*-------------------------------------------------------------------------*/
#endif // __T_MUIINIT__
