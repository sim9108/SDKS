/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef NO_PRAGMA_ONCE
#pragma once
#endif

/****************************************************************************
/
/ Purpose
/	Director services callback interface definitions.
/	Director-specific common interface defintions.
/
****************************************************************************/

#ifndef DRISERVC_H
#define DRISERVC_H


#ifdef PRECOMPILED_HEADER
	#error "moaxtra.h should not be precompiled"
#endif


#include "drtypes.h"
#include "mmiservc.h"
#include "MIXSND.H"


#ifdef	__cplusplus
extern "C" {
#endif


struct IMoaDrScoreAccess;
struct IMoaDrMovie;

/* ----------------------------------------------------------------------------
/
/	Director specific notification types
/
/ -------------------------------------------------------------------------- */

DEFINE_GUID(NID_DrNCastMemberModified, 0x010840D0L, 0x3E12, 0x11D0, 0x99,
							0xA6, 0x00, 0x05, 0x9A, 0x80, 0xE8, 0x2F);
DEFINE_GUID(NID_DrNCastSelectionChanged, 0x03F13356L, 0x3E12, 0x11D0, 0x99,
							0xA6, 0x00, 0x05, 0x9A, 0x80, 0xE8, 0x2F);
DEFINE_GUID(NID_DrNScoreModified, 0x065EBFA0L, 0x3E12, 0x11D0, 0x99, 0xA6, 0x00,
							0x05, 0x9A, 0x80, 0xE8, 0x2F);
DEFINE_GUID(NID_DrNScoreSelectionChanged, 0x092315BAL, 0x3E12, 0x11D0, 0x99, 0xA6,
							0x00, 0x05, 0x9A, 0x80, 0xE8, 0x2F);
DEFINE_GUID(NID_DrNCuePointPassed, 0x66C0FB00L, 0x46C9, 0x11D0, 0xBD, 0xEB, 0x00, 0x05,
							0x9A, 0x80, 0xE8, 0x2F);

DEFINE_GUID(NID_DrNPaletteChanged, 0x0FF83488L, 0x3E12, 0x11D0, 0x99, 0xA6, 0x00,
							0x05, 0x9A, 0x80, 0xE8, 0x2F);
DEFINE_GUID(NID_DrNStep, 0x12ABC686L, 0x3E12, 0x11D0, 0x99, 0xA6, 0x00, 0x05,
							0x9A, 0x80, 0xE8, 0x2F);
DEFINE_GUID(NID_DrNIdle, 0x3B881B5EL, 0x3E12, 0x11D0, 0x99, 0xA6, 0x00, 0x05,
							0x9A, 0x80, 0xE8, 0x2F);

DEFINE_GUID(NID_DrNDocumentOpened, 0x90366D2CL, 0x5CB2, 0x11D0, 0xA1, 0xBD, 0x00, 0x05,
							0x9A, 0x80, 0xE8, 0x2F);
DEFINE_GUID(NID_DrNDocumentClosed, 0xA7B849FCL, 0x5CB2, 0x11D0, 0xA1, 0xBD, 0x00, 0x05,
							0x9A, 0x80, 0xE8, 0x2F);							
DEFINE_GUID(NID_DrNAnimationState, 0x8338a8ea, 0x7ae8, 0x11d3, 0xb3, 0xdd, 0x40, 0x2, 
							0xcf, 0x0, 0x0, 0x0);

DEFINE_GUID(NID_DrNRewind,
			0x1c155800, 0xab70, 0x11d4, 0xa6, 0x15, 0x0, 0x1, 0x2, 0x67, 0x22, 0x1a);

DEFINE_GUID(NID_DrNEnteringDebugger, 0x403a1c31, 0x2a8, 0x11d5, 0xa6, 0x5, 0x0, 0x1, 0x2, 
			0x75, 0xa8, 0x85);

DEFINE_GUID(NID_DrNLeavingDebugger, 0x403a1c33, 0x2a8, 0x11d5, 0xa6, 0x5, 0x0, 0x1, 0x2, 
			0x75, 0xa8, 0x85);

//{95A08D02-4F2A-11B2-A62D-003065838ECE}
DEFINE_GUID(NID_DrNStageWindowOpen,
			0x95A08D02, 0x4F2A, 0x11B2, 0xa6, 0x2D, 0x00, 0x30, 0x65, 0x83, 0x8E, 0xCE);
			
//{1B8E63BD-501E-11B2-9434-003065838ECE}
DEFINE_GUID(NID_DrNStageWindowClose,
			0x1B8E63BD, 0x501E, 0x11B2, 0x94, 0x34, 0x00, 0x30, 0x65, 0x83, 0x8E, 0xCE);			
			
/* ----------------------------------------------------------------------------
/
/	IMoaDrMediaOwner
/
/ -------------------------------------------------------------------------- */

/* IID_IMoaDrMediaOwner: AC542D520003AEED00000800072C6326 */
DEFINE_GUID(IID_IMoaDrMediaOwner, 0xAC542D52L, 0x0003, 0xAEED, 0x00,
				0x00, 0x08, 0x00, 0x07, 0x2C, 0x63, 0x26);

#undef INTERFACE
#define INTERFACE IMoaDrMediaOwner

DECLARE_INTERFACE_(IMoaDrMediaOwner, IMoaMmPropOwner)

/* Description

The IMoaDrMediaOwner interface adds three methods to the
IMoaMmPropOwner interface, AttachMedia(),
GetMedia(), and SetMedia().  These methods enable an Xtra
to retrieve media information from objects such as cast members and movies.

Methods of this interface include pointers to a MoaDrMediaInfo
structure as parameters. Before a GetMedia(), SetMedia(), or
AttachMedia() call, use IMoaDrUtils::NewMediaInfo() to fill
out the structure, specifying NULL for the aux field and
kMoaDrMediaOpts_None for the options field. Using this call
forces you to fill out all the needed parameters.  The aux information
currently applies only when setting image media.

The formatSymbol and labelSymbol members of this structure
are MoaMmSymbols.	These symbols are obtained from strings using
the host application symbol dictionary, accessed through the
StringToSymbol() method of the IMoaMmUtils interface

The labelSymbol refers to the specific chunk of media data you want
from the media owner.  These labels are generic descriptors for the content
media.	In the IMoaDrCastMem interface, which inherits from
IMoaDrMediaOwner, the labels used are different than the cast member
types.	For example, a film loop cast member has score media label, a bitmap
cast member has an image media label, and so on. These generic labels are used
to allow multiple representations for specific media data types.  For example,
a bitmap is really just one type of image.	

The formatSymbol refers to the specific media data type you want to
get or set.  In Director, the media types represented by these sy mbols are
largely platform-specific, except for opaque handles and strings.  For example,
a bitmap cast member provides image media.	On the Macintosh, the format for
getting and setting this data is a Macintosh PICT.	You use the
formatSymbol "macPICT" to access this data.  On Windows,
packed DIBs are supported; you use the formatSymbol "winDIB"
to access this data.

This mechanism uses symbols rather than hardwired constants for future
expansion of media access support.	The goal is to allow Xtra media assets to
export arbitrary labels and formats of their media to other Xtras.	(Currently,
Xtras can only access media data of other Xtras through the opaque handle
provided by composite media. label.  So, for example, in the future, someone
may make a 3D plug-in, and other Xtras could use GetMedia() to get
the "geometry" (label) media in "DXF" (format)).
*/

{
	STD_IUNKNOWN_METHODS

	STDMETHOD(GetProp)(THIS_
		MoaMmSymbol symbol,  /* MoaMmSymbol representing the property
	to get */
		PMoaMmValue pPropValue)  /* Pointer to a MoaMmValue to receive
	the property's value */
		PURE;

/* Category
Property owner methods
*/

/* Returns
<TABLE BORDER="2">
<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=235>kMoaErr_NoErr
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Property exists and
value was returned
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=235>kMoaErr_BadParam
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Invalid <FONT SIZE=2
FACE="Courier New">pPropValue </FONT>passed in
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=235>kMoaMmErr_PropertyNotFound
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Property isn't supported by this
class
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=235>kMoaMmErr_InternalError
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Property exists but couldn't get due
to internal err
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=235>kMoaMmErr_NoMemForString
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Couldn't allocate memory for string
value data
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=235>kMoaErr_OutOfMem
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Couldn't allocate memory for other
value data
</TD></TR>
</TABLE>
*/

/* Description

Retrieves the value of a specified property.
*/

	STDMETHOD(SetProp)(THIS_
		MoaMmSymbol symbol,  /* MoaMmSymbol representing the property
	to set */
		ConstPMoaMmValue pPropValue)  /* Pointer to a ConstPMoaMmValue
	containing the property value */
		PURE;

/* Category
Property owner methods
*/

/* Returns
<TABLE BORDER="2">
<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=253>kMoaErr_NoErr
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Property exists and value was set
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=253>kMoaErr_BadParam
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Invalid <FONT SIZE=2
FACE="Courier New">pPropValue </FONT>passed in
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_PropertyNotFound
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Property isn't supported by this class
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_InternalError
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Property exists, value ok, couldn't -
- internal error
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_NoMemForString
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Couldn't allocate memory for
setting string value data
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=253>kMoaErr_OutOfMem
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Couldn't allocate memory for
setting other value data
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_IntegerExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: integer value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_SymbolExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: symbol value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_FloatExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: float value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_StringExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: string value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_PointExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: point value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_RectExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch:
MoaRect value expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_ValueTypeMismatch
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: other value
expected (non-specific)
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaDrErr_CastMemberExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch:
CMRef expected
</TD></TR>
</TABLE>
*/

/* Description

Sets the value of a specified property.
*/

	STDMETHOD(GetMedia)(THIS_
		PMoaDrMediaInfo pMediaInfo)
		PURE;

/* Category
Media owner methods
*/

/* Description

Gets media data.  This method obtains a copy of the media associated with
an object. Before	making this call, use IMoaDrUtils::NewMediaInfo()
to fill out the media information structure, specifying NULL for the
auxfield and kMoaDrMediaOpts_None for the options
field. The caller partially populates a MoaDrMediaInfo struct with
symbols indicating the requested chunk of media (labelSymbol) and
the requested format (formatSymbol).  After the call, the
mediaDatafield is populated with the requested data; the type of
this field depends on the format requested.  The caller then owns the data
and is responsible for disposing it, if applicable.  Typically this data is
either a MoaHandle, a Macintosh Handle or Pointer, or a Windows
global handle. 
*/

	STDMETHOD(SetMedia)(THIS_
		PMoaDrMediaInfo pMediaInfo)
		PURE;

/* Category
Media owner methods
*/

/* Description

Sets media data.  This method copies caller-supplied media data and associates
it with an object. Before making this call, use IMoaDrUtils::NewMediaInfo()
to fill out the media information structure, specifying NULL for the aux field
and kMoaDrMediaOpts_None for the optionsfield.  The caller
populates a MoaDrMediaInfo structure with symbols indicating the
supplied chunk of media (labelSymbol), the supplied format
(formatSymbol), and the media data itself (mediaData).
If the label and format are supported by the object, a call to this method
copies the caller's data and replaces any existing media data for the supplied
label for the cast	member.  Since the data is copied, the caller retains
ownership of the media data passed in.	Typically, this data is either a
MoaHandle, a Macintosh Handle or pointer, or a Windows global handle. 
*/

	STDMETHOD(AttachMedia)(THIS_
		PMoaDrMediaInfo pMediaInfo)
		PURE;

/* Category
Media owner methods
*/

/* Description

Attaches media to an object, releasing it from the caller. Before making
this call, use IMoaDrUtils::NewMediaInfo() to fill out the media
information structure, specifying NULL for the auxfield and
kMoaDrMediaOpts_Nonefor the options field. 

On enter, the labelSymbol and formatSymbol fields should
be populated with symbols indicating which chunk of media is to be attach
(labelSymbol), and what format the media is supplied in
(formatSymbol).  The mediaData field should hold the data
itself (typically a MoaHandle , Macintosh Handle, or Windows global
handle) 

Upon return, if there is no error, the media has changed ownership and
belongs to the host application, and should no longer be referenced by the
caller.
*/

};

typedef IMoaDrMediaOwner * PIMoaDrMediaOwner;


/* Old synonyms for IMoaDrMediaOwner */
#define IMoaDrPropAndMediaOwner 	IMoaDrMediaOwner
#define PIMoaDrPropAndMediaOwner	PIMoaDrMediaOwner
#define IID_IMoaDrPropAndMediaOwner IID_IMoaDrMediaOwner


/* ----------------------------------------------------------------------------
/
/	IMoaDrNotificationClient - interface for receiving
/		notification from the host application.  The type
/		of notification to be received is specified by the
/		clientType.  See DRTYPES.H for the notification
/		type constants available.
/
/ --------------------------------------------------------------------------- */

/* IID_IMoaDrNotificationClient: ACE09C3101BEE5D2000008000757DC04 */
DEFINE_GUID(IID_IMoaDrNotificationClient, 0xACE09C31L, 0x01BE, 0xE5D2, 0x00,
				0x00, 0x08, 0x00, 0x07, 0x57, 0xDC, 0x04);

#undef INTERFACE
#define INTERFACE IMoaDrNotificationClient

DECLARE_INTERFACE_(IMoaDrNotificationClient, IMoaUnknown)

/* Description

Interface for receiving notification from the host application.  The type of
notification to be received is specified by the notifyType.  See "Types" for
the notification type constants available.	See
IMoaDrUtils::RegisterNotificationClient() and
IMoaDrUtils::UnregisterNotificationClient() for information on
enabling and disabling callback notification for internal events.

The IMoaDrNotificationClient interface consists of the Notify()
method.
*/

{
	STD_IUNKNOWN_METHODS

	STDMETHOD(Notify)(THIS_
		MoaLong msgCode,  /* The MoaLong specifying the type of event
that occurred */
		PMoaVoid data)
		PURE;

/* Description

Handles notification from host application.  msgCode
specifies the type of event that occurred; a given notificationType
can support one or more message codes.	refCon is used to
pass additional message-specific data from the host application to the
notification client.  

Valid message codes are:

kMoaDrMsgCode_DocFileIO		<b>Windows(TM) version only</b>

Used by notification type kMoaDrNotifyType_DocFileIO.	This
notification message is sent just before a chunk read or write occurs from
or to a host application document (a movie or cast file, specifically).
For this message, refCon contains a PMoaChar
pointing to a C string containing the pathname of the filename being read
to or written from.
*/

};

typedef IMoaDrNotificationClient * PIMoaDrNotificationClient;

/* ----------------------------------------------------------------------------
/
/  IMoaDrMediaAccess - Generic media interface
/
/  This interface contains the core methods supported by all
/  MediaAccess classes.  MediaAccess objects, such as ScoreAccess, handle
/  communication with the media owner object to provide a task-specific
/  interface for accessing its media.
/
/ -------------------------------------------------------------------------- */

/* IID_IMoaDrMediaAccess: AC401A980000C62600000800072C6326 */
DEFINE_GUID(IID_IMoaDrMediaAccess, 0xAC401A98L, 0x0000, 0xC626, 0x00, 0x00,
				0x08, 0x00, 0x07, 0x2C, 0x63, 0x26);


#undef INTERFACE
#define INTERFACE IMoaDrMediaAccess

DECLARE_INTERFACE_(IMoaDrMediaAccess, IMoaUnknown)

/* Description

This interface contains the core methods supported by all MediaAccess
classes.  MediaAccess objects, such as ScoreAccess, handle
communication with the media owner object to provide a task-specific interface
for accessing its media.

MediaAccess is typically buffered; the  MediaAccess interface
operates on a copy of media data owned by an associated
IMoaDrMediaOwner object.  Changes are only reflected in the associated
owner object when Commit() is called.	Using SetAccessInfo(),
the owner object can be changed on-the-fly, allowing you to obtain media data
from one object, modify it, and commit it back to a different object.

In Director 5.0, the only type of MediaAccess interface supported is
IMoaDrScoreAccess, used for editing score data associated with a
filmloop cast member or Director movie.  Director API provides three ways to
obtain a MediaAccess interface. 

<ul><ul>	IMoaDrMovie::GetScoreAccess() - This method returns a score access
interface for the Director movie.</ul></ul>
<ul><ul>	IMoaDrCastMem::GetScoreAccess() - This method returns a score access
interface for a filmloop cast member</ul></ul>
<ul><ul>	IMoaDrPlayer::NewScoreAccess() - This method creates a new score
access interface without an owner.	To save the resulting score to a filmloop
cast member or movie, use IMoaDrScoreAccess::SetOwner() or
MoaDrMediaAccess::SetAccessInfo() to set the owner before
committing.</ul></ul>
*/

{
	STD_IUNKNOWN_METHODS

	STDMETHOD(New)(THIS)
		PURE;

/* Category
Creating
*/

/* Description

Obtains the current accessInfo for the media accessor.
pAccessInfo is a pointer to a MoaDrAccessInfo
structure to receive the information.  This call populates the structure with
the PIMoaDrMediaOwner for the media owner, the label symbol for the
media being accessed, and the format symbol for the media being accessed.
Since an interface is being supplied in this structure (pOwner), the
caller is responsible for releasing it.
*/

	STDMETHOD(Refresh)(THIS)
		PURE;

/* Category
Editing
*/

/* Description

Disposes of the current working media and obtains a fresh copy of the
media data from the associated media owner object.	If there is no media owner,
the call fails and kMoaDrErr_OwnerNotSet is returned.	This call
causes you to lose any edits made to your media data since the last
Refresh() or Commit() calls.	
*/

	STDMETHOD(Commit)(THIS)
		PURE;

/* Category
Editing
*/

/* Description

Commits changes made to the media being accessed back to the object
owning the media data.	Changes are not reflected in the media-owning object
unless this method is called.  For example, after creating a sequence of score
frames using IMoaDrScoreAccess, calling Commit()updates the
corresponding movie or film loop cast member.  If no media owner has been set,
this call fails and returns kMoaDrErr_OwnerNotSet.  The media owner is
set automatically if the IMoaDrScoreAccess interface was obtained
directly from the owner using a GetScoreAccess() call.  If you have
created a new ScoreAccess object from scratch (using
IMoaDrPlayer::NewScoreAccess()), it is up to you to associate the
media owner object using SetAccessInfo() (or SetOwner() in
IMoaDrScoreAccess).  The owner is the object that gets updates when
Commit() is called
*/

	STDMETHOD(GetAccessInfo)(THIS_
		PMoaDrAccessInfo pAccessInfo)  /* Pointer to MoaDrAccessInfo  structure to receive information */
		PURE;

/* Category
Access Information
*/

/* Description

Obtains the current accessInfo for the media accessor.
pAccessInfo is a pointer to a MoaDrAccessInfo
structure to receive the information.  This call populates the structure with
the PIMoaDrMediaOwner for the media owner, the label symbol for the
media being accessed, and the format symbol for the media being accessed.
Since an interface is being supplied in this structure (pOwner), the
caller is responsible for releasing it.
*/

	STDMETHOD(SetAccessInfo)(THIS_
		ConstPMoaDrAccessInfo pAccessInfo)	/* Pointer to a MoaDrAccessInfo structure to receive the information */
		PURE;

/* Category
Access Information
*/

/* Description

Sets the current accessInfo for the media accessor.
pAccessInfo is a pointer to a MoaDrAccessInfo
structure containing the new access information to be used.  This call
sets a new media owner, media label, and media format for the media being
accessed.  If a mediaOwner is already set, the media accessor's hold
on the previous owner interface is released.  The caller owns the owner
interface (pOwner) provided in the structure.	

You may change the accessInfo during an editing session.  For example,
you could obtain an IMoaDrScoreAccess interface for a film loop cast
member, edit the score, set the access information to different film loop cast
member (or movie), and call Commit().	This leaves the original film
loop untouched, and updates the new filmloop or movie with the edited version
of the source film loop score.
*/

};

typedef IMoaDrMediaAccess * PIMoaDrMediaAccess;



/* ----------------------------------------------------------------------------
/
/  IMoaDrCastMem
/
/ -------------------------------------------------------------------------- */

/* IID_IMoaDrCastMem: AC401AB50000CD0300000800072C6326 */
DEFINE_GUID(IID_IMoaDrCastMem, 0xAC401AB5L, 0x0000, 0xCD03, 0x00, 0x00, 0x08,
				0x00, 0x07, 0x2C, 0x63, 0x26);


#undef INTERFACE
#define INTERFACE IMoaDrCastMem

DECLARE_INTERFACE_(IMoaDrCastMem, IMoaDrMediaOwner)

/* Description

The IMoaDrCastMem interface provides access to specific
cast members within a cast.

Cast member properties

IMoaDrCastMem inherits from the IMoaMmPropOwner
interface, providing for access to data through the properties
mechanism described in the &quot;Multimedia Services&quot; chapter
in the Development Guide document. See the &quot;Properties&quot;
section for information on the properties defined for objects
providing the IMoaDrCastMem interface. 


Cast member media types


Because this interface inherits from IMoaDrMediaOwner,
you can use it to access the properties and media data associated
with a cast member.

Methods of the IMoaDrMediaOwner interface include pointers
to a MoaPrMediaInfo structure as parameters. Before a
GetMedia(), SetMedia(), or AttachMedia()
call, use IMoaDrUtils::NewMediaInfo() to fill out the
structure, specifying NULL for the aux field and kMoaDrMediaOpts_None
for the options field. Using this call forces you
to fill out all the needed parameters. The aux information
currently applies only when setting image media.

The formatSymbol and labelSymbol members of
this structure are MoaMmSymbols. These symbols are obtained
from strings using the host app symbol dictionary, accessed through
the StringToSymbol() method of the IMoaMmUtils
interface For more on how this mechanism is implemented, see the
IMoaDrMediaOwner.

Here's a brief summary of the media labels supported for Director
cast members:

<table border="2">
<tr><th align="left" valign="middle" width="105"><b>Media</b></th>
<th align="left" valign="middle"><b>Label Description</b></th></tr>

<tr><td align="left" valign="top">Composite</td>
<td align="left" valign="top">Cast member media data in a portable (byte-swapped) opaque
handle.</td></tr>

<tr><td align="left" valign="top">Image</td>
<td align="left" valign="top">Primary image data (for a bitmap, PICT, and so on). </td></tr>

<tr><td align="left" valign="top">Text</td>
<td align="left" valign="top">Text character string</td></tr>

<tr><td align="left" valign="top">TextStyles</td>
<td align="left" valign="top">Text style run data</td></tr>

<tr><td align="left" valign="top">Sound</td>
<td align="left" valign="top">Sound samples</td></tr>

<tr><td align="left" valign="top">Palette</td>
<td align="left" valign="top">Palette entries</td></tr>

<tr><td align="left" valign="top">Score</td>
<td align="left" valign="top">Score data for a movie or film loop</td></tr>
</table>


The following table provides a brief summary of the media formats
supported for Director cast members.

<table border="2">
<tr><th align="left" valign="middle" width="125"><b>Media Format</b></th>
<th align="left" valign="middle"> <b>Description</b></th></tr>

<tr><td align="left" valign="top">moaHandle</td>
<td align="left" valign="top">Generic MoaHandle of data</td></tr>

<tr><td align="left" valign="top">moaTEStyles</td>
<td align="left" valign="top">textStyles stored in a MoaHandle</td></tr>

<tr><td align="left" valign="top">macTEStyles</td>
<td align="left" valign="top">textStyles in TextEdit StScrpHandle format stored in a
Macintosh handle</td></tr>

<tr><td align="left" valign="top">macPICT</td>
<td align="left" valign="top">Macintosh  PicHandle</td></tr>

<tr><td align="left" valign="top">macSnd</td>
<td align="left" valign="top">Macintosh  sndHdl. Handle in Macintosh sound resource format.
</td></tr>

<tr><td align="left" valign="top">macColorTable</td>
<td align="left" valign="top">Macintosh CTabHandle . Handle to a ColorTable
record</td></tr>

<tr><td align="left" valign="top">winDIB</td>
<td align="left" valign="top">Windows packed DIB GlobalHandle with bitmap information, color
table, and bits</td></tr>

<tr><td align="left" valign="top">winWAVE</td>
<td align="left" valign="top">Windows RIFF WAVE GlobalHandle . RIFF sound format.
</td></tr>

<tr><td align="left" valign="top">winPALETTE</td>
<td align="left" valign="top">Windows HPALETTE GlobalHandle. RIFF palette format
</td></tr>
</table>


Composite media label

All media types support the composite label; that is, you can
get the media data for any cast member as a single, opaque portable
MoaHandle. Because Director handles the byteswapping
of these, they're safe to write out to disk and read back in on
another platform. The format for composite media is always MoaHandle.
Other media labels


In addition to composite, each built-in cast member type supports
one or more other labels or formats. For example, bitmaps, PICTs,
Rich Text, and OLE support the image label (RichText and OLE are
Get-only for this); the bitmap format supported depends on the
platform (macPICT on mac, winDIB on Windows). In the future, we
may add additional formats, such as a portable pixel map format
which is identical on both platforms.

Text supports multiple labels: text to get or set the
ASCII text, textStyles (Macintosh-only) to get or set
the style data independently from the ASCII.

Both movies and cast members support the score media
label, with the format MoaHandle. This lets you get and
set the score data itself. You can attach a ScoreAccess interface
to one of these data handles using IMoaDrUtils::NewScoreAccess().
However, if you want to edit the score of an existing movie or
cast member, it's easier just to obtain the ScoreAccess interface
directly from the object itself using IMoaDrCastMem::GetScoreAccess(),

Here's a complete list of the media labels and formats supported
by the Director cast member types:

<TABLE BORDER="2">
<TR><TH ALIGN="LEFT" WIDTH=151><B>Cast member type</B></TH>
<TH ALIGN="LEFT" WIDTH=220><B>Media label</B></TH><TH ALIGN="LEFT"
WIDTH=220><B>Media format</B></TH></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=151>BITMAP</TD>
<TD ALIGN="LEFT" VALIGN="TOP" WIDTH=220>Composite
Image</TD>
<TD ALIGN="LEFT" VALIGN="TOP" WIDTH=220>moaHandle 
mac: macPICT
win: winDIB</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=151>FILMLOOP</TD>
<TD ALIGN="LEFT" VALIGN="TOP" WIDTH=220>Composite
Score</TD>
<TD ALIGN="LEFT" VALIGN="TOP" WIDTH=220>moaHandle
moaHandle</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=151>TEXTFIELD</TD>
<TD ALIGN="LEFT" VALIGN="TOP" WIDTH=220>Composite
Text
TextStyles</TD>
<TD ALIGN="LEFT" VALIGN="TOP" WIDTH=220>moaHandle 
moaHandle	(null-terminated string)
mac: macTEStyles
mac &amp; win: moaTEStyles</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=151>PALETTE</TD>
<TD ALIGN="LEFT" VALIGN="TOP" WIDTH=220>Composite
Palette
Palette</TD>
<TD ALIGN="LEFT" VALIGN="TOP" WIDTH=220>moaHandle 
mac: macColorTable
win: winPALETTE</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=151>PICT</TD>
<TD ALIGN="LEFT" VALIGN="TOP" WIDTH=220>Composite
Image
Image</TD>
<TD ALIGN="LEFT" VALIGN="TOP" WIDTH=220>moaHandle 
mac: macPICT
win: winDIB</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=151>SOUND</TD>
<TD ALIGN="LEFT" VALIGN="TOP" WIDTH=220>Composite
Sound
Sound</TD>
<TD ALIGN="LEFT" VALIGN="TOP" WIDTH=220>moaHandle 
mac: macSnd 
win: winWAVE</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=151>BUTTON</TD>
<TD ALIGN="LEFT" VALIGN="TOP" WIDTH=220>Composite
Text
TextStyles</TD>
<TD ALIGN="LEFT" VALIGN="TOP" WIDTH=220>moaHandle
moaHandle (null-terminated string handle) 
mac: macTEStyles 
mac &amp; win: moaTEStyles</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=151>SHAPE</TD>
<TD ALIGN="LEFT" VALIGN="TOP" WIDTH=220>Composite</TD>
<TD ALIGN="LEFT" VALIGN="TOP" WIDTH=220>moaHandle</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=151>MOVIE</TD>
<TD ALIGN="LEFT" VALIGN="TOP" WIDTH=220>Composite</TD>
<TD ALIGN="LEFT" VALIGN="TOP" WIDTH=220>moaHandle</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=151>DIGITAL VIDEO</TD>
<TD ALIGN="LEFT" VALIGN="TOP" WIDTH=220>Composite</TD>
<TD ALIGN="LEFT" VALIGN="TOP" WIDTH=220>moaHandle</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=151>SCRIPT</TD>
<TD ALIGN="LEFT" VALIGN="TOP" WIDTH=220>Composite</TD>
<TD ALIGN="LEFT" VALIGN="TOP" WIDTH=220>moaHandle
NOTE: Script text can be accessed using GetProp() and SetProp()</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=151>RICH TEXT</TD>
<TD ALIGN="LEFT" VALIGN="TOP" WIDTH=220>Composite
Text
Image  (Get Only)</TD>
<TD ALIGN="LEFT" VALIGN="TOP" WIDTH=220>moaHandle
moaHandle  (null-terminated string handle). Set allowed only in authoring applications
mac: macPICT 
win: winDIB</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=151>OLE</TD>
<TD ALIGN="LEFT" VALIGN="TOP" WIDTH=220>Composite
Image  (Get Only)</TD>
<TD ALIGN="LEFT" VALIGN="TOP" WIDTH=220>moaHandle
mac: macPICT 
win: winDIB</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=151>XTRA</TD>
<TD ALIGN="LEFT" VALIGN="TOP" WIDTH=220>Composite</TD>
<TD ALIGN="LEFT" VALIGN="TOP" WIDTH=220>moaHandle</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=151>TRANS</TD>
<TD ALIGN="LEFT" VALIGN="TOP" WIDTH=220>Composite</TD>
<TD ALIGN="LEFT" VALIGN="TOP" WIDTH=220>moaHandle</TD></TR>
</TABLE>


Media access in sprite Xtras

Sprite Xtras, which provide cast members through extensions to
Director, only support the composite media label, which
is a combination of the Xtra's media, properties, and built-in
properties combined in an opaque format. You can get or set the
media data of any Xtra-based cast member type as a composite MoaHandle.
*/

{
	STD_IUNKNOWN_METHODS

	/* IMoaDrMediaOwner methods */

	STDMETHOD(GetProp)(THIS_
		MoaMmSymbol symbol,  /* The MoaMmSymbol for the property of interest */
		PMoaMmValue pPropValue)  /* Pointer to a caller-owned MoaMmValue structure to receive the value of the property */
		PURE;

/* Category
Property owner methods
*/

/* Returns
<TABLE BORDER="2">
<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=235>kMoaErr_NoErr
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Property exists and value was
returned
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=235>kMoaErr_BadParam
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Invalid <FONT SIZE=2
FACE="Courier New">pPropValue </FONT>passed in
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=235>kMoaMmErr_PropertyNotFound
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Property isn't supported by this
class
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=235>kMoaMmErr_InternalError
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Property exists but couldn't get due
to internal err
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=235>kMoaMmErr_NoMemForString
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Couldn't allocate memory for string
value data
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=235>kMoaErr_OutOfMem
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Couldn't allocate memory for other
value data
</TD></TR>
</TABLE>
*/

/* Description
Obtains the value of the specified cast member property. To get the symbol
from a string, use the IMoaMmUtils::StringToSymbol(). The caller
is responsible for releasing the value withIMoaMmUtils::ValueRelease()
when it is no longer needed.
*/

	STDMETHOD(SetProp)(THIS_
		MoaMmSymbol symbol,  /* The MoaMmSymbol for the property of interest.*/
		ConstPMoaMmValue pPropValue)  /* Pointer to a caller-owned MoaMmValue structure to copy the new value for the property from.*/
		PURE;

/* Category
Property owner methods
*/

/* Returns
<TABLE BORDER="2">
<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=253>kMoaErr_NoErr
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=343>Property exists and value was set
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=253>kMoaErr_BadParam
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=343>Invalid <FONT SIZE=2
FACE="Courier New">pPropValue </FONT>passed in
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_PropertyNotFound
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=343>Property isn't supported by this class
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_InternalError
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=343>Property exists, value ok, couldn't
set--internal error
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_NoMemForString
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=343>Couldn't allocate memory for
setting string value data
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=253>kMoaErr_OutOfMem
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=343>Couldn't allocate memory for
setting other value data
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_IntegerExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=343>Type mismatch: integer value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_SymbolExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=343>Type mismatch: symbol value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_FloatExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=343>Type mismatch: float value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_StringExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=343>Type mismatch: string value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_PointExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=343>Type mismatch: point value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_RectExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=343>Type mismatch: rect value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_ValueTypeMismatch
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=343>Type mismatch: other value
expected (non-specific)
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaDrErr_CastMemberExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=343>Type mismatch:
CMRef value expected
</TD></TR>
</TABLE>
*/

/* Description
Sets a cast member	property to a new value.  To get the symbol from a string,
use the IMoaMmUtils::StringToSymbol()., The caller continues to
maintain ownership of the value passed in at pPropValue, and
should release it using IMoaMmUtils::ValueRelease() when it is no
longer needed.
*/

	STDMETHOD(GetMedia)(THIS_
		PMoaDrMediaInfo pMediaInfo)  /* Pointer to a caller-owned media information structure */
		PURE;

/* Category
Media owner methods
*/

/* Description
Gets cast member media data.  This method obtains a copy of the media
associated with a cast member. Before making this call, use
IMoaDrUtils::NewMediaInfo() to fill out the media information
structure, specifying NULL for the aux field and
kMoaDrMediaOpts_None for the options field.  The caller
partially populates a MoaDrMediaInfo struct with symbols indicating
the requested chunk of media (labelSymbol) and the requested format
(formatSymbol).  After the call, the mediaData field is
populated with the requested data; the type of this field depends on the format
requested.	The caller then owns the data and is responsible for disposing it,
if applicable.	Typically this data is either a MoaHandle, a Macintosh
Handle or Pointer, or a Windows global handle.
*/

	STDMETHOD(SetMedia)(THIS_
		PMoaDrMediaInfo pMediaInfo)  /* Pointer to a caller-owned media
information structure */
		PURE;

/* Category
Media owner methods
*/

/* Description
Sets cast member media data.  This method copies caller-supplied media data and
associates it with a cast member. Before making this call, use
IMoaDrUtils::NewMediaInfo() to fill out the media information
structure, specifying NULL for the aux field and
kMoaDrMediaOpts_None for the options field.  The caller
populates a MoaDrMediaInfo structure with symbols indicating the
supplied chunk of media (labelSymbol), the supplied format
(formatSymbol), and the media data itself (mediaData).  If
the label and format are supported by the cast member, a call to this method
copies the caller's data and replaces any existing media data for the supplied
label for the cast member.	Since the data is copied, the caller retains
ownership of the media data passed in. Typically, this data is either a
MoaHandle, a Macintosh Handle or pointer, or a Windows global
handle.
*/

	STDMETHOD(AttachMedia)(THIS_
		PMoaDrMediaInfo pMediaInfo)  /* Pointer to a caller- owned media information structure */
		PURE;

/* Category
Media owner methods
*/

/* Description

Attaches media to a cast member, releasing it from the caller. This is the same
as the SetMedia() method except instead of copying the data, it is
moved to the cast member.  (In effect this method is a SetMedia() call
followed by a ReleaseMedia() call.)  

Before making this call, use IMoaDrUtils::NewMediaInfo() to fill out
the media information structure, specifying NULL for the aux field and
kMoaDrMediaOpts_None for the options field. 

On enter, the labelSymbol and formatSymbol fields should be
populated with symbols indicating which chunk of media is to be attach
(labelSymbol), and what format the media is supplied in
(formatSymbol).  The mediaData field should hold the data
itself (typically a MoaHandle, Macintosh Handle, or Windows global
handle) 

Upon return, if there is no error, the media has changed ownership and belongs
to the host application, and should no longer be referenced by the caller.
This method is provided to allow the host application to optimize
media-transfer if possible; it may prevent an extra copy of the media data,
which may occur with separate SetMedia() and ReleaseMedia()
calls).
*/

	STDMETHOD(CallFunction)(THIS_
		MoaMmSymbol methodName,  /* Symbol of the method (function) to call */
		MoaLong nArgs, /* Number of arguments, excluding the Xtra instance in pArgs[0] */
		ConstPMoaMmValue pArgs,  /* Array of arguments, with the first valid argument at pArgs[1] */
		PMoaMmValue pResult)  /* Pointer to a MoaMmValue to receive a result value, if any.	*/
		PURE;

/* Category
Scripting Support
*/

/* Returns
<TABLE BORDER="2">
<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=295>kMoaErr_NoErr
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=295>Successful</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=295>kMoaMmErr_FunctionNotFound
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=295>Function not
supported
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=295>kMoaMmErr_WrongNumberOfArgs
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=295>Argument count
wrong,
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=295>Other Property/CallHandler errors
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=295>Also acceptable here
</TD></TR>
</TABLE>
*/

/* Description

Calls a cast member Lingo function. Pass the symbol of the function to call in
the methodName parameter, along with the argument count
(nArgs), the argument array (pArgs), and
optionally, a pointer to a MoaMmValue to receive any result value. The
argument array, pArgs, contains an array of MoaMmValues
corresponding to the function arguments.  pArgs[0] is a
reserved value and should not be referenced.  The function parameters are
supplied in pArgs[1] through
pArgs[nArgs]. If the method has a return value, it
should populate pResult with a new MoaMmValue
containing the result.  If the cast member does not support the function
specified by methodName, it returns the error code
kMoaMmErr_FunctionNotFound.  If the number or types of arguments are
incorrect, the function returns one of the appropriate MoaMmErrors
(see mmtypes.h).
*/

	STDMETHOD(GetMemberIndex) (THIS_
		MoaDrMemberIndex * pMemberIndex)  /* Pointer to position of cast member in cast */
		PURE;

/* Category
Member information
*/

/* Description

Returns the position of the cast member This  in its cast.	
*/

	STDMETHOD(GetScoreAccess) (THIS_
		struct IMoaDrScoreAccess ** ppScore)
		PURE;

/* Category
Acquiring IMoaMmScoreAccess
*/

/* Description
Obtains a IMoaDrScoreAccess interface for accessing or editing the
score associated with a film loop cast member.	This method is only valid for
film loop cast members
*/

	STDMETHOD(CallHandler)(THIS_
		MoaMmSymbol handlerName,  /* Symbol for handler (message) name */
		MoaLong nArgs,	/* Number of arguments you're passing */
		PMoaMmValue pArgs,	/* Pointer to an array of MoaMmValues containing arguments */
		PMoaMmValue pResult,  /* Pointer to an array of MoaMmValues containing arguments */
		MoaBool * pHandled)  /* Pointer to a MoaBool to receive TRUE if the handler exists in the script (message handled), or FALSE if it does not.*/
		PURE;

/* Category
Scripting support
*/

/* Description
Calls a handler handlerName  defined in the script associated
with this cast member. The nArgs  argument is the number of
arguments to the handler, the pArgs argument is a MoaMmList of
arguments as MoaMmValues.	You must pass in NULL to
pResult  if you do not expect a result.  You must pass in a
valid pointer if you do expect a result.  The handler call will be passed up
the standard Lingo messaging hierarchy.  The argument pHandled reports
whether the handler was handled by any object in the hierarchy.
*/

};

typedef IMoaDrCastMem * PIMoaDrCastMem;


/* ----------------------------------------------------------------------------
/
/  IMoaDrCast
/
/ -------------------------------------------------------------------------- */

/* IID_IMoaDrCast: AC401AEC0000D9F800000800072C6326 */
DEFINE_GUID(IID_IMoaDrCast, 0xAC401AECL, 0x0000, 0xD9F8, 0x00, 0x00, 0x08,
				0x00, 0x07, 0x2C, 0x63, 0x26);


#undef INTERFACE
#define INTERFACE IMoaDrCast

DECLARE_INTERFACE_(IMoaDrCast, IMoaMmPropOwner)

/* Description

The IMoaDrCast interface represents a cast in a movie.  This interface
can be acquired by calling the IMoaDrMovie methods NewCast(),
GetCastFromName() and GetNthCast().  

Cast properties

IMoaDrCast inherits from the IMoaMmPropOwner interface,
providing for access to data through the properties mechanism described in the
chapter "Properties" earlier in this document.	The following table lists
properties defined for objects providing the IMoaDrCast interface.  
*/

{
	STD_IUNKNOWN_METHODS

	STDMETHOD(GetProp)(THIS_
		MoaMmSymbol symbol, /* The MoaMmSymbol for the property of interest */
		PMoaMmValue pPropValue)  /* Pointer to a MoaMmValue structure to receive the value */
		PURE;

/* Category
Property owner methods
*/

/* Returns
<TABLE BORDER="2">
<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=235>kMoaErr_NoErr
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Property exists and value was
returned
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=235>kMoaErr_BadParam
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Invalid <FONT SIZE=2
FACE="Courier New">pPropValue</FONT> passed in
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=235>kMoaMmErr_PropertyNotFound
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Property isn't supported by this
class
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=235>kMoaMmErr_InternalError
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Property exists but couldn't get due
to internal err
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=235>kMoaMmErr_NoMemForString
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Couldn't allocate memory for string
value data
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=235>kMoaErr_OutOfMem
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Couldn't allocate memory for other
value data
</TD></TR>
</TABLE>
*/

/* Description

Obtains the value of the specified cast property. To get the symbol from a
string, use the IMoaMmUtils::StringToSymbol().The caller  is
responsible for releasing the value referenced by pPropValue
with IMoaMmUtils::ValueRelease() when it is no longer
needed.
*/

	STDMETHOD(SetProp)(THIS_
		MoaMmSymbol symbol,  /* The MoaMmSymbol for the property of interest */
		ConstPMoaMmValue pPropValue)  /* Pointer to a caller-owned ConstPMoaMmValue structure containing the new value */
		PURE;

/* Category
Property owner methods
*/

/* Returns
<TABLE BORDER="2">
<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=253>kMoaErr_NoErr
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Property exists and value was set
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=253>kMoaErr_BadParam
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Invalid <FONT SIZE=2
FACE="Courier New">pPropValue </FONT>passed in
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_PropertyNotFound
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Property isn't supported by this
class
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_InternalError
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Property exists, value ok, couldn't
set--internal error
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_NoMemForString
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Couldn't allocate memory for
setting string value data
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=253>kMoaErr_OutOfMem
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Couldn't allocate memory for
setting other value data
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_IntegerExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: integer value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_SymbolExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: symbol value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_FloatExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: float value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_StringExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: string value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_PointExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: point value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_RectExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: rect value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_ValueTypeMismatch
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: other value
expected (non-specific)
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaDrErr_CastMemberExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch:
CMRef value expected
</TD></TR>
</TABLE>
*/

/* Description

Sets a cast property to a new value.  The caller continues to maintain
ownership of the value passed in, and should release it using
IMoaMmUtils::ValueRelease() when it is no longer needed. 
*/

	STDMETHOD(GetIndexInMovie)(THIS_
		struct IMoaDrMovie * pIMoaDirMovie,  /* Pointer to a caller-owned interface for the movie of interest. */
		MoaDrCastIndex * pCastIndex)  /* Pointer to a MoaDrCastIndex to receive the index of the cast in the specified movie */
		PURE;

/* Category
Movie Interaction
*/

/* Description

Obtains the index of the cast in the specified movie's cast list. Casts can be
shared among several simultaneously-playing movies; each cast can have a
different position within each movie's cast list.  The caller must supply an
interface to the movie of interest, which continues to be owned by the caller.

*/

	STDMETHOD(GetFirstUsedMemberIndex)(THIS_
		MoaDrMemberIndex * pMemberIndex)  /* Pointer to a MoaDrMemberIndex to receive the memberIndex. */
		PURE;

/* Category
Cast member access
*/

/* Description

Obtains the index of the first occupied (non-empty) cast member  slot in the
cast. Returns a NULL member index if there are no cast members in the
cast. 
*/

	STDMETHOD(GetNextUsedMemberIndex)(THIS_
		MoaDrMemberIndex afterMemberIndex,	/* Index of cast member slot after which to begin search for a non-empty cast member */
		MoaDrMemberIndex * pMemberIndex)  /* Pointer to a MoaDrMemberIndex to receive the member index */
		PURE;

/* Category
Cast member access
*/

/* Description
Obtains the index of the next occupied (non-empty) cast member slot in the cast
after the specified cast member slot. Returns a NULL member index if
there are no cast members in the cast after afterMemberIndex.
*/

	STDMETHOD(GetLastUsedMemberIndex)(THIS_
		MoaDrMemberIndex * pMemberIndex)  /* Pointer to a MoaDrMemberIndex to receive the memberIndex.*/
		PURE;

/* Category
Cast member access
*/

/* Description
Obtains the index of the last occupied (non-empty) cast member slot in the
cast.  Returns a NULL member index if there are no cast members in the cast.
*/

	STDMETHOD(GetFirstFreeMemberIndex)(THIS_
		MoaDrMemberIndex * pMemberIndex)  /* Pointer to a MoaDrMemberIndex to receive the memberIndex.*/
		PURE;

/* Category
Cast member access
*/

/* Description

Obtains the index of the first empty cast member slot in the cast. Returns a
NULL member index if there are no empty cast member slots	remaining
in the cast.
*/

	STDMETHOD(GetNextFreeMemberIndex)(THIS_
		MoaDrMemberIndex afterMemberIndex,	/* Index of cast member slot after which to begin searching for an empty cast member slot. */
		MoaDrMemberIndex * pMemberIndex)  /* Pointer to a MoaDrMemberIndex to receive the member index */
		PURE;

/* Category
Cast member access
*/

/* Description
Obtains the index of the next empty cast member slot in the cast after the
specified cast member slot. Returns a NULL member index if there are
no empty cast member slots in the cast after afterMemberIndex.
*/

	STDMETHOD(GetLastFreeMemberIndex)(THIS_
		MoaDrMemberIndex * pMemberIndex)  /* Pointer to a MoaDrMemberIndex to receive the memberIndex */
		PURE;

/* Category
Cast member access
*/

/* Description
Obtains the index of the last empty cast member slot in the cast.  Returns a
NULL member index if there are no empty cast member slots	remaining
in the cast.
*/

	STDMETHOD(GetMemberIndexFromName)(THIS_
		PMoaChar pCastMemName,	/* Pointer to a C string buffer containing the name of the cast member of interest */
		MoaDrMemberIndex * pMemberIndex)  /* Pointer to a MoaDrMemberIndex to receive the index of the cast member */
		PURE;

/* Category
Cast member access
*/

/* Description
Obtains the index of a cast member in the cast given the cast member name.
Returns 0 if the cast member cannot be found in the cast.  Name comparisons are
case-insensitive.
*/

	STDMETHOD(GetCastMem)(THIS_
		MoaDrMemberIndex memberIndex,  /* Index of the cast member of interest. This must be an occupied cast member slot. */
		PIMoaDrCastMem * ppIMoaDrCastMem)  /* Pointer to a PIMoaDrCastMem to receive a pointer to the cast member's interface.  Interface is then
			owned by caller which must dispose when no longer needed.*/
		PURE;

/* Category
Cast member access
*/

/* Description

Obtains a cast member interface for the specified cast member. The caller
subsequently owns the interface and is responsible for releasing it when it is
no longer needed.  A cast member interface may no longer be valid if the
associated cast member is moved or deleted from the cast.  You should always
release the cast member interface before returning from the method
implementation that calls this method.	
*/

	STDMETHOD(CreateCastMem)(THIS_
		MoaDrMemberIndex memberIndex,  /* Index of the cast member slot into which the newly created cast member will be placed */
		MoaMmSymbol typeSymbol)  /* The MoaMmSymbol of the type of cast member to create */
		PURE;

/* Category
Cast member management
*/

/* Description

Creates a new cast member of the specified type in the specified cast member
slot.  The caller must provide the symbol of the type to create.  Symbols of
all registered types can be obtained from the Player cast member type methods
(GetNthCastMemTypeSymbol).  If you know that the type is registered
and the text string for the type, you can get the symbol directly using
IMoaMmUtils::StringToSymbol(). If the specified cast member slot is
occupied, the existing cast member is deleted.	At the time of creation, the
cast member probably will not have any media associated with it; use the
IMoaDrCastMem::SetMedia() (or its media accessor interface) to supply
it. 
*/

	STDMETHOD(DeleteCastMem)(THIS_
		MoaDrMemberIndex memberIndex)  /* Index of the cast member to delete */
		PURE;

/* Category
Cast member management
*/

/* Description

Deletes the cast member in the specified slot. The cast member is removed
immediately.  Any references to the cast member in the score will be
dangling.
*/

	STDMETHOD(DuplicateCastMem)(THIS_
		MoaDrMemberIndex sourceMemberIndex,  /* Index of the cast member in this cast to duplicate */
		struct IMoaDrCast * pDestCast,	/* Pointer to a caller-owned IMoaDrCast interface for the destination cast */
		MoaDrMemberIndex destMemberIndex)  /* Index in pDestCast of the cast member slot in which to place the copied cast member */
		PURE;

/* Category
Cast member management
*/

/* Description

Duplicates a cast member, placing the copy in the specified slot within the
specified cast.  The caller must pass in an interface to the destination cast;
this interface continues to be owned by the caller.  This method duplicates the
cast member entirely, including its properties (name, and so on) and media
data.  If the destination cast member slot is occupied, the existing cast
member is deleted. 
*/

	STDMETHOD(Save)(THIS_
		PMoaChar pNewPathName)	/* Pointer to a specified path */
		PURE;

/* Category
Cast management
*/

/* Returns
<TABLE BORDER="2">
<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=157>kMoaErr_NoErr
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=433>Successful</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=157>kMoaDrErr_DiskIO
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=433>IO error encountered during file
access.
</TD></TR>
</TABLE>
*/

/* Description
 
Saves the cast to the specified path, which should include the filename.  This
method applies only to external cast members.  
*/

};

typedef IMoaDrCast * PIMoaDrCast;


/* ----------------------------------------------------------------------------
/
/  IMoaDrScoreFrame
/
/ -------------------------------------------------------------------------- */


/* IID_IMoaDrScoreFrame: AC5E874200FE19A700000800072C6326 */
DEFINE_GUID(IID_IMoaDrScoreFrame, 0xAC5E8742L, 0x00FE, 0x19A7, 0x00, 0x00,
				0x08, 0x00, 0x07, 0x2C, 0x63, 0x26);


#undef INTERFACE
#define INTERFACE IMoaDrScoreFrame

DECLARE_INTERFACE_(IMoaDrScoreFrame, IMoaMmPropOwner)

/* Description

The IMoaDrScoreFrame interface represents a particular frame in a
score.	You acquire this interface through the
IMoaDrScoreAccess::GetFrame() method. 
Score frame properties

IMoaDrScoreFrame inherits from the IMoaMmPropOwner interface,
providing for access to data through the properties mechanism described in the
"Multimedia Services" chapter in the Development Guide document.  See
the "Properties" section for information on the properties defined for objects
providing the IMoaDrScoreFrame interface.
*/

{
	STD_IUNKNOWN_METHODS

	STDMETHOD(GetProp)(THIS_
		MoaMmSymbol symbol,  /* The MoaMmSymbol for the property of interest */
		PMoaMmValue pPropValue)  /* Pointer to a caller-owned MoaMmValue structure to receive the value of the property */
		PURE;

/* Returns
<TABLE BORDER="2">
<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=235>kMoaErr_NoErr
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Property exists and value was
returned
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=235>kMoaErr_BadParam
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Invalid <FONT SIZE=2
FACE="Courier New">pPropValue</FONT> passed in
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=235>kMoaMmErr_PropertyNotFound
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Property isn't supported by this
class
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=235>kMoaMmErr_InternalError
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Property exists but couldn't get due
to internal error
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=235>kMoaMmErr_NoMemForString
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Couldn't allocate memory for string
value data
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=235>kMoaErr_OutOfMem
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Couldn't allocate memory for other
value data
</TD></TR>
</TABLE>
*/

/* Description

Obtains the value of the specified score frame property. To get the
symbol from a string, use the
IMoaMmUtils::StringToSymbol().The caller is responsible for releasing
the value with IMoaMmUtils::ValueRelease() when it is no longer
needed.
*/

	STDMETHOD(SetProp)(THIS_
		MoaMmSymbol symbol,  /* The MoaMmSymbol for the property of interest */
		ConstPMoaMmValue pPropValue)  /* Pointer to a caller-owned ConstPMoaMmValue structure from which to copy the value of the property */
		PURE;

/* Returns
<TABLE BORDER="2">
<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=253>kMoaErr_NoErr
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Property exists and value was set
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=253>kMoaErr_BadParam
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Invalid <FONT SIZE=2
FACE="Courier New">pPropValue </FONT>passed in
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_PropertyNotFound
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Property isn't supported by this
class
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_InternalError
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Property exists, value ok, but can't
set--internal error
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_NoMemForString
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Can't allocate memory for setting
string value data
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=253>kMoaErr_OutOfMem
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Can't allocate memory for setting
other value data
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_IntegerExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: integer value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_SymbolExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: symbol value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_FloatExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: float value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_StringExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: string value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_PointExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: point value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_RectExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: rect value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_ValueTypeMismatch
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: other value
expected (non-specific)
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaDrErr_CastMemberExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch:
CMRef value expected
</TD></TR>
</TABLE>
*/

/* Description

Sets a score frame property to a new value. To get the symbol
from a string, use the IMoaMmUtils::StringToSymbol().The
caller continues to maintain ownership of the value passed in, and should
release it using IMoaMmUtils::ValueRelease() when it is no longer
needed. 
*/

};

typedef IMoaDrScoreFrame * PIMoaDrScoreFrame;



/* ----------------------------------------------------------------------------
/
/  IMoaDrScoreSound
/
/ -------------------------------------------------------------------------- */

/* IID_IMoaDrScoreSound: AC5E876500FE21EC00000800072C6326 */
DEFINE_GUID(IID_IMoaDrScoreSound, 0xAC5E8765L, 0x00FE, 0x21EC, 0x00, 0x00,
				0x08, 0x00, 0x07, 0x2C, 0x63, 0x26);

#undef INTERFACE
#define INTERFACE IMoaDrScoreSound

DECLARE_INTERFACE_(IMoaDrScoreSound, IMoaMmPropOwner)

/* Description

The IMoaDrScoreSound interface represents the sound in a particular
channel in a particular frame in a score.  You acquire this interface through
the IMoaDrScoreAccess::GetSound() method. 
Score sound properties

IMoaDrScoreSound inherits from the IMoaMmPropOwner interface,
providing for access to data through the properties mechanism described in the
"Multimedia Services" chapter in the Development Guide document.  See
the "Properties" section for information on the properties defined for objects
providing the IMoaDrScoreSound interface.
*/

{
	STD_IUNKNOWN_METHODS

	STDMETHOD(GetProp)(THIS_
		MoaMmSymbol symbol,  /* The MoaMmSymbol for the property of interest */
		PMoaMmValue pPropValue)  /* Pointer to a caller-owned MoaMmValue structure to receive the value of the property */
		PURE;

/* Returns
<TABLE BORDER="2">
<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=235>kMoaErr_NoErr
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Property exists and value was
returned
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=235>kMoaErr_BadParam
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Invalid <FONT SIZE=2
FACE="Courier New">pPropValue</FONT> passed in
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=235>kMoaMmErr_PropertyNotFound
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Property isn't supported by this
class
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=235>kMoaMmErr_InternalError
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Property exists but couldn't get due
to internal error
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=235>kMoaMmErr_NoMemForString
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Couldn't allocate memory for string
value data
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=235>kMoaErr_OutOfMem
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Couldn't allocate memory for other
value data
</TD></TR>
</TABLE>
*/

/* Description
Obtains the value of the specified score sound property.  To get the
symbol from a string, use the
IMoaMmUtils::StringToSymbol().  The caller  is responsible for
releasing the value with IMoaMmUtils::ValueRelease() when it is no
longer needed.
*/

	STDMETHOD(SetProp)(THIS_
		MoaMmSymbol symbol,  /* The MoaMmSymbol for the property of interest */
		ConstPMoaMmValue pPropValue)  /* Pointer to a caller-owned ConstPMoaMmValue structure from which to copy the value of the property */
		PURE;

/* Returns
<TABLE BORDER="2">
<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=253>kMoaErr_NoErr
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Property exists and value was set
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=253>kMoaErr_BadParam
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Invalid <FONT SIZE=2
FACE="Courier New">pPropValue </FONT>passed in
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_PropertyNotFound
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Property isn't supported by this
class
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_InternalError
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Property exists, value ok, but can't
set--internal error
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_NoMemForString
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Can't allocate memory for setting
string value data
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=253>kMoaErr_OutOfMem
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Can't allocate memory for setting
other value data
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_IntegerExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: integer value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_SymbolExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: symbol value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_FloatExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: float value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_StringExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: string value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_PointExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: point value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_RectExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: rect value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_ValueTypeMismatch
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: other value
expected (non-specific)
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaDrErr_CastMemberExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch:
CMRef value expected
</TD></TR>
</TABLE>
*/

/* Description

Sets a score sound property to a new value. To get the symbol
from a string, use the IMoaMmUtils::StringToSymbol().The
caller continues to maintain ownership of the value passed in, and should
release it using IMoaMmUtils::ValueRelease() when it is no longer
needed. 
*/

};

typedef IMoaDrScoreSound * PIMoaDrScoreSound;


/* ----------------------------------------------------------------------------
/
/  IMoaDrScoreSprite
/
/ -------------------------------------------------------------------------- */

/* IID_IMoaDrScoreSprite: AC5E878200FE28B400000800072C6326 */
DEFINE_GUID(IID_IMoaDrScoreSprite, 0xAC5E8782L, 0x00FE, 0x28B4, 0x00, 0x00,
				0x08, 0x00, 0x07, 0x2C, 0x63, 0x26);


#undef INTERFACE
#define INTERFACE IMoaDrScoreSprite

DECLARE_INTERFACE_(IMoaDrScoreSprite, IMoaMmPropOwner)

/* Description

The IMoaDrScoreSprite interface represents the sprite in a particular
channel in a particular frame in a score.  You acquire this interface through
the IMoaDrScoreAccess::GetSprite() method. 

Score sprite properties

IMoaDrScoreSprite inherits from the IMoaMmPropOwner
interface, providing for access to data through the properties mechanism
described in the "Multimedia Services" chapter in the Development Guide
document.  See the "Properties" section for information on the properties
defined for objects providing the IMoaDrScoreSprite interface.
*/

{
	STD_IUNKNOWN_METHODS

	STDMETHOD(GetProp) (THIS_
		MoaMmSymbol symbol,  /* The MoaMmSymbol for the property of interest */
		PMoaMmValue pPropValue)  /* Pointer to a caller-owned MoaMmValue structure to receive the value of the property */
		PURE;

/* Returns
<TABLE BORDER="2">
<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=235>kMoaErr_NoErr
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Property exists and value was
returned
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=235>kMoaErr_BadParam
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Invalid <FONT SIZE=2
FACE="Courier New">pPropValue</FONT> passed in
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=235>kMoaMmErr_PropertyNotFound
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Property isn't supported by this
class
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=235>kMoaMmErr_InternalError
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Property exists but couldn't get due
to internal error
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=235>kMoaMmErr_NoMemForString
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Couldn't allocate memory for string
value data
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=235>kMoaErr_OutOfMem
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Couldn't allocate memory for other
alue data
</TD></TR>
</TABLE>
*/

/* Description

Obtains the value of the specified score sprite property. To get the
symbol from a string, use the
IMoaMmUtils::StringToSymbol().  The caller  is responsible for
releasing the value with IMoaMmUtils::ValueRelease() when it is no
longer	needed.
*/

	STDMETHOD(SetProp) (THIS_
		MoaMmSymbol symbol,  /* The MoaMmSymbol for the property of interest */
		ConstPMoaMmValue pPropValue)  /* Pointer to a caller-owned ConstPMoaMmValue structure from which to copy the value of the property */
		PURE;

/* Returns
<TABLE BORDER="2">
<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=253>kMoaErr_NoErr
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Property exists and value was set
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=253>kMoaErr_BadParam
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Invalid <FONT SIZE=2
FACE="Courier New">pPropValue </FONT>passed in
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_PropertyNotFound
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Property isn't supported by this
class
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_InternalError
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Property exists, value ok, but can't
set--internal error
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_NoMemForString
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Can't allocate memory for setting
string value data
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=253>kMoaErr_OutOfMem
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Can't allocate memory for setting
other value data
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_IntegerExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: integer value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_SymbolExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: symbol value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_FloatExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: float value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_StringExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: string value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_PointExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: point value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_RectExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: rect value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_ValueTypeMismatch
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: other value
expected (non-specific)
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaDrErr_CastMemberExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch:
CMRef value expected
</TD></TR>
</TABLE>
*/

/* Description

Sets a score sprite property to a new value  To get the
symbol from a string, use the
IMoaMmUtils::StringToSymbol().  The caller continues to maintain
ownership of the value passed in, and should release it using
IMoaMmUtils::ValueRelease() when it is no longer needed.
*/

};

typedef IMoaDrScoreSprite * PIMoaDrScoreSprite;



/* ----------------------------------------------------------------------------
/
/  IMoaDrScoreAccess
/
/ -------------------------------------------------------------------------- */


/* INTERFACEID */
/* IID_IMoaDrScoreAccess: AC401B2A0000E88800000800072C6326 */
DEFINE_GUID(IID_IMoaDrScoreAccess, 0xAC401B2AL, 0x0000, 0xE888, 0x00, 0x00,
				0x08, 0x00, 0x07, 0x2C, 0x63, 0x26);


#undef INTERFACE
#define INTERFACE IMoaDrScoreAccess

DECLARE_INTERFACE_(IMoaDrScoreAccess, IMoaDrMediaAccess)

/* Description

IMoaDrScoreAccess inherits from IMoaDrMediaAccess.  This
interface provides specific methods for accessing and editing the contents of a
Director movie's score.  You can acquire a score interface by calling the
IMoaDrMovie::GetScoreAccess() method.	Film loop cast members also
have scores, which can be acquired by IMoaDrCastMem::GetScoreAccess(). 

See the section entitled IMoaDrMediaAccess for information regarding
the inherited IMoaDrMediaAccess-specific methods of this interface.
*/

{
	STD_IUNKNOWN_METHODS

	STDMETHOD(New)(THIS)
		PURE;

/* Category
Editing support
*/

/* Description
See IMoaDrMediaAccess
*/

	STDMETHOD(Refresh)(THIS)
		PURE;

/* Category
Editing support
*/

/* Description
See IMoaDrMediaAccess
*/

	STDMETHOD(Commit)(THIS)
		PURE;

/* Category
Editing support
*/

/* Description
Commits changes made to media back since the last BeginUpdate(),
Commit(), or Refresh().  Call this method to apply changes to
media back to the owner.
*/

	STDMETHOD(GetAccessInfo)(THIS_
		PMoaDrAccessInfo pAccessInfo)  /* Pointer to MoaDrAccessInfo structure to receive information */
		PURE;

/* Category
Media access
*/

/* Description

Obtains the current accessInfo for the score accessor.
pAccessInfo is a pointer to a MoaDrAccessInfo
structure to receive the information.  This call populates the structure with
the PIMoaDrMediaOwner for the media owner, the label symbol for the
media being accessed, and the format symbol for the media being accessed.
Since an interface is being supplied in this structure (pOwner), the
caller is responsible for releasing it.
*/

	STDMETHOD(SetAccessInfo)(THIS_
		ConstPMoaDrAccessInfo pAccessInfo)	/* Pointer to MoaDrAccessInfo structure to receive information */
		PURE;

/* Category
Media access
*/

/* Description

Sets the current accessInfo for the score accessor.
pAccessInfo is a pointer to a MoaDrAccessInfo
structure providing the information.  This call populates the structure with
the PIMoaDrMediaOwner for the media owner, the label symbol for the
media being accessed, and the format symbol for the media being accessed.
Since the caller supplies an interface in this structure (pOwner), the
caller is responsible for releasing it.
*/

	/* ScoreAccess methods */

	STDMETHOD(SetOwner)(THIS_
		PIMoaDrMediaOwner pOwner)
		PURE;

/* Category
Media access
*/

/* Description
Convenience method to set the access information for the ScoreAccess
object.  Calling this method results in the same behavior as calling
SetAccessInfo() with score and MoaHandle as the
label and format symbols, respectively.
*/

	STDMETHOD(BeginUpdate)(THIS)
		PURE;

/* Category
Editing support
*/

/* Description
Begins a score editing session.  Before inserting or deleting frames, or
modifying score frames, sounds, or sprites, you must call
BeginUpdate().  When done with your changes, call EndUpdate().
*/

	STDMETHOD(EndUpdate)(THIS)
		PURE;

/* Category
Editing support
*/

/* Description
Finishes an update session initiated by a call to BeginUpdate().  If
you wish to keep score changes, call Commit() before calling this
method; to revert to previous score, simply call this method without committing
the media.
*/

	STDMETHOD(SetCurFrameIndex)(THIS_
		MoaDrFrameIndex frameIndex)  /* Frame number of the new current frame */
		PURE;

/* Category
Frame access
*/

/* Description
Sets the current frame being accessed.	frameIndex is the frame
number of the new current frame.
*/

	STDMETHOD(GetCurFrameIndex)(THIS_
		PMoaDrFrameIndex pFrameIndex)
		PURE;

/* Category
Frame access
*/

/* Description
Obtains the frame number of the current frame being accessed.
*/

	STDMETHOD(GetLastFrameIndex)(THIS_
		PMoaDrFrameIndex pFrameIndex)
		PURE;	

/* Category
Frame access
*/

/* Description
Obtains the frame number of the last occupied frame in the score.
 */

	STDMETHOD(UpdateFrame)(THIS)
		PURE;

/* Category
Frame editing
*/

/* Description

Updates the current frame.	This method has two effects, it
<ul><ul> saves any changes made to frame, sound, and sprite channels to the working
score data</ul></ul>
<ul><ul> increments the current frame by 1. </ul></ul>
*/

	STDMETHOD(InsertFrame)(THIS)
		PURE;

/* Category
Frame Editing
*/

/* Description
Inserts a frame at the current frame position.	This has the same effect as
DuplicateFrame().
*/

	STDMETHOD(DuplicateFrame)(THIS)
		PURE;

/* Category
Frame Editing
*/

/* Description
Duplicates the current frame.  The new frame is inserted at position
&lt;currentFrame + 1&gt;.  The current frame is incremented to the new
frame (&lt;currentFrame + 1&gt;).
*/

	STDMETHOD(ClearFrame)(THIS)
		PURE;

/* Category
Frame Editing
*/

/* Description
Clears all of the cells in the current frame to their default (empty) values.
*/

	STDMETHOD(DeleteFrame)(THIS)
		PURE;

/* Category
Frame Editing
*/

/* Description
Deletes the current frame from the score.
*/

	STDMETHOD(GetFrame)(THIS_
		PIMoaDrScoreFrame * ppFrame)
		PURE;

/* Category
Frame access
*/

/* Description
Obtains the IMoaDrScoreFrame interface for the score data being
accessed.  This interface is used to get and set frame-level properties of the
score data.
*/

	STDMETHOD(GetSound)(THIS_
		MoaDrSoundChanIndex chanIndex,	/* Specifies the sound channel to access */
		PIMoaDrScoreSound * ppSound)
		PURE;

/* Category
Channel access
*/

/* Description
Obtains the IMoaDrScoreSound interface for a certain sound channel of
the score data being accessed.	chanIndex specifies the sound
channel to access; in Director 5.0, this must be either 1 or 2.  This interface
is used to get and set sound channel-level properties of the score data.
*/

	STDMETHOD(GetSprite)(THIS_
		MoaDrSpriteChanIndex chanIndex,  /* Specifies the sprite channel to access */
		PIMoaDrScoreSprite * ppSprite)
		PURE;

/* Category
Channel access
*/

/* Description
Obtains the IMoaDrScoreSprite interface for a certain sprite channel
of the score data being accessed.  chanIndex specifies the
sprite channel to access; in Director 5.0, this must be in the range 1 to 48.
This interface is used to get and set sprite channel-level properties of the
score data.
*/

};

typedef IMoaDrScoreAccess * PIMoaDrScoreAccess;




/* ----------------------------------------------------------------------------
/
/  IMoaDrMovie
/
/ -------------------------------------------------------------------------- */


/* IID_IMoaDrMovie: AC401B610000F57600000800072C6326 */
DEFINE_GUID(IID_IMoaDrMovie, 0xAC401B61L, 0x0000, 0xF576, 0x00, 0x00, 0x08,
				0x00, 0x07, 0x2C, 0x63, 0x26);


#undef INTERFACE
#define INTERFACE IMoaDrMovie

DECLARE_INTERFACE_(IMoaDrMovie, IMoaDrMediaOwner)

/* Description

The IMoaDrMovie interface represents open movies in Director.  You acquire a
movie interface by calling the IMoaDrPlayer methods
GetActiveMovie() or GetNthMovie().

Movie properties

IMoaDrMovie inherits from the IMoaMmPropOwner interface,
providing for access to data through the properties mechanism described in the
"Multimedia Services" chapter in the Development Guide document.  See
the "Properties" section for information on the properties defined for objects
providing the IMoaDrMovie interface.  

Movie media

IMoaDrMovie inherits from the IMoaDrMediaOwner interface,
providing for access to media through the media owner mechanism described in
the "Director Services" chapter in the Development Guide document.
Movies have one kind of media: scores.	Scores are editable through the
IMoaDrScoreAccess interface.  You acquire a movie's score data by
calling either IMoaDrMovie::GetMedia(), which returns a handle to the
score data, or GetScoreAccess(), which returns an
IMoaDrScoreAccess interface to the score data.  
*/

{
	STD_IUNKNOWN_METHODS

	STDMETHOD(GetProp)(THIS_
		MoaMmSymbol symbol,  /* The MoaMmSymbol for the property of interest.*/
		PMoaMmValue pPropValue)  /* Pointer to a MoaMmValue to receive the value */
		PURE;

/* Category
Property owner methods
*/

/* Returns
<TABLE BORDER="2">
<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=235>kMoaErr_NoErr
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Property exists and value was
returned
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=235>kMoaErr_BadParam
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Invalid <FONT SIZE=2
FACE="Courier New">pPropValue </FONT>passed in
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=235>kMoaMmErr_PropertyNotFound
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Property isn't supported by this
class
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=235>kMoaMmErr_InternalError
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Property exists but couldn't get due
to internal error
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=235>kMoaMmErr_NoMemForString
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Couldn't allocate memory for string
value data
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=235>kMoaErr_OutOfMem
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Couldn't allocate memory for other
value data
</TD></TR>
</TABLE>
*/

/* Description

Obtains the value of the specified movie property. To get the symbol from a
string, use the IMoaMmUtils::StringToSymbol().  The caller is
responsible for releasing the value at pPropValue  with
IMoaMmUtils::ValueRelease() when it is no longer  needed.
*/

	STDMETHOD(SetProp)(THIS_
		MoaMmSymbol symbol,  /* The MoaMmSymbol for the property of interest */
		ConstPMoaMmValue pPropValue)  /* Pointer to a caller-owned ConstPMoaMmValue structure from which to copy the new value */
		PURE;

/* Category
Property owner methods
*/

/* Returns
<TABLE BORDER="2">
<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=253>kMoaErr_NoErr
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Property exists and value was set
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=253>kMoaErr_BadParam
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Invalid <FONT SIZE=2
FACE="Courier New">pPropValue </FONT>passed in
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_PropertyNotFound
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Property isn't supported by this
class
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_InternalError
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Property exists, value ok, couldn't
set--internal error
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_NoMemForString
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Couldn't allocate memory for
setting string value data
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=253>kMoaErr_OutOfMem
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Couldn't allocate memory for
setting other value data
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_IntegerExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: integer value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_SymbolExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: symbol value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_FloatExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: float value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_StringExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: string value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_PointExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: point value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_RectExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: rect value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_ValueTypeMismatch
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: other value
expected (non-specific)
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaDrErr_CastMemberExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch:
CMRef value expected
</TD></TR>
</TABLE>
*/

/* Description

Sets the value of the specified property. To get the symbol from a string, use
the IMoaMmUtils::StringToSymbol() method.	The caller continues to
maintain ownership of the value passed in, and should release it using
IMoaMmUtils::ValueRelease() when it is no longer needed.
*/

	STDMETHOD(GetMedia)(THIS_
		PMoaDrMediaInfo pMediaInfo)  /* Pointer to a caller-owned media information structure */
		PURE;

/* Category
Media owner methods
*/

/* Description

Gets movie media data by obtaining a copy of the media associated with a movie.
This is how one obtains the score data associated with a movie. Before making
this call, use IMoaDrUtils::NewMediaInfo() to fill out the structure,
specifying NULL for the aux field and kMoaDrMediaOpts_None
for the options field.  The caller partially populates a
MoaDrMediaInfo structure with symbols indicating the requested chunk
of media (labelSymbol) and the requested format
(formatSymbol).  After the call, the mediaData field is
populated with the requested data.	The type of this field depends on the
format requested.  The caller owns the data and is responsible for disposing
it, if applicable.	Typically, this data is either a MoaHandle, a
Macintosh handle or pointer, or a Windows global handle.  See Director
property.rtf for a table of mediaLabels and mediaFormats
supported for movies. 
*/

	STDMETHOD(SetMedia)(THIS_
		PMoaDrMediaInfo pMediaInfo)  /* Pointer to a caller-owned media information structure */
		PURE;

/* Category
Media owner methods
*/

/* Description
Sets movie media data.	This is how one replaces the score data associated with
a movie.  This method copies caller-supplied media data and associates it with
the movie. Before making this call, use IMoaDrUtils::NewMediaInfo() to
fill out the structure, specifying NULL for the aux field and
kMoaDrMediaOpts_None for the options field.  The caller
populates a MoaDrMediaInfo structure with symbols indicating the
supplied chunk of media (labelSymbol) and the supplied format
(formatSymbol), and the media data itself (mediaData).  If
the label and format are supported by the movie, a call to this method copies
the caller's data and replaces any existing media data for the supplied label
for the movie.	Since the data is copied, the caller retains ownership of the
media data passed in. Typically this data is either a MoaHandle, a
Macintosh handle or pointer, or a Windows global handle.   See Director
property.rtf for a table of mediaLabels  and mediaFormats
supported for movies.
*/

	STDMETHOD(AttachMedia)(THIS_
		PMoaDrMediaInfo pMediaInfo)  /* Pointer to a caller-owned media information structure */
		PURE;

/* Category
Media owner methods
*/

/* Description

Attaches media to a movie, releasing it from the caller. This is the same as
SetMedia() except instead of copying the data, it is moved to the
movie.	(In effect, a SetMedia() call followed by
ReleaseMedia().)  Before making this call, use
IMoaDrUtils::NewMediaInfo() to fill out the structure, specifying NULL
for the aux field and kMoaDrMediaOpts_None for the
options field.

On enter, the labelSymbol and formatSymbol fields should be
populated with symbols indicating which chunk of media is to be attach
(labelSymbol), and what format the media is supplied in
(formatSymbol).  The mediaData field should hold the data
itself (typically a MoaHandle, Macintosh Handle, or Windows global
handle) 

Upon return, if there is no error, the media has changed ownership and belongs
to the host application, and should no longer be referenced by the caller.
This method is provided to allow the host application to optimize
media-transfer if possible, preventing an extra copy of the media data, which
may occur with separate SetMedia() and ReleaseMedia()
calls).
*/

	STDMETHOD(CallHandler)(THIS_
		MoaMmSymbol name,  /* Symbol of the handler name to call MoaMmSymbol */
		MoaLong nArgs,	/* Number of arguments to pass */
		PMoaMmValue pArgs,	/* Array of MoaDrValues containing the arguments to the call */
		PMoaMmValue pResult)  /* Pointer to a caller-owned MoaMmValue to receive the return value */
		PURE;

/* Category
Scripting support
*/

/* Description
Calls the Lingo handler name in the movie. The
nArgs  argument is the number of arguments to the handler, the
pArgs argument is a MoaMmList of arguments as
MoaMmValues.  You must pass in NULL to pResult  if you
do not expect a result.  You must pass in a valid pointer if you do expect a
result.
*/

	STDMETHOD(GetCastCount)(THIS_
		MoaLong * pCastCount)  /* Pointer to a MoaLong to receive the number of casts */
		PURE;

/* Category
Managing casts
*/

/* Description
Obtains the number of casts in the movie.
*/

	STDMETHOD(GetNthCast)(THIS_
		MoaDrCastIndex movieCastIndex,	/* Index of the cast (from 1 to GetCastCount()) of interest */
		PIMoaDrCast * ppIMoaDrCast)  /* Pointer to a PIMoaDrCast to receive a pointer to the cast interface */
		PURE;

/* Category
Managing casts
*/

/* Description
Obtains an interface to one of the movie's casts by index. The interface is then
owned by the caller, and the caller is responsible for releasing it when it is no longer
needed.
*/

	STDMETHOD(GetCastFromName)(THIS_
		PMoaChar pCastName,  /* Pointer to a null-terminated C string containing the cast name of interest */
		PIMoaDrCast * ppIMoaDrCast)  /* Pointer to a PIMoaDrCast to receive a pointer to the cast interface */
		PURE;

/* Category
Managing casts
*/

/* Description
Obtains an interface to one of the movie's casts by name, as it appears in
the Cast Properties dialog. The interface is then owned by the caller, and
the caller is responsible for releasing it when it is no longer needed.
*/

	STDMETHOD(GetCastIndexFromName)(THIS_
		PMoaChar pCastName,  /* Pointer to a null-terminated C string containing the cast name of interest */
		MoaDrCastIndex * pCastIndex)  /* Pointer to a MoaDrCastIndex to receive the index */
		PURE;

/* Category
Managing casts
*/

/* Description
Obtains the movie cast index associated with a named cast. This is a value
from 1 to GetCastCount().
*/

	STDMETHOD(NewCast) (THIS_
		PMoaChar pCastName,
		MoaBoolParam bExternal,
		PMoaDrCastIndex pNewCastIndex)
		PURE;

/* Category
Managing casts
*/

/* Description
Creates a new cast and add it to the movie's cast list.  Returns the position
of the new cast in the pNewCastIndex argument.
*/

	STDMETHOD(AddExternalCast) (THIS_
		PMoaChar pCastName,  /* The user reference name for the cast */
		PMoaChar pPathName,  /* The full path name for an external cast */
		PMoaDrCastIndex pNewCastIndex)	/* PMoaDrCastIndex for the position in the cast list */
		PURE;

/* Category
Managing casts
*/

/* Description

Adds an existing external cast to the movie's cast list.  Returns in
pNewCastIndex the position of the cast in the movie's cast
list.  
*/

	STDMETHOD(RemoveCast)(THIS_
		MoaDrCastIndex castIndexToRemove)  /* MoaDrCastIndex for the cast to remove */
		PURE;

/* Category
Managing casts
*/

/* Description
Removes the cast specified by castToRemove from the movie's
cast list.
*/

	STDMETHOD(GetCMRefFromMemberName)(THIS_
		PMoaChar pMemberName,  /* Pointer to a null-terminated C string containing the cast member name of interest */
		PMoaDrCMRef pCastMemRef)  /* Pointer to a MoaDrCMRef to receive cast member reference */
		PURE;

/* Category
Managing casts
*/

/* Returns
<TABLE BORDER="2">
<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=229>kMoaErr_NoErr
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=361>Successful</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=229>kMoaDrErr_CastMemNotFound
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=361>Cast member not found
</TD></TR>
</TABLE>
*/

/* Description

Obtains the cast member reference (MoaDrCMRef) for a cast member from
its name.  This method scans all of the casts for the movie, and returns the
MoaDrCMRef for the first cast member whose name matches the one
supplied.  If no cast member is found with the specified name,
kMoaDrErr_CastMemNotFound is returned and
pCastMemRef is set to a null reference .  Use
CMRef_IsNull() to test the result.
*/

	STDMETHOD(GetCastMemFromCMRef)(THIS_
		PMoaDrCMRef pCastMemRef,  /* Pointer to a MoaDrCMRef which specifies the cast member of interest */
		PIMoaDrCastMem * ppIMoaDrCastMem)  /* Pointer to a PIMoaDrCastMem to receive the interface for the cast member */
		PURE;

/* Category
Acquiring ImoaDrCastMem
*/

/* Description

Obtains the IMoaDrCastMem interface for the cast member with the
supplied cast member reference.  The cast member reference specifies the
movieCastIndex (index to cast in the movie) and memberIndex
(index to cast member slot position within the cast); use the CMRef_
macros in drtypes.h to create and access a 0.	The caller owns the
returned  interface and is responsible for releasing it when it is no longer
needed.  
*/

	STDMETHOD(UpdateStageRect) (THIS_
		MoaRect * pWindowRect,	/* Pointer to a MoaRect describing the area of the stage window to update */
		MoaRect * pBufferRect)	/* Pointer to a MoaRect describing the area of the offscreen buffer from where to update */
		PURE;

/* Category
Imaging support
*/

/* Description
Updates a rectangular area of the stage window from a rectangular area of the
stage's offscreen buffer.  Stretching or shrinking of parts of the image can be
achieved by using a windowRect which differs in size from the
bufferRect.  Note that this call does not cause all sprites on the
stage to be reimaged; it simply refreshes the stage window from Director's
offscreen compositing buffer.
*/

	STDMETHOD(GetStageWindowGC)(THIS_
		PIMoaMmGC * ppWindowGC)  /* Pointer to a IMoaMmGC interface for the stage window */
		PURE;

/* Category
Imaging support
*/

/* Description

Obtains the graphics context for the stage window.	This includes the bounds
rectangle, pixel depth, as well as platform-specific information (such as the
WindowPtr of the window on the Macintosh, or the HWND on Windows). This
information is valid only for the duration of the current call into your Xtra
method, because the stage window can change in depth or size at any time (and
may be disposed of and reallocated in the process).   You must release the
graphics context when done by calling its Release() method.   

Important note about graphic contexts

This method should only be called by Lingo and Tool Xtras to do temporary
drawing into the stage window.	It should be called just before doing your
drawing, and the acquired interface should be released before returning control
to the calling application. This is because nativeGCInfo for the stage
buffer can become invalid at any time.	For example, the buffer may be dumped
and recreated if window size, monitor depth, or other display characteristics
change.  

It's not always possible to obtain the a graphics context for the stage window.
During registration, startup, and shut-down, internal movie data structures may
not be initialized, thus trying to get the GC for it will return the err
kMoaDrErr_MovieNotOpen.  Xtra developers should never attempt to
acquire a graphics context and hold onto it; instead, you should acquire the
interface each time you need to draw and release it before your method
returns.

Also, GetStageWindowGC() should not be used to get a parent window for
Windows(TM) dialogs.  The correct procedure is to use the IMoaMmUtils
Windows API cover methods such as WinDialogBox() and
WinDialogBoxParam().  If you're putting up a system dialog on Windows,
use WinGetParent() to get the parent HWND to use, and bracket your
dialog call with WinPrepareDialogBox() and
WinUnprepareDialogBox().

Finally, these calls should not be used for sprite or transition drawing.
Instead, use the graphic context passed to you explicitly in
IMoaMmSpriteActor::Image() or
IMoaDrTransitionActor::Continue().  If you do attempt to use this
context, your Xtra will not work correctly in MIAWs, export, or other
applications such as Authorware.
 
*/

	STDMETHOD(GetStageBufferGC)(THIS_
		PIMoaMmGC * ppBufferGC)  /* Pointer to a IMoaMmGC interface for the stage offscreen buffer */
		PURE;

/* Category
Imaging support
*/

/* Description
Obtains the graphics context for the stage offscreen buffer.  This includes the
bounds rectangle, pixel depth, as well as platform-specific information (such
as the WindowPtr of the window on the Macintosh or the HWND on Windows). This
information is valid only for the duration of the current call into your Xtra
method, because the stage window can change in depth or size at any time (and
may be disposed of and reallocated in the process).   You must release the
graphics context when done by calling its Release() method.

Important note about graphic contexts

This method should only be called by Lingo and Tool Xtras to do temporary
drawing into the stage window.	It should be called just before doing your
drawing, and the acquired interface should be released before returning control
to the calling application. This is because nativeGCInfo for the stage
buffer can become invalid at any time.	For example, the buffer may be dumped
and recreated if window size, monitor depth, or other display characteristics
change.  

It's not always possible to obtain the a graphics context for the stage buffer.
During registration, startup, and shut-down, internal movie data structures may
not be initialized, thus trying to get the GC for it will return the err
kMoaDrErr_MovieNotOpen.  Xtra developers should never attempt to
acquire a graphics context and hold onto it; instead, you should acquire the
interface each time you need to draw and release it before your method
returns.

Finally, this method should not be called to get a context for sprite or
transition drawing.  Instead, use the graphic context passed to you explicitly
in IMoaMmSpriteActor::Image() or
IMoaDrTransitionActor::Continue().  If you attempt to use this
context, your Xtra will not work correctly in MIAWs, export, or other
applications such as Authorware. 
*/

	STDMETHOD(GetFrameIndexFromLabel)(THIS_
		PMoaChar pLabelName,  /* Pointer to a null-terminated C-string containing the name of the marker associated with the score frame (case-insensitive) */
		PMoaDrFrameIndex pFrameIndex)  /* Pointer to a MoaDrFrameIndex to receive the frame number of the label (marker) */
		PURE;

/* Category
Accessing frame labels
*/

/* Returns
<TABLE BORDER="2">
<TR><TD WIDTH=133>kMoaErr_NoErr</TD><TD WIDTH=457>if successful,
kMoaDrErr_LabelNotFound if marker doesn't exist.
</TD></TR>
</TABLE>
*/

/* Description

Obtains the frame number from a label (marker) name. 
*/

	STDMETHOD(GetFrameLabelFromIndex)(THIS_
		MoaDrFrameIndex frameIndex,  /* A MoaDrFrameIndex holding the frame number of interest */
		PMoaChar pLabelName,  /* Pointer to a string buffer to receive the name of the marker associated with the given score frame */
		MoaLong maxLen)  /* The length in bytes of the caller's C string buffer */
		PURE;

/* Category
Accessing frame labels
*/

/* Description
Obtains the name of the label (marker) at a given score frame number. Returns
an empty string if no marker exists for the frame.
*/

	STDMETHOD(SetFrameLabel)(THIS_
		MoaDrFrameIndex frameIndex,  /* A MoaDrFrameIndex holding the number of the frame */
		PMoaChar pLabelName)  /* Pointer to a MoaChar to receive the name of the marker associated with the given score frame */
		PURE;

/* Category
Accessing frame labels
*/

/* Description
Adds, modifies, or deletes a score label (marker) or a frame.  You pass in the
frame number and a C string for the new label.	If a label doesn't exist for
that frame, one will be added.	If one does exist, it'll be replaced with the
new one you specify.  If you pass in NULL for pLabelName, any
current label for that frame is deleted.
*/

	STDMETHOD(GetScoreAccess)(THIS_
		PIMoaDrScoreAccess * ppScore)
		PURE;

/* Category
Acquiring IMoaDrScoreAccess
*/

/* Description
Gets a ScoreAccess interface provider for the movie's score.
*/

	STDMETHOD(Save)(THIS_
		PMoaChar pNewPathName,	/* Null-terminated C string for the path */
		MoaBoolParam bSaveExtCasts)
		PURE;

/* Category
Managing movies
*/

/* Returns
<TABLE BORDER="2">
<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=157>kMoaErr_NoErr
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=433>Successful</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=157>kMoaDrErr_DiskIO
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=433>IO error during file access
</TD></TR>
</TABLE>
*/

/* Description

Saves the movie to the path. Specify the complete path, including file name, to
save the cast to a new file.  Pass NULL as pNewPathName  to
save the cast in its previous file.  
*/

	STDMETHOD(SendSpriteMessage)(THIS_
		MoaDrSpriteChanIndex chanIndex,  /* Channel number of sprite to which to send the message. Valid values are from 1 to 48. */
		MoaMmSymbol handlerName,  /* Symbol for the handler (message) name */
		MoaLong nArgs,	/* Number of arguments you're passing */
		PMoaMmValue pArgs,	/* Pointer to an array of MoaMmValues containing arguments */
		PMoaMmValue pResult,  /* Pointer to a MoaMmValue to receive a result */
		MoaBool * pHandled)  /* arguments to a MoaBool to receive
			TRUE if the message was handled somewhere down the chain or FALSE if it was not. */
		PURE;

/* Category
Scripting support
*/

/* Description
Sends a sprite message to be passed through the standard Director sprite
message hierarchy beginning with the sprite script of the sprite in the
specified channel. This method is similar to
IMoaDrSpriteCallback::SendSpriteMessage().You must pass in NULL to
pResult if you do not expect a result; you must pass in a valid
pointer if you do expect a result.
*/

};

typedef IMoaDrMovie * PIMoaDrMovie;



/* ----------------------------------------------------------------------------
/
/  IMoaDrMovie2
/
/ -------------------------------------------------------------------------- */


/* IID_IMoaDrMovie2: 5B0D82A7-3257-11d0-A222-00A02453444C */
DEFINE_GUID(IID_IMoaDrMovie2,
0x5b0d82a7, 0x3257, 0x11d0, 0xa2, 0x22, 0x0, 0xa0, 0x24, 0x53, 0x44, 0x4c);


#undef INTERFACE
#define INTERFACE IMoaDrMovie2

DECLARE_INTERFACE_(IMoaDrMovie2, IMoaUnknown)

/* Description

The IMoaDrMovie2 interface represents open movies in Director.	You acquire a
movie interface by calling the IMoaDrPlayer methods
GetActiveMovie() or GetNthMovie(). This returns to you an IMoaDrMovie
interface.	You can calling the QueryInterface method off this interface to retrieve
an IMoaDrMovie2 interface

Movie properties

IMoaDrMovie2 inherits from the IMoaMmPropOwner interface,
providing for access to data through the properties mechanism described in the
"Multimedia Services" chapter in the Development Guide document.  See
the "Properties" section for information on the properties defined for objects
providing the IMoaDrMovie interface.  

Movie media

IMoaDrMovie inherits from the IMoaDrMediaOwner interface,
providing for access to media through the media owner mechanism described in
the "Director Services" chapter in the Development Guide document.
Movies have one kind of media: scores.	Scores are editable through the
IMoaDrScoreAccess interface.  You acquire a movie's score data by
calling either IMoaDrMovie::GetMedia(), which returns a handle to the
score data, or GetScoreAccess(), which returns an
IMoaDrScoreAccess interface to the score data.  
*/

{
	STD_IUNKNOWN_METHODS

	STDMETHOD(GetProp)(THIS_
		MoaMmSymbol symbol,  /* The MoaMmSymbol for the property of interest.*/
		PMoaMmValue pPropValue)  /* Pointer to a MoaMmValue to receive the value */
		PURE;

/* Category
Property owner methods
*/

/* Returns
<TABLE BORDER="2">
<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=235>kMoaErr_NoErr
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Property exists and value was
returned
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=235>kMoaErr_BadParam
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Invalid <FONT SIZE=2
FACE="Courier New">pPropValue </FONT>passed in
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=235>kMoaMmErr_PropertyNotFound
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Property isn't supported by this
class
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=235>kMoaMmErr_InternalError
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Property exists but couldn't get due
to internal error
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=235>kMoaMmErr_NoMemForString
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Couldn't allocate memory for string
value data
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=235>kMoaErr_OutOfMem
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Couldn't allocate memory for other
value data
</TD></TR>
</TABLE>
*/

/* Description

Obtains the value of the specified movie property. To get the symbol from a
string, use the IMoaMmUtils::StringToSymbol().  The caller is
responsible for releasing the value at pPropValue  with
IMoaMmUtils::ValueRelease() when it is no longer  needed.
*/

	STDMETHOD(SetProp)(THIS_
		MoaMmSymbol symbol,  /* The MoaMmSymbol for the property of interest */
		ConstPMoaMmValue pPropValue)  /* Pointer to a caller-owned ConstPMoaMmValue structure from which to copy the new value */
		PURE;

/* Category
Property owner methods
*/

/* Returns
<TABLE BORDER="2">
<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=253>kMoaErr_NoErr
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Property exists and value was set
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=253>kMoaErr_BadParam
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Invalid <FONT SIZE=2
FACE="Courier New">pPropValue </FONT>passed in
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_PropertyNotFound
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Property isn't supported by this
class
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_InternalError
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Property exists, value ok, couldn't
set--internal error
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_NoMemForString
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Couldn't allocate memory for
setting string value data
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=253>kMoaErr_OutOfMem
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Couldn't allocate memory for
setting other value data
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_IntegerExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: integer value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_SymbolExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: symbol value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_FloatExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: float value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_StringExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: string value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_PointExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: point value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_RectExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: rect value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_ValueTypeMismatch
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: other value
expected (non-specific)
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaDrErr_CastMemberExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch:
CMRef value expected
</TD></TR>
</TABLE>
*/

/* Description

Sets the value of the specified property. To get the symbol from a string, use
the IMoaMmUtils::StringToSymbol() method.	The caller continues to
maintain ownership of the value passed in, and should release it using
IMoaMmUtils::ValueRelease() when it is no longer needed.
*/

	STDMETHOD(GetMedia)(THIS_
		PMoaDrMediaInfo pMediaInfo)  /* Pointer to a caller-owned media information structure */
		PURE;

/* Category
Media owner methods
*/

/* Description

Gets movie media data by obtaining a copy of the media associated with a movie.
This is how one obtains the score data associated with a movie. Before making
this call, use IMoaDrUtils::NewMediaInfo() to fill out the structure,
specifying NULL for the aux field and kMoaDrMediaOpts_None
for the options field.  The caller partially populates a
MoaDrMediaInfo structure with symbols indicating the requested chunk
of media (labelSymbol) and the requested format
(formatSymbol).  After the call, the mediaData field is
populated with the requested data.	The type of this field depends on the
format requested.  The caller owns the data and is responsible for disposing
it, if applicable.	Typically, this data is either a MoaHandle, a
Macintosh handle or pointer, or a Windows global handle.  See Director
property.rtf for a table of mediaLabels and mediaFormats
supported for movies. 
*/

	STDMETHOD(SetMedia)(THIS_
		PMoaDrMediaInfo pMediaInfo)  /* Pointer to a caller-owned media information structure */
		PURE;

/* Category
Media owner methods
*/

/* Description
Sets movie media data.	This is how one replaces the score data associated with
a movie.  This method copies caller-supplied media data and associates it with
the movie. Before making this call, use IMoaDrUtils::NewMediaInfo() to
fill out the structure, specifying NULL for the aux field and
kMoaDrMediaOpts_None for the options field.  The caller
populates a MoaDrMediaInfo structure with symbols indicating the
supplied chunk of media (labelSymbol) and the supplied format
(formatSymbol), and the media data itself (mediaData).  If
the label and format are supported by the movie, a call to this method copies
the caller's data and replaces any existing media data for the supplied label
for the movie.	Since the data is copied, the caller retains ownership of the
media data passed in. Typically this data is either a MoaHandle, a
Macintosh handle or pointer, or a Windows global handle.   See Director
property.rtf for a table of mediaLabels  and mediaFormats
supported for movies.
*/

	STDMETHOD(AttachMedia)(THIS_
		PMoaDrMediaInfo pMediaInfo)  /* Pointer to a caller-owned media information structure */
		PURE;

/* Category
Media owner methods
*/

/* Description

Attaches media to a movie, releasing it from the caller. This is the same as
SetMedia() except instead of copying the data, it is moved to the
movie.	(In effect, a SetMedia() call followed by
ReleaseMedia().)  Before making this call, use
IMoaDrUtils::NewMediaInfo() to fill out the structure, specifying NULL
for the aux field and kMoaDrMediaOpts_None for the
options field.

On enter, the labelSymbol and formatSymbol fields should be
populated with symbols indicating which chunk of media is to be attach
(labelSymbol), and what format the media is supplied in
(formatSymbol).  The mediaData field should hold the data
itself (typically a MoaHandle, Macintosh Handle, or Windows global
handle) 

Upon return, if there is no error, the media has changed ownership and belongs
to the host application, and should no longer be referenced by the caller.
This method is provided to allow the host application to optimize
media-transfer if possible, preventing an extra copy of the media data, which
may occur with separate SetMedia() and ReleaseMedia()
calls).
*/

	STDMETHOD(CallHandler)(THIS_
		MoaMmSymbol name,  /* Symbol of the handler name to call MoaMmSymbol */
		MoaLong nArgs,	/* Number of arguments to pass */
		PMoaMmValue pArgs,	/* Array of MoaDrValues containing the arguments to the call */
		PMoaMmValue pResult)  /* Pointer to a caller-owned MoaMmValue to receive the return value */
		PURE;

/* Category
Scripting support
*/

/* Description
Calls the Lingo handler name in the movie. The
nArgs  argument is the number of arguments to the handler, the
pArgs argument is a MoaMmList of arguments as
MoaMmValues.  You must pass in NULL to pResult  if you
do not expect a result.  You must pass in a valid pointer if you do expect a
result.
*/

	STDMETHOD(GetCastCount)(THIS_
		MoaLong * pCastCount)  /* Pointer to a MoaLong to receive the number of casts */
		PURE;

/* Category
Managing casts
*/

/* Description
Obtains the number of casts in the movie.
*/

	STDMETHOD(GetNthCast)(THIS_
		MoaDrCastIndex movieCastIndex,	/* Index of the cast (from 1 to GetCastCount()) of interest */
		PIMoaDrCast * ppIMoaDrCast)  /* Pointer to a PIMoaDrCast to receive a pointer to the cast interface */
		PURE;

/* Category
Managing casts
*/

/* Description
Obtains an interface to one of the movie's casts by index. The interface is then
owned by the caller, and the caller is responsible for releasing it when it is no longer
needed.
*/

	STDMETHOD(GetCastFromName)(THIS_
		PMoaChar pCastName,  /* Pointer to a null-terminated C string containing the cast name of interest */
		PIMoaDrCast * ppIMoaDrCast)  /* Pointer to a PIMoaDrCast to receive a pointer to the cast interface */
		PURE;

/* Category
Managing casts
*/

/* Description
Obtains an interface to one of the movie's casts by name, as it appears in
the Cast Properties dialog. The interface is then owned by the caller, and
the caller is responsible for releasing it when it is no longer needed.
*/

	STDMETHOD(GetCastIndexFromName)(THIS_
		PMoaChar pCastName,  /* Pointer to a null-terminated C string containing the cast name of interest */
		MoaDrCastIndex * pCastIndex)  /* Pointer to a MoaDrCastIndex to receive the index */
		PURE;

/* Category
Managing casts
*/

/* Description
Obtains the movie cast index associated with a named cast. This is a value
from 1 to GetCastCount().
*/

	STDMETHOD(NewCast) (THIS_
		PMoaChar pCastName,
		MoaBoolParam bExternal,
		PMoaDrCastIndex pNewCastIndex)
		PURE;

/* Category
Managing casts
*/

/* Description
Creates a new cast and add it to the movie's cast list.  Returns the position
of the new cast in the pNewCastIndex argument.
*/

	STDMETHOD(AddExternalCast) (THIS_
		PMoaChar pCastName,  /* The user reference name for the cast */
		PMoaChar pPathName,  /* The full path name for an external cast */
		PMoaDrCastIndex pNewCastIndex)	/* PMoaDrCastIndex for the position in the cast list */
		PURE;

/* Category
Managing casts
*/

/* Description

Adds an existing external cast to the movie's cast list.  Returns in
pNewCastIndex the position of the cast in the movie's cast
list.  
*/

	STDMETHOD(RemoveCast)(THIS_
		MoaDrCastIndex castIndexToRemove)  /* MoaDrCastIndex for the cast to remove */
		PURE;

/* Category
Managing casts
*/

/* Description
Removes the cast specified by castToRemove from the movie's
cast list.
*/

	STDMETHOD(GetCMRefFromMemberName)(THIS_
		PMoaChar pMemberName,  /* Pointer to a null-terminated C string containing the cast member name of interest */
		PMoaDrCMRef pCastMemRef)  /* Pointer to a MoaDrCMRef to receive cast member reference */
		PURE;

/* Category
Managing casts
*/

/* Returns
<TABLE BORDER="2">
<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=229>kMoaErr_NoErr
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=361>Successful</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=229>kMoaDrErr_CastMemNotFound
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=361>Cast member not found
</TD></TR>
</TABLE>
*/

/* Description

Obtains the cast member reference (MoaDrCMRef) for a cast member from
its name.  This method scans all of the casts for the movie, and returns the
MoaDrCMRef for the first cast member whose name matches the one
supplied.  If no cast member is found with the specified name,
kMoaDrErr_CastMemNotFound is returned and
pCastMemRef is set to a null reference .  Use
CMRef_IsNull() to test the result.
*/

	STDMETHOD(GetCastMemFromCMRef)(THIS_
		PMoaDrCMRef pCastMemRef,  /* Pointer to a MoaDrCMRef which specifies the cast member of interest */
		PIMoaDrCastMem * ppIMoaDrCastMem)  /* Pointer to a PIMoaDrCastMem to receive the interface for the cast member */
		PURE;

/* Category
Acquiring ImoaDrCastMem
*/

/* Description

Obtains the IMoaDrCastMem interface for the cast member with the
supplied cast member reference.  The cast member reference specifies the
movieCastIndex (index to cast in the movie) and memberIndex
(index to cast member slot position within the cast); use the CMRef_
macros in drtypes.h to create and access a 0.	The caller owns the
returned  interface and is responsible for releasing it when it is no longer
needed.  
*/

	STDMETHOD(GetCMRefFromCMId)(THIS_
		PMoaDrCMId	pCastMemId,/* Pointer to a MoaDrCMId identifying the cast member to search for*/
		PMoaDrCMRef pCastMemRef)/* Pointer to a MoaDrCMRef to receive cast member reference */
		PURE;

	/* Category
Managing casts
*/

/* Returns
<TABLE BORDER="2">
<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=229>kMoaErr_NoErr
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=361>Successful</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=229>kMoaDrErr_CastMemNotFound
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=361>Cast member not found
</TD></TR>
</TABLE>
*/

/* Description

Obtains the cast member reference (MoaDrCMRef) for a cast member from
its unique identifier.	This method scans all of the casts for the movie, and returns the
MoaDrCMRef for the cast member whose id matches the one
supplied.  If no cast member is found with the specified id,
kMoaDrErr_CastMemNotFound is returned and
pCastMemRef is set to a null reference .  Use
CMRef_IsNull() to test the result.
*/

	STDMETHOD(GetCMIdFromCMRef)(THIS_
		PMoaDrCMRef pCastMemRef,/* Pointer to a MoaDrCMRef identifying the cast member we want a unique ID from*/
		PMoaDrCMId	pCastMemId) /* Pointer to a MoaDrCMId to recieve the unique cast member identifier */
		PURE;

/* Category
managing casts
*/

/* Description

Obtains a unique identifier ( MoaDrCMId) for the cast member referred to
by pCastMemRef.	This identifer can be used to retrieve the MoaDrCMRef for
this cast member at a later time, even if the cast member has been moved across cast boundaries.
*/

	STDMETHOD(UpdateStageRect) (THIS_
		MoaRect * pWindowRect,	/* Pointer to a MoaRect describing the area of the stage window to update */
		MoaRect * pBufferRect)	/* Pointer to a MoaRect describing the area of the offscreen buffer from where to update */
		PURE;

/* Category
Imaging support
*/

/* Description
Updates a rectangular area of the stage window from a rectangular area of the
stage's offscreen buffer.  Stretching or shrinking of parts of the image can be
achieved by using a windowRect which differs in size from the
bufferRect.  Note that this call does not cause all sprites on the
stage to be reimaged; it simply refreshes the stage window from Director's
offscreen compositing buffer.
*/

	STDMETHOD(GetStageWindowGC)(THIS_
		PIMoaMmGC * ppWindowGC)  /* Pointer to a IMoaMmGC interface for the stage window */
		PURE;

/* Category
Imaging support
*/

/* Description

Obtains the graphics context for the stage window.	This includes the bounds
rectangle, pixel depth, as well as platform-specific information (such as the
WindowPtr of the window on the Macintosh, or the HWND on Windows). This
information is valid only for the duration of the current call into your Xtra
method, because the stage window can change in depth or size at any time (and
may be disposed of and reallocated in the process).   You must release the
graphics context when done by calling its Release() method.   

Important note about graphic contexts

This method should only be called by Lingo and Tool Xtras to do temporary
drawing into the stage window.	It should be called just before doing your
drawing, and the acquired interface should be released before returning control
to the calling application. This is because nativeGCInfo for the stage
buffer can become invalid at any time.	For example, the buffer may be dumped
and recreated if window size, monitor depth, or other display characteristics
change.  

It's not always possible to obtain the a graphics context for the stage window.
During registration, startup, and shut-down, internal movie data structures may
not be initialized, thus trying to get the GC for it will return the err
kMoaDrErr_MovieNotOpen.  Xtra developers should never attempt to
acquire a graphics context and hold onto it; instead, you should acquire the
interface each time you need to draw and release it before your method
returns.

Also, GetStageWindowGC() should not be used to get a parent window for
Windows(TM) dialogs.  The correct procedure is to use the IMoaMmUtils
Windows API cover methods such as WinDialogBox() and
WinDialogBoxParam().  If you're putting up a system dialog on Windows,
use WinGetParent() to get the parent HWND to use, and bracket your
dialog call with WinPrepareDialogBox() and
WinUnprepareDialogBox().

Finally, these calls should not be used for sprite or transition drawing.
Instead, use the graphic context passed to you explicitly in
IMoaMmSpriteActor::Image() or
IMoaDrTransitionActor::Continue().  If you do attempt to use this
context, your Xtra will not work correctly in MIAWs, export, or other
applications such as Authorware.
 
*/

	STDMETHOD(GetStageBufferGC)(THIS_
		PIMoaMmGC * ppBufferGC)  /* Pointer to a IMoaMmGC interface for the stage offscreen buffer */
		PURE;

/* Category
Imaging support
*/

/* Description
Obtains the graphics context for the stage offscreen buffer.  This includes the
bounds rectangle, pixel depth, as well as platform-specific information (such
as the WindowPtr of the window on the Macintosh or the HWND on Windows). This
information is valid only for the duration of the current call into your Xtra
method, because the stage window can change in depth or size at any time (and
may be disposed of and reallocated in the process).   You must release the
graphics context when done by calling its Release() method.

Important note about graphic contexts

This method should only be called by Lingo and Tool Xtras to do temporary
drawing into the stage window.	It should be called just before doing your
drawing, and the acquired interface should be released before returning control
to the calling application. This is because nativeGCInfo for the stage
buffer can become invalid at any time.	For example, the buffer may be dumped
and recreated if window size, monitor depth, or other display characteristics
change.  

It's not always possible to obtain the a graphics context for the stage buffer.
During registration, startup, and shut-down, internal movie data structures may
not be initialized, thus trying to get the GC for it will return the err
kMoaDrErr_MovieNotOpen.  Xtra developers should never attempt to
acquire a graphics context and hold onto it; instead, you should acquire the
interface each time you need to draw and release it before your method
returns.

Finally, this method should not be called to get a context for sprite or
transition drawing.  Instead, use the graphic context passed to you explicitly
in IMoaMmSpriteActor::Image() or
IMoaDrTransitionActor::Continue().  If you attempt to use this
context, your Xtra will not work correctly in MIAWs, export, or other
applications such as Authorware. 
*/

	STDMETHOD(GetFrameIndexFromLabel)(THIS_
		PMoaChar pLabelName,  /* Pointer to a null-terminated C-string containing the name of the marker associated with the score frame (case-insensitive) */
		PMoaDrFrameIndex pFrameIndex)  /* Pointer to a MoaDrFrameIndex to receive the frame number of the label (marker) */
		PURE;

/* Category
Accessing frame labels
*/

/* Returns
<TABLE BORDER="2">
<TR><TD WIDTH=133>kMoaErr_NoErr</TD><TD WIDTH=457>if successful,
kMoaDrErr_LabelNotFound if marker doesn't exist.
</TD></TR>
</TABLE>
*/

/* Description

Obtains the frame number from a label (marker) name. 
*/

	STDMETHOD(GetFrameLabelFromIndex)(THIS_
		MoaDrFrameIndex frameIndex,  /* A MoaDrFrameIndex holding the frame number of interest */
		PMoaChar pLabelName,  /* Pointer to a string buffer to receive the name of the marker associated with the given score frame */
		MoaLong maxLen)  /* The length in bytes of the caller's C string buffer */
		PURE;

/* Category
Accessing frame labels
*/

/* Description
Obtains the name of the label (marker) at a given score frame number. Returns
an empty string if no marker exists for the frame.
*/

	STDMETHOD(SetFrameLabel)(THIS_
		MoaDrFrameIndex frameIndex,  /* A MoaDrFrameIndex holding the number of the frame */
		PMoaChar pLabelName)  /* Pointer to a MoaChar to receive the name of the marker associated with the given score frame */
		PURE;

/* Category
Accessing frame labels
*/

/* Description
Adds, modifies, or deletes a score label (marker) or a frame.  You pass in the
frame number and a C string for the new label.	If a label doesn't exist for
that frame, one will be added.	If one does exist, it'll be replaced with the
new one you specify.  If you pass in NULL for pLabelName, any
current label for that frame is deleted.
*/

	STDMETHOD(GetScoreAccess)(THIS_
		PIMoaDrScoreAccess * ppScore)
		PURE;

/* Category
Acquiring IMoaDrScoreAccess
*/

/* Description
Gets a ScoreAccess interface provider for the movie's score.
*/

	STDMETHOD(Save)(THIS_
		PMoaChar pNewPathName,	/* Null-terminated C string for the path */
		MoaBoolParam bSaveExtCasts)
		PURE;

/* Category
Managing movies
*/

/* Returns
<TABLE BORDER="2">
<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=157>kMoaErr_NoErr
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=433>Successful</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=157>kMoaDrErr_DiskIO
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=433>IO error during file access
</TD></TR>
</TABLE>
*/

/* Description

Saves the movie to the path. Specify the complete path, including file name, to
save the cast to a new file.  Pass NULL as pNewPathName  to
save the cast in its previous file.  
*/

	STDMETHOD(SendSpriteMessage)(THIS_
		MoaDrSpriteChanIndex chanIndex,  /* Channel number of sprite to which to send the message. Valid values are from 1 to 48. */
		MoaMmSymbol handlerName,  /* Symbol for the handler (message) name */
		MoaLong nArgs,	/* Number of arguments you're passing */
		PMoaMmValue pArgs,	/* Pointer to an array of MoaMmValues containing arguments */
		PMoaMmValue pResult,  /* Pointer to a MoaMmValue to receive a result */
		MoaBool * pHandled)  /* arguments to a MoaBool to receive
			TRUE if the message was handled somewhere down the chain or FALSE if it was not. */
		PURE;

/* Category
Scripting support
*/

/* Description
Sends a sprite message to be passed through the standard Director sprite
message hierarchy beginning with the sprite script of the sprite in the
specified channel. This method is similar to
IMoaDrSpriteCallback::SendSpriteMessage().You must pass in NULL to
pResult if you do not expect a result; you must pass in a valid
pointer if you do expect a result.
*/
	STDMETHOD(MoveCastMember)(THIS_
		PMoaDrCMRef pSrcMemRef,/* Pointer to a MoaDrCMRef containing the cast member you want to move  */
		PMoaDrCMRef pDestMemRef)/* Pointer to the MoaDrCMRef containing the location you want to move it to	*/
		PURE;
/* Category
Managing Casts
*/

/* Description
Moves the cast member in the location referred to by pSrcMemRef to the location
referred to by pDestMemRef.  If there is an existing cast member in the destination
slot, it will be removed.
*/

};

typedef IMoaDrMovie2 * PIMoaDrMovie2;




/* ----------------------------------------------------------------------------
/
/  IMoaDrMovieStage
/
/ -------------------------------------------------------------------------- */

/* IID_IMoaDrMovieStage */
DEFINE_GUID(IID_IMoaDrMovieStage, 0x4418DAB2L, 0xFEF7, 0x11D2, 0x8C, 0xDC, 
			0x00, 0x05, 0x02, 0x0E, 0x2E, 0x6D);



#undef INTERFACE
#define INTERFACE IMoaDrMovieStage

DECLARE_INTERFACE_(IMoaDrMovieStage, IMoaUnknown)

/* Description

The IMoaDrMovieStage interface provides access to new stage-related
functions associated with a movie.	IMoaDrMovieStage is an additional interface
on the same movie class that supports IMoaDrMovie and IMoaDrMovie2.  You acquire a
movie interface by calling the IMoaDrPlayer methods
GetActiveMovie() or GetNthMovie(). This returns to you an IMoaDrMovie
interface.	You can calling the QueryInterface method off this interface to retrieve
an IMoaDrMovieStage interface

Movie properties

*/

{
	STD_IUNKNOWN_METHODS

	STDMETHOD(TransformStagePointUnscaledToScaled)(THIS_
				PMoaPoint pStagePoint)			/* IN: The unscaled point, OUT: The scaled point */
		PURE;

/* Category
	Movie stage utility functions
*/

/* Returns
<TABLE BORDER="2">
<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=235>kMoaErr_NoErr
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Point valid and transformed point was
returned
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=235>kMoaErr_BadParam
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Invalid <FONT SIZE=2
FACE="Courier New">pStagePoint </FONT>passed in
</TD></TR>
</TABLE>
*/

/* Description

Transforms a given point from unscaled to scaled stage coordinates.
Unscaled is the coordinate space of the original, un-zoomed
movie.	Scaled is the coordinate space of the movie scaled to its
current drawRect.

*/

	STDMETHOD(TransformStagePointScaledToUnscaled)(THIS_
				PMoaPoint pStagePoint)			/* IN: The scaled point, OUT: The unscaled point */
		PURE;

/* Category
	Movie stage utility functions
*/

/* Returns
<TABLE BORDER="2">
<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=235>kMoaErr_NoErr
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Point valid and transformed point was
returned
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=235>kMoaErr_BadParam
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Invalid <FONT SIZE=2
FACE="Courier New">pStagePoint </FONT>passed in
</TD></TR>
</TABLE>
*/

/* Description

Transforms a given point from scaled to unscaled stage coordinates.
Scaled is the coordinate space of the movie scaled to its
current drawRect.  Unscaled is the coordinate space of the original, 
un-zoomed movie. 

*/

};

typedef IMoaDrMovieStage * PIMoaDrMovieStage;


/* ----------------------------------------------------------------------------
/
/  IMoaDrPlayer
/
/ -------------------------------------------------------------------------- */


/* IID_IMoaDrPlayer: AC401B780000FA7D00000800072C6326 */
DEFINE_GUID(IID_IMoaDrPlayer, 0xAC401B78L, 0x0000, 0xFA7D, 0x00, 0x00, 0x08,
				0x00, 0x07, 0x2C, 0x63, 0x26);


#undef INTERFACE
#define INTERFACE IMoaDrPlayer

DECLARE_INTERFACE_(IMoaDrPlayer, IMoaMmPropOwner)

/* Descripiton

The IMoaDrPlayer interface provides top-level access to the interfaces
representing the Director object model.  IMoaDrPlayer is provided by
the Director callback object, and is acquired by calling
QueryInterface() on the IMoaCallback interface, provided
through the pCallback instance variable of all MOA objects.  

From an instance of the IMoaDrPlayer interface, you can access the
movies, casts, cast members, and scores that make up all open movies.  

Player properties

IMoaDrPlayer inherits from the IMoaMmPropOwner interface,
providing for access to data through the properties mechanism described in the
chapter "Properties" earlier in this document.
*/

{
	STD_IUNKNOWN_METHODS

	STDMETHOD(GetProp)(THIS_
		MoaMmSymbol symbol,  /* The MoaMmSymbol for the property of interest */
		PMoaMmValue pPropValue)  /* Pointer to a caller-owned MoaMmValue structure to receive the value of the property */
		PURE;

/* Category
Property owner methods
*/

/* Returns
<TABLE BORDER="2">
<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=235>kMoaErr_NoErr
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Property exists and value was
returned
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=235>kMoaErr_BadParam
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Invalid <FONT SIZE=2
FACE="Courier New">pPropValue</FONT> passed in
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=235>kMoaMmErr_PropertyNotFound
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Property isn't supported by this
class
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=235>kMoaMmErr_InternalError
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Property exists but couldn't get due
to internal error
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=235>kMoaMmErr_NoMemForString
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Couldn't allocate memory for string
value data
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=235>kMoaErr_OutOfMem
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=355>Couldn't allocate memory for other
value data
</TD></TR>
</TABLE>
*/

/* Description

Obtains the value of the specified player property. To get the symbol from a
string, use the IMoaMmUtils::StringToSymbol().The caller  is
responsible for releasing the value with  IMoaMmUtils::ValueRelease()
when it is no longer needed.
*/

	STDMETHOD(SetProp)(THIS_
		MoaMmSymbol symbol,  /* The MoaMmSymbol for the property of interest */
		ConstPMoaMmValue pPropValue)  /* Pointer to a caller-owned MoaMmValue structure from which to copy the value */
		PURE;

/* Category
Property owner methods
*/

/* Returns
<TABLE BORDER="2">
<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=253>kMoaErr_NoErr
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Property exists and value was set
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=253>kMoaErr_BadParam
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Invalid <FONT SIZE=2
FACE="Courier New">pPropValue </FONT>passed in
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_PropertyNotFound
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Property isn't supported by this
class
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_InternalError
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Property exists, value ok, can't
set--internal error
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_NoMemForString
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Can't allocate memory for setting
string value data
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=253>kMoaErr_OutOfMem
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Can't allocate memory for setting
other value data
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_IntegerExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: integer value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_SymbolExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: symbol value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_FloatExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: float value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_StringExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: string value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_PointExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: point value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_RectExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: rect value
expected
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaMmErr_ValueTypeMismatch
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: other value
expected (non-specific)
</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaDrErr_CastMemberExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch:
CMRef value expected
</TD></TR>
</TABLE>
*/

/* Description

Sets a player property to a new value. To get the symbol from a string, use the
IMoaMmUtils::StringToSymbol().  The caller continues to maintain
ownership of the value passed in, and should release it using
IMoaMmUtils::ValueRelease() when it is no longer needed.

*/

	STDMETHOD(CallHandler) (THIS_
		MoaMmSymbol name,  /* The MoaMmSymbol for the name of the method to call */
		MoaLong nArgs,	/* The number of arguments you are passing to the handler */
		PMoaMmValue pArgs,	/* The array of MoaMmValue arguments */
		PMoaMmValue pResult)  /* Pointer to a caller-owned MoaMmValue to receive a result value from the handler */
		PURE;

/* Category
Scripting support
*/

/* Description

Calls the Lingo handler name in the currently active movie. The
nArgs  argument is the number of arguments to the handler, the
pArgs argument is a MoaMmList of arguments as
MoaMmValues.  You must pass in NULL to pResult  if you
do not expect a result.  You must pass in a valid pointer if you do expect a
result. 

This method has the same behavior as obtaining the active movie interface by
calling GetActiveMovie(), and then calling
IMoaDrMovie::CallHandler().  This method is supplied as a convenience,
as it eliminates the extra call needed to GetActiveMovie() for this
common operation.
*/

	STDMETHOD(GetActiveMovie)(THIS_
		PIMoaDrMovie * ppIMoaDrMovie)  /* Pointer to a PIMoaDrMovie to receive new interface */
		PURE;

/* Category
Movie access
*/

/* Description
Gets an interface to the currently active movie. The active movie may
vary depending upon the context in which this method is called. If there
are no MIAWs executing, the active movie is the single main movie
currently being played. If MIAWs are present, the active movie can be a
MIAW. This is the case if a Lingo Xtra is being called by a MIAW Lingo
script; if an asset Xtra is being used by a MIAW; or if a Transition Xtra is
being called on behalf of a MIAW. The caller is responsible for releasing
the movie interface when it is no longer needed.
*/

	STDMETHOD(GetMovieCount)(THIS_
		MoaLong * pMovieCount) /* Pointer to a MoaLong to receive count of open movies */
		PURE;

/* Category
Movie access
*/

/* Description
Obtains the current number of movies open in the player. In
Director 5.0, there is always at least one open movie, the main stage movie.
Each open movie in a window also contributes to the movie count. Using
GetNthMovie(), you can access any open movie directly.
*/

	STDMETHOD(GetNthMovie)(THIS_
		MoaLong movieIndex,  /* Index of movie to obtain interface for; ranges from 1 to the number of open movies */
		PIMoaDrMovie * ppIMoaDrMovie)  /* Pointer to a PIMoaDrMovie to receive new interface */
		PURE;

/* Category
Movie access
*/

/* Description
Gets an interface to the nth movie in the player's movie list. There's
always at least one open movie in Director 5.0, the main stage movie.  Use
GetMovieCount() to determine the number of open movies.  Using this
method, you could, for example, access data in a Movie In A Window even though
you're currently being called in the context of the main stage movie. The
caller is responsible for releasing the movie interface when it is no longer
needed.Gets an interface to the nth movie in the player's movie list. There's
*/

	STDMETHOD(ResolveFileName)(THIS_
		ConstPMoaChar pFileName,  /* Pointer to C string of filename in native platform format to resolve */
		PMoaChar pPathName,  /* Pointer to C string to receive resolved full pathname */
		MoaLong maxLen)  /* Size of the caller's pPathName buffer */
		PURE;

/* Category
Accessing files
*/

/* Description
Resolves a file name to a full path name using Director's standard
filename resolution algorithm. This can include scanning the searchPaths,
and so on, for the appropriate file. Director's "@:" filename notation is also
supported here.
*/

	STDMETHOD(GetCastMemTypeCount)(THIS_
		MoaLong * pTypeCount)  /* Pointer to a MoaLong to receive the count */
		PURE;

/* Category
Cast member access
*/

/* Description
Obtains the number of currently registered cast member types. This value
is the sum of Director's built-in types and any types registered by external
asset Xtras.
*/

	STDMETHOD(GetNthCastMemTypeSymbol)(THIS_
		MoaLong typeIndex,	/* Index (from 1 to GetCastMemTypeCount()) of the cast member type of interest */
		PMoaMmSymbol pSymbol)  /* Pointer to a MoaMmSymbol to receive the type's symbol */
		PURE;

/* Category
Cast member access
*/

/* Description
Obtains the unique run-time symbol for the specified cast member type
by index. This symbol is then passed as a parameter to callback methods
operating on cast member types, such as GetCastMemTypeDisplayName().
*/

	STDMETHOD(GetCastMemTypeDisplayName)(THIS_
		MoaMmSymbol typeSymbol,  /* The symbol of the type of interest */
		PMoaChar pName,  /* Pointer to buffer to receive null-terminated C string text of the display name */
		MoaLong maxLen)  /* Size of the caller's name buffer */
		PURE;

/* Category
Cast member access
*/

/* Description
Obtains the user-readable text string associated with a cast member type. This
is the same string that appears in the Insert submenu in Director.	You can
obtain typeSymbol using StringToSymbol() or
GetNthCastMemTypeSymbol().
*/

	STDMETHOD(GetGlobalVarValue)(THIS_
		MoaMmSymbol globalVarName,	/* The symbol of the global variable of interest */
		PMoaMmValue pValue)  /* Pointer to a MoaMmValue to receive the value */
		PURE;

/* Category
Scripting support
*/

/* Description
Obtains the value of a Lingo global variable.  Use
IMoaMmUtils::StringToSymbol() to get a symbol from a string for
globalVarName. The caller is responsible for releasing the
value with IMoaMmUtils::ValueRelease() when it is no longer needed.
*/

	STDMETHOD(SetGlobalVarValue)(THIS_
		MoaMmSymbol globalVarName,	/* The symbol of the global variable of interest */
		PMoaMmValue pValue)  /* Pointer to a MoaMmValue to copy the new value from */
		PURE;

/* Category
Scripting support
*/

/* Description
Sets the value of a Lingo global variable. Use
IMoaDrUtils-&gt;StringToSymbol() to get a symbol from a string for
globalVarName. The caller maintains ownership of the value
passed in, and should release it using IMoaMmUtils::ValueRelease()
when it is no longer needed.
*/

};

typedef IMoaDrPlayer * PIMoaDrPlayer;


/* Synonyms for old method names */
#define CallHandlerInActiveMovie(me, name, nargs, pargs, presult) \
			CallHandler(me, name, nargs, pargs, presult)


/* ----------------------------------------------------------------------------
/
/	IMoaDrUtils - Director-specific utility functions
/
/ -------------------------------------------------------------------------- */

/* IID_IMoaDrUtils: AC96CFC00045659700000040105023FB */
DEFINE_GUID(IID_IMoaDrUtils, 0xAC96CFC0L, 0x0045, 0x6597, 0x00, 0x00, 0x00,
				0x40, 0x10, 0x50, 0x23, 0xFB);

#undef INTERFACE
#define INTERFACE IMoaDrUtils

DECLARE_INTERFACE_(IMoaDrUtils, IMoaUnknown)

/* Description
This interface provides Director-specific utilities.  It complements the
features of the IMoaMmUtils interface, providing application-specific
services to Xtras.
*/

{
	STD_IUNKNOWN_METHODS

	STDMETHOD(ValueToCMRef)(THIS_
		ConstPMoaMmValue pValue,  /* Pointer to a cast member reference type */
		PMoaDrCMRef pCMRef)  /* Pointer to a MoaDrCMRef to receive the result */
		PURE;

/* Category
Data conversion
*/

/* Returns
<TABLE BORDER="2">
<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=253>kMoaErr_NoErr
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>successful</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP"
WIDTH=253>kMoaDrErr_CastMemberExpected
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch:
CMRef expected
</TD></TR>
</TABLE>
*/

/* Description

Obtains the MoaDrCMRef for a cast member reference-type
MoaMmValue.  pCMRef is a pointer to a MoaDrCMRef
to receive the result.   Returns kMoaDrErr_CastMemberExpected if
pValue is not an cast member reference-type value.
*/

	STDMETHOD(CMRefToValue)(THIS_
		ConstPMoaDrCMRef pCMRef,  /* Pointer to a ConstPMoaDrCMRef to be used as the basis for the new value */
		PMoaMmValue pValue)  /* Pointer to MoaMmValue to receive the result */
		PURE;

/* Category
Data conversion
*/

/* Description
Creates a new cast member reference-type MoaMmValue from a
MoaDrCMRef.  pCMRef is a pointer to a MoaDrCMRef
to be used as the basis for the new value.  pValue
contains a pointer to a MoaMmValue to receive the result.
This call populates the MoaMmValue at pValue with a
new MoaMmValue, overwriting any current value.  Make sure to release
any preexisting value before making this call.	The caller is responsible for
releasing the returned value using IMoaMmUtils::ValueRelease().
*/

	STDMETHOD(NewMediaInfo)(THIS_
		MoaMmSymbol labelSymbol,
		MoaMmSymbol formatSymbol,
		PMoaVoid mediaData,
		MoaLong options,  /* Used when setting image media. Use a kMoaDrImgMediaOpts_ constant */
		PMoaVoid aux,  /* Pointer to a MoaDrImageAuxInfo structure if you use the "AuxInfo" option */
		PMoaDrMediaInfo pMediaInfo)
		PURE;

/* Category
Media management
*/

/* Description

Populates a MoaDrMediaInfo structure.	This call does not allocate any
media data, it simply populates the supplied structure.  If populating the
structure for a call to SetMedia(), you must populate the
labelSymbol, formatSymbol, mediaData
, options , and aux fields
accordingly.  options tells Director how the palette and color
depth of the image should be set; it currently varies only for "image" label
media.	aux is used only when specifying AuxInfo
options; it is ignored for other options.	If populating the structure for
a call to GetMedia(), only the labelSymbol and
formatSymbol fields are required.

Before a GetMedia(), SetMedia(), or AttachMedia()
call, use this method to fill out the supplied structure, specifying NULL for
the aux field.  Using this call forces you to fill out all the needed
parameters.

When setting or getting any media label other than "image", use the value
kMoaDrMediaOpts_None for the options argument. When
setting or attaching an "image" media label type, there are a number of image
media constants to use as the options argument.  These are
described in the "Constants" section later in this document.  
*/

	STDMETHOD(MediaRelease)(THIS_
		PMoaDrMediaInfo pMediaInfo)
		PURE;

/* Category
Media management
*/

/* Description
Releases the bulk media data referenced by the MoaDrMediaInfo
structure in pMediaInfo.  The formatSymbol
and mediaData fields of pMediaInfo must be valid on
entry.	If the format of the media data is unknown to the host application,
kMoaDrErr_MediaFormatNotSupported is returned and the media data is
not be released.  You can also release the data yourself using the appropriate
native memory manager call (if the media data is a native data type), such as
KillPicture() for "macPICT".	The host application does not
guarantee that it can dispose media data of all formats; only those that are
built-in data types of the host application are supported by this call.
*/

	STDMETHOD(NewScoreAccess)(THIS_ 
		PIMoaDrScoreAccess * ppScore)
		PURE;

/* Category
Acquiring IMoaDrScoreAccess
*/

/* Description
Creates a new instance of an object supporting IMoaDrScoreAccess.	The
IMoaDrScoreAccess interface is used to access and edit score data for
movies and film loops.	Normally, if you want to access an existing movie or
film loop, you would call IMoaDrMovie::GetScoreAccess() or
IMoaDrCastMem::GetScoreAccess().  However, if you do not want to
create new score from scratch which is not yet associated with a movie or film
loop, this method can be used.	To save the resulting score, you must call the
IMoaDrScoreAccess::SetOwner() method to associate an owner object with
the score, and then call Commit() to save the score to the object
*/

	STDMETHOD(RegisterNotificationClient)(THIS_
		PIMoaDrNotificationClient pNotificationClient,	/* Pointer to a pre-existing IMoaDrNotificationClient interface */
		MoaDrNotifyType notifyType,  /* Value indicating the type of notification requested */
		PMoaVoid refCon)
		PURE;

/* Category
Managing notification clients
*/

/* Description

Registers a notification client object with the host application.  Notification
clients are used to receive notification from the host application when certain
events occur.  pClient is a pointer to a pre-existing
IMoaDrNotificationClient interface.  notifyType is a
value indicating the type of notification requested.  When the internal event
occurs, the host application calls the Notify() method of the
specified  IMoaDrNotificationClient interface.  Additional
notification-specific information can be supplied by the host application in
the Notify() method's refCon parameter.  To cease
notification for a given PIMoaDrNotifyClient and
notifyType, call UnregisterNotifyClient().

The same IMoaDrNotificationClient interface can be used to service
multiple notifyTypes.  When calling
UnregisterNotificationClient(), only the notification for the supplied
notifyType is disabled.

Acceptable values for notifyType are:

kMoaDrNotifyType_DocFileIO 		Windows(TM) versiononly

Notification occurs whenever a chunk is read from or written to any a movie or
cast file document in use by the host application.	This is most interesting
for Windows 3.1, where the operating system fails to read from a CD-ROM if
Redbook audio is playing off of the same device.  Use this hook to cease
Redbook audio playing, so that subsequent document chunk file reads and writes
will be successful.  In the refCon argument of the Notify()
method, the host application passes a PMoaChar, a pointer to a
character array specifying the pathname of the file being read to or written
from (a null-terminated C string). 
*/

	STDMETHOD(UnregisterNotificationClient)(THIS_	
		PIMoaDrNotificationClient pNotificationClient,	/* Pointer to a pre-existing IMoaDrNotificationClient interface */
		MoaDrNotifyType notifyType,  /* Value indicating the type of notification requested */
		PMoaVoid refCon)
		PURE;

/* Category
Managing notification clients
*/

/* Description
Unregisters a notification client previously registered by a call to
RegisterNotificationClient().	The same
IMoaDrNotificationClient interface can be used to service multiple
notifyTypes.  When calling
UnregisterNotificationClient(), only the notification for the supplied
notifyType is disabled.
*/

};

typedef IMoaDrUtils * PIMoaDrUtils;


/* ----------------------------------------------------------------------------
/
/	IMoaDrStyleAccess2
/
/	Text style-related utility functions
/
/
/				These are equivalent to the Macintosh GetFNum() and
/				GetFontName() calls.
/
/ -------------------------------------------------------------------------- */

/* IID_IMoaDrStyleAccess2 */
DEFINE_GUID(IID_IMoaDrStyleAccess2, 0xD974E0C4L, 0x5D88, 0x11CF, 0xBA, 0x41,
			0x08, 0x00, 0x07, 0x9F, 0x01, 0x6C);

#undef INTERFACE
#define INTERFACE IMoaDrStyleAccess2

DECLARE_INTERFACE_(IMoaDrStyleAccess2, IMoaUnknown)

/* Descripiton
Provides helpers for accessing text edit style data.  This interface is
available on Macintosh and Windows.  On the Macintosh, the functionality is
also available directly through the Macintosh toolbox.
*/

{
	STD_IUNKNOWN_METHODS

	STDMETHOD(GetFontNumber)(THIS_
		PMoaChar pFontName,  /* Pointer to a C null-terminated C string containing the name of the font of interest. Case is not significant.*/
		MoaLong * pFontNumber,	/* Pointer to a MoaLong to receive the font number corresponding with pFontName */
		PIMoaDrCast   pCast)	/* pointer to the director cast storing the font information we are accessing */
		;

/* Description
Gets the font identification number associated with a font name string.  This
identification number corresponds to values in the "scrpFont" field of
the ScrpSTElement sub-structure in moaTEStyles.  This method
is equivalent to the Macintosh GetFNum() toolbox call.  Currently this
method always returns kMoaErr_NoErr.
*/

	STDMETHOD(GetFontName)(THIS_
		MoaLong fontNumber,  /* Number of the font of interest */
		PMoaChar pFontName,  /* Pointer to a buffer to receive a null-terminated C string for the name of the font */
		MoaLong maxLen,  /* Size of the string buffer at pFontName */
		PIMoaDrCast   pCast)	/* pointer to the director cast storing the font information we are accessing */
		;

/* Description
Gets the font name associated with a font identification number.  This
identification number corresponds to values in the "scrpFont" field of
the ScrpSTElement sub-structure in moaTEStyles. This method
is equivalent to Macintosh GetFontName() toolbox call.
*/

};

typedef IMoaDrStyleAccess2 * PIMoaDrStyleAccess2;



/* ----------------------------------------------------------------------------
/
/	IMoaDrStyleAccess
/
/	WARNING: This interface is obsolete!  Using this interface in Director 6.0 and on
/	may have unpredictable results.  Use the IMoaDrStyleAccess2 interface instead.
/
/ -------------------------------------------------------------------------- */

/* IID_IMoaDrStyleAccess */
DEFINE_GUID(IID_IMoaDrStyleAccess, 0xD974E0C4L, 0x5D88, 0x11CF, 0xBA, 0x41,
			0x08, 0x00, 0x07, 0x9F, 0x01, 0x6C);

#undef INTERFACE
#define INTERFACE IMoaDrStyleAccess

DECLARE_INTERFACE_(IMoaDrStyleAccess, IMoaUnknown)

/* Descripiton
Provides helpers for accessing text edit style data.  This interface is
available on Macintosh and Windows.  On the Macintosh, the functionality is
also available directly through the Macintosh toolbox.
*/

{
	STD_IUNKNOWN_METHODS

	STDMETHOD(GetFontNumber)(THIS_
		PMoaChar pFontName,  /* Pointer to a C null-terminated C string containing the name of the font of interest. Case is not significant.*/
		MoaLong * pFontNumber)	/* Pointer to a MoaLong to receive the font number corresponding with pFontName */
		;

/* Description
Gets the font identification number associated with a font name string.  This
identification number corresponds to values in the "scrpFont" field of
the ScrpSTElement sub-structure in moaTEStyles.  This method
is equivalent to the Macintosh GetFNum() toolbox call.  Currently this
method always returns kMoaErr_NoErr.
*/

	STDMETHOD(GetFontName)(THIS_
		MoaLong fontNumber,  /* Number of the font of interest */
		PMoaChar pFontName,  /* Pointer to a buffer to receive a null-terminated C string for the name of the font */
		MoaLong maxLen)  /* Size of the string buffer at pFontName */
		;

/* Description
Gets the font name associated with a font identification number.  This
identification number corresponds to values in the "scrpFont" field of
the ScrpSTElement sub-structure in moaTEStyles. This method
is equivalent to Macintosh GetFontName() toolbox call.
*/

};

typedef IMoaDrStyleAccess * PIMoaDrStyleAccess;



/* ---------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
/
/	IMoaDrAssetCallback - Director-specific callback interface for asset xtras.
/
/ --------------------------------------------------------------------------- */


/* IID_IMoaDrAssetCallback: ACC33F91013724E70000080007FC20C1 */
DEFINE_GUID(IID_IMoaDrAssetCallback, 0xACC33F91L, 0x0137, 0x24E7, 0x00, 0x00, 0x08, 0x00,
0x07, 0xFC, 0x20, 0xC1);

#undef INTERFACE
#define INTERFACE IMoaDrAssetCallback

DECLARE_INTERFACE_(IMoaDrAssetCallback, IMoaMmPropOwner)

/* Description

The IMoaDrAssetCallback interface is provided in Director by the class
that implements IMoaMmAssetCallback, which is provided by through the
asset initialization method IMoaMmXAsset::SetCallback().  To acquire
IMoaDrAssetCallback, the asset can call QueryInterface() on
IMoaMmAssetCallback.

This interface provides additional, Director-specific callback services to a
media asset.  In Director, an asset Xtra has both an external representation,
implemented by the Xtra developer, and an internal representation, provided by
Director.  The Xtra developer defines custom behavior by implementing standard
asset Xtra interfaces.	This interface provides controlled access to the
internal representation of the Xtra provided by Director.  
*/

{
	STD_IUNKNOWN_METHODS

	STDMETHOD(GetProp)(THIS_
		MoaMmSymbol symbol,  /* MoaMmSymbol representing the property to get */
		PMoaMmValue pPropValue)  /* Pointer to a MoaMmValue to receive the property's value */
		PURE;

/* Category
Property owner
*/

/* Returns
kMoaErr_NoErr Property exists and value was returned
kMoaErr_BadParam Invalid pPropValue passed in
kMoaMmErr_PropertyNotFound Property isn't supported by this class
kMoaMmErr_InternalError Property exists but couldn't get due to internal err
kMoaMmErr_NoMemForString Couldn't allocate memory for string value data
kMoaErr_OutOfMem Couldn't allocate memory for other value data
*/

/* Description
Gets any property of the cast member associated with your asset.  This includes
both built-in ones handled by the host application (such as name in
Director for cast member name), as well as your own properties.  If you
retrieve your own properties, it may result in a callback to your own
GetProp()  method; be careful not to get stuck in a loop.
*/

	STDMETHOD(SetProp)(THIS_
		MoaMmSymbol symbol,  /* MoaMmSymbol representing the property to set */
		ConstPMoaMmValue pPropValue)  /* Pointer to a ConstPMoaMmValue containing the property value */
		PURE;

/* Category
Property owner
*/

/* Returns
kMoaErr_NoErr Property exists and value was set
kMoaErr_BadParam Invalid pPropValue passed in
kMoaMmErr_PropertyNotFound Property isn't supported by this class
kMoaMmErr_InternalError Property exists, value ok, can't set--internal error
kMoaMmErr_NoMemForString Can't allocate memory for setting string value data
kMoaErr_OutOfMem Can't allocate memory for setting other value data
kMoaMmErr_IntegerExpected Type mismatch: integer value expected
kMoaMmErr_SymbolExpected Type mismatch: symbol valueexpected
kMoaMmErr_FloatExpected Type mismatch: float value expected
kMoaMmErr_StringExpected Type mismatch: string value expected
kMoaMmErr_PointExpected Type mismatch: point value expected
kMoaMmErr_RectExpected Type mismatch: rect valueexpected
kMoaMmErr_ValueTypeMismatch Type mismatch: other value expected (non-specific)
kMoaDrErr_CastMemberExpected Type mismatch: CMRef value expected
*/

/* Description
Sets any property of the cast member associated with your asset.  This includes
both built-in ones handled by the host application (such as name in
Director for cast member name), as well as your own properties.  This may
result in a call back to your own SetProp() method; be careful not to
get stuck in a loop.
*/

	STDMETHOD(GetCMRef)(THIS_
		PMoaDrCMRef pCMRef)  /* Pointer to a MoaDrCMRef to receive the reference for the cast member associated with your asset */
		PURE;

/* Category
Internal cast member access
*/

/* Description
Gets the cast member reference for the cast member associated with
your asset. This lets you determine the Director cast member with
your asset is associated. The cast index supplied in this reference is
relative to the active movie.
*/

	STDMETHOD(CallCMHandler)(THIS_
		MoaMmSymbol handlerName,  /* MoaMmSymbol representing the handler to call */
		MoaLong nArgs,	/* Number of arguments, excluding the Xtra instance in pArgs[0] */
		PMoaMmValue pArgs,	/* Array of arguments, with the first valid argument at pArgs[1] */
		PMoaMmValue pResult,  /* Pointer to a MoaMmValue to receive the result */
		MoaBool * pHandled)  /* Boolean value */
		PURE;

/* Category
Scripting support
*/

/* Description

Calls a handler defined in the asset's cast member script. For
CallCMHandler(), only the cast member script is checked; the message
does not proceed up to the movie or frame level, as it does with the sprite
SendMessage() call.

Caller supplies a symbol for handler to call (name), the
arguments (nArgs, pArgs), and a pointer to a
MoaMmValue to receive a result, if any, from the handler call.

Upon return, pHandled is set to TRUE if the call was
handled, that is the handler existed in the cast member script. If message was
not handled, it silently disappears and FALSE is returned.

*/

};

typedef IMoaDrAssetCallback * PIMoaDrAssetCallback;


/* ----------------------------------------------------------------------------
/
/	IMoaDrSpriteCallback - Director-specific callback interface for
/							sprite asset xtras.
/
/ --------------------------------------------------------------------------- */

/* IID_IMoaDrSpriteCallback: ACC33FCA013732510000080007FC20C1 */
DEFINE_GUID(IID_IMoaDrSpriteCallback, 0xACC33FCAL, 0x0137, 0x3251, 0x00,
				0x00, 0x08, 0x00, 0x07, 0xFC, 0x20, 0xC1);

#undef INTERFACE
#define INTERFACE IMoaDrSpriteCallback

DECLARE_INTERFACE_(IMoaDrSpriteCallback, IMoaUnknown)

/* Description

The IMoaDrSpriteCallback interface is provided in Director by the
class that implements IMoaMmSpriteCallback, which is provided through
the sprite actor initialization method
IMoaMmXSpriteActor::SetCallback().  To acquire
IMoaDrSpriteCallback, the asset can call QueryInterface() on
IMoaMmSpriteCallback.	

This interface provides additional, Director-specific callback services to a
media asset.  
*/

{
	STD_IUNKNOWN_METHODS

	STDMETHOD(GetMovie)(THIS_
		PIMoaDrMovie * ppIMoaDrMovie)
		PURE;

/* Category
Acquiring IMoaDrMovie
*/

/* Description
Gets the movie interface for the movie in which the sprite is appearing. You
are responsible for releasing this interface when you're done with it.
*/

	STDMETHOD(GetSpriteChanIndex)(THIS_
		PMoaDrSpriteChanIndex pChanIndex)
		PURE;

/* Category
Sprite access
*/

/* Description
Gets the sprite channel number for the Director sprite associated with this
sprite instance. Channel numbers are 1-based.
*/

	STDMETHOD(SendSpriteMessage)(THIS_
		MoaMmSymbol handlerName,
		MoaLong nArgs,
		PMoaMmValue pArgs,
		PMoaMmValue pResult,
		MoaBool * pHandled)
		PURE;

/* Category
Scripting support
*/

/* Description

Sends a message along the standard message hierarchy starting with the sprite.
The message proceeds along the path:

sprite --&gt; cast member --&gt; frame --&gt; movie 

until it is consumed by a script at some level.  (If no such handler exists at
a given level, the message proceeds to the next level.	Alternatively, a Lingo
script can intentionally pass a message to the next level using the
pass Lingo command.

The caller supplies symbol for handler to call (name), the
arguments (nArgs, pArgs), and a pointer to a
MoaMmValue to receive a result, if any, from the handler call.

Upon return, pHandled is set to TRUE if the call was
handled.  If the message is not handled, it silently disappears and FALSE
is returned.

This method is ideal for implementing widget-type sprite Xtras.  For example,
if you're implementing a button Xtra, you could send a buttonClicked
message whenever the button was clicked.  Developers could then handle this
message by putting an on buttonClicked handler in the sprite script
for any object in the hierarchy: sprite, cast member, frame, movie.).  The Xtra
could, of course, emit multiple messages,  which would be needed to implement a
multi-part widget such as a scrollbar.

*/

};

typedef IMoaDrSpriteCallback * PIMoaDrSpriteCallback;


/* ----------------------------------------------------------------------------
/
/  IMoaDrPaletteAccess
/
/ -------------------------------------------------------------------------- */


/* IID_IMoaDrPlayer: AC401B780000FA7D00000800072C6326 */

DEFINE_GUID(IID_IMoaDrPaletteAccess, 0x7C29A966L, 0x4150, 0x11D0, 0xAD, 0xEF, 0x00,
				0x05, 0x9A, 0x80, 0xE8, 0x2F);
/* Description

The IMoaDrPaletteAccess interface is provided in Director by the
main callback object.  Use this interface to access the media of the built in
palettes supplied by director.
To acquire IMoaDrPaletteAccess, call QueryInterface() on the main
callback object. 
*/

#undef INTERFACE
#define INTERFACE IMoaDrPaletteAccess
DECLARE_INTERFACE_(IMoaDrPaletteAccess,IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
	STDMETHOD(GetBuiltInPaletteCount)(THIS_
		MoaUlong * pCount /* returns the number of build in palettes in Director*/
		)
		PURE;
/* Category
Media access
*/

/* Description

Returns the number of palettes supported internally by Director.
*/
	
	STDMETHOD(GetNthBuiltInPaletteSymbol)(THIS_
		MoaUlong	 nPalette, /* the index of the palette to access. */
		PMoaMmSymbol pPaletteSymbol /* returns the symbol for the palette */
		)
		
		PURE;
/* Category
Media access
*/
/* Description

Returns the symbol for the nth palette.  You can pass this symbol to
GetBuildInPaletteMedia to get the actual palette.
*/
	STDMETHOD(GetBuiltInPaletteMedia)(THIS_
		MoaMmSymbol paletteSymbol, /* the symbol for the palette to access */
		PMoaVoid	* pPaletteMedia /* returns the actual palette media */
		)
		PURE;
/* Category
Media access
*/
/* Description

returns the palette media for the built in palette referred to by paletteSymbol.
You can call GetNthBuildInPaletteSymbol to get this symbol. the palette
returned is either a CTabHandle on the macintosh, or an HPALETTE on windows.
*/
};

typedef IMoaDrPaletteAccess * PIMoaDrPaletteAccess;

/* ---------------------------------------------------------------------------- */



DEFINE_GUID(IID_IMoaDrSound, 0x57A629DEL, 0x43FD, 0x11D0, 0x91, 0x7B,
									0x00, 0x05, 0x9A, 0x80, 0xE8, 0x2F);
#undef INTERFACE
#define INTERFACE IMoaDrSound

DECLARE_INTERFACE_(IMoaDrSound,IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
	STDMETHOD(GetSoundChannelCount)(THIS_
		MoaUlong *	pCount)
		PURE;
	STDMETHOD_(MoaDrSoundChannelPlayStatus,GetSoundChannelStatus)(THIS_
		MoaUlong	iChannel)		
		PURE;
	STDMETHOD(GetFreeSoundChannel)(THIS_
		MoaUlong *	piChannel)
	PURE;
	STDMETHOD(PlaySoundFormat)(THIS_
		MoaUlong		iChannel,
		PIMoaStream 	pSoundStream,
		ConstPMoaChar	psSoundFormat,
		PMoaMmValue 	pProxyChannel,
		PIMoaDrMovie	pMovieContext)
	PURE;
	STDMETHOD(PlaySoundRaw)(THIS_
		MoaUlong		iChannel,
		PIMoaStream 	pRawSoundStream,
		PMoaSoundFormat pSndFormat,
		PMoaMmCuePoint	pCuePointList,
		MoaUlong		nCuePoints,
		PMoaMmValue 	pProxyChannel,
		PIMoaDrMovie	pMovieContext)
	PURE;	
	STDMETHOD(StopSoundChannel)(THIS_
		MoaUlong		iChannel,
		MoaUlong*		pTime)
	PURE;
	STDMETHOD(SetSoundChannelVolume)(THIS_
		MoaUlong		iChannel,
		MoaUshort		nVolume)
	PURE;
	STDMETHOD(GetTime)(THIS_
		MoaUlong		iChannel,
		MoaUlong*		pTime)
	PURE;
};

typedef IMoaDrSound * PIMoaDrSound; 


#undef INTERFACE
/* ---------------------------------------------------------------------------- */


DEFINE_GUID(IID_IMoaDrSound2, 0x250cab9e, 0x9bb9, 0x11d3, 0x88, 0x8e,
									0x0, 0x90, 0x27, 0x72, 0x4, 0xfa);
#undef INTERFACE
#define INTERFACE IMoaDrSound2

DECLARE_INTERFACE_(IMoaDrSound2,IMoaDrSound)
{
	STD_IUNKNOWN_METHODS
	STDMETHOD(GetSoundChannelCount)(THIS_
		MoaUlong *	pCount)
		PURE;
	STDMETHOD_(MoaDrSoundChannelPlayStatus,GetSoundChannelStatus)(THIS_
		MoaUlong	iChannel)		
		PURE;
	STDMETHOD(GetFreeSoundChannel)(THIS_
		MoaUlong *	piChannel)
	PURE;
	STDMETHOD(PlaySoundFormat)(THIS_
		MoaUlong		iChannel,
		PIMoaStream 	pSoundStream,
		ConstPMoaChar	psSoundFormat,
		PMoaMmValue 	pProxyChannel,
		PIMoaDrMovie	pMovieContext)
	PURE;
	STDMETHOD(PlaySoundRaw)(THIS_
		MoaUlong		iChannel,
		PIMoaStream 	pRawSoundStream,
		PMoaSoundFormat pSndFormat,
		PMoaMmCuePoint	pCuePointList,
		MoaUlong		nCuePoints,
		PMoaMmValue 	pProxyChannel,
		PIMoaDrMovie	pMovieContext)
	PURE;	
	STDMETHOD(StopSoundChannel)(THIS_
		MoaUlong		iChannel,
		MoaUlong*		pTime)
	PURE;
	STDMETHOD(SetSoundChannelVolume)(THIS_
		MoaUlong		iChannel,
		MoaUshort		nVolume)
	PURE;
	STDMETHOD(GetTime)(THIS_
		MoaUlong		iChannel,
		MoaUlong*		pTime)
	PURE;
	STDMETHOD(PauseSound)(THIS_
		MoaUlong		iChannel,
		MoaBoolParam	bPauseState)
	PURE;
};

typedef IMoaDrSound2 * PIMoaDrSound2;	


#undef INTERFACE
/* ---------------------------------------------------------------------------- */

/* IMoaDrPreferenceAccess */
DEFINE_GUID(IID_IMoaDrPreferenceAccess, 0x4BA52EC4L, 0x64B2, 0x11CF, 0x98, 0x4A, 0x08,
	0x00, 0x07, 0x4F, 0x01, 0x6C);

#define INTERFACE IMoaDrPreferenceAccess

DECLARE_INTERFACE_(IMoaDrPreferenceAccess, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS

	STDMETHOD(SetDataPref) (THIS_ 
			PMoaChar					prefKey,
			MoaID						prefGUID,
			PMoaVoid					pData,
			MoaUlong					size)  PURE;
	STDMETHOD(SetStringPref) (THIS_ 
			PMoaChar					prefKey,
			MoaID						prefGUID,
			PMoaChar					pValue)  PURE;
	STDMETHOD(SetFlagPref) (THIS_ 
			PMoaChar					prefKey,
			MoaID						prefGUID,
			MoaBool 					value)	PURE;
	STDMETHOD(SetLongPref) (THIS_ 
			PMoaChar					prefKey,
			MoaID						prefGUID,
			MoaLong 					value)	PURE;
	STDMETHOD(SetDoublePref) (THIS_ 
			PMoaChar					prefKey,
			MoaID						prefGUID,
			MoaDouble					value)	PURE;
	STDMETHOD(SetPointPref) (THIS_ 
			PMoaChar					prefKey,
			MoaID						prefGUID,
			MoaPoint					value)	PURE;
	STDMETHOD(SetRectPref) (THIS_ 
			PMoaChar					prefKey,
			MoaID						prefGUID,
			MoaRect 					value)	PURE;

	STDMETHOD(GetDataPref) (THIS_ 
			PMoaChar					prefKey,
			MoaID						prefGUID,
			PMoaVoid					pValue,
			MoaUlong					size)  PURE;
	STDMETHOD(GetStringPref) (THIS_ 
			PMoaChar					prefKey,
			MoaID						prefGUID,
			PMoaChar*					ppValue,
			MoaLong 					bufLen)  PURE;
	STDMETHOD(GetFlagPref) (THIS_ 
			PMoaChar					prefKey,
			MoaID						prefGUID,
			MoaBool*					pValue)  PURE;
	STDMETHOD(GetLongPref) (THIS_ 
			PMoaChar					prefKey,
			MoaID						prefGUID,
			MoaLong*					pValue)  PURE;
	STDMETHOD(GetDoublePref) (THIS_ 
			PMoaChar					prefKey,
			MoaID						prefGUID,
			PMoaDouble					pValue)  PURE;
	STDMETHOD(GetPointPref) (THIS_ 
			PMoaChar					prefKey,
			MoaID						prefGUID,
			PMoaPoint					pValue)  PURE;
	STDMETHOD(GetRectPref) (THIS_ 
			PMoaChar					prefKey,
			MoaID						prefGUID,
			PMoaRect					pValue)  PURE;

};

typedef IMoaDrPreferenceAccess * PIMoaDrPreferenceAccess;
#undef INTERFACE


/*MOA Error block for IMoaDrPreferences----------0x1490->0x149F  */
#define 	kMoaDrPrefsErr_Base 					0x1490

#define 	kMoaDrPrefsErr_BufferOverrun			MAKE_MOAERR(kMoaMmErr_Base)
#define 	kMoaDrPrefsErr_InvalidKey				MAKE_MOAERR(kMoaMmErr_Base + 1)
#define 	kMoaDrPrefsErr_InvalidPointer			MAKE_MOAERR(kMoaMmErr_Base + 2)
#define 	kMoaDrPrefsErr_InvalidData				MAKE_MOAERR(kMoaMmErr_Base + 3)
#define 	kMoaDrPrefsErr_ClassIDNotValid			MAKE_MOAERR(kMoaMmErr_Base + 4)


/* ---------------------------------------------------------------------------- */

DEFINE_GUID(IID_IMoaDrCursor,
0xb8bca0d1, 0x7388, 0x11d2, 0x91, 0x20, 0x0, 0xa0, 0xc9, 0x2e, 0x3a, 0x0f);
															
#undef INTERFACE
#define INTERFACE IMoaDrCursor

DECLARE_INTERFACE_(IMoaDrCursor, IMoaUnknown)
{
	/* IMoaUnknown methods */
	STD_IUNKNOWN_METHODS

	/* The cursorID can be one of the built in types, above, or
	a resource ID.	The pCursorBitmap is the CMRef of a cast member,
	(either a 1-bit bitmap or a Cursor Xtra Asset) to be used as the sprite 
	cursor.  pCursorMask is an optional 1-bit mask bitmap (only used with 
	a 1-bit bitmap cursor)*/
	/* a spriteNum of 0 indicates setting of the stage cursor*/
	
	STDMETHOD(SetSpriteCursor)	(THIS_
			MoaDrSpriteChanIndex		spriteNum,
			MoaDrCursorID				cursorID,
			PMoaDrCMRef 				pCursorBitmap,
			PMoaDrCMRef 				pCursorMask
			)
			PURE;
			
};
								
typedef IMoaDrCursor * PIMoaDrCursor;

#undef INTERFACE

/* ---------------------------------------------------------------------------- */
/* IMoaDrMovieContext */
DEFINE_GUID(IID_IMoaDrMovieContext,
			0x99cd6df0, 0x49e8, 0x11d2, 0xa6, 0x6d, 0x00, 0xa0, 0xc9, 0xe7, 0x37, 0x36);

#define INTERFACE IMoaDrMovieContext

typedef struct
{
	MoaLong dontUseEver[4];
} DrContextState, * PDrContextState;

DECLARE_INTERFACE_(IMoaDrMovieContext, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS

	STDMETHOD(PushXtraContext) (THIS_
			PDrContextState pDrContextState)  PURE;
	STDMETHOD(PopXtraContext) (THIS_ 
			PDrContextState pDrContextState)  PURE;
	STDMETHOD(ReleaseExclusiveThread) (THIS_
			PDrContextState pDrContextState) PURE;
	STDMETHOD(ReacquireExclusiveThread) (THIS_
			PDrContextState pDrContextState) PURE;
};

typedef IMoaDrMovieContext * PIMoaDrMovieContext;

#undef INTERFACE


#ifdef	__cplusplus
}
#endif


#include "drivalue.h"


#endif	/* DRISERVC_H */
