/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

/* This header file contains the interfaces for MIX sound access.
/
/	The class hierarchy for these interfaces is thus:
/
/	IMoaReceptor (base class from MIX headers)
/		IMoaReceptorSound (abstract)
/			IMoaReceptorSoundBuffer
/			IMoaReceptorSoundStream
/
/	We require two different approaches to data such as sound.
/
/	Stream:
/		This is absolutely necessary because of the huge amount of
/		data that can be represented. It eliminates the requirement
/		to read the entire sound into RAM. This approach is necessary
/		for applications to be able to play the sound back using a
/		double buffered mechanism, or for applications to edit large
/		sound files. Note that the normal seeking mechanism is insufficient
/		because byte offsets into the sound stream are not particularly
/		meaningful. The client of the stream needs to specify locations
/		in terms of time or frame/sample offsets. The calculations necessary
/		to convert these offsets into byte offsets must be done within the
/		MIX Xtra (which knows how the file is structured).
/
/	Buffered:
/		Some acquisition devices, like tape recorders, etc cannot
/		provide a stream. The can only feed the data directly to
/		the application as it becomes available. This is also
/		an easier method of implementing Writers.
/
/	NOTE: All callers of Readers must provide a IMoaReceptorSoundBuffer. There
/		Will be a Moa service to provide this automatically from a Stream.
/
/	Compression:
/		There are different ways that compressed sound data will be used:
/	
/	- Sound editing applications:
/		These don't want the data in compressed form, but want it
/		decompressed before being passed to the Receptor, and recompressed
/		when writing the data out.
/
/	- Sound playback applications:
/		These want the data decompressed if there is no hardware to do
/		so on the platform, or if hardware is available the data needs to
/		be passed directly to such hardware.
/
/	In both of the above cases, the Moa standard compression interface
/	will be used if the application needs to decompress the data itself.
*/		

#ifndef _MIXSND_H
#define _MIXSND_H

#include "mixagent.h"
#include "moaxtra.h"

/*
	MIX 1.02 / MOA 2 now use this macro as preferred...
*/
#ifndef MoaDeclareMethod
	#define MoaDeclareMethod(macro_NAME, macro_ARGS) \
		STDMETHOD(macro_NAME) macro_ARGS PURE;
#endif
#ifndef MoaDeclareMethod_
	#define MoaDeclareMethod_(macro_RET, macro_NAME, macro_ARGS) \
		STDMETHOD_(macro_RET, macro_NAME) macro_ARGS PURE;
#endif

typedef PMoaVoid	MoaSampleBuffer;

typedef MoaID MoaSoundHeaderType;

#if 0
/* 6a527444-0810-11d0-b28a-000502e85810 */
DEFINE_GUID(kMoaSoundHeader_None,
				0x6A527444L, 0x0810, 0x11D0,
				0xB2, 0x8A, 0x00, 0x05, 0x02, 0xE8, 0x58, 0x10);
#else
#define kMoaSoundHeader_None IID_NULL
#endif

/* 6a528ef2-0810-11d0-b28a-000502e85810 */
DEFINE_GUID(kMoaSoundHeader_AIFF,
				0x6A528EF2L, 0x0810, 0x11D0,
				0xB2, 0x8A, 0x00, 0x05, 0x02, 0xE8, 0x58, 0x10);
/* 6a5293a2-0810-11d0-b28a-000502e85810 */
DEFINE_GUID(kMoaSoundHeader_WAVE,
				0x6A5293A2L, 0x0810, 0x11D0,
				0xB2, 0x8A, 0x00, 0x05, 0x02, 0xE8, 0x58, 0x10);

typedef MoaID MoaSoundCompressionType;

#define kMoaCompressionType_None IID_NULL

/* These defines are dict keys for a sound Writer's options dict.
 * You can call GetDefaultWriterOptions or GetWriterOptions to get an options dict,
 * and then use these keys to get the following info:
 *		- the number of compressors found for this sound format.
 *		- an array (NumCompressors long) of MoaID's of found compressors.
 *		  Will include IID_NULL if no compression is an option.
 *		- the MoaID of the preferred compressor. Might be IID_NULL (e.g. no compression).
 *		- the preferred frame rate of the sound.
 *		- the preferred number of channels (1 or 2).
 *		- the preferred number of bits per sample (8 or 16, usually).
 *
 * This dict can also be used when creating a transformed stream in IID_IMoaSoundServices.
 */
#define kWriterOptKey_NumCompressors		"NumCompressors"	/* kMoaDictType_Long */
#define kWriterOptKey_CompressorList		"ListCompressors"	/* kMoaDictType_Bytes */
#define kWriterOptKey_PreferredCompressor	"PrefCompressor"	/* kMoaDictType_MoaID */
#define kWriterOptKey_PreferredFrameRate	"PrefFrameRate" 	/* kMoaDictType_Long */
#define kWriterOptKey_PreferredNumChannels	"PrefNumChannels"	/* kMoaDictType_Long */
#define	kWriterOptKey_PreferredBitsSample	"PrefBitsSample"	/* KMoaDictType_Long */

typedef struct
{
	/* Terminology: A "sample" is one sound level value in one channel
	/  at one given instant, the smallest discrete unit of sound data.
	/  A "frame" is the set of samples for all simultaneous channels.
	/  For example, if a sample is one byte and there are two channels
	/  in the sound, a frame consists of two samples, one for left and
	/  one for right.  In monaural sound, a sample and a frame are
	/  usually the same thing, except in the rare case where there is
	/  padding within frames.
	/
	/  We distinguish between samples and frames to avoid terminology
	/  confusion.  For example, if a stereo sound has a "sample rate"
	/  of 44 kHz, this means that 44,000 samples pass by per second.
	/  Since the sound is in stereo, this would technically mean there
	/  are two simultaneous 22kHz sounds, left and right, for a total
	/  bandwidth of 44kHz!	This is probably not the intended meaning
	/  as the term "sample" has been overloaded.  We use the term
	/  "frame" to refer to all simultaneous samples per instant.  Thus
	/  the sound has a "frame rate" of 44kHz, meaning both the left and
	/  right channels play at 44kHz, for a total data rate of 88,000
	/  samples per second. */

	/* Byte offset from the beginning of the file or memory handle to the
	/  first byte of the sound sample data. */
	MoaLong   iOffset;

	/* Total byte size of all sound sample data present in the sound. */
	MoaLong   iSize;

	/* The next four values determine exactly how to play the sound.
	/  They are given as both byte and frame offsets to handle compressed
	/  sound. For playback applications, it should be possible to seek
	/  to the byte offset and start playing the sound. It is assumed
	/  that this point is a good synchronization point in the compressed
	/  sound stream. For other applications, the sound can be decompressed and
	/  the frame offset will apply in the resulting output.
	/
	/  For uncompressed sound, the byte and frame offsets must refer to the same
	/  smae sample and either can be used depending on which is more convenient.
	/
	/  Playback starts at iPlaybackStart(Frame) and proceeds on up to, but
	/  not including, iPlaybackEnd(Frame).	If iLoopEnd(Frame) is nonzero, the
	/  sound has looping.  In this case, the sound plays in three
	/  phases.	First, the sound plays from iPlaybackStart(Frame) up to, but
	/  not including, iLoopStart(Frame).  Next, the sound loops from
	/  iLoopStart(Frame) up to, but not including, iLoopEnd(Frame) until the loop
	/  is broken.  Finally, the sound plays from iLoopEnd up to,
	/  but not including, iPlaybackEnd(Frame).
	/
	/  When playing non-looping sounds, the four values must relate
	/  as follows:
	/	 iPlaybackStart(Frame) < iPlaybackEnd(Frame), iLoopStart(Frame) = xxx, iLoopEnd(Frame) = 0.
	/
	/  When playing looping sounds, the four values must relate as
	/  follows:
	/	 iPlaybackStart(Frame) <= iLoopStart(Frame) < iLoopEnd(Frame) <= iPlaybackEnd(Frame). */

	/* Byte and frame offset within the sound samples of the first whole sample
	/  at which to begin playback. */
	MoaLong   iPlaybackStart;
	MoaLong   iPlaybackStartFrame;

	/* Byte and frame offset within the sound samples of the first whole sample
	/  of the loopback start point (set to iPlaybackStart if the sound
	/  contains no intrinsic looping). */
	MoaLong   iLoopStart;
	MoaLong   iLoopStartFrame;

	/* Byte and frame offset within the sound samples of the first whole sample
	/  after the loopback end point (set to iPlaybackStart if the sound
	/  contains no intrinsic looping). */
	MoaLong   iLoopEnd;
	MoaLong   iLoopEndFrame;

	/* Byte and frame offset within the sound samples of the first whole sample
	/  not to play. */
	MoaLong   iPlaybackEnd;
	MoaLong   iPlaybackEndFrame;

	/* Total number of frames in the sound. */
	MoaLong   iNumFrames;

	/* Playback rate of the sound in frames per second.  This statistic
	/  has traditionally been called the "sample rate."  We call it the
	/  frame rate to clarify the difference between samples and frames. */
	MoaLong   iFrameRate;

	/* Data rate of sound playback in bytes per second.  This figure can
	/  be used to determine the duration of a sound or a portion of a
	/  sound given its byte size.  This is the frame rate multiplied by
	/  the number of bytes per frame. */
	MoaLong   iByteRate;

	/* MoaID to identify compression format used within the sound sample data.
	/ Is IID_NULL if no compression. */
	MoaSoundCompressionType   iCompressionType;

	/* Bits per sample. */
	MoaLong   iBitsPerSample;

	/* Bytes per sample. */
	MoaLong   iBytesPerSample;

	/* Number of channels in the sound (1 = mono, 2 = stereo).	This is
	/  also the number of samples per frame. */
	MoaLong   iNumChannels;

	/* Number of bytes per frame.  This is not necessarily the same as the
	/  number of channels multiplied by the bytes per sample because there
	/  may actually be padding within frames. */
	MoaLong   iBytesPerFrame;

	/* MoaID to identify the type of samples in the file, which can be used by
	/  platform dependent playback code to play native sound formats.
	/ Is IID_NULL if sample data is "raw." */
	MoaSoundHeaderType	  SampleType;

	/* This is a buffer which may hold platform dependent data pertaining
	/  to the sound format.  The client application must preserve this data
	/  and must not write anything here. */
	MoaUlong  uPlatformData[63];

	/* Number of bytes in a decompression block. WAVE files put nBlockAlign
	/  into this field. */
	MoaLong   iBytesPerBlock;

}	MoaSoundFormat, *PMoaSoundFormat;

/* ------------------
/ * The rate of the sound is represented as the actual
/ rate of the sound multiplied by 1000.
/ So 11.025 Khz is represented by the number: 11025.
/ ------------------
/ ** The base frequency is the actual NOTE that the sound was
/ sampled at. Middle C is 60.
/ ------------------
/ The sound data in a MoaSampleBuffer is defined as follows:
/
/ Each sample of a sound contains each channel of the sound
/ interleaved.
/
/ So if I have 3 channels of 8 bit sound, each sample
/ will contain (each number is 1 bit):
/	111111112222222233333333
/
/	Each number represents 1 bit of sound in that channel.
/
/ 4 channels of 4 bit sound will contain:
/	1111222233334444
/ ------------------ */

/* ----------------------------------- IMoaReceptorSound */

/* There are two sound receptors. A Stream based receptor
/ called "IMoaReceptorSoundStream" and another called
/ "IMoaReceptorSoundBuffer". Both of these receptors
/ inherit from IMoaReceptorSound, which contains methods
/ used to get information about the sound.
/
/	InquireCompression
/		Just after calling "SetFormat", a Reader will inquire what
/		it should do about compressed sound with this method.
/		The receptor will be passed an interface ID of a standard
/		compression interface to use to decompress the data. If this
/		is the case, the data will be passed on in it's original
/		raw form.
/		Alternatively, the application can return flags that tell the
/		reader what to do with the data. (see the flags at the end
/		for a definition of each).
/
/	SetFormat
/		Just after calling "InquireCompression", this will pass a data
/		structure which contains necessary
/		information about the format of the sound (see above). The
/		format of the data depends on the settings chosen in the compression
/		query.
/
/	Note that this is an Abstract interface. there is no class id
/	defined it. It is used to group useful methods common to all
/	sound receptors.
*/

#undef INTERFACE
#define INTERFACE IMoaReceptorSound
DECLARE_INTERFACE_(IMoaReceptorSound, IMoaReceptor)
{
	STD_IUNKNOWN_METHODS

	/* standard IMoaReceptor methods. */
	
	MoaDeclareMethod(SetRefCon, (THIS_
		moa_in PMoaVoid refCon
	))

	MoaDeclareMethod(SetStatus, (THIS_
		moa_in MoaError status
	))

	MoaDeclareMethod(InquireFormat, (THIS_
		moa_in MoaLong nRates, moa_inout MoaLong *rates, moa_in MoaBoolParam rateRange,
		moa_in MoaLong nDepths, moa_inout MoaLong *depths,
		moa_in MoaLong nHeaderTypes, moa_inout MoaSoundHeaderType *headerTypes,
		moa_in MoaLong nCompressors, moa_inout MoaSoundCompressionType *compressors
	))

	MoaDeclareMethod(SetFormat, (THIS_
		moa_in PMoaSoundFormat pFormat
	))		
};

typedef IMoaReceptorSound * PIMoaReceptorSound;

#define kMoaSoundFormat_Compressed			0x00000000L
	/* used on its own, will cause the data to be passed into
	/  the receptor uncompressed. It is up to the receptor
	/  to decompress the data based on the compression interface
	/  passed in. */
	
#define kMoaSoundFormat_Decompress			0x00000001L
	/* this flag tell's the reader to decompress the data before
	/  passing it to the receptor. The format of the data will
	/  reflect the fact that it is decompressed. i.e. The receptor
	/  will be unaware that the data is in any other format. */
	
#define kMoaSoundFormat_NativeHeader	0x00000002L
	/* This will place a
	/  Native platform header at the start of the data.
	/
	/  This is primarily used by the IML, but it has the affect
	/  of making a stream of data look just like an AIFF, SND or WAVE
	/  file to the receptor.
	/
	/  NOTE: It may be possible that the Reader can setup the data
	/  in a way that hardware will be used to decompress this data.
	/  This is the purpose of this flag. After setting this flag,
	/  the data should NOT be directly interpreted by the receptor. */


/* ----------------------------------- IMoaReceptorSoundBuffer */

/*	This receptor MUST be implemented. It is the simplest to implement
/	and can be based directly on the top of the Stream interface.
/
/	This is typically the only Receptor that a Writer will define because
/	it's easiest to work with, but it is possible for a Writer to define
/	a stream if it is more efficient to do so.
/
/	While using this receptor, the Read method of a Sound Reader
/	will probably be called back
/	a number of times. This is so that an application can use the
/	mix interface for streaming if it desires to do so. When the
/	Reader asks the receptor for a buffer (GetBuffer), it MAY NOT
/	get back all of the space that it needs to place the samples.
/	It should get get enough bytes to fill the buffer (however it
/	wants to), and then call FinishedBuffer() when it has finished
/	writing to the buffer. The reader then returns success to the
/	application. The application will continue to call back the
/	reader until it get's a success rather than a
/	kMoaReaderErr_ReadNotFinished error which
/	tells it that the sound data has not been completed. This effectively
/	means that the code that loops and allocates the buffer is on the
/	application side.
/
/	pseudo code for the above reader may be:
/
/	if you haven't already got and opened a stream then do so.
/		(also do whatever "parsing of the stream is necessary",
/		and call back the "SetFormat()" on the receptor)
/
/	receptor->GetBuffer(sndlen, &samples, &bufsize);
/	
/	seek to the last known location in the stream.
/
/	fill the buffer with data.
/
/	remember where you are in the stream
/
/	receptor->FinishedBuffer(nsamples, sizewritten);
/
/	if you reached the end of the stream, then return
/	success otherwise return kMoaReaderErr_ReadNotFinished.
*/

#undef INTERFACE
#define INTERFACE IMoaReceptorSoundBuffer
DECLARE_INTERFACE_(IMoaReceptorSoundBuffer, IMoaReceptorSound)
{
	STD_IUNKNOWN_METHODS

	/* Methods from IMoaReceptorSound */
	
	MoaDeclareMethod(SetRefCon, (THIS_
		moa_in PMoaVoid refCon
	))

	MoaDeclareMethod(SetStatus, (THIS_
		moa_in MoaError status
	))

	MoaDeclareMethod(InquireFormat, (THIS_
		moa_in MoaLong nRates, moa_inout MoaLong *rates, moa_in MoaBoolParam rateRange,
		moa_in MoaLong nDepths, moa_inout MoaLong *depths,
		moa_in MoaLong nHeaderTypes, moa_inout MoaSoundHeaderType *headerTypes,
		moa_in MoaLong nCompressors, moa_inout MoaSoundCompressionType *compressors
	))
	
	MoaDeclareMethod(SetFormat, (THIS_
		moa_in PMoaSoundFormat pFormat
	))	
	
	/* New methods for Sound buffers. */
	
	MoaDeclareMethod(GetBuffer, (THIS_
		moa_in	MoaLong maxBufferSize,
		moa_out MoaSampleBuffer * sampleBuffer,
		moa_out MoaLong * bufferByteSize
	))

	MoaDeclareMethod(BufferFilled, (THIS_
		moa_in MoaSampleBuffer sampleBuffer,
		moa_in	MoaLong bufferByteCount
	))
};
typedef IMoaReceptorSoundBuffer * PIMoaReceptorSoundBuffer;

/* 0d28c804-1ad0-11cf-11d0-a429000502e85810 */
DEFINE_GUID(IID_IMoaReceptorSoundBuffer,
				0x0D28C804L, 0x1AD0, 0x11D0,
				0xA4, 0x29, 0x00, 0x05, 0x02, 0xE8, 0x58, 0x10);
/*
	Note: Mix Sound Receptors have the error range 0x03a0 - 0x03af
*/

#define  kMoaReaderErr_ReadNotFinished_ObsoleteValueDoNotUse	MAKE_MOAERR(200)
#define  kMoaReaderErr_ReadNotFinished							MAKE_MOAERR(0x03a0)
			/*	reading has not been completed */

/* ----------------------------------- IMoaReceptorSoundStream */

/*	Here is the definition of the receptor that will return a Stream.
/
/	Note that this is not the most effective way to achieve this, a method
/	in the Reader() called "GetStream()", would be better.
/
/	Remember that you must call "Read()" on a reader to make it
/	call this receptor back so that you may use the stream.
/
/	SetStream( PIMoaStream stream )
/		This is a stream that may be used to seek around in the sound
/		data. */

#undef INTERFACE
#define INTERFACE IMoaReceptorSoundStream
DECLARE_INTERFACE_(IMoaReceptorSoundStream, IMoaReceptorSound)
{
	STD_IUNKNOWN_METHODS

	/* Methods from IMoaReceptorSoundStream */
	
	MoaDeclareMethod(SetRefCon, (THIS_
		moa_in PMoaVoid refCon
	))

	MoaDeclareMethod(SetStatus, (THIS_
		moa_in MoaError status
	))

	MoaDeclareMethod(InquireFormat, (THIS_
		moa_in MoaLong nRates, moa_inout MoaLong *rates, moa_in MoaBoolParam rateRange,
		moa_in MoaLong nDepths, moa_inout MoaLong *depths,
		moa_in MoaLong nHeaderTypes, moa_inout MoaSoundHeaderType *headerTypes,
		moa_in MoaLong nCompressors, moa_inout MoaSoundCompressionType *compressors
	))

	MoaDeclareMethod(SetFormat, (THIS_
		moa_in PMoaSoundFormat pFormat
	))	
	
	/* single method for IMoaReceptorSoundStream */
	
	MoaDeclareMethod(SetStream, (THIS_
		moa_in	PIMoaStream	pStream
	))
};
typedef IMoaReceptorSoundStream * PIMoaReceptorSoundStream;

/* 0d28ad06-1ad0-11cf-11d0-a429000502e85810 */
DEFINE_GUID(IID_IMoaReceptorSoundStream,
				0x0D28AD06L, 0x1AD0, 0x11D0,
				0xA4, 0x29, 0x00, 0x05, 0x02, 0xE8, 0x58, 0x10);

#endif /* _MIXSND_H */
