/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_MuiItem
#define _H_MuiItem

/* ================================================================================

	Purpose:
	
	Provides access to XSupport library. To use XSupport, you must link the
	XSupport.lib into your Xtra. Not that XSupport is not available for win16.
	Header for C++ objects to create, manipulate, and destroy items in an itemArray
	used in conjunction with muidialog.
	
=================================================================================== */

///////////////////////////////////////////////////////////////////////////////////
//
// Description  
//
///////////////////////////////////////////////////////////////////////////////////

/*

	This file provides wrappers for all of the supported widget types. These give
	you a simple way to create dialogs programmatically.  You need to use these
	wrappers in order to use MuiDialogWrapper.  See the example in the XDK for usage.

	Each widget object you create will wrap around that data for one MuiDialog
	item in your item array, which you pass into the dialog on initialization.
	See MuiInit.T for a full understanding of the data that is being created/modified
	with the wrapper objects methods described below. The structures created
	are TMuiItems.
*/



///////////////////////////////////////////////////////////////////////////////////
//
// Includes  
//
///////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
#include "MuiHelpr.h"
#endif

// NOTE: If OLDROUTINENAMES if defined Apple declares GetItem and SetItem as macros.
#if defined( MACINTOSH ) && OLDROUTINENAMES
	#ifdef GetItem
		#undef GetItem
	#endif
	#ifdef SetItem
		#undef SetItem
	#endif
#endif

///////////////////////////////////////////////////////////////////////////////////
//
// Constants  
//
///////////////////////////////////////////////////////////////////////////////////

// Button Styles, yes it is yet another enumeration
enum
{
	kMuiItemNormal = 0,
	kMuiItemToggle,			// Toggle style button
	kMuiItemCancel,			// This button is selected when user hits Escape or Command-Period
	kMuiItemDefault			// This button is selected when user hits Enter or Return 
};
typedef MoaShort TMuiHelpButtonStyle;

enum
{
	kMuiDirectionVertical   = 1,
	kMuiDirectionHorizontal = 0
};
typedef MoaBool TMuiDirection;

///////////////////////////////////////////////////////////////////////////////////
//
// MuiItemHelp  
//
///////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus


class MuiItemWrapper
{
	protected:
		friend class MuiDialogWrapper;
		
		virtual TMuiItem& 	GetItemRef( void );
		
	public:
		MuiItemWrapper( TMuiItem pItem[], MoaLong index, TMuiWindowType type, MoaLong x, MoaLong y, 
					MoaLong width = 0, MoaLong height = 0, MoaBool enabled = TRUE );

		// for derived classes
		MuiItemWrapper( TMuiItem pItem[], MoaLong index );

		virtual ~MuiItemWrapper( void );
		
		virtual void 			SetParameters( MoaLong x, MoaLong y, MoaLong width = 0, MoaLong height = 0, MoaBool enabled = TRUE );
		
		virtual MoaLong 		GetIndex( void ) const;
		
		virtual TMuiWidgetType  GetType( void ) const;
		virtual void			SetType( TMuiWidgetType );

		virtual const PTMuiItem	GetItem( void ) const;
		virtual void			SetItem( PTMuiItem );	// replaces contents
		
		virtual MoaLong			GetX( void ) const;
		virtual void			SetX( MoaLong );
		
		virtual MoaLong			GetY( void ) const;
		virtual void			SetY( MoaLong );
		
		virtual MoaLong			GetWidth( void ) const;
		virtual void			SetWidth( MoaLong );

		virtual MoaLong			GetHeight( void ) const;
		virtual void			SetHeight( MoaLong );

		virtual MoaBool			GetEnabled( void );
		virtual void			SetEnabled( MoaBool );

		virtual ConstPMoaChar	GetTitle( void );
		virtual void			SetTitle( ConstPMoaChar );

		virtual ConstPMoaChar	GetTooltip( void ) const;
		virtual void			SetTooltip( ConstPMoaChar );
		
		virtual MoaMmValue& 	GetMmValue( void ) const;
		virtual void			SetValue( const MoaMmValue& );

		virtual MoaBitmap 		GetBitmapValue( void ) const;
		virtual void			SetValue( MoaBitmap );

		virtual MoaBool 		GetBoolValue( void ) const;
		virtual void			SetValue( MoaBool );

		virtual MoaDouble 		GetDoubleValue( void ) const;
		virtual void			SetValue( MoaDouble );

		virtual MoaLong 		GetIntValue( void ) const;
		virtual void			SetValue( MoaLong );
		
		// default argument is there to maintain compatibility with older less wise code!
		virtual void 			GetStringValue( PMoaChar someStringPtr, MoaUlong bufLength = 0 ) const;
		virtual void			SetValue( PMoaChar );

		
		virtual MoaMmValue& 	GetAttributes( void ) const;
		virtual void			SetAttributes( const MoaMmValue& );

		// cast operator calls GetValue
		virtual operator MoaMmValue& ( void ) const;
		virtual operator PMoaMmValue ( void ) const;
				
	private:
		TMuiItem&	 	mItem;
		MoaLong			mIndex;
};
typedef MuiItemWrapper * PMuiItemWrapper;

class MuiItemWindowBegin : public MuiItemWrapper
{
	public:
		MuiItemWindowBegin( TMuiItem pItem[], MoaLong index );
};

class MuiItemWindowEnd : public MuiItemWrapper
{
	public:
		MuiItemWindowEnd( TMuiItem pItem[], MoaLong index );
};

class MuiItemTabBegin : public MuiItemWrapper
{
	public:
		MuiItemTabBegin( TMuiItem pItem[], MoaLong index );
};

class MuiItemTabEnd : public MuiItemWrapper
{
	public:
		MuiItemTabEnd( TMuiItem pItem[], MoaLong index );
};

class MuiItemGroupBegin : public MuiItemWrapper
{
	public:
		MuiItemGroupBegin( TMuiItem pItem[], MoaLong index, TMuiDirection dir );
};

class MuiItemGroupEnd : public MuiItemWrapper
{
	public:
		MuiItemGroupEnd( TMuiItem pItem[], MoaLong index, TMuiDirection dir );
};




class MuiItemButton : public MuiItemWrapper
{
	public:
		MuiItemButton( TMuiItem pItem[], MoaLong index, ConstPMoaChar title, MoaBool tiny = FALSE, TMuiHelpButtonStyle buttonType = 0 );
};


class MuiItemCheckbox : public MuiItemWrapper
{
	public:
		MuiItemCheckbox( TMuiItem pItem[], MoaLong index, ConstPMoaChar title, MoaBool selected = FALSE );
};


class MuiItemRadio : public MuiItemWrapper
{
	public:
		MuiItemRadio( TMuiItem pItem[], MoaLong index, ConstPMoaChar title, MoaBool selected = FALSE );
};


class MuiItemText : public MuiItemWrapper
{
	public:
		MuiItemText( TMuiItem pItem[], MoaLong index, ConstPMoaChar title, MoaBool editable, MoaBool tiny = FALSE, TMuiSymbol align = kMuiTextAlignDefault );
		virtual ~MuiItemText( void );
		
		// overrides
		virtual ConstPMoaChar 	GetTitle( void );
		virtual void			SetTitle( ConstPMoaChar );
	
	protected:
		PMoaChar		mTextString;
};

class MuiItemDivider : public MuiItemWrapper
{
	public:
		MuiItemDivider( TMuiItem pItem[], MoaLong index, TMuiDirection dir );
};

class MuiItemBitmap : public MuiItemWrapper
{
	public:
		MuiItemBitmap( TMuiItem pItem[], MoaLong index, MoaShort iconID, PMoaBitmap bitmap = NULL );
};

class MuiItemSlider : public MuiItemWrapper
{
	public:
		MuiItemSlider( TMuiItem pItem[], MoaLong index, MoaLong min, MoaLong max, MoaLong initial, 
					MoaBool showTicks = FALSE, MoaBool showValue = FALSE, MoaLong increment = 0, MoaDouble accel = 0.0f );

		MuiItemSlider( TMuiItem pItem[], MoaLong index, MoaDouble min, MoaDouble max, MoaDouble initial, 
					MoaBool showTicks = FALSE, MoaBool showValue = FALSE, MoaDouble increment = 0, MoaDouble accel = 0.0f );
};

class MuiItemPopup : public MuiItemWrapper
{
	public:	
		// Note: when the popup is editable it's value is just like that of edit text (the text)
		// to retrieve the value index use the GetPopupValueIndex method, this will do the right
		// thing for you.
		MuiItemPopup( TMuiItem pItem[], MoaLong index,  MoaBool tiny = FALSE, MoaBool editable = FALSE );

		MoaError 			AppendItems( PMoaChar stringArray[], MoaLong count = 1 );
		MoaError 			AppendItems( MoaLong intArray[], MoaLong count = 1);
		MoaError 			AppendItems( MoaDouble doubleArray[], MoaLong count = 1 );
		/*	Extract a value out of the popups list of values, given an index into the
			list. Useful if you don't want to store a separate list of values, or if the
			list is changing while the dialog is running. */
		MoaError 			GetPopupValueIndex( MoaLong& index );
		
		// Overrides
		MoaBool		GetEnabled( void );
		void		SetEnabled( MoaBool );
};
typedef MuiItemPopup * PMuiItemPopup;

class MuiItemListBox : public MuiItemWrapper
{
	public:	
		// Note: when the popup is editable it's value is just like that of edit text (the text)
		// to retrieve the value index use the GetPopupValueIndex method, this will do the right
		// thing for you.
		MuiItemListBox( TMuiItem pItem[], MoaLong index,  MoaBool tiny = FALSE, MoaBool editable = FALSE );

		MoaError 			AppendItems( PMoaChar stringArray[], MoaLong count = 1 );
		MoaError			DeleteItems(MoaLong intArray[], MoaLong count = 1);
		/* Get selected items */
		MoaError 			GetSelection( MoaLong intArray[], MoaLong* count );
		
		// Overrides
		//MoaBool		GetEnabled( void );
		//void		SetEnabled( MoaBool );
};
typedef MuiItemListBox * PMuiItemListBox;

#endif	// __cplusplus


#ifdef __cplusplus
extern "C" {
#endif

// Dimensions, parameters -- Use this before or after creating item to set parameters.
MoaError MuiSetParameters( PTMuiItem pItem, MoaLong x, MoaLong y, 
					MoaLong width, MoaLong height, MoaBool enabled, ConstPMoaChar tooltip = NULL );

// Groups, delimiters -- No destroy method for these
MoaError MuiNewWindowBegin( PTMuiItem pItem );
MoaError MuiNewWindowEnd( PTMuiItem pItem );

MoaError MuiNewTabBegin( PTMuiItem pItem );
MoaError MuiNewTabEnd( PTMuiItem pItem );

MoaError MuiNewGroupBegin( PTMuiItem pItem, MoaBool vertical );
MoaError MuiNewGroupEnd( PTMuiItem pItem, MoaBool vertical );


// Buttons -- default param is for Default/Cancel/Toggle item
MoaError MuiNewButton( PTMuiItem pItem, ConstPMoaChar title, MoaBool tiny, TMuiHelpButtonStyle buttonType );
MoaError MuiDestroyButton( PTMuiItem pItem );

// Checkbox
MoaError MuiNewCheckbox( PTMuiItem pItem, ConstPMoaChar title, MoaBool selected );
MoaError MuiDestroyCheckbox( PTMuiItem pItem );

// Radio
MoaError MuiNewRadio( PTMuiItem pItem, ConstPMoaChar title, MoaBool selected );
MoaError MuiDestroyRadio( PTMuiItem pItem );

// Text
MoaError MuiNewText( PTMuiItem pItem, ConstPMoaChar title, MoaBool editable, MoaBool tiny, TMuiSymbol align );
MoaError MuiDestroyText( PTMuiItem pItem );

// Dividers
MoaError MuiNewDivider( PTMuiItem pItem, MoaBool vertical );
MoaError MuiDestroyDivider( PTMuiItem pItem );

// Bitmaps
MoaError MuiNewBitmap( PTMuiItem pItem, MoaShort iconID, PMoaBitmap bitmap );
MoaError MuiDestroyBitmap( PTMuiItem pItem, PMoaBitmap bitmap );

// Sliders
MoaError MuiNewIntegerSlider( PTMuiItem pItem, MoaLong min, MoaLong max, MoaLong initial, 
						MoaBool showTicks, MoaBool showValue, MoaLong increment, MoaDouble accel );
MoaError MuiNewFloatSlider( PTMuiItem pItem, MoaDouble min, MoaDouble max, MoaDouble initial, 
						MoaBool showTicks, MoaBool showValue, MoaDouble increment, MoaDouble accel );

MoaError MuiDestroySlider( PTMuiItem pItem );

// Popup menus
MoaError MuiNewPopup( PTMuiItem pItem, MoaBool tiny );
MoaError MuiNewEditablePopup( PTMuiItem pItem, MoaBool tiny );

MoaError MuiPopupAddStrings( PTMuiItem pItem, PMoaChar stringArray[], MoaLong count );
MoaError MuiPopupAddIntegers( PTMuiItem pItem, MoaLong intArray[], MoaLong count );
MoaError MuiPopupAddDoubles( PTMuiItem pItem, MoaDouble doubleArray[], MoaLong count );
MoaError MuiPopupGetIndexInRange( PTMuiItem pItem, MoaLong* index );

MoaError MuiPopupSetEnableState( PTMuiItem pItem, MoaBool enabledState );
MoaBool  MuiPopupGetEnableState( PTMuiItem pItem );

MoaError MuiDestroyPopup( PTMuiItem pItem );

//list box
MoaError MuiNewListBox( PTMuiItem pItem );
MoaError MuiListBoxAddStrings( PTMuiItem pItem, PMoaChar stringArray[], MoaLong count );
MoaError MuiListDeleteItems( PTMuiItem pItem, MoaLong intArray[], MoaLong count );
MoaError MuiListBoxGetSelection( PTMuiItem pItem, MoaLong intArray[],  MoaLong* count );

MoaError MuiDestroyListBox( PTMuiItem pItem );

// end all -- this one is basically a switch statement
MoaError MuiDestroyItem( PTMuiItem );

#ifdef __cplusplus
}
#endif


#endif 	// _H_MuiItem

// EOF
