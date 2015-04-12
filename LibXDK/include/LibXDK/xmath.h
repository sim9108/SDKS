/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_XMath
#define _H_XMath

///////////////////////////////////////////////////////////////////////////////////
//
//  Includes
//
///////////////////////////////////////////////////////////////////////////////////

#include "moatypes.h"

// ANSI
#include <math.h>

// defined in CW10 libraries but not CWPro2 or Windows
static const MoaDouble __x_pi = 3.1415926535897;	// stuff gets promoted to doubles anyway
static const MoaDouble __x_epsilon = 0.00000001f;	// good enough for me.


///////////////////////////////////////////////////////////////////////////////////
//
//  Macros / old school inline
//
///////////////////////////////////////////////////////////////////////////////////

#define RoundMoaFixed( i )	(((i) + 0x00008000) & 0xFFFF0000) 
#define LongToMoaFixed( i ) ((MoaLong)((i)) << 16)
#define MoaFixedToLong( i ) ((MoaLong)(RoundMoaFixed( i )) >> 16)

#define ULongToMoaFixed( i ) ((MoaUlong)((i)) << 16)
#define MoaFixedToULong( i ) ((MoaUlong)(RoundMoaFixed( i )) >> 16)

#define RoundMoaFloat( i )	(((i) + 0.5f))	// rounds up
#define MoaFloatToLong( i ) ((MoaLong)RoundMoaFloat( i ))

#define MoaFixedToFloat( i ) ((i) / 65536.0f)
#define MoaFloatToFixed( i ) ((MoaLong)((i) * 65536.0f))

///////////////////////////////////////////////////////////////////////////////////
//
//  Functions / Classes
//
///////////////////////////////////////////////////////////////////////////////////

// no name space support
template< class T > inline T XAbs( T value )
{
	return value < 0 ? -value : value;
}

template< class T > inline T XMin( T v1, T v2 )
{
	return v1 < v2 ? v1 : v2;
}

template< class T > inline T XMax( T v1, T v2 )
{
	return v1 > v2 ? v1 : v2;
}

// clamps values to min and max
template< class T > inline void XClamp( T& value, T min, T max )
{
	if( value > max )
		value = max;
	if( value < min )
		value = min;
}

inline void XScaleRect( MoaRect& value, MoaFloat xScale, MoaFloat yScale )
{
	// well lets multiply - round instead of truncate
	value.top 	 = MoaFloatToLong( (MoaFloat)value.top * yScale );
	value.bottom = MoaFloatToLong( (MoaFloat)value.bottom * yScale );
	value.left   = MoaFloatToLong( (MoaFloat)value.left * xScale );
	value.right  = MoaFloatToLong( (MoaFloat)value.right * xScale );
}

inline MoaBool XRectEqual( MoaRect& cmp1, MoaRect& cmp2, MoaBool exact = FALSE )
{
	if( exact )
	{
		// i guess I could do a memcmp
		return (cmp1.top == cmp2.top) && (cmp1.left == cmp2.left) && (cmp1.bottom == cmp2.bottom) && (cmp1.right == cmp2.right);
	}
	else
	{
		// compare width and height
		return ((cmp1.right - cmp1.left) == (cmp2.right - cmp2.left) &&
				(cmp1.bottom - cmp1.top) == (cmp2.bottom - cmp2.top));
	}
}

// gives you the arc tangent of a line
inline MoaDouble XAngle( MoaFloat x, MoaFloat y )
{
	// returns the angle whose tangent is y/x
	// all anomalies x = 0 are also checked
	if( ::fabs( x ) < __x_epsilon ) {
		if( ::fabs( y ) < __x_epsilon )
			return 0.0f;
		else if( y > 0.0f )
			return (__x_pi * 0.5f);
		else
			return (__x_pi * 1.5f);
	} else if( x < 0.0f )
		return (::atan( y/x ) + __x_pi);
	else
		return ::atan( y/x );
}

// this translates a Point
inline void XTranslatePoint( MoaPoint& value, MoaLong x, MoaLong y )
{
	// add the x and y values respectively
	value.x  += x;
	value.y += y;
}

// this rotates a point about the z axis -- angle is in degrees
inline void XRotatePoint( MoaPoint& value, MoaFloat angle )
{
	MoaDouble theta = angle * __x_pi / 180;

	// must make a copy -- duh ;-)
	MoaFloat xTemp = (MoaFloat)value.x;
	MoaFloat yTemp = (MoaFloat)value.y;
	
	// rotate the point
	value.x = MoaFloatToLong( xTemp * ::cos(theta) - yTemp * ::sin(theta) );
	value.y = MoaFloatToLong( xTemp * ::sin(theta) + yTemp * ::cos(theta) );
}

inline void XScalePoint( MoaPoint& value, MoaFloat xScale, MoaFloat yScale )
{
	value.x = MoaFloatToLong( (MoaFloat)value.x * xScale );
	value.y = MoaFloatToLong( (MoaFloat)value.y * yScale );
}


// input angle is in radians -- pass NULL for origin to get 0,0 as the origin
inline void XPointPolar( MoaFloat len, MoaDouble angle, MoaPoint& newPoint, ConstPMoaPoint origin = 0 )
{
	newPoint.x = MoaFloatToLong( len * ::cos( angle ) );
	newPoint.y = MoaFloatToLong( len * ::sin( angle ) );
	
	if( origin )
	{
		newPoint.x += origin->x;
		newPoint.y += origin->y;
	}
}

// angle returned is in radians
inline MoaDouble XPointAngle( const MoaPoint& point, ConstPMoaPoint origin = 0 )
{
	if( !origin )
		return ::XAngle( (MoaFloat)point.x, (MoaFloat)point.y );
	else
		return ::XAngle( (MoaFloat)(point.x - origin->x), (MoaFloat)(point.y - origin->y) );
}

inline MoaBool XPointEqual( const MoaPoint& p1, const MoaPoint& p2, MoaLong errorTolerance = 0 )
{
	return 	::XAbs( p1.x - p2.x ) <= errorTolerance && 
			::XAbs( p1.y - p2.y ) <= errorTolerance;
}

inline MoaFloat XPointLength( const MoaPoint& point )
{
	return (MoaFloat)::sqrt( (double)point.x * (double)point.x + (double)point.y * (double)point.y );
}

inline MoaLong XPointFastLength( const MoaPoint& point )
{
	MoaLong dx = ::XAbs( point.x );
	MoaLong dy = ::XAbs( point.y );
	return dx + dy - (::XMin( dx, dy ) >> 1);
}


inline MoaFloat XPointDistance( const MoaPoint& p1, const MoaPoint& p2 )
{
	MoaPoint vec;
	vec.x = p2.x - p1.x;
	vec.y = p2.y - p1.y;
	
	return ::XPointLength( vec );
}

inline MoaLong XPointFastDistance( const MoaPoint& p1, const MoaPoint& p2 )
{
	MoaLong dx = p2.x - p1.x;
	MoaLong dy = p2.y - p1.y;
	if ( dx < 0 ) 
		dx = -dx;
		
	if ( dy < 0 ) 
		dy = -dy;

	return (dx + dy - ::XMin( dx, dy )) >> 1;
}

// default to unit length
inline void XPointNormalize( MoaPoint& vector, MoaFloat thickness = 1.0f )	
{
	MoaFloat len = ::XPointLength( vector );
	
	// don't divide by zero - negative lengths are bad
	if( len > __x_epsilon )
	{
		len /= thickness;
		vector.x = MoaFloatToLong( (MoaFloat)vector.x / len);
		vector.y = MoaFloatToLong( (MoaFloat)vector.y / len);
	}	
}



// returns TRUE if lines intersect
inline MoaBool XLineIntersect( const MoaPoint& line1p1, const MoaPoint& line1p2, const MoaPoint& line2p1, const MoaPoint& line2p2, MoaPoint& ptIntersect )
{
	// we need to convert to base and direction vectors
	MoaPoint dirVector1, dirVector2;
	
	dirVector1.x = line1p2.x - line1p1.x;
	dirVector1.y = line1p2.y - line1p1.y;

	dirVector2.x = line2p2.x - line2p1.x;
	dirVector2.y = line2p2.y - line2p1.y;
	
	// finds the point vector of intersection (line1P1 + mu * line1p2 and line2p1 + lambda * line2p2)
	MoaFloat delta = (MoaFloat)(dirVector1.x * dirVector2.y - dirVector1.y * dirVector2.x);
	
	// if delta is zero then the lines are parallel -- no intersection sorry!
	if( ::XAbs( delta ) < __x_epsilon )
		return FALSE;
	
	// find mu value for (ptIntersect.x, ptIntersect.y) on first line
	MoaFloat mu = ((line2p1.x - line1p1.x) * dirVector2.y - (line2p1.y - line1p1.y) * dirVector2.x) / delta;
	
	// calculate x and y corrdinates of ptIntersect
	ptIntersect.x = MoaFloatToLong( line1p1.x + mu * dirVector1.x );
	ptIntersect.y = MoaFloatToLong( line1p1.y + mu * dirVector1.y );
	return TRUE;
}


inline int XSign( MoaFloat value )
{
	if( value > __x_epsilon )
		return 1;
	else if( value < -__x_epsilon )
		return -1;

	return 0;
}

// function returns -1 clockwise / 1 counterclockwise / 0 line or point
inline int XOrient( const MoaPoint& p0, const MoaPoint& p1, const MoaPoint& p2 )
{
	MoaPoint vector1, vector2;
	
	vector1.x = p1.x - p0.x; 
	vector1.y = p1.y - p0.y;
	
	vector2.x = p2.x - p1.x; 
	vector2.y = p2.y - p1.y;
	
	return ::XSign( (MoaFloat)(vector1.x * vector2.y - vector1.y * vector2.x) );
}

inline MoaDouble XDegreesToRadians( MoaDouble degrees )
{
	return degrees * (__x_pi / 180);
}

inline MoaDouble XRadiansToDegrees( MoaDouble radians )
{
	return radians / (__x_pi / 180);
}


////////// FIXED POINT OVERRIDES ///////////

// MoaFixed operator/( MoaFixed &lhs, MoaFixed &rhs );
// MoaFixed operator*( MoaFixed &lhs, MoaFixed &rhs );


#endif	// _H_XMath

// EOF
