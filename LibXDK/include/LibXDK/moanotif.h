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
		Moa notification callbacks interface definitions.
*/

#ifndef	NO_PRAGMA_ONCE
#pragma	once
#endif

#ifndef	MOANOTIF_H
#define	MOANOTIF_H


#include "moaxtra.h"	 	/* MOA shell */
#include "moastdif.h"		/* MOA standard interfaces */
#include "mmtypes.h"		/* multimedia types */


#ifdef	__cplusplus
extern "C" {
#endif


typedef MoaID MoaNotifyID, * PMoaNotifyID;
typedef const MoaNotifyID * ConstPMoaNotifyID;


/* ----------------------------------------------------------------------------
/
/  IMoaNotificationClient
/
/ -------------------------------------------------------------------------- */
DEFINE_GUID(IID_IMoaNotificationClient, 0xE58EE2CCL, 0x3D86, 0x11D0, 0x90, 0xAC, 
										0x00, 0x05, 0x9A, 0x80, 0xE8, 0x2F);
#undef INTERFACE
#define INTERFACE IMoaNotificationClient
DECLARE_INTERFACE_(IMoaNotificationClient, IMoaUnknown)
/* Description
<p>
The IMoaNotificationClient interface must be supported by any Xtra that wishes to recieve
notification of specific certain event by either the host app, or by other xtras that want
to generate system-wide events.  An Xtra wanting notification will obtain an <tt> IMoaNotificationClient</tt> 
Interface to itself and an <tt>IMoaNotification</tt> interface from the main callback object of
the application. it then registers the client interface with the callback interface.
*/
{
	STD_IUNKNOWN_METHODS
	STDMETHOD(Notify) 		(THIS_ 
			ConstPMoaNotifyID	nid, /* The event type you are being notified of */
			PMoaVoid		pNData,  /* event-type specific data decribing the event that occured */
			PMoaVoid		pRefCon) /* the refcon value passed in when the client registered for this event */
			PURE;			

/* Category
Notification
*/

/* Description
The host application calls the Notify method of a registered Notification Client whenever the 
event type that was passed in at registration time occurs.  For each event type, additional
data is passed in the pNData parameter that further describes the event that occured.
*/

};

typedef IMoaNotificationClient FAR* PIMoaNotificationClient;


/* ----------------------------------------------------------------------------
/
/  IMoaNotificationClient
/
/ -------------------------------------------------------------------------- */

DEFINE_GUID(IID_IMoaNotification, 0xCA015FDAL, 0x3D86, 0x11D0, 0x81,
				 0x25, 0x00, 0x05, 0x9A, 0x80, 0xE8, 0x2F);

#undef INTERFACE
#define INTERFACE IMoaNotification
DECLARE_INTERFACE_(IMoaNotification, IMoaUnknown)
{
/* Description
<p>
The IMoaNotification interface is supported by any Moa Application that wants to provide 
system wide notification hooks to xtras.  These notifications are mainly provided by the 
application, but can also be provided by specific Xtras.  This Interface can be obtained by
Querying for it off of the main Application callback Interface.
*/
	STD_IUNKNOWN_METHODS
	STDMETHOD(RegisterNotificationClient) 		(THIS_ 
			PIMoaNotificationClient				pClient, /* the client to notified when the event occurs */
			ConstPMoaNotifyID					nid,	 /* the id for the event you want to be notified for */
			ConstPMoaVoid						pNData,  /* additional event-specific data describing when you want to be notified */
			ConstPMoaVoid						pRefCon) /* a refcon pointer that will be passed back to you when you are notified */
			PURE;
/* Category
Notification
*/

/* Description
Registers a <tt>IMoaNotificationClient</tt> interface to receive notification whenever a 
specific type of event occurs.  The type of event is specified by nid.  With most event types,
 can further specialize the kind of notification you receive by passing event-specific data
 in the pNData parameter.
*/
	STDMETHOD(UnregisterNotificationClient)		(THIS_ 
			PIMoaNotificationClient				pClient,
			ConstPMoaNotifyID					nid,
			ConstPMoaVoid						pNData)
			PURE;
/* Category
Notification
*/

/* Description
Unregisters an <tt>IMoaNotificationClient</tt> interface that was previously registered by 
a call to <tt>RegisterNotificationClient.</tt>  The type of event to unregister for is specified
by nid.  If the client was registered using specific data passed in the pNData parameter, you
must pass similar data in to the pNData parameter to unregister the client.
*/
	STDMETHOD(SendNotification)					(THIS_
			ConstPMoaNotifyID					nid,
			ConstPMoaVoid						pNData)
			PURE;	
/* Category
Notification
*/

/* Description
Sends a notification call to all <tt>IMoaNotificationClients</tt> registered for the event
specified by nid.  Generally, an xtra would call this only to send events that it defines,
not events specified by the host app.  any data passed in the pNData parameter will be passed
on to the client interfaces in the pNData parmaeter of the <tt>Notify</tt> method.
*/
			
};

typedef IMoaNotification FAR* PIMoaNotification;

#ifdef	__cplusplus
}
#endif


#endif	/* MOANOTIF_H */
