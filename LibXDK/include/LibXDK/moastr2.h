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
	Abstract:
		Public header for MoaStream2.
*/

#ifndef _H_MoaStream2_
#define _H_MoaStream2_

/* ------------------------------------------------------------------------
** Includes
*/

#ifndef _H_moastdif
	#include "moastdif.h"
#endif

#ifndef _H_MoaIOAttr_
	#include "mioattr.h"
#endif

/* ------------------------------------------------------------------------
** Macros
*/

#define	kMoaStreamIOFlag_UseAsyncIO			0x0001L
#define kMoaStreamIOFlag_UseBufferredIO		0x0002L
#define kMoaStreamIOFlag_LockStreamBuffer	0x0004L

#define kMoaStreamOpenAccess_ResourceFork	0x8000

/*
** MoaStream2's error code range is 0x12E0 to 12EF
*/
#define	kMoaStreamErr_DataNotAvail 			MAKE_MOAERR(0x12E0)	/* Non-blocking read: data hasn't yet arrived */
#define	kMoaStreamErr_OpNotDone 			MAKE_MOAERR(0x12E1)	/* Op hasn't finished, so results not avail */
#define kMoaErr_ShMemLockError				MAKE_MOAERR(0x12E2)	/* Sharedm memory:  locked but shouldn't be; unlocked but should be */
#define kMoaErr_CacheIncompatible			MAKE_MOAERR(0x12E3)	/* Running at same time as an app with incompatible version of our cache */
#define kMoaErr_CacheDownloadStopped		MAKE_MOAERR(0x12E4)	/* Download:  Transfer halted */
#define kMoaErr_CacheWrongState				MAKE_MOAERR(0x12E5)	/* Client made a cache call at a bad time */
#define kMoaStream2Err_NoFileInterface		MAKE_MOAERR(0x12E6)	/* No underlying file interface */

/* ------------------------------------------------------------------------
** Types
*/

/*
** The following stream specific types are defined in moastdif.h:
**	MoaStreamCount
**	MoaStreamPosition
*/

/*
** MoaStreamPositionMode: used for relative SetPosition calls
*/
enum {
	kMoaStreamPositionMode_FromStart,
	kMoaStreamPositionMode_FromCurrent,
	kMoaStreamPositionMode_FromEnd
};

typedef MoaLong MoaStreamPositionMode;


/*
** MoaStreamIOFlags: used to request async or bufferred I/O
*/
typedef MoaLong MoaStreamIOFlags, * PMoaStreamIOFlags;


/* forward declare stream interface ptr */
typedef struct IMoaStream2 * PIMoaStream2, ** PPIMoaStream2;

/* forward declare stream operation ptr */
struct MoaStreamIOOperation;

/*
** MoaStream2IOCompletionProc: used to signal async I/O completion
*/
typedef STDPROCPTR(STDMETHODCALLTYPE, MoaError, MoaStream2IOCompletionProc)(
	struct MoaStreamIOOperation *	pStreamOp,
	PMoaVoid						pClientData
);


/* ------------------------------------------------------------------------
** MoaStreamIOOperation structure
*/
typedef struct MoaStreamIOOperation
{
	/* inputs */
	MoaStreamCount				ioRequestCount;
	PMoaVoid					pClientBuffer;
	MoaStreamIOFlags			ioFlags;
	MoaStream2IOCompletionProc	pCompletionProc;

	/* outputs */
	MoaError					ioResult;
	MoaStreamCount				actualIOCount;
	PMoaVoid					pStreamBuffer;

	/* both */
	PMoaVoid					pClientData;
} MoaStreamIOOperation, * PMoaStreamIOOperation;


/* ------------------------------------------------------------------------
** MoaStream2 Interface
*/
#undef	INTERFACE
#define INTERFACE IMoaStream2

DECLARE_INTERFACE_(IMoaStream2, IMoaUnknown)

/* description

MoaStream2 is a direct superset of MoaStream. It has new methods for
dealing with streams on remote files, for buffered and asynchronous I/O,
and for getting a MoaDict that can be a used for extended functionality.

*/

{
	STD_IUNKNOWN_METHODS

	STDMETHOD(Open)			(THIS_ 
		MoaLong accessMode, 			/* access mode for the stream */
		MoaLong setPositionType) 		/* set position type for the stream */
		PURE;

	/* returns
	
	MoaError
	
	kMoaErr_NoErr						- Success
	kMoaStreamErr_StreamAlreadyOpen		- Stream is already open
	kMoaStreamErr_BadAccessMode			- not an allowable r/w perm 
	kMoaStreamErr_BadSetPositionMode	- not an allowable seek perm
	
	*/
	
	/* description
	
	Opens the stream. The access mode paramater indicates the type of access
	required for the stream. Valid access modes are:
	
	Read		- Stream will only be read from
	Write		- Stream will only be written to
	ReadWrite	- Stream will be read from and written to
	
	The set position parameter determines what set position mode to use with
	the stream. Valid set position modes are:
	
	None		- SetPosition will never be called.
	Cheap		- Set position will be called often
	Expensive	- Set position will be called occasionally
	
	*/

	STDMETHOD(Close)		(THIS) PURE;
	
	/* returns
	
	MoaError
	
	kMoaErr_NoErr						- Success
	kMoaStreamErr_StreamAlreadyOpen		- Stream is already open
	
	*/
	
	/* description
	
	Closes the stream.
	
	*/
	
	STDMETHOD(Read)			(THIS_ 
		PMoaVoid			pBuf, 					/* pointer to buffer for data */
		MoaStreamCount		numToRead,				/* number of bytes to read from stream */
		MoaStreamCount *	pNumActuallyRead)		/* number of bytes actually read */
		PURE;
	
	/* returns
	
	MoaError
	
	kMoaErr_NoErr							- Success
	kMoaStreamErr_StreamNotOpen				- returned by everything but Open
	kMoaStreamErr_BadParameter 				- you passed me bad parm 
	kMoaStreamErr_IoError					- catch-all r/w/pos error 
	kMoaStreamErr_ReadPastEnd				- attempted to read past end 		
	kMoaStreamErr_DataNotAvail				- attempted to read past current length (partially downloaded streams)
	
	*/
	
	/* description
	
	Attempts to read numToRead bytes from the stream and write them into the buffer
	passed in. The number of bytes actually read from the stream is return in 
	numActuallyRead. The position will be incremented to reflect the number of bytes
	read from the stream.
	
	*/
	
	STDMETHOD(ReadAhead)	(THIS_ 
		PMoaVoid pBuf,						/* pointer to buffer for data */ 
		MoaStreamCount numToRead,			/* number of bytes to read from stream */
		MoaStreamCount * pNumActuallyRead) 	/* number of bytes actually read */
		PURE;
	
	/* returns
	
	MoaError
	
	kMoaErr_NoErr							- Success
	kMoaStreamErr_StreamNotOpen				- returned by everything but Open
	kMoaStreamErr_BadParameter 				- you passed me bad parm 
	kMoaStreamErr_IoError					- catch-all r/w/pos error 
	kMoaStreamErr_ReadPastEnd				- attempted to read past end
	kMoaStreamErr_ReadAheadTooFar			- you asked for too much ReadAhead
	kMoaStreamErr_DataNotAvail				- attempted to read past current length (partially downloaded streams)
	
	*/
	
	/* description
	
	Attempts to read numToRead bytes from the stream and write them into the buffer
	passed in. The number of bytes actually read from the stream is return in 
	numActuallyRead. The position is not incremented when performing a ReadAhead().
	
	*/
	
	STDMETHOD(Write)		(THIS_ 
		ConstPMoaVoid pBuf, 					/* buffer to be written in to stream */
		MoaStreamCount numToWrite,				/* number of bytes to write */
		MoaStreamCount * pNumActuallyWritten) 	/* number of bytes written */
		PURE;

	/* returns
	
	MoaError
	
	kMoaErr_NoErr							- Success
	kMoaStreamErr_StreamNotOpen				- returned by everything but Open
	kMoaStreamErr_BadParameter 				- you passed me bad parm 
	kMoaStreamErr_IoError					- catch-all r/w/pos error 
	kMoaStreamErr_WrotePastEnd				- attempted to write past end
	
	*/
	
	/* description
	
	Attempts to write numToWrite bytes into the stream. The number of bytes actually
	written to the stream is returned in numActuallyWritten. The stream position is
	updated to reflect the number of bytes written to the stream.
	
	*/
	
	STDMETHOD(GetPosition)	(THIS_ 
		MoaStreamPosition * pPos) /* on return contains stream position */ 
		PURE;
	
	/* returns
	
	MoaError
	
	kMoaErr_NoErr							- Success
	kMoaStreamErr_StreamNotOpen				- Stream no open
	kMoaStreamErr_IoError					- catch-all r/w/pos error
	
	*/
	
	/* description
	
	Returns the current position in the stream.
	
	*/
	
	STDMETHOD(SetPosition)	(THIS_ 
		MoaStreamPosition pos) /* new stream position */
		PURE;
	
	/* returns
	
	MoaError
	
	kMoaErr_NoErr							- Success
	kMoaStreamErr_StreamNotOpen				- Stream no open
	kMoaStreamErr_IoError					- catch-all r/w/pos error
	
	*/
	
	/* description
	
	Sets the current stream position to pos.
	
	*/
	
	STDMETHOD(GetEnd)		(THIS_ 
		MoaStreamPosition * pPos)		/* on return contains stream position */
		PURE;

	/* returns
	
	MoaError
	
	kMoaErr_NoErr							- Success
	kMoaStreamErr_StreamNotOpen				- Stream no open
	kMoaStreamErr_IoError					- catch-all r/w/pos error
	
	*/
	
	/* description
	
	Returns the position of the end of the stream.
	
	*/

	STDMETHOD(Flush)		(THIS) PURE;
	
	/* returns
	
	MoaError
	
	kMoaErr_NoErr							- Success
	kMoaStreamErr_StreamNotOpen				- Stream no open
	
	*/
	
	/* description
	
	Forces any data not written to the stream to be written.
	
	*/
	
	STDMETHOD(GetModes)		(THIS_ 
		MoaLong * pAccessMode,			/* on return contains access mode */ 
		MoaLong * pSetPositionType) 	/* on return contains set position type */
		PURE;

	/* returns
	
	MoaError
	
	kMoaErr_NoErr							- Success
	kMoaStreamErr_StreamNotOpen				- Stream no open
	
	*/
	
	/* description
	
	Returns the access mode and set position type for the current stream.
	
	*/

	/* New method for relative seeking */
	STDMETHOD(SetPositionRelative)	(THIS_ 
		MoaStreamPosition pos, 				/* new stream position offset */
		MoaStreamPositionMode mode) 		/* new stream position mode */
		PURE;

	/* returns
	
	MoaError
	
	kMoaErr_NoErr							- Success
	kMoaStreamErr_StreamNotOpen				- Stream no open
	kMoaStreamErr_IoError					- catch-all r/w/pos error
	
	*/
	
	/* description
	
	Sets the stream position relative to the stream position mode. Valid stream
	position modes are:
	
	kMoaStreamPositionMode_FromStart
	kMoaStreamPositionMode_FromCurrent
	kMoaStreamPositionMode_FromEnd
	
	*/

	/* New method for setting stream extent */
	STDMETHOD(SetEnd)				(THIS_ 
		MoaStreamPosition pos)				/* new stream extent */
		PURE;

	/* returns
	
	MoaError
	
	kMoaErr_NoErr							- Success
	kMoaStreamErr_StreamNotOpen				- Stream no open
	kMoaStreamErr_IoError					- catch-all r/w/pos error
	
	*/
	
	/* description
	
	Sets the end of the stream.

	*/

	/* New method for getting current stream extent (e.g. partial files) */
	STDMETHOD(GetCurrentLength)		(THIS_ MoaStreamPosition *pPos) PURE;

	/* returns
	
	MoaError
	
	kMoaErr_NoErr							- Success
	kMoaStreamErr_IoError					- catch-all r/w/pos error
	
	*/
	
	/* description
	
	Gets the available length of the stream, which might be different from its
	total length, esp. for streams on remote objects.

	*/



	/* New method for closing streams used as pipes */
	STDMETHOD(CloseWithError)		(THIS_ 
		MoaError err)						/* error code */ 
		PURE;

	/* returns
	
	MoaError
	
	kMoaErr_NoErr							- Success
	kMoaStreamErr_StreamNotOpen				- Stream no open
	
	*/
	
	/* description
	
	Closes the stream with an error condition of err. Used for closing streams
	used as pipes or as transformers between pull-mode and push-mode streams.

	The producer should call CloseWithError with a termination error, which in turn
	will be passed to the consumer on the next stream operation.
	
	*/

	/* New method for getting IMoaIOAttributeDict */
	STDMETHOD(GetMoaIOAttributeDict)(THIS_ 
		PIMoaIOAttributeDict *ppIDict)	/* on return contains IMoaIOAttributeDict pointer */
		PURE;

	/* returns
	
	MoaError
	
	kMoaErr_NoErr							- Success
	or standard MOA QueryInterface errors
	
	*/
	
	/* description
	
	Returns an IMoaIOAttributeDict interface which is used to enumerate through various
	stream and file attributes. This interface is identical to IMoaDict. The client is
	responsible for releasing this interface when done.
	
	The list of available keys include:

			kMoaIOKey_StreamFlags
			kMoaIOKey_StreamAccessMode
			kMoaIOKey_StreamBufferSize
			kMoaIOKey_StreamDataRate
			kMoaIOKey_StreamEnd
			kMoaIOKey_StreamCurrentLength
			kMoaIOKey_StreamPosition
			kMoaIOKey_StreamPositionType

			kMoaIOKey_FileFlags				
			kMoaIOKey_FileAccessPerm
			kMoaIOKey_FileLength
			kMoaIOKey_FileType
			kMoaIOKey_FileEncoding
			kMoaIOKey_FileCreator
			kMoaIOKey_FileCreateDate
			kMoaIOKey_FileModifyDate
			kMoaIOKey_FileAccessDate
			kMoaIOKey_FileCachePriority	
	
			kMoaIOKey_VolumeFlags
			kMoaIOKey_VolumeBlockSize
			kMoaIOKey_VolumeSectorSize
			kMoaIOKey_VolumeFreeSpace

			kMoaIOKey_LASTPREDEFINED

	Attributes are designed to be accessed thru the Get/Put methods of the IMoaDict 
	interface. The type of an attribute's value pointer is specific to the attribute:

			Stream Attribute	Type of valuePtr	Length
			----------------	----------------	------
			Flags				PMoaLong			4
			AccessMode			PMoaLong			4
			BufferSize			PMoaLong			4
			DataRate			PMoaLong			4
			End					PMoaLong			4
			CurrentLength		PMoaLong			4
			Position			PMoaLong			4
			PositionType		PMoaLong			4

			File Attribute		Type of valuePtr	Length
			--------------		----------------	------
			Flags				PMoaLong			4
			AccessPerm			PMoaLong			4
			Length				PMoaLong			4
			Type				PMoaChar			variable
			Encoding			PMoaChar			variable
			Creator				PMoaChar			variable
			CreateDate			PMoaWide			8
			ModifyDate			PMoaWide			8
			AccessDate			PMoaWide			8
			CachePriority		PMoaLong			4

			Volume Attribute	Type of valuePtr	Length
			----------------	----------------	------
			Flags				PMoaLong			4
			BlockSize			PMoaLong			4
			SectorSize			PMoaLong			4
			FreeSpace			PMoaLong			4

	*/
	
	/* see also
	
	IMoaDict
	
	*/

	/* New i/o methods: */
	STDMETHOD(ReadOperation)		(THIS_ 
		PMoaStreamIOOperation pOperation)	/* pointer to an MoaStreamIOOperation structure */ 
		PURE;

	/* returns
	
	MoaError
	
	kMoaErr_NoErr							- Success
	kMoaStreamErr_StreamNotOpen				- returned by everything but Open
	kMoaStreamErr_BadParameter 				- you passed me bad parm 
	kMoaStreamErr_IoError					- catch-all r/w/pos error 
	kMoaStreamErr_ReadPastEnd				- attempted to read past end 		
	kMoaStreamErr_DataNotAvail				- attempted to read past current length (partially downloaded streams)
	

	Sets output fields of MoaStreamIOOperation structure.
	*/
	
	/* description
	
	Performs a READ operation using parameters specified by the MoaStreamIOOperation structure.
	There are 4 basic modes of I/O specified using the ioFlags parameter:
		IOOperations that are synchronous (blocking) and un-bufferred
		IOOperations that are asynchronous (non-blocking)
		IOOperations that are bufferred
		IOOperations that are provide direct access to a stream's buffer

	The asynchronous, bufferred and direct-access modes are optional per implementation.
	Their availability can be queried using the MoaDict returned by GetMoaIOAttributeDict().

	The asynchronous and buffered modes can be combined to request async, buffered I/O.

	The direct-access mode can be used to bypass memory copies that would normally result in 
	bufferred operation between a stream's buffer and a client's buffer.
	Direct-Access mode cannot be combined with any other modes.

	The client must also specify other inputs in MoaStreamIOOperation structure:
	- ioFlags:			the I/O mode (as described above)
	- ioRequestCount:	the number of bytes to be transferred or 
						for direct-access mode, bytes to be accessed in the stream buffer
	- pClientBuffer:	the destination for the data that will be read, ignored
						for direct-access mode
	- pCompletionProc:	a function to be called when an asynchronous read has completed
						only called if the read was actually asynchronous.
	- pClientData:		any piece of data to be passed to the pCompletionProc.

	Upon return, the output fields of the MoaStreamIOOperation structure are set as follows:
	- ioResult:			the result of the I/O operation (should match return value on synchronous reads)
	- actualIOCount:	the actual number of bytes transferred or 
						for direct-access mode, bytes made available in the stream buffer
	- pStreamBuffer:	a pointer to a region of the stream's buffer,
						the size of the region is returned in the actualIOCount field.
						
	*/
	
	STDMETHOD(WriteOperation)		(THIS_ 
		PMoaStreamIOOperation pOperation)	/* pointer to an MoaStreamIOOperation structure */ 
		PURE;
	
	/* returns
	
	MoaError
	
	kMoaErr_NoErr							- Success
	kMoaStreamErr_StreamNotOpen				- returned by everything but Open
	kMoaStreamErr_BadParameter 				- you passed me bad parm 
	kMoaStreamErr_IoError					- catch-all r/w/pos error 
	kMoaStreamErr_WrotePastEnd				- attempted to write past end
	
	*/
	
	/* description
	
	Performs a WRITE operation using parameters specified by the MoaStreamIOOperation structure.
	There are 4 basic modes of I/O specified using the ioFlags parameter:
		IOOperations that are synchronous (blocking) and un-bufferred
		IOOperations that are asynchronous (non-blocking)
		IOOperations that are bufferred
		IOOperations that are provide direct access to a stream's buffer

	The asynchronous, bufferred and direct-access modes are optional per implementation.
	Their availability can be queried using the MoaDict returned by GetMoaIOAttributeDict().

	The asynchronous and buffered modes can be combined to request async, buffered I/O.

	The direct-access mode can be used to bypass memory copies that would normally result in 
	bufferred operation between a stream's buffer and a client's buffer.
	Direct-Access mode cannot be combined with any other modes.

	The client must also specify other inputs in MoaStreamIOOperation structure:
	- ioFlags:			the I/O mode (as described above)
	- ioRequestCount:	the number of bytes to be transferred or 
						for direct-access mode, bytes to be accessed in the stream buffer
	- pClientBuffer:	the source for the data that will be written, ignored
						for direct-access mode.
	- pCompletionProc:	a function to be called when an asynchronous write has completed
						only called if the write was actually asynchronous.
	- pClientData:		any piece of data to be passed to the pCompletionProc.

	Upon return, the output fields of the MoaStreamIOOperation structure are set as follows:
	- ioResult:			the result of the I/O operation (should match return value on synchronous writes)
	- actualIOCount:	the actual number of bytes transferred or 
						for direct-access mode, bytes made be available in the stream buffer
	- pStreamBuffer:	a pointer to a region of the stream's buffer,
						the size of the region is returned in the actualIOCount field.
						
	*/

	STDMETHOD(ReleaseStreamBuffer)	(THIS_ 
		PMoaStreamIOOperation pOperation)	/* pointer to an MoaStreamIOOperation structure */ 
		PURE;

	/* returns
	
	MoaError
	
	kMoaErr_NoErr							- Success
	
	*/
	
	/* description
	
	This method will only be supported by stream implementations that support direct-access mode
	MoaIOStreamOperations.
	
	After a MoaStream2 client has acquired a region of a stream's buffer for read or write, the
	client will read from (or write to) the buffer, then the client must return that region
	of the stream's buffer back to the stream implementation.

	This method returns the stream buffer region specified by the pOperation parameter back to the 
	stream implementation. The pOperation structure must have been used by the client for a direct-access
	read or write to the stream's buffer.

	*/

};

/* Original MoaStream GUID for reference purposes: */
/* DEFINE_GUID(IID_IMoaStream, 0xAC734D52L, 0x005D, 0x042A, 0x00, 0x00, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3); */

/* IID_IMoaStream2: 6A9CE612-78EC-11d0-9037-00059A20195A */
DEFINE_GUID(IID_IMoaStream2, 0x6A9CE612L, 0x78EC, 0x11D0, 0x90, 0x37, 0x00, 0x05, 0x9A, 0x20, 0x19, 0x5A);


#endif /* _H_MoaStream2_ */
