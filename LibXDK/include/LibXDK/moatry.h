/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.	
*/

#ifndef _H_moatry
#define _H_moatry

#include "moatypes.h"

/* #define MOATRY_DEBUG */

/* ------------------------------------------------------------------------ */

#if (defined(__APPLE__) && defined(__MACH__))
#define moa_is_unused __attribute__((unused))
#else
#define moa_is_unused
#endif

#define _moa_val(x) #x
#define _moa_str(x) _moa_val(x)

#ifdef MOATRY_DEBUG
	#define _moa_try_debug \
		ConstPMoaChar __throwloc = "";

	#define _moa_throw_debug \
		__throwloc = __FILE__ ":" _moa_str(__LINE__);

	#define moa_get_exception_location	(__throwloc)
#else
	#define _moa_try_debug
	#define _moa_throw_debug
	#define moa_get_exception_location	("")
#endif

#ifndef moa_report_exception
	#define moa_report_exception
#endif

/* ------------------------------------------------------------------------ */

#define Throw(x)		do { __err = (MoaError)(x); _moa_throw_debug; MOA_ASSERT(__bad_throw_location == (PMoaVoid)0, "Bad Throw Location!"); goto __failure; } while (0)

#define ThrowErr(x) 	do { MoaError __temperr = (MoaError)(x); if (__temperr) Throw(__temperr); } while (0)

#define ThrowFailed(x)	do { MoaError __temperr = (MoaError)(x); if FAILED(__temperr) Throw(__temperr); } while (0)

#define ThrowNull(x)	do { if (!(x)) Throw(kMoaErr_OutOfMem); } while (0)

/* ------------------------------------------------------------------------ */

#define moa_get_exception		(__err)

#define moa_set_exception(e)	do { __err = (e); } while (0)

#define moa_try \
			{ \
			X_ENTER \
			MoaError __err = kMoaErr_NoErr; \
			const PMoaVoid moa_is_unused __bad_throw_location = (PMoaVoid)0; \
			_moa_try_debug 

#define moa_catch \
			if (0) { \
			goto __failure; /* just to prevent an "unused label" warning */ \
			__failure:; \
			moa_report_exception \
			{ \
			const MoaLong moa_is_unused __bad_throw_location = 0;

#define moa_catch_end	\
			__bad_throw_location; } } { \
			const MoaLong moa_is_unused __bad_throw_location = 0; 

#define moa_try_end \
			__bad_throw_location; } \
			X_STD_RETURN(__err); \
			__err; __bad_throw_location; X_EXIT \
			}

#define moa_try_end_void \
			__bad_throw_location; } \
			X_RETURN_VOID; \
			__err; __bad_throw_location; X_EXIT \
			}

#define moa_try_end_(type, value) \
			__bad_throw_location; } \
			X_RETURN(type, value); \
			__err; __bad_throw_location; X_EXIT \
			}

#endif
