/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_Mui
#define _H_Mui

/* ================================================================================

	Mui.h		

	Purpose:
	
	Provides interfaces for using generic XP dialogs that match macromedia
	user interface guidelines. Also provides specific interfaces for internet
	file dialogs, and generic file dialogs.
	
=================================================================================== */

/****************************************************************************
**
** Includes
**
*****************************************************************************/


#ifndef _H_moanotif
#include "moanotif.h"
#endif

#ifndef _H_moastdif
#include "moastdif.h"
#endif

#ifndef _H_MoaStream2_
#include "moastr2.h"
#endif

#ifndef _H_MoaPathName_
#include "moapath.h"
#endif

/* defs of types used in interface methods. */
#ifndef __T_MUIINIT__ 
#include "MuiInit.T"
#endif

/****************************************************************************
**
** Defines
**
*****************************************************************************/

/****************************************************************************
**
**  IMuiDialog
**
*****************************************************************************/

#undef INTERFACE
#define INTERFACE IMuiDialog

DECLARE_INTERFACE_(IMuiDialog, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
	
	/* Service methods */
	STDMETHOD(Initialize) (THIS_
			PTMuiWindow	PWindow,
			MoaLong		iInputArraySize,
			TMuiItem	PInputStrucArray[]
			) PURE;
			
	/* Runs the dialog, if the window is set to be modal this routine blocks */
	STDMETHOD(Run)	(THIS)	PURE;
	
	/* Callback interface needs to call this to stop modality */
	STDMETHOD(Stop)	(THIS_
			MoaError	statusCode
			)	PURE;

	STDMETHOD(SetCallback) (THIS_
			PIMoaNotificationClient	notifyInterface,
			PMoaVoid				clientDataReference
	 		) PURE;
				
	STDMETHOD(WindowOp) (THIS_
			MoaLong		iWindowOp 
			) PURE;

	STDMETHOD(ItemUpdate) (THIS_
			MoaLong		iItemIndex,
			PTMuiItem	PItemInitStructure
			) PURE;

	/* Utility methods */
	STDMETHOD(GetWindowGC) (THIS_ PMoaMmNativeGCInfo pWindowInfo ) PURE;
	STDMETHOD(SetWindowParent) (THIS_ MoaMmWinHdl32 window ) PURE;
};

/* IID_IMuiDialog: 0xEA7478804DE711D08DE400059A20195A */
DEFINE_GUID(IID(IMuiDialog), 0xEA747880L, 0x4DE7, 0x11D0, 0x8D, 0xE4, 0x00, 0x05, 0x9A, 0x20, 0x19, 0x5A);


/****************************************************************************
**
** MuiDlgPersist Interface
**
*****************************************************************************/

#undef INTERFACE

#define INTERFACE IMuiDlgPersist

DECLARE_INTERFACE_(IMuiDlgPersist, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS

	/* Methods to stream the dialog definition in/out of a MoaStream2 */

	STDMETHOD(StreamIn)			(THIS_ PIMoaStream2 pInputStream,
									   PTMuiWindow	pWindowData,
									   PTMuiItem	*ppItemData,		 
									   PMoaLong		pItemArraySize) PURE;
	
	/* description
	
	Tries to construct a Mui Dialog by parsing the inputStream from its current position.
	Returns kMoaErr_NoErr if successful and sets ppWindowData and ppItemData to 
	IMoaCalloc'd pointers to structures describing the Dialog.
	The client is responsible for calling IMoaCalloc:Free to free the returned pointers.
	
	*/

	STDMETHOD(StreamOut)		(THIS_ PIMoaStream2		pOutputStream,
									   ConstPTMuiWindow	pWindowData,
									   ConstPTMuiItem	pItemData,
									   MoaLong			itemCount) PURE;
	
	/* description
	
	Converts the given MuiDialog to a text stream and writes it to the outputStream
	from its current position.
	Returns kMoaErr_NoErr if successful.
	
	*/
	   
	/* Methods to convert native dialog resources */
	STDMETHOD(ConvertNativeDialog)	(THIS_ MoaLong		resourceID,
										   PTMuiWindow	*ppWindowData,
										   PTMuiItem	*ppItemData,			   
										   PMoaLong		pItemArraySize) PURE;

			
	/* description
	
	Tries to construct a Mui Dialog by parsing the native dialog resource.
	Returns kMoaErr_NoErr if successful and sets ppWindowData and ppItemData to 
	IMoaCalloc'd pointers to structures describing the Dialog.
	The client is responsible for calling IMoaCalloc:Free to free the returned pointers.
	
	*/
	
};

/* IID_IMuiDlgPersist: 1a507b40-b74a-11d1-82c0-00a0c9696bf5 */
DEFINE_GUID(IID(IMuiDlgPersist), 0xA7EBAA8AL, 0x78EC, 0x11D0, 0x90, 0x37, 0x00, 0x05, 0x9A, 0x20, 0x19, 0x5A);


/****************************************************************************
**
**  IMuiAlert
**
*****************************************************************************/

#undef INTERFACE
#define INTERFACE IMuiAlert

DECLARE_INTERFACE_(IMuiAlert, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
	
	/* displays the alert (always modal), returns with an error only if the alert failed, returns
	   a non-zero code which is the button clicked, indexed as follows, 1 based, in order of TMuiAlertButtonType,
	   kMuiAlertButtons_YesNo ==> 1 = yes, 2 = no */
	STDMETHOD(Alert) (THIS_
			PMoaChar				message,
			PMoaChar				title,
			TMuiAlertButtonType		buttons,
			MoaLong					defaultButton,	/* 1 based			*/
			TMuiAlertIcon			icon			/* 0 for no icon	*/
			) PURE;
	
	/* Alert functionality, only in a movable-modal dialog. */
	STDMETHOD(MovableAlert) (THIS_
			PMoaChar				message,
			PMoaChar				title,
			TMuiAlertButtonType		buttons,
			MoaLong					defaultButton,	/* 1 based			*/
			TMuiAlertIcon			icon			/* 0 for no icon	*/
			) PURE;
		
};

/* IID_IMuiAlert: 0xAEF7FCD30047B2200000000502A0A813 */
DEFINE_GUID(IID(IMuiAlert), 0xAEF7FCD3L, 0x0047, 0xB220, 0x00, 0x00, 0x00, 0x05, 0x02, 0xA0, 0xA8, 0x13);


/****************************************************************************
**
**  IMuiUrl
**
*****************************************************************************/

#undef INTERFACE
#define INTERFACE IMuiUrl

DECLARE_INTERFACE_(IMuiUrl, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
	
	/* The only method, if pMoaPath is null, then ModalGetUrlPath
	   will create pMoaPath. In either case, the caller owns the interface.  */
	STDMETHOD(ModalGetUrlPath) (THIS_
			PIMoaPathName *	ppMoaPath) PURE;

	STDMETHOD(MovableGetUrlPath) (THIS_
			PIMoaPathName *	ppMoaPath) PURE;
	
};

/* IID_IMuiUrl: 0xAEF7FCD30047B2210000000502A0A813 */
DEFINE_GUID(IID(IMuiUrl), 0xAEF7FCD3L, 0x0047, 0xB221, 0x00, 0x00, 0x00, 0x05, 0x02, 0xA0, 0xA8, 0x13);


/****************************************************************************
**
**  IMuiFile
**
*****************************************************************************/

#undef INTERFACE
#define INTERFACE IMuiFile

DECLARE_INTERFACE_(IMuiFile, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
	
	/* Presents the basic OS file open dialog.
	   If pMoaPath is NULL, one will be allocated which the caller will own.
	   If the dialog is cancelled, the path will be an empty string. */
	STDMETHOD(SimpleFileOpen) (THIS_
			PIMoaPathName *	ppMoaPath, ConstPMoaChar prompt ) PURE;

	/* Presents the basic OS file save dialog.
	   If pMoaPath is NULL, one will be allocated which the caller will own.
	   If the dialog is cancelled, the path will be an empty string. */
	STDMETHOD(SimpleFileSave) (THIS_
			PIMoaPathName *	ppMoaPath, ConstPMoaChar prompt) PURE;
};

/* IID_IMuiFile 0x9A3146C48A6811D098DC000502482E5F */
DEFINE_GUID(IID(IMuiFile), 0x9A3146C4L, 0x8A68, 0x11D0, 0x98, 0xDC, 0x00, 0x05, 0x02, 0x48, 0x2E, 0x5F);


/****************************************************************************
**
**  CLSIDs
**
*****************************************************************************/

/* CLSID_CMui: 5CE7501E4DEC11D0937A00059A20195A	*/
DEFINE_GUID(CLSID(CMui), 0x5CE7501EL, 0x4DEC, 0x11D0, 0x93, 0x7A, 0x00, 0x05, 0x9A, 0x20, 0x19, 0x5A);

/* CLSID_CMuiAlert: AF0A4857003962A10000000502A0A813 */
DEFINE_GUID(CLSID(CMuiAlert), 0xAF0A4857L, 0x0039, 0x62A1, 0x00, 0x00, 0x00, 0x05, 0x02, 0xA0, 0xA8, 0x13);

/* CLSID_CMuiUrl: AF0A4857003962A20000000502A0A813 */
DEFINE_GUID(CLSID(CMuiUrl), 0xAF0A4857L, 0x0039, 0x62A2, 0x00, 0x00, 0x00, 0x05, 0x02, 0xA0, 0xA8, 0x13);

/* CLSID_CMuiFile: 0xD5B0695A8A6811D098DC000502482E5F */
DEFINE_GUID(CLSID(CMuiFile), 0xD5B0695AL, 0x8A68, 0x11D0, 0x98, 0xDC, 0x00, 0x05, 0x02, 0x48, 0x2E, 0x5F);


#endif	/* _H_Mui */

