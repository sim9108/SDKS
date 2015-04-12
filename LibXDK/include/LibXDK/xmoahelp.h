/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/


#ifndef _H_XMoaHelp
#define _H_XMoaHelp

#include "MuiHelpr.h"

template <class Tpointer> class MoaHandleLocker
{
	public:
		MoaHandleLocker( MoaHandle& handleToLock ) :
			mHandle( handleToLock ),
			mMemory( NULL )
		{
			// lock handle
			if( mHandle )
				mMemory = (Tpointer)IMoaHandleHelper().Use()->Lock( mHandle );
			
			if( !mMemory )
				kThrowMoaException( CMoaInvalidArgument );
		}
			
		~MoaHandleLocker( void )
		{
			if( mHandle )
			{
				IMoaHandleHelper().Use()->Unlock( mHandle );
				mMemory = NULL;
			}
		}
		

#ifdef WINDOWS
	#pragma warning( disable : 4284 )
#endif
		Tpointer operator->( void )			{ return mMemory; }
 		operator Tpointer ( void )			{ return mMemory; }
		MoaByte* GetBytePtr( void )			{ return (MoaByte*)mMemory; }
		MoaLong	 GetSize( void )			{ return IMoaHandleHelper().Use()->GetSize( mHandle ); }
		
	protected:
		MoaHandle&	mHandle;
		Tpointer	mMemory;
};
typedef MoaHandleLocker<MoaByte*> MoaHandleLock;
typedef MoaHandleLocker<PMoaChar> CharLocker;

class MoaReleaser
{
	public:
		MoaReleaser( PIMoaUnknown& ptr ) : mInterface( ptr ) {}
		~MoaReleaser( void )
		{
			if( mInterface )
				mInterface->Release();
		}
	
	private:
		PIMoaUnknown& mInterface;
};

#ifdef WINDOWS
#pragma warning( default: 4284 )
#endif

#endif // _H_XMoaHelp

// EOF
