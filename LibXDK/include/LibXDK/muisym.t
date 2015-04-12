/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _T_MuiSym
#define _T_MuiSym

/* ================================================================================

	MuiSym.T

	Purpose:
	
	Provides access to XSupport library. To use XSupport, you must link the
	XSupport.lib into your Xtra. Not that XSupport is not available for win16.
	MuiSym manipulation can be used to do quick runtime conversion of muiSymbols
	(which are constant) to lingo symbols, which vary with each execution, and are
	based on non-unique strings. MuiSym.T documents many MuiSyms which are used
	with the c and lingo interfaces for MuiDialog.
	
=================================================================================== */

///////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
///////////////////////////////////////////////////////////////////////////////////

// on WINDOWS, include windows.h before any moa files
#ifdef _WINDOWS
	#ifndef _WINDOWS_
	#include <windows.h>
	#endif
#endif

#include "mmtypes.h"

///////////////////////////////////////////////////////////////////////////////////
//
// Declares
//
///////////////////////////////////////////////////////////////////////////////////

typedef MoaLong TMuiSymbol;

typedef struct
{
	TMuiSymbol 	enumeration;
	char*		string;
} MmSymbolTableEntry;

typedef MmSymbolTableEntry * PMmSymbolTableEntry;
typedef const PMmSymbolTableEntry ConstPMmSymbolTableEntry;

///////////////////////////////////////////////////////////////////////////////////
//
// Defines
//
///////////////////////////////////////////////////////////////////////////////////

const TMuiSymbol kMuiBadSymbol = -1L;


///////////////////////////////////////////////////////////////////////////////////
//
// Our Symbol table
//
///////////////////////////////////////////////////////////////////////////////////

enum
{
	// Property symbols
	kMuiPropTextSize = 0,						// 0
	kMuiPropTextStyle,
	kMuiPropTextAlignment,
	kMuiPropValueRange, 
	kMuiPropValueList,

	kMuiPropPopupStyle,							// 5
	kMuiPropSliderStyle,
	kMuiPropEditStyle,

	// Text Size symbols
	kMuiTextSizeNormal,
	kMuiTextSizeTiny,
	kMuiTextSizeLarge,							// 10

	// Text Styles
	kMuiTextStyleNormal 	= kMuiTextSizeNormal,	// - if a symbol matches another you
	kMuiTextStyleBold 		= 12,					// need to match that enum and restart
	kMuiTextStyleItalic,							// the enumeration + 1
	kMuiTextStyleUnderline,

	// Alignment
	kMuiTextAlignLeft,							// 15
	kMuiTextAlignRight,
	kMuiTextAlignCenter,
	kMuiTextAlignDefault,

	// Popup Styles
	kMuiPopupStyleNormal 	= kMuiTextSizeNormal,
	kMuiPopupStyleTiny 		= kMuiTextSizeTiny,		// 20
	kMuiPopupStyleCramped	= 21,

	// Slider Styles
	kMuiSliderStyleTicks,
	kMuiSliderStyleValue,
	kMuiSliderStyleBoth,

	// Edit text specific symbols
	kMuiEditStyleVScrollbar,					// 25
	kMuiEditStyleHScrollbar,
	kMuiEditStyleBoth		= kMuiSliderStyleBoth,

	// Range specific symbols
	kMuiRangeMinimum 		= 28,
	kMuiRangeMaximum,
	kMuiRangeIncrement,							// 30
	kMuiRangeJump,
	kMuiRangeAcceleration,

	// lingo window ops
	kMuiLingoWindowOpHide,
	kMuiLingoWindowOpShow,
	kMuiLingoWindowOpCenter,					// 35
	kMuiLingoWindowOpZoom,
	kMuiLingoWindowOpTipsOn,
	kMuiLingoWindowOpTipsOff,

	// lingo props
	kMuiLingoPropHeight,
	kMuiLingoPropWidth,							// 40
	kMuiLingoPropData,
	kMuiLingoPropPixel,
	kMuiLingoPropDialogUnit,
	kMuiLingoPropNormal	= kMuiTextSizeNormal,
	kMuiLingoPropPalette = 45,					// 45
	kMuiLingoPropAlert,		
	kMuiLingoPropDefaultWidget,					

	// lingo props for window init list
	kMuiLingoPropType,
	kMuiLingoPropName,
	kMuiLingoPropCallback,						// 50
	kMuiLingoPropMode,
	kMuiLingoPropXPosition,						
	kMuiLingoPropYPosition,
	kMuiLingoPropModal,
	kMuiLingoPropToolTips,						// 55
	kMuiLingoPropCloseBox,
	kMuiLingoPropCanZoom,

	// lingo  props for item prop list
	kMuiLingoPropItemValue	= kMuiSliderStyleValue,
	kMuiLingoPropWidget	= 59,
	kMuiLingoPropAttributes,					// 60
	kMuiLingoPropTitle,
	kMuiLingoPropTip,
	kMuiLingoPropLocH,
	kMuiLingoPropLocV,
	kMuiLingoPropEnabled,						// 65

	// lingo for names of the big lingo lists
	kMui_Lingo_WindowPropList,
	kMui_Lingo_WindowItemList,

	// lingo widget props
	kMuiLingoWidgetUNSUPPORTED,

	// lingo props for widget types, note, none can be out of this range
	kMuiLingoWidgetNone,

	kMuiLingoWidgetVoid_VDivider,				// 70
	kMuiLingoWidgetVoid_VDividerExpander,
	kMuiLingoWidgetVoid_HDivider,
	kMuiLingoWidgetVoid_HDividerExpander,

	kMuiLingoWidget_Bitmap,

	kMuiLingoWidgetBoolean_Checkbox,			// 75
	kMuiLingoWidgetBoolean_RadioButton,

	kMuiLingoWidgetEnum_TriStateCheckbox,
	kMuiLingoWidgetEnum_TriStateRadioButton,

	kMuiLingoWidgetEnum_PopupList,
	kMuiLingoWidgetEnum_EditablePopupList,		// 80
	kMuiLingoWidgetEnum_PopupMenu,		
	kMuiLingoWidgetEnum_ListBox,

	kMuiLingoWidgetChar_EditText,
	kMuiLingoWidgetChar_EditTextVScroll,
	kMuiLingoWidgetChar_EditTextHScroll,		// 85
	kMuiLingoWidgetChar_EditTextVHScroll,

	kMuiLingoWidgetGroup_WindowBegin,
	kMuiLingoWidgetGroup_WindowEnd,
	kMuiLingoWidgetGroup_HBegin,
	kMuiLingoWidgetGroup_HEnd,					// 90
	kMuiLingoWidgetGroup_VBegin,
	kMuiLingoWidgetGroup_VEnd,
	kMuiLingoWidgetGroup_TabBegin,
	kMuiLingoWidgetGroup_TabEnd,
	kMuiLingoWidgetGroup_HScrollbarBegin,		// 95
	kMuiLingoWidgetGroup_HScrollbarEnd,
	kMuiLingoWidgetGroup_VScrollbarBegin,
	kMuiLingoWidgetGroup_VScrollbarEnd,

	kMuiLingoWidgetLabel_Normal,

	kMuiLingoWidgetDragWell_Blank,				// 100
	kMuiLingoWidgetDragWell_Text,
	kMuiLingoWidgetDragWell_Graphic,
	kMuiLingoWidgetDragWell_Color,

	kMuiLingoWidgetLong_HSlider,
	kMuiLingoWidgetLong_VSlider,				// 105
	kMuiLingoWidgetLong_Dial,

	kMuiLingoWidgetFloat_HSlider,
	kMuiLingoWidgetFloat_VSlider,
	kMuiLingoWidgetFloat_Dial,

	kMuiLingoWidgetPoint2d_Picker,				// 110
	kMuiLingoWidgetPoint2d_Vector,
	kMuiLingoWidgetPoint3d_Picker,
	kMuiLingoWidgetPoint3d_Vector,

	kMuiLingoWidgetRect_Sizer,

	kMuiLingoWidgetButton_DefaultPushButton,	// 115
	kMuiLingoWidgetButton_CancelPushButton,
	kMuiLingoWidgetButton_PushButton,
	kMuiLingoWidgetButton_Toggle,
	kMuiLingoWidgetButton_EllipseButton,
	kMuiLingoWidgetButton_Push_Up,				// 120
	kMuiLingoWidgetButton_Push_Down,

	kMuiLingoWidgetButton_NavUp,
	kMuiLingoWidgetButton_NavDown,
	kMuiLingoWidgetButton_NavLeft,
	kMuiLingoWidgetButton_NavRight,				// 125
	kMuiLingoWidgetButton_NavInfo,
	kMuiLingoWidgetButton_NavScript,
	kMuiLingoWidgetButton_NavPlus,
	kMuiLingoWidgetButton_NavPlusPopMenu,
	kMuiLingoWidgetButton_NavMinus,				// 130
	kMuiLingoWidgetButton_NavMinusPopMenu,
	kMuiLingoWidgetButton_NavTrash,

	kMuiLingoWidgetRGB_ColorPicker,
	kMuiLingoWidgetIndex_ColorPicker,

	kMuiLingoWidgetLastWidgetType,				// 135

	// bitmap icons
	kMuiPropBitmapIcon,
	kMuiBitmapIcon_Stop,
	kMuiBitmapIcon_Note,
	kMuiBitmapIcon_Caution,
	kMuiBitmapIcon_Question,					// 140
	kMuiBitmapIcon_Error,

	// lingo alert button styles
	kMuiAlertLingoButtons_Ok,
	kMuiAlertLingoButtons_OkCancel,
	kMuiAlertLingoButtons_AbortRetryIgnore,
	kMuiAlertLingoButtons_YesNoCancel,			// 145
	kMuiAlertLingoButtons_YesNo,	
	kMuiAlertLingoButtons_RetryCancel,	

	// lingo alert init properties
	kMuiAlertLingoButtons,
	kMuiAlertLingoDefault = kMuiTextAlignDefault,
	kMuiAlertLingoTitle	= kMuiLingoPropTitle,	// 150
	kMuiAlertLingoMessage = 151,
	kMuiAlertLingoIcon,
	kMuiAlertLingoMovable,						

	// lingo callback flags
	kMuiLingoEvent_itemChanged,
	kMuiLingoEvent_itemClicked,					// 155
	kMuiLingoEvent_windowOpening,
	kMuiLingoEvent_windowClosed,
	kMuiLingoEvent_windowZoomed,				
	kMuiLingoEvent_windowResized,
	kMuiLingoEvent_ItemEnteringFocus,			// 160
	kMuiLingoEvent_ItemLosingFocus,

	kMuiLingoWidget_Actor,						

	// layout styles
	kMuiLayoutStyle,
	kMuiLayout_LockPosition,
	kMuiLayout_LockSize,						// 165
	kMuiLayout_CenterH,
	kMuiLayout_Left = kMuiTextAlignLeft,		
	kMuiLayout_Right = kMuiTextAlignRight,
	kMuiLayout_CenterV = 169,
	kMuiLayout_Top,								// 170				
	kMuiLayout_Bottom,
	kMuiLayout_Minimize,
	kMuiLayout_MaxSize,


	// classID
	kMuiControlClassID,
	
	kMuiControlPopupEnabledState,

	// Insert new Items HERE

	kMuiSymItemCount							// MUST be last item in list.
};

#endif	// _T_MuiSym

// EOF