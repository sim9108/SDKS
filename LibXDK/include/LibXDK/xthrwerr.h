/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_XThrwErr
#define _H_XThrwErr


///////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
///////////////////////////////////////////////////////////////////////////////////

#include "XMoaExpt.h"

///////////////////////////////////////////////////////////////////////////////////
//
// Declares
//
///////////////////////////////////////////////////////////////////////////////////

template<class T,int safeValue> class XThrowingErr
{
	public:
		// constructor
		XThrowingErr() { m_errVal = safeValue; }
		
		
		// Methods
		const T& operator=(const T& rhs)
		{
			m_errVal = rhs;
			if (m_errVal != safeValue)
				throw m_errVal;
			return m_errVal;
		}

		operator T() const			{ return m_errVal; }
		operator MoaBool() const 	{ return (m_errVal != safeValue); }
		T& 		 Value() 			{ return m_errVal; }
		
	protected:
		T m_errVal;
};


class TMoaError
{
	public:
		// default constructor
		TMoaError() : m_errVal(kMoaErr_NoErr) {}
		// constructor for single line use
		TMoaError( const MoaError& err ) : m_errVal(err) 
		{ 
			if(m_errVal != kMoaErr_NoErr)
				kThrowMoaExceptionErr(CMoaRuntimeError,m_errVal);
		}
		
		const MoaError& operator=(const MoaError& rhs)
		{
			m_errVal = rhs;
			if (m_errVal != kMoaErr_NoErr)
				kThrowMoaExceptionErr(CMoaRuntimeError,m_errVal);
			return m_errVal;
		}
		operator MoaError() const	{ return m_errVal; }
		operator MoaBool() const	{ return (m_errVal != kMoaErr_NoErr); }
		MoaError& Value() 			{ return m_errVal; }

	protected:
		MoaError m_errVal;
};

#if XDEBUG || defined( XPtrCheck_USE_MACRO )
// when you are debugging you want this to be a macro,
// otherwise you will have to find the right breakpoint
// somewhere in a mess of different template instanciations.
#define XPtrCheck( ptr ) if( !(ptr) ) kThrowMoaException( CMoaInvalidArgument )

#else
// ptr checker -- throws when pointer is NOT valid.
template< class T > inline void XPtrCheck( T ptr )
{
	if( !ptr ) 
		kThrowMoaException( CMoaInvalidArgument );
}
#endif

#endif	// !_H_XThrwErr

// EOF
