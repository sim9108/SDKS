/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_MuiNotif
#define _H_MuiNotif

/* ================================================================================

	Purpose:
	
	MuiNotif provides a c++ wrapper for the standard interface clients using 
	MuiDialog must provide in order to be notified of changes/events occuring
	within their dialog.
	
=================================================================================== */

///////////////////////////////////////////////////////////////////////////////////
//
// Description  
//
///////////////////////////////////////////////////////////////////////////////////

/*

	This file provides wrappers for the notification object.  To use this simply
	override this class and the 6 pure virtual functions and pass that object to
	MuiDialogWrapper as the callback.
	
	Please see example in XDK for usage.
	
*/


///////////////////////////////////////////////////////////////////////////////////
//
// Includes  
//
///////////////////////////////////////////////////////////////////////////////////

#include "MuiItem.h"
#include "MuiDlg.h"

///////////////////////////////////////////////////////////////////////////////////
//
// MuiNotify Declaration  
//
///////////////////////////////////////////////////////////////////////////////////

class MuiNotify
{
	public:
		MuiNotify( PMuiDialogWrapper refCon );
		virtual ~MuiNotify( void );
		
		// PURE VIRTUALS - You must override these
		virtual MoaBool HandleClick( PMuiDialogWrapper, MoaLong ) = 0;
		virtual MoaBool HandleChanged( PMuiDialogWrapper, MoaLong ) = 0;
		virtual MoaBool HandleShowHide( PMuiDialogWrapper, MoaBool show ) = 0;
		virtual MoaBool HandleZoom( PMuiDialogWrapper, MoaBool zoomIn ) = 0;
		virtual MoaBool HandleResize( PMuiDialogWrapper, PMoaRect newSize ) = 0;
		virtual MoaBool HandleFocus( PMuiDialogWrapper, MoaLong, MoaBool enteringFocus ) = 0;
		
		// accessors
		virtual PMuiDialogWrapper GetDialogRef( void ) const;
		
	private:
		// this is so that MuiDialogWrapper can call GetCallback();
		friend class MuiDialogWrapper;

		PIMoaNotificationClient GetCallback( void ) const;
		
		PIMoaNotificationClient	mCallback;
		PMuiDialogWrapper		mDialogRef;
	public:
		// added to XSupport in D11
		//HandleSelectionChange function is called when user changes the selection in the list box
		//HandleDoubleClick is called when user double clicks on an item in the list box
		virtual MoaBool HandleSelectionChange( PMuiDialogWrapper, MoaLong, void* pSelectionArray ) = 0;
		virtual MoaBool HandleDoubleClick( PMuiDialogWrapper, MoaLong, void* pSelectionArray ) = 0;
};

typedef MuiNotify * PMuiNotify;
typedef const MuiNotify ConstPMuiNotify;



///////////////////////////////////////////////////////////////////////////////////
//
// CMuiWedge Declaration  
//
///////////////////////////////////////////////////////////////////////////////////

// CMuiWedge 0xEBE0808885E611D0813A000502482E5F
DEFINE_GUID(CLSID( CMuiNotifyWedge ), 0xEBE08088L, 0x85E6, 0x11D0, 0x81, 0x3A, 0x00, 0x05, 0x02, 0x48, 0x2E, 0x5F);


//---------------------------------------------------------------------------------
//  CLASS INSTANCE VARIABLES
//---------------------------------------------------------------------------------

EXTERN_BEGIN_DEFINE_CLASS_INSTANCE_VARS( CMuiNotifyWedge )
EXTERN_END_DEFINE_CLASS_INSTANCE_VARS


//---------------------------------------------------------------------------------
//  CLASS INTERFACE(S)
//---------------------------------------------------------------------------------

EXTERN_BEGIN_DEFINE_CLASS_INTERFACE( CMuiNotifyWedge, IMoaNotificationClient )
	EXTERN_DEFINE_METHOD( MoaError, Notify,  (THIS_ ConstPMoaNotifyID, PMoaVoid, PMoaVoid) )
EXTERN_END_DEFINE_CLASS_INTERFACE

#endif 	// _H_MuiNotif

// EOF
