/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_XStrmHlp
#define _H_XStrmHlp

/*
	Purpose:
	
	Provides utility functions that make it easier to manage and access files
	in a platform independent manner.
*/

///////////////////////////////////////////////////////////////////////////////////
//
// Description
//
///////////////////////////////////////////////////////////////////////////////////

/*
	This file is used create and access streams.
*/

///////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
///////////////////////////////////////////////////////////////////////////////////

// XStrmHlp.h


#include "MoaStdIf.h"

class MMString;
///////////////////////////////////////////////////////////////////////////////////
//
// Stream helper moved from MuiHelpr.h due to incompatabilities with OLE2 SDK on MAC
//
///////////////////////////////////////////////////////////////////////////////////

// an x/platform stream helper.
class IMoaStreamHelper
{
public:
	IMoaStreamHelper(PIMoaStream pStrm,MoaBool winStream =
#if defined(WINVER) || (defined(MACINTOSH) && defined(__LITTLE_ENDIAN__))
		TRUE
#else
		FALSE
#endif
	);

	virtual ~IMoaStreamHelper();

	MoaError Open(MoaLong accessMode,MoaLong setPositionType)
	{
		return m_pStream->Open(accessMode,setPositionType);
	}
	
	MoaError Close()
	{
		return m_pStream->Close();
	}

	MoaError Read(PMoaVoid pBuf, MoaStreamCount numToRead, MoaStreamCount * pNumActuallyRead)
	{
		return m_pStream->Read(pBuf,numToRead,pNumActuallyRead);
	}

	MoaError ReadAhead(PMoaVoid pBuf, MoaStreamCount numToRead, MoaStreamCount * pNumActuallyRead)
	{
		return m_pStream->ReadAhead(pBuf,numToRead,pNumActuallyRead);
	}

	MoaError Write(PMoaVoid pBuf, MoaStreamCount numToWrite, MoaStreamCount * pNumActuallyWritten)
	{
		return m_pStream->Write(pBuf,numToWrite,pNumActuallyWritten);
	}

	MoaError GetPosition(MoaStreamPosition * pPos)
	{
		return m_pStream->GetPosition(pPos);
	}

	MoaError SetPosition(MoaStreamPosition pos)
	{
		return m_pStream->SetPosition(pos);
	}

	MoaError GetEnd(MoaStreamPosition * pPos)
	{
		return m_pStream->GetEnd(pPos);
	}

	MoaError Flush()
	{
		return m_pStream->Flush();
	}

	MoaError GetModes(MoaLong * pAccessMode, MoaLong * pSetPositionType)
	{
		return m_pStream->GetModes(pAccessMode,pSetPositionType);
	}

	IMoaStreamHelper& operator<<(MoaLong rhs);
	IMoaStreamHelper& operator<<(MoaShort rhs);
	IMoaStreamHelper& operator<<(MoaUlong rhs);
	IMoaStreamHelper& operator<<(MoaUshort rhs);
	IMoaStreamHelper& operator<<(MoaDouble rhs);
	IMoaStreamHelper& operator<<(MoaChar rhs);
	IMoaStreamHelper& operator<<(MoaBool rhs);
	IMoaStreamHelper& operator<<(PMoaChar rhs);
	IMoaStreamHelper& operator<<(unsigned char* rhs);
	IMoaStreamHelper& operator<<(const MMString& rhs);

	IMoaStreamHelper& operator>>(MoaLong& rhs);
	IMoaStreamHelper& operator>>(MoaShort& rhs);
	IMoaStreamHelper& operator>>(MoaUlong& rhs);
	IMoaStreamHelper& operator>>(MoaUshort& rhs);
	IMoaStreamHelper& operator>>(MoaDouble& rhs);
	IMoaStreamHelper& operator>>(MoaChar& rhs);
	IMoaStreamHelper& operator>>(MoaBool& rhs);
	IMoaStreamHelper& operator>>(PMoaChar rhs);
	IMoaStreamHelper& operator>>(MMString& rhs);
	IMoaStreamHelper& operator>>(unsigned char* rhs);

	static MoaLong XLong(MoaLong rhs);
	static MoaShort XShort(MoaShort rhs);

protected:
	PIMoaStream m_pStream;
	MoaBool		m_swap;
};
typedef IMoaStreamHelper * PIMoaStreamHelper;

#endif // _H_XStrmHlp

// EOF

