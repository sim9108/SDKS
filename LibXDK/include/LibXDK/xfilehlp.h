/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_XFileHlp
#define _H_XFileHlp

/* ================================================================================

	Purpose:
	
	Provides utility functions that make it easier to manage and access files
	in a platform independent manner.
	
=================================================================================== */

///////////////////////////////////////////////////////////////////////////////////
//
// Description
//
///////////////////////////////////////////////////////////////////////////////////

/*
	This file is used create and access files.
	Provides class for temp file location.
	Provides class for saving state before doing stream manipulation.
*/

///////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
///////////////////////////////////////////////////////////////////////////////////

// XFileHlp.h

#include "MoaFile2.h"
#include "XSupport.h"

#include "XStrmHlp.h"


class MMString;

///////////////////////////////////////////////////////////////////////////////////
//
// TempFilePathHelper
//		This class returns either an IMoaPathName that use can own
//		or simply the path as an ascii string.
//
///////////////////////////////////////////////////////////////////////////////////

class TempFilePathHelper
{
	public:
		TempFilePathHelper( void ) : mPath( NULL )
		{
			MoaError err = XSupport::GetCallback()->MoaCreateInstance( &CLSID(CMoaPath),
							&IID(IMoaPathName), (PMoaVoid*)&mPath );
			if( err )
				kThrowMoaExceptionErr( CMoaBadInterface, err );

			// now ask for temp path
			err = mPath->SetTempPath( NULL );
			if( err )
				kThrowMoaExceptionErr( CMoaRuntimeError, err );
		}

		~TempFilePathHelper( void )
		{
			// dump our path
			if( mPath )
				mPath->Release();
		}


		void GetPathString( PMoaChar pathString, MoaUlong bufSize ) const
		{
			if( !pathString || bufSize <= 0 )
				kThrowMoaException( CMoaInvalidArgument );

			MoaError err = mPath->GetPath( pathString, bufSize );
			if( err )
				kThrowMoaExceptionErr( CMoaFileException, err );
		}

		MoaUlong GetPathLength( void ) const
		{
			MoaLong size = 0;
			mPath->GetPathSize( &size );
			return (MoaUlong)size;
		}

		PIMoaPathName GetPathObject( void ) const { return mPath; }

	private:
		PIMoaPathName 	mPath;
};



///////////////////////////////////////////////////////////////////////////////////
//
// FilePositionSaver
// 		This grabs the file position and restores upon destruction
//
///////////////////////////////////////////////////////////////////////////////////
class FilePositionSaver
{
	public:
		FilePositionSaver( PIMoaStream2 file ) :
			mFile( file ),
			mPosition( 0 )
		{
			mFile->GetPosition( &mPosition );
		}

		~FilePositionSaver( void )
		{
			mFile->SetPosition( mPosition );
		}

	private:
		PIMoaStream2		mFile;
		MoaStreamPosition 	mPosition;

};


#endif // _H_XFileHlp

// EOF
