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

/****************************************************************************
/
/ Purpose
/	MoaDrValue services interface definitions.
/
****************************************************************************/

#ifndef	DRIVALUE_H
#define	DRIVALUE_H


#include "moaxtra.h"	 	/* MOA shell */
#include "moastdif.h"		/* MOA standard interfaces */
#include "drtypes.h"		/* multimedia types */


#ifdef	__cplusplus
extern "C" {
#endif


/* ----------------------------------------------------------------------------
/
/	IMoaMmValue - MoaMmValue services interface
/
/ --------------------------------------------------------------------------- */

/* IID_IMoaDrValue: 151A5780-32BD-11d0-8151-00AA005F3C08 */
DEFINE_GUID(IID_IMoaDrValue, 0xC479AE20L, 0x41B2, 0x11D0, 0x96, 0xBF, 
								0x00, 0x05, 0x9A, 0x80, 0xE8, 0x2F);
#undef INTERFACE
#define INTERFACE IMoaDrValue

DECLARE_INTERFACE_(IMoaDrValue, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
		
/* Description
This interface provides Director-specific value types.  It complements the
features of the <tt>IMoaMmValue</tt> interface, providing application-specific
services to Xtras.
*/

	STDMETHOD(ValueToCMRef)(THIS_ 
		ConstPMoaMmValue pValue,  /* Pointer to a cast member
reference type */
		PMoaDrCMRef pCMRef)  /* Pointer to a MoaDrCMRef 
to receive the result */
		PURE;

/* Category
Data conversion
*/

/* Returns
<TABLE BORDER="2">
<TR><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=253><TT>kMoaErr_NoErr</TT>
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>successful</TD></TR>

<TR><TD ALIGN="LEFT" VALIGN="TOP" 
WIDTH=253><TT>kMoaDrErr_CastMemberExpected</TT>
</TD><TD ALIGN="LEFT" VALIGN="TOP" WIDTH=337>Type mismatch: 
<TT>CMRef</TT> expected
</TD></TR>
</TABLE>
*/

/* Description
<p>
Obtains the <tt>MoaDrCMRef </tt>for a cast member reference-type
<tt>MoaMmValue</tt>.  <tt><i>pCMRef </i></tt>is a pointer to a <tt>MoaDrCMRef
</tt>to receive the result.   Returns <tt>kMoaDrErr_CastMemberExpected </tt>if
<tt><i>pValue </i></tt>is not an cast member reference-type value.<p>
*/

	STDMETHOD(CMRefToValue)(THIS_ 
		ConstPMoaDrCMRef pCMRef,  /* Pointer to a <tt>ConstPMoaDrCMRef</tt> 
to be used as the basis for the new value */
		PMoaMmValue pValue)  /* Pointer to <tt>MoaMmValue</tt> to receive the result */
		PURE;

/* Category
Data conversion
*/

/* Description
Creates a new cast member reference-type <tt>MoaMmValue </tt>from a
<tt>MoaDrCMRef</tt>.  <tt><i>pCMRef </i></tt>is a pointer to a <tt>MoaDrCMRef
</tt>to be used as the basis for the new value.  <tt><i>pValue
</i></tt>contains a pointer to a <tt>MoaMmValue </tt>to receive the result.
This call populates the <tt>MoaMmValue </tt>at <tt><i>pValue </i></tt>with a
new <tt>MoaMmValue</tt>, overwriting any current value.  Make sure to release
any preexisting value before making this call.  The caller is responsible for
releasing the returned value using <tt>IMoaMmUtils::ValueRelease()</tt>.
*/

	STDMETHOD(ValueToSpriteRef)(THIS_
		ConstPMoaMmValue pValue,
		PMoaDrSpriteRef pSRef)
		PURE;
		
	STDMETHOD(SpriteRefToValue)(THIS_
		ConstPMoaDrSpriteRef pSRef,
		PMoaMmValue	pValue)
		PURE;

	STDMETHOD(ValueToCastLibRef)(THIS_
		ConstPMoaMmValue pValue,
		PMoaDrCastLibRef pCLRef)
		PURE;
		
	STDMETHOD(CastLibRefToValue)(THIS_
		ConstPMoaDrCastLibRef pCLRef,
		PMoaMmValue	pValue)
		PURE;
		
	STDMETHOD(ValueToSoundSpriteRef)(THIS_
		ConstPMoaMmValue pValue,
		PMoaDrSoundSpriteRef pSRef)
		PURE;
		
	STDMETHOD(SoundSpriteRefToValue)(THIS_
		ConstPMoaDrSoundSpriteRef pSRef,
		PMoaMmValue	pValue)
		PURE;

};

typedef IMoaDrValue * PIMoaDrValue;

/* ---------------------------------------------------------------------------- */

#undef INTERFACE

/* ----------------------------------------------------------------------------
/
/	IID_IMoaDrMediaValue - value services interface for accessing media 
/
/ --------------------------------------------------------------------------- */

/* IID_IMoaDrMediaValue: BA9FA4A5-363E-11d2-AA59-00A0C9091428 */
DEFINE_GUID(IID_IMoaDrMediaValue, 
0xba9fa4a5, 0x363e, 0x11d2, 0xaa, 0x59, 0x0, 0xa0, 0xc9, 0x9, 0x14, 0x28);
								
#undef INTERFACE
#define INTERFACE IMoaDrMediaValue

DECLARE_INTERFACE_(IMoaDrMediaValue, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
		
/* Description
This interface provides Director-specific value types for media.  It complements the
features of the <tt>IMoaDrValue</tt> interface, providing application-specific
services to Xtras.
*/

	STDMETHOD(ValueToMedia)(THIS_ 
		ConstPMoaMmValue pValue,  /* Pointer to a value */
		MoaHandle *phMediaData)  /* Pointer to handle to receive the media data */
		PURE;

	STDMETHOD(MediaToValue)(THIS_ 
		const MoaHandle hMediaData,  /* Pointer to a handle which has the media data */
		PMoaMmValue pValue)  /* Pointer to <tt>MoaMmValue</tt> to receive the result */
		PURE;

	STDMETHOD(ValueToPicture)(THIS_ 
		ConstPMoaMmValue pValue,  /* Pointer to a value */
		MoaHandle *phPictureData)  /* Pointer to handle to receive the media data */
		PURE;

	STDMETHOD(PictureToValue)(THIS_ 
		const MoaHandle hPictureData,  /* Pointer to a handle which has the media data */
		PMoaMmValue pValue)  /* Pointer to <tt>MoaMmValue</tt> to receive the result */
		PURE;

/* Category
Data conversion
*/


};

typedef IMoaDrMediaValue * PIMoaDrMediaValue;

/* ---------------------------------------------------------------------------- */

#undef INTERFACE


#ifdef	__cplusplus
}
#endif


#endif	/* DRIVALUE_H */
