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
	Purpose:
	Basic types for 3d
*/

#ifndef __M3dTypes_H
#define __M3dTypes_H

#include "mmtypes.h"		/* multimedia types */

#ifndef	NO_PRAGMA_ONCE
#pragma	once
#endif

/* ----------------------------------------------------------------------------
/
/	Set Alignment/Packing
/
/ -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
/
/	Adaptive Types
/
/	These types can vary depending on the build platform.
/
/ -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
/    Type definitions for 3d vectors and matricies.  Note that float is the default
/      rather than double.  This is mostly to reduce runtime memory requirements.
/    
/* ------------------------------------------------------------------------- */

typedef MoaFloat 	Moa3dCoord;

/* ----------------------------------------------------------------------------
/
/	c definitions for vector and matrix types
/
/ -------------------------------------------------------------------------- */

typedef Moa3dCoord 	Moa3dVector[4];
typedef Moa3dCoord 	Moa3dMatrix[16];

/* Note: this 3dPoint structure is compatible with the Moa3dVector type */

typedef struct
{
	Moa3dCoord x;
	Moa3dCoord y;
	Moa3dCoord z;
	Moa3dCoord h;	/* also known as w */
} Moa3dPoint;

typedef Moa3dPoint * PMoa3dPoint;
typedef const Moa3dPoint * ConstPMoa3dPoint;

#endif /* __M3dTypes_H */
