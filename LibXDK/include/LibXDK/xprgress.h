/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

// XPrgress.h

#ifndef _H_XPrgress
#define _H_XPrgress

#include "MuiHelpr.h"

// declare our base class -- also useful if you don't what to use wrapper.
typedef IMoaHelper< &IID(IMoaProgressBox), PIMoaProgressBox > IMoaProgressHelper;

class XProgressHelper : IMoaProgressHelper
{
	public:
		XProgressHelper( void ) :
			mShowStop( FALSE ),
			mOpen( FALSE )
		{
		}

		XProgressHelper( MoaBool showStopButton, MoaBool show = FALSE ) : 
			mShowStop( showStopButton ),
			mOpen( FALSE )
		{
			if( show )
			{
				Use()->OpenProgress( mShowStop );
				mOpen = TRUE;
			}
		}
		
		virtual ~XProgressHelper( void ) throw()	// ensures calling baseclass
		{
			if( mOpen )		
				Use()->CloseProgress();
		}
		
		// methods
		void Open( void )
		{
			if( !mOpen )
				Use()->OpenProgress( mShowStop );
		}

		void Close( void )
		{
			if( mOpen )
				Use()->CloseProgress();
		}

		void SetCaption( ConstPMoaChar s )	
		{
			// call through
			if( mOpen )
				Use()->SetCaption( s );
		}

		void SetMessage( ConstPMoaChar s )	
		{
			// call through
			if( mOpen )
				Use()->SetUserMessage( s );
		}

		void SetProgress( MoaUlong done, MoaUlong total )	
		{
			// call through
			if( mOpen )
				Use()->SetUserProgress( done, total );
		}
		
		MoaBool TestUserCanceled( void )
		{
			if( mOpen )
				return (MoaBool)Use()->TestUserCanceled();
			
			return FALSE;
		}
	
	private:
		MoaBool	 mOpen, mShowStop;
};
typedef XProgressHelper * PXProgressHelper;


#endif // _H_XPrgress

		
// EOF
