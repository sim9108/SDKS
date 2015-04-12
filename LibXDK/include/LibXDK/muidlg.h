/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

/* ================================================================================

	MuiDlg.h
	
	Purpose:
	
	C++ Wrapper around MuiDialog.
	
================================================================================ */

//////////////////////////////////////////////////////////////////////////////////
//
// Description  
//
///////////////////////////////////////////////////////////////////////////////////

/*
	This file provides a C++ wrapper for MuiDialog.  To use this you must first
	create a list of items using the item wrappers (MuiItemWrapper) and pass it 
	to the constructor of MuiDialogWrapper.  The MuiDialogWrapper stores a 
	TMuiWindow structure, as seen in MuiInit.T. Familiarization with this structure
	will make most of the wrapper calls obvious (their accessors to data in this
	structure.) The rest of the methods are wrappers on interface calls which
	can been seen in Mui.H.
	
	This provides exception safe usage of MuiDialog.  See MuiDialog example 
	in XDK for usage.
*/


#ifndef _H_MuiDlg
#define _H_MuiDlg

///////////////////////////////////////////////////////////////////////////////////
//
// Includes  
//
///////////////////////////////////////////////////////////////////////////////////

#ifndef _H_Mui
#include "Mui.h"
#endif

#ifndef _H_MuiItem
#include "MuiItem.h"
#endif

class MuiNotify;
typedef MuiNotify * PMuiNotify;

//////////////////////////////////////////////////////////////////////////////////
//
// Declaration  
//
///////////////////////////////////////////////////////////////////////////////////

class MuiDialogWrapper
{
	public:
		MuiDialogWrapper( TMuiWindowType type, TMuiLayoutType layout, PMuiItemWrapper items[], MoaLong itemCount, ConstPMoaChar title = NULL );
		~MuiDialogWrapper( void );

		// Control Methods
		MoaError 		Run( void );
		void			Stop( MoaError stopCode ) const;

		MoaError		ShowHide( MoaBool show ) const;
		MoaError		Center( void ) const;
		
		MoaError		ItemUpdate( MoaLong index );
		
		// accessors
		PMuiNotify		GetCallback( void ) const;
		void 			SetCallback( PMuiNotify );

		PIMuiDialog		GetDialog( void ) const;
		PMuiItemWrapper*GetItemArray( void ) const;
		MoaLong 		GetItemCount( void ) const;

		// window information accessors
		void			SetParameters( MoaLong x, MoaLong y, MoaLong width, MoaLong height );

		TMuiWindowType  GetWindowType( void ) const;
		TMuiLayoutType  GetLayoutType( void ) const;

		TMuiWindowPosition GetWindowPosition( void ) const;
		void			   SetWindowPosition( TMuiWindowPosition );

		MoaLong			GetX( void ) const;
		void			SetX( MoaLong );
		
		MoaLong			GetY( void ) const;
		void			SetY( MoaLong );
		
		MoaLong			GetWidth( void ) const;
		void			SetWidth( MoaLong );

		MoaLong			GetHeight( void ) const;
		void			SetHeight( MoaLong );

		MoaBool			GetModal( void ) const;
		void			SetModal( MoaBool );

		MoaBool			GetCanZoom( void ) const;
		void			SetCanZoom( MoaBool );

		MoaBool			GetHasCloseBox( void ) const;
		void			SetHasCloseBox( MoaBool );

		ConstPMoaChar	GetTitle( void ) const;
		void			SetTitle( ConstPMoaChar );

		void			ShowHideTooltip( MoaBool show );

		
		// get a single item like its your own
		MuiItemWrapper& GetItem( MoaLong ) const;
		
	protected:
		PIMuiDialog			mDialog;
		PMuiItemWrapper*	mItemArray;
		MoaLong				mItemCount;
		TMuiWindow			mDialogParams;
		PMuiNotify			mNotify;
};

typedef MuiDialogWrapper * PMuiDialogWrapper;
typedef const PMuiDialogWrapper ConstPMuiDialogWrapper;


#endif 	// _H_MuiDlg

// EOF
