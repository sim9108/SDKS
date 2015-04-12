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
		Interface definition for converting between MoaMmValues of
		ValueType() == kMoaMmValueType_Date and MoaWide values, as
		well routines to convert SumTotalOfDays values to Year/Month/Day
		triplets and visa versa.
*/

#ifndef	NO_PRAGMA_ONCE
#pragma	once
#endif

#ifndef	MMIDATE_H
#define	MMIDATE_H


#include "moaxtra.h"	 	/* MOA shell */
#include "moastdif.h"		/* MOA standard interfaces */
#include "mmtypes.h"		/* multimedia types */
#include "mmiservc.h"


#ifdef	__cplusplus
extern "C" {
#endif


/* ----------------------------------------------------------------------------
/
/	IID_IMoaMmDate - MoaMmValue date manipulation functions.
/
/ --------------------------------------------------------------------------- */

/* IID_IMoaMmDate: f67eacf0-11e7-11d2-9D18-00104b2797f3  */
DEFINE_GUID(IID_IMoaDrDate, 0xf67eacf0, 0x11e7, 0x11d2, 0x9D, 0x18, 0x00, 
							0x10, 0x4b, 0x27, 0x97, 0xf3);

#undef INTERFACE
#define INTERFACE IMoaDrDate

DECLARE_INTERFACE_(IMoaDrDate, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS
				
	STDMETHOD(TotalSecondsToDateValue) 	(THIS_
			MoaWide				 		 TotalSecondsSince1970,
			PMoaMmValue 				 pValue)
			PURE;

	STDMETHOD(DateValueToTotalSeconds)	(THIS_ 
			ConstPMoaMmValue 			 pValue, 
			MoaWide			 			 *pTotalSecondsSince1970) 
			PURE; 
};

typedef IMoaDrDate * PIMoaDrDate;

#if 0 /* Not ready yet.  Perhaps a MoaDrDate structure is neater? */
	STDMETHOD(DateValueToYearMonthDaySeconds)	(THIS_ 
			ConstPMoaMmValue 			 pValue, 
			PMoaLong					 pYear,
			PMoaLong					 pMonth,
			PMoaLong					 pDay,
			PMoaLong					 pSeconds)
			PURE;

	STDMETHOD(YearMonthDaySecondsToDateValue)	(THIS_ 
			MoaLong						 Year,
			MoaLong						 Month,
			MoaLong						 Day,
			MoaLong						 Seconds,
			PMoaMmValue 				 pValue) 
			PURE;
#endif
/* ---------------------------------------------------------------------------- */

#undef INTERFACE


#ifdef	__cplusplus
}
#endif


#endif	/* MMIDATE_H */
