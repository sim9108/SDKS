/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_mixsvc
#define _H_mixsvc

#ifndef _H_moastdif
	#include "moastdif.h"
#endif
#ifndef _H_mixdatob
	#include "mixdatob.h"
#endif
#ifndef _H_mixagent
	#include "mixagent.h"
#endif

#ifndef moa_in
	#define moa_in
#endif
#ifndef moa_inout
	#define moa_inout
#endif
#ifndef moa_out
	#define moa_out
#endif
#ifndef MAKE_MOASTATUS
	#define MAKE_MOASTATUS(code) ((MoaError)((MoaUlong)(0L << 31) | (MoaUlong)(4L << 16) | (MoaUlong)(code)))
#endif
#ifndef MoaDeclareMethod
	#define MoaDeclareMethod(macro_NAME, macro_ARGS) \
		STDMETHOD(macro_NAME) macro_ARGS PURE;
#endif
#ifndef MoaDeclareMethod_
	#define MoaDeclareMethod_(macro_RET, macro_NAME, macro_ARGS) \
		STDMETHOD_(macro_RET, macro_NAME) macro_ARGS PURE;
#endif
#ifndef EXTERN_DEFMETHOD
	#define EXTERN_DEFMETHOD(macro_NAME, macro_ARGS) \
		EXTERN_DEFINE_METHOD(MoaError, macro_NAME, macro_ARGS)
#endif
#ifndef EXTERN_DEFMETHOD_
	#define EXTERN_DEFMETHOD_(macro_RET, macro_NAME, macro_ARGS) \
		EXTERN_DEFINE_METHOD(macro_RET, macro_NAME, macro_ARGS)
#endif

/* -------------------------------------------------------------------------- */
/* ------------------------ Service Interface Definitions ----------------- */
/* -------------------------------------------------------------------------- */

#define kMoaMixErr_NoSuchAgent				MAKE_MOAERR(0x12c0)
#define kMoaMixErr_NoSuchReader 			MAKE_MOAERR(0x12c1)
#define kMoaMixErr_NotMyType				MAKE_MOAERR(0x12c2)
#define kMoaMixErr_BadDataSourceType		MAKE_MOAERR(0x12c3)
#define kMoaMixErr_UnknownReceptor			MAKE_MOAERR(0x12c4)
#define kMoaMixErr_DuplicateReceptor		MAKE_MOAERR(0x12c5)
#define kMoaMixErr_NoReceptor				MAKE_MOAERR(0x12c6)
#define kMoaMixErr_AppNotFound				MAKE_MOAERR(0x12c7)
#define kMoaMixErr_NotEnoughMemForApp		MAKE_MOAERR(0x12c8)
#define kMoaMixErr_BufferTooSmall			MAKE_MOAERR(0x12c9)
#define kMoaMixErr_NoSelection				MAKE_MOAERR(0x12ca)
#define kMoaMixErr_NoLeggerFound			MAKE_MOAERR(0x12cb)
#define kMoaMixErr_WrongCallSequence		MAKE_MOAERR(0x12cc)
#define kMoaMixErr_NoSuchWriter 			MAKE_MOAERR(0x12cd)
#define kMoaMixErr_UserCanceled 			MAKE_MOAERR(0x5208) /* same as kMoaErr_ProgressUserCancel */
#define kMoaMixErr_UserCancelled			MAKE_MOAERR(0x5208) /* Webster's sez this is an acceptable alternate spelling... */

#define kMoaMixStatus_IntermediateData		MAKE_MOASTATUS(0x12c1)
#define kMoaMixStatus_ProbablyCanRead		MAKE_MOASTATUS(0x12c2)
#define kMoaMixStatus_DontKnowCanRead		MAKE_MOASTATUS(0x12c3)

/*
	The Moa MIX Services Xtra provides an implementation of this class
	that implements all of the Service interfaces. To use one,
	use MoaCreateInstance (or QueryInterface on another service interface).
*/
DEFINE_GUID(CLSID_CMoaMixServices, 0xF3C0E046L, 0x33C0, 0x11CF, 0xB9, 0x6E, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);
/* same class ID, just a shorter name for it. */
DEFINE_GUID(CLSID_CMix, 0xF3C0E046L, 0x33C0, 0x11CF, 0xB9, 0x6E, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

/* -------------------------------------------------------------------------- */

enum {
	kMixLeggerType_Mix = 0x0001,
	kMixLeggerType_Ole = 0x0002,
	kMixLeggerType_Synthetic = 0x0004,
	kMixLeggerType_All = 0x7fff
};

typedef struct MoaLeggerInfoSignature {
	MoaByte data[1536];
} MoaLeggerInfoSignature;
typedef MoaLeggerInfoSignature * PMoaLeggerInfoSignature;
typedef const MoaLeggerInfoSignature * ConstPMoaLeggerInfoSignature;

#undef INTERFACE
#define INTERFACE IMoaLeggerInfo
DECLARE_INTERFACE_(IMoaLeggerInfo, IMoaUnknown)
/*	Description

	This interface is used to obtain & hold information about a legger. Why
	is it useful? Primarily because it allows a client to hold useful
	information about a legger without forcing the legger itself to
	be loaded; this is important when enumerating all available leggers,
	which would otherwise cause many external code modules to be loaded
	unnecessarily.
*/
{
	STD_IUNKNOWN_METHODS

	MoaDeclareMethod(GetLeggerName, (THIS_
		moa_in MoaLong nameBufLen,
		moa_out PMoaChar pName
	))
/*	Description

	Returns the user-readable name of the legger.
*/

	MoaDeclareMethod(GetSignature, (THIS_
		moa_out PMoaLeggerInfoSignature pSignature
	))

/*	Description

	This returns a MoaLeggerInfoSignature that can be used to 
	re-create a LeggerInfo at a later date.

	A MoaLeggerInfoSignature is a 512-byte "magic-cookie" that 
	records all the information about a Legger, to allow you to 
	re-create the same one during a later run. Why would you 
	want to do this? Very simple: this allows you to have the 
	user select a "preferred" Legger for a data type. All you 
	have to do is present the user with a dialog listing the 
	user names of the leggers, then save the 
	MoaLeggerInfoSignature for the legger he/she chooses. The 
	next time that same data type is to be edited, don't bother 
	enumerating all the leggers again, but just re-create the 
	same legger based on the MoaLeggerInfoSignature you saved.

	MoaLeggerInfoSignatures contain no runtime-specific 
	information; they can safely be saved to files and re-used 
	during a later launch. MoaLeggerInfoSignatures are 
	platform-specific, however; if you attempt to use (say) one 
	created on the Macintosh on a Windows machine, you'll get an 
	error back from ReCreateLeggerInfo. You may also get back an 
	error if the system configuration has changed sufficiently 
	(e.g., the editing app is gone or has been moved). The 
	contents of a MoaLeggerInfoSignatures is secret, so try not 
	to peek into them (we might change 'em in the future).

*/

	MoaDeclareMethod(CreateLegger, (THIS_
		moa_out PIMoaLegger * ppLegger
	))

/*	Description

	Create an IMoaLegger. It belongs to the caller, who 
	must release it.
*/

	MoaDeclareMethod(GetLeggerType, (THIS_
		moa_out MoaLong * pLeggerType
	))
/*	Description

	Returns the type of the legger, which will be one of 
	the kMixLeggerType_ enumeration values. Note that 
	although leggers will generally be returned with all 
	Mix leggers first, followed by OLE leggers, then 
	Synthetic leggers, this isn't guaranteed, so if you 
	are building UI based on this, be sure to check this.
*/

};
typedef IMoaLeggerInfo * PIMoaLeggerInfo;
DEFINE_GUID(IID_IMoaLeggerInfo, 0xF662C570L, 0xAE78, 0x11CF, 0x91, 0xF9, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

#undef INTERFACE
#define INTERFACE IMoaEnumLeggerInfo
DECLARE_INTERFACE_(IMoaEnumLeggerInfo, IMoaUnknown)
/*	Description

	IMoaEnumLeggerInfo is just a generic enumeration 
	interface, and follows the general COM enumerator 
	interface (documented by Microsoft).

	This interface actually returns information about 
	prospective leggers in an interface called 
	IMoaLeggerInfo (see below for more info). Note that 
	IMoaEnumLeggerInfo doesn't actually enumerate the 
	leggers, but the legger infos (you can use a legger 
	info to create a "real" legger). Why is this? Simple: 
	enumerating all the leggers could cause many external 
	code modules to be loaded, and usually the client will 
	just choose one from the enumeration and discard the 
	rest. This allows the client to enumerate the leggers 
	without forcing code loading.

	For simple usage, a loop like this will do:


	PIMoaLeggerInfo pInfo;

	pLegSvc->EnumLeggersForData(pDataIn, FALSE, 
	&pEnum);
	while (pEnum->Next(1, &pInfo, NULL) == 
	kMoaStatus_OK) {
		if (WeWantToUseThisLegger(pInfo)) {
			pInfo->CreateLegger(&pLegger);
			...
			pLegger->Release();
		}
		pInfo->Release();
	}	
	pEnum->Release();
*/
{
	STD_IUNKNOWN_METHODS

	MoaDeclareMethod(Next, (THIS_
		moa_in MoaUlong celt,
		moa_out PIMoaLeggerInfo * pLI,
		moa_out MoaUlong * pCeltFetched
	))

/*	Description

	Used to obtain info on the next legger. You should 
	always pass 1 for celt and NULL for pCeltFetched. Note 
	that the IMoaLeggerInfo returned belongs to the 
	caller, who must release it.
*/

	MoaDeclareMethod(Skip, (THIS_
		moa_in MoaUlong celt
	))

/*	Description

	This is used to skip items in the enumeration. You 
	won't normally need to call this.
*/

	MoaDeclareMethod(Reset, (THIS))

/*	Description

	This resets the enumerator back to the beginning of 
	its list of leggers. You won't normally need to call 
	this.
*/

	MoaDeclareMethod(Clone, (THIS_
		moa_out struct IMoaEnumLeggerInfo ** ppEnum
	))

/*	Description

	Makes a copy of the enumerator. You won't normally 
	need to call this.
*/
};
typedef IMoaEnumLeggerInfo * PIMoaEnumLeggerInfo;
DEFINE_GUID(IID_IMoaEnumLeggerInfo, 0x7443B70CL, 0x9933, 0x11CF, 0x82, 0x54, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

#undef INTERFACE
#define INTERFACE IMoaLegServices
DECLARE_INTERFACE_(IMoaLegServices, IMoaUnknown)
/*	Description

	This interface (along with two helper interfaces) is 
	used to initiate launch/edit/get of a given data set.
*/
{
	STD_IUNKNOWN_METHODS

	MoaDeclareMethod(LegServicesIdle, (THIS))
/*	Description

	This method should be called periodically (i.e., every 
	second or two) during your application's idle loop. 
	This call provides a "hook" for synthetic Leggers to 
	do their magic (i.e., checking file modification dates 
	and sending out notifications to IMoaLeggerListeners).

	Note: it is very important to call this periodically 
	if you want Leggers to work reliably!
*/

	MoaDeclareMethod(EnumLeggersForData, (THIS_
		moa_in PIMoaDataObject pDataIn,
		moa_in MoaLong leggerTypes,
		moa_out PIMoaEnumLeggerInfo * ppEnum
	))
/*	Description

	This return a PIMoaEnumLeggerInfo , which is a 
	temporary object used to enumerate information about 
	possible leggers for a given data source (see below 
	for details). This temporary enumerator belongs to the 
	caller, who must release it. If no leggers are found, 
	then kMoaLegErr_NoLeggerFound will be returned (and no 
	enumerator produced).

	The leggers enumerated will be able to deal with at 
	least one of the formats contained in the pDataIn 
	DataObject.

	If kMixLeggerType_Mix is set in leggerTypes, then 
	leggers that represent genuine MIX IMoaLegger 
	interfaces defined by a real MIX Agent will be 
	enumerated. 

	If kMixLeggerType_Ole is set in leggerTypes, then 
	leggers that can be simulated using OLE will be 
	enumerated.

	If kMixLeggerType_Synthetic is set in leggerTypes, 
	then Mix Services may "synthesize" legger interfaces 
	for non-MIX, non-OLE applications. (These synthesized 
	leggers will be produced by scanning the desktop 
	database/system registry for apps that can open a 
	given file; edits will be accomplished by launching 
	the given app, then polling the file modification 
	date.)

	If all bits are set, then all types of leggers will be 
	returned (with the preference being MIX, OLE, 
	Synthetic).

	Note: EnumLeggersForData is the only method in 
	IMoaLegServices that most clients will ever need to 
	call.
*/

	MoaDeclareMethod(EnumLeggersForFormats, (THIS_
		moa_in MoaLong numFormats,
		moa_in ConstPMixFormat pFormats,
		moa_in MoaLong leggerTypes,
		moa_out PIMoaEnumLeggerInfo * ppEnum
	))
/*	Description

	Essentially the same as EnumLeggersForData, except 
	that you give a format list rather than a DataObject. 
	This is used internally by MIX and will not normally 
	be needed by most clients.
*/

	MoaDeclareMethod(ReCreateLeggerInfo, (THIS_
		moa_in ConstPMoaLeggerInfoSignature pSignature,
		moa_out PIMoaLeggerInfo * ppLeggerInfo
	))
/*	Description

	This is used to re-create a LeggerInfo that you saved 
	a MoaLeggerInfoSignature for earlier. (See 
	IMoaLeggerInfo::GetSignature for more info.)
*/

	MoaDeclareMethod(SynthesizeLeggerInfo, (THIS_
		moa_in ConstPMoaSystemFileSpec pAppSpec,
		moa_in MoaLong appSig,
		moa_in MoaLong leggerTypes,
		moa_out PIMoaLeggerInfo * ppLeggerInfo
	))
/*	Description

	This is used to "synthesize" an IMoaLeggerInfo (see 
	below) for an application that doesn't support MIX. 
	You normally won't need to call this 
	(EnumLeggersForData will call it internally). However, 
	you may wish to call it in order to force a certain 
	app to edit a file (by synthesizing an IMoaLeggerInfo, 
	then calling IMoaLeggerInfo::CreateLegger).

	If the app spec is NULL, the app signature will be 
	used (on the Macintosh) to synthesize a legger ID. If 
	both are NULL, an error is returned.

	leggerTypes allows you to control whether the legger 
	may use OLE, simple file-polling, or either one to 
	accomplish its task. (Note that it's meaningless to 
	ask for a MIX legger in this method.) 

	A synthesized legger responds to its RequestLegData 
	method by writing the data to a file (if not there 
	already) and launching the given app; its GetLegData 
	method polls the file modification date. (If the app 
	supports OLE, the legger may use OLE instead.)
*/
};
typedef IMoaLegServices * PIMoaLegServices;
DEFINE_GUID(IID_IMoaLegServices, 0x037E627AL, 0xCA88, 0x11CF, 0xAD, 0x29, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

/* -------------------------------------------------------------------------- */

#undef INTERFACE
#define INTERFACE IMoaMixFormatInfo
DECLARE_INTERFACE_(IMoaMixFormatInfo, IMoaUnknown)
/*	Description

	This interface is used to hold information that 
	relates a MixFormat into a platform-specific container 
	(e.g., clipboard format, file type or extension).
*/
{
	STD_IUNKNOWN_METHODS

	MoaDeclareMethod(GetBestFileTypes, (THIS_
		moa_out MoaFileType * pFileType,
		moa_out MoaFileType * pFileCreator
	))
/*	Description

	Return the "best" file type and/or creator for the 
	given format. When creating a new file to hold this 
	format, you should set the file type and creator to 
	these values. You can pass NULL for either parameter 
	if not desired.
*/

	MoaDeclareMethod(GetBestExtension, (THIS_
		moa_in ConstPMoaChar pFileNameIn,
		moa_in MoaLong fileNameOutLen,
		moa_out PMoaChar pFileNameOut
	))
/*	Description

	Return the "best" file extension for the given format. 
	When creating a new file to hold this format, you 
	should use this as the file extension (if possible). 

	If pFileNameIn is NULL, the extension is simply 
	returned in pFileNameOut (which is assumed to be a 
	buffer of size fileNameOutLen).

	If pFileNameIn is not NULL, this intelligently appends 
	the extension to it and places the resulting new 
	filename in pFileNameOut. It is legal for the input 
	and output buffers to point to the same storage. If 
	the filename already has the given extension, it isn't 
	changed. If the filename already has a different 
	extension, that extension is removed and the correct 
	one appended.
*/

	MoaDeclareMethod(GetBestScrapType, (THIS_
		moa_out MoaScrapType * pType
	))
/*	Description

	Return the "best" Macintosh scrap/drag type for the 
	given format. When placing this data format onto the 
	scrap or drag, you should use this as the type.
*/

	MoaDeclareMethod(GetFileTypes, (THIS_
		moa_in MoaLong maxNumTypes,
		moa_out MoaLong * pNumTypes,
		moa_out MoaFileType * pTypes
	))
/*	Description

	Return the list of file types that should be 
	interpreted as being of this format. Pass the number 
	of entries in pTypes buffer in maxNumTypes (in 
	MoaFileTypes, not bytes); the number of entries used 
	will be returned in pNumTypes. If there are more 
	formats than will fit, the maximum will be used and 
	kMoaStatus_False returned. 
*/

	MoaDeclareMethod(GetScrapTypes, (THIS_
		moa_in MoaLong maxNumTypes,
		moa_out MoaLong * pNumTypes,
		moa_out MoaScrapType * pTypes
	))
/*	Description

	Like GetFileTypes, but for scrap types. (Duh!)
*/

	MoaDeclareMethod(GetFileExtList, (THIS_
		moa_in MoaLong bufLen,
		moa_out PMoaChar pBuf
	))
/*	Description

	returns a file extension list in a form suitable for 
	passing directly to the Windows API call 
	GetOpenFileName().
*/

	MoaDeclareMethod(GetMimeType, (THIS_
		moa_in MoaLong bufLen,
		moa_out PMoaChar pBuf
	))
/*	Description

	Returns the MIME type for this format.
*/

	MoaDeclareMethod(MatchTypeOrExtension, (THIS_
		moa_in MoaFileType fileType,
		moa_in ConstPMoaChar pFileName
	))
/*	Description

	A method useful mainly under MacOS for implementing 
	Standard File dialog filters. Given a file type and/or 
	file name, this method returns kMoaErr_NoErr if the 
	filename ends in one of the extensions for this 
	format, or if the file type matches one of the file 
	types for this format. If neither the extension nor 
	filetype match, some other error will be returned.

	You may pass NULL for either file type or file name, 
	in which case those match tests are skipped.
*/
};
typedef IMoaMixFormatInfo * PIMoaMixFormatInfo;
DEFINE_GUID(IID_IMoaMixFormatInfo, 0x08117906L, 0xAD7A, 0x11CF, 0xB7, 0xB5, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

#undef INTERFACE
#define INTERFACE IMoaFormatServices
DECLARE_INTERFACE_(IMoaFormatServices, IMoaUnknown)
/*	Description

	MIX uses the concepts of data formats  extensively. A 
	data format is essentially identical to a Windows 
	clipboard format or a Macintosh scrap format or drag 
	flavor (i.e., it's a well-defined ordering of bits). 
	Since MIX uses the DataObject model extensively, we 
	model our format technique on the OLE format model. 
	(In fact, on Windows platforms we actually use system 
	services to register the formats.)
*/
{
	STD_IUNKNOWN_METHODS

	MoaDeclareMethod(RegisterFormat, (THIS_
		moa_in ConstPMoaChar pFormatName,
		moa_out PMixFormat pFormat
	))
/*	Description

	Used to register a new format. If the given named 
	format already exists, the existing format value will 
	be returned. Format names are not user-viewable and 
	need not be localized. The format value returned
	is generally runtime-only; if you need to write
	a format identifier to persistent storage, you
	should write the format name rather than the
	value of a MixFormat.
*/

	MoaDeclareMethod(GetFormatName, (THIS_
		moa_in MixFormatParam format,
		moa_in MoaLong formatNameBufLen,
		moa_out PMoaChar pFormatName
	))
/*	Description

	The inverse of RegisterFormat; given a format value, 
	it returns the format name. If format is not a valid 
	format, kMoaErr_BadParam is returned. You should 
	normally never need to use this call, but it is 
	included for completeness.
*/

	MoaDeclareMethod(AddTypeInfoToFormat, (THIS_
		moa_in MixFormatParam format,
		moa_in MoaFileType fileCreator,
		moa_in MoaLong numFileTypes,
		moa_in ConstPMoaFileType pFileTypes,
		moa_in MoaLong numScrapTypes,
		moa_in ConstPMoaScrapType pScrapTypes,
		moa_in ConstPMoaChar pFileExts,
		moa_in ConstPMoaChar pMimeTypes
	))
/*	Description

	Used to associate type info with a given format. The 
	idea is that we'd like to have a format carry some 
	"hints" with it so that we can make plausible 
	assumptions about what format a given file is. You 
	normally won't need to call this directly (MIX 
	services will call it internally). The information you 
	pass in is basically that returned by 
	IMoaMixFormatInfo (q.v.), except that any parameters 
	can be NULL if you don't want to pass them (e.g., pass 
	NULL for pFileExts if you don't want to associate any 
	file extensions with this format). Note that the type 
	info will be added to any that already exists for this 
	format.
*/

	MoaDeclareMethod(GetFormatInfo, (THIS_
		moa_in MoaLong numFormats,
		moa_in ConstPMixFormat pFormats,
		moa_out PIMoaMixFormatInfo * ppInfo
	))
/*	Description

	Used to find the type info for a given format. It's 
	usually more convenient to obtain this info by calling 
	IMoaMixAgentInfo::GetAgentFormatInfo, so you shouldn't 
	normally need to use this one.

	Given a list of formats, it indicates which file 
	types/extensions can be assumed to reasonably provide 
	data for that format.

	numFormats/pFormats are used to specify which 
	format(s) to interrogate for information. If 
	numFormats > 1, the type info returned will the union 
	for all the formats passed in. Passing NULL/NULL 
	returns the union of info for all known formats.

	The format info is return in *ppInfo, which belongs to 
	the caller (who must release it).
*/

	MoaDeclareMethod(FindFormatsForFile, (THIS_
		moa_in ConstPMoaSystemFileSpec pFileSpec,
		moa_in MoaFileType fileType,
		moa_in MoaLong maxFormats,
		moa_out MoaLong * pNumFormats,
		moa_out PMixFormat pFormats
	))
/*	Description

	Used to guess the format of a given file. You usually 
	won't need to call this (it will be called for you by 
	IMoaDataObjectServices::AddFileToDataObject). Pass the 
	file spec in pFileSpec. You may optionally pass the 
	file type if it is known (or pass zero and the method 
	will calculate the file type). Pass the number of 
	entries in pFormats buffer in maxFormats (in 
	MoaFileTypes, not bytes); the number of entries used 
	will be returned in pNumFormats. If there are more 
	formats than will fit, the maximum will be used and 
	kMoaStatus_False returned. If the file doesn't match 
	any format, an kMoaErr_BadParam will be returned.

	How does it decide? File extensions are used on non-
	Macintosh systems. Macintoshes use file types and file 
	extensions.
*/

	MoaDeclareMethod(FindFormatsForScrapType, (THIS_
		moa_in MoaLong scrapType,
		moa_in MoaLong maxFormats,
		moa_out MoaLong * pNumFormats,
		moa_out PMixFormat pFormats
	))
/*	Description

	Similar to FindFormatsForFile, but is used for a 
	Macintosh scrap type. Note that scrap types are 
	generally much less likely to have multiple formats.
*/

	MoaDeclareMethod(FindFormatsForMime, (THIS_
		moa_in ConstPMoaChar pMimeType,
		moa_in MoaLong maxFormats,
		moa_out MoaLong * pNumFormats,
		moa_out PMixFormat pFormats
	))
/*	Description

	Similar to FindFormatsForFile, but is used for a MIME 
	type. Note that MIME types are generally much less 
	likely to have multiple formats.
*/
};
typedef IMoaFormatServices * PIMoaFormatServices;
DEFINE_GUID(IID_IMoaFormatServices, 0x0DC991D2L, 0xCA88, 0x11CF, 0xAD, 0x29, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

enum {
	kMoaAgentInfoFlags_HasUI = 0x0001,
	kMoaAgentInfoFlags_Hidden = 0x0002
};

#undef INTERFACE
#define INTERFACE IMoaMixAgentInfo
DECLARE_INTERFACE_(IMoaMixAgentInfo, IMoaUnknown)
/*	Description

	This interface is used to obtain & hold information 
	about a reader or writer. Why is it useful? Primarily 
	because it allows a client to hold useful information 
	about a legger without forcing the reader or writer 
	itself to be loaded; this is important when enumerating 
	all available readers or writers, which would otherwise 
	cause many external code modules to be loaded
	unnecessarily.
*/
{
	STD_IUNKNOWN_METHODS

	MoaDeclareMethod(GetAgentName, (THIS_
		moa_in MoaLong nameBufLen,
		moa_out PMoaChar pName
	))
/*	Description

	Return the user-visible name of the agent.
*/

	MoaDeclareMethod(GetAgentFormatInfo, (THIS_
		moa_out MixFormat * pFormat,
		moa_out PIMoaMixFormatInfo * ppInfo
	))
/*	Description

	Return the agent's format code and an 
	IMoaMixFormatInfo describing the format's file type, 
	extension, etc. Note that the format code and info 
	come solely from this agent; format information from 
	other agents that claim the same format will be 
	ignored.

	You can pass NULL for either of these if you don't 
	need that info. 

	Note that the PIMoaMixFormatInfo returned belongs to 
	the caller, who must release it.
*/

	MoaDeclareMethod(GetAgentFlags, (THIS_
		moa_out MoaLong * pFlags
	))
/*	Description

	Returns flags about the agent:
		kMoaAgentInfoFlags_HasUI	Reader has a dialog to get user options.
		kMoaAgentInfoFlags_Hidden	Reader is not to be listed in any UI context.
*/

	MoaDeclareMethod(CreateReader, (THIS_
		moa_in PIMoaDataObject pSourceData,
		moa_out PIMoaReader * ppReader
	))
/*	Description

	Create the reader for this agent info. This method 
	essentially acquires an IMoaCache, calls 
	CreateNewInstanceFromRegistry() to create the reader, 
	then calls SetReaderDataSource with the source you 
	pass. 

	pSourceData is the source DataObject. If you pass NULL 
	for this, SetReaderDataSource will not be called.
*/

	MoaDeclareMethod(CreateWriter, (THIS_
		moa_in PIMoaReader pSourceReader,
		moa_out PIMoaWriter * ppWriter
	))
/*	Description

	Create the writer for this agent info. This method 
	essentially acquires an IMoaCache, calls 
	CreateNewInstanceFromRegistry() to create the writer, 
	then calls SetWriterDataSource with the source reader 
	you pass. 

	pSourceReader is the Reader that will provide the data 
	to the Writer. It is assumed that the Writer will call 
	the Reader with a Receptor provided by the Writer. 
	(Note: earlier specs allowed passing an IMoaDataObject 
	here; that's no longer allowed. If you want to do 
	this, use IMoaAgentServices::CreateMoaReaderDataObject 
	to create a "wrapper" around the dataobject.) If you 
	pass NULL for this, SetWriterDataSource will not be 
	called.
*/

	MoaDeclareMethod(GetAgentRegInfo, (THIS_
		moa_out PIMoaRegistryEntryDict * ppRegDict,
		moa_out MoaClassID * pClassID
	))
/*	Description

	Most apps won't need to use this call; it returns the 
	agent's registry entry dict and class ID. (You can 
	pass NULL for either if you don't need the info 
	returned.) The registry dict belongs to the caller, 
	who must Release() it.

	Although you could use the registry dict and/or the 
	class ID to instantiate the reader or writer, we 
	recommend you use CreateReader/CreateWriter instead. 
	This method is really provided for two reasons:

	(1) It allows the client to access the agent's 
	registry dict (and thus look for possibly special 
	keys)

	(2) It allows you to ensure that the *same* agent is 
	being used by being able to compare the class IDs. 
	Example: your app uses a writer as a part of a 
	distributed rendering process; you want to ensure the 
	same writer is used on foreign machines. The 
	controlling machine sends the class ID gotten from 
	GetAgentRegInfo() to the slave machines. The slave 
	machines iterate thru the available agents, call 
	GetAgentRegInfo() for each, then call CreateWriter() 
	for the one that matches the class ID. (Please don't 
	just call MoaCreateInstance() on the class ID; that 
	may circumvent some internal processing done by MIX.) 
*/
};
typedef IMoaMixAgentInfo * PIMoaMixAgentInfo;
DEFINE_GUID(IID_IMoaMixAgentInfo, 0x17CA3FB0L, 0xCA88, 0x11CF, 0xAD, 0x29, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

#undef INTERFACE
#define INTERFACE IMoaEnumMixAgentInfo
DECLARE_INTERFACE_(IMoaEnumMixAgentInfo, IMoaUnknown)
/*	Description

	IMoaEnumMixAgentInfo is just a generic enumeration 
	interface, and follows the general COM enumerator 
	interface (documented by Microsoft).

	This interface actually returns information about 
	prospective readers/writers (i.e., agents) in an 
	interface called IMoaMixAgentInfo (see below for more 
	info). Note that IMoaEnumMixAgentInfo doesn't 
	actually enumerate the agents, but the agent infos 
	(you can use IMoaMixAgentInfo to create a "real" 
	reader or writer). Why is this? Simple: enumerating 
	all the readers/writers could cause many external code 
	modules to be loaded, and usually the client will just 
	choose one from the enumeration and discard the rest. 
	This allows the client to enumerate them without 
	forcing code loading.

	For simple usage, a loop like this will do:


	PIMoaEnumMixAgentInfo pEnum;
	PIMoaMixAgentInfo pAI;
	PIMoaWriter pWriter;

	pAgtSvc->GetWriterInfo(1, &IID_IMoaReceptorText, 
	&pEnum);
	while (pEnum->Next(1, &pAI, NULL) == 
	kMoaStatus_OK) {
		if (WeWantToUseThisWriter(pAI)) {
			pAI->CreateWriter(pSrcReader, &pWriter);
			...
			pWriter->Release();
		}
		pAI->Release();
	}	
	pEnum->Release();
*/
{
	STD_IUNKNOWN_METHODS

	MoaDeclareMethod(Next, (THIS_
		moa_in MoaUlong celt,
		moa_out PIMoaMixAgentInfo * ppAgentInfo,
		moa_out MoaUlong * pCeltFetched
	))
/*	Description

	Used to obtain info on the next legger. You should 
	always pass 1 for celt and NULL for pCeltFetched. Note 
	that the IMoaMixAgentInfo returned belongs to the 
	caller, who must release it.
*/

	MoaDeclareMethod(Skip, (THIS_
		moa_in MoaUlong celt
	))
/*	Description

	This is used to skip items in the enumeration. You 
	won't normally need to call this.
*/

	MoaDeclareMethod(Reset, (THIS))
/*	Description

	This resets the enumerator back to the beginning of 
	its list of leggers. You won't normally need to call 
	this.
*/

	MoaDeclareMethod(Clone, (THIS_
		moa_out struct IMoaEnumMixAgentInfo ** ppEnum
	))
/*	Description

	Makes a copy of the enumerator. You won't normally 
	need to call this.
*/
	
	MoaDeclareMethod(GetAgentSummaryFormatInfo, (THIS_
		moa_out PIMoaMixFormatInfo * ppInfo
	))
/*	Description

	This is a very handy method used to create an 
	IMoaMixFormatInfo (see below) for the union of all the 
	agents in the enumerator. This is handy for use in 
	implementing a file-open or file-save dialog; the code 
	combines all the file type, extension, etc. 
	information about all the agents in the enumerator 
	and returns a IMoaMixFormatInfo for it as a whole. 
*/
};
typedef IMoaEnumMixAgentInfo * PIMoaEnumMixAgentInfo;
DEFINE_GUID(IID_IMoaEnumMixAgentInfo, 0x150EAF20L, 0xAD7A, 0x11CF, 0xB7, 0xB5, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

#undef INTERFACE
#define INTERFACE IMoaAgentServices
DECLARE_INTERFACE_(IMoaAgentServices, IMoaUnknown)
/*	Description

	This interface (along with a couple of helper 
	interfaces) is used by client apps to acquire readers 
	and writers; it is implemented on the class 
	CMoaMixServices.
*/
{
	STD_IUNKNOWN_METHODS

	MoaDeclareMethod(GetWriterInfo, (THIS_
		moa_in MoaLong numReceptors,
		moa_in ConstPMoaInterfaceID pReceptors,
		moa_out PIMoaEnumMixAgentInfo * ppEnum
	))
/*	Description

	Returns an enumerator of writers that support a given 
	receptor. It does this by scanning the Moa registry 
	for classes that implement IMoaWriter, then examining 
	the registry to see if this writer can provide a 
	receptor listed in the arguments. Note that this 
	method attempts to determine this list based solely on 
	the information in the registry.

	numReceptors/pReceptors is a list of interfaceIDs of 
	receptors that can be provided by a writer. Writers 
	accept only IMoaReaders as data sources, and is 
	expected to provide the Reader with a Receptor to 
	receive the data provided by the Reader; thus, each 
	writer must declare what kind of receptors it can 
	provide.

	ppEnum is an IMoaEnumMixAgentInfo, which is used to 
	enumerate through the information about each writer 
	found. It belongs to the caller, who must release it. 
	You may pass NULL for ppEnum , in which case a result 
	of kMoaErr_NoErr indicates that there exist writers 
	that can provide any of the listed Receptors.

	If there are no writers found (or none match the 
	pReceptors), kMoaMixErr_NoSuchWriter is returned.
*/

	MoaDeclareMethod(GetReaderInfo, (THIS_
		moa_in PIMoaDataObject pSourceData,
		moa_in MoaLong numReceptors,
		moa_in ConstPMoaInterfaceID pReceptors,
		moa_out PIMoaEnumMixAgentInfo * ppEnum
	))
/*	Description

	Returns an enumerator  of the possible reader(s) for 
	the data source. It does this by scanning the Moa 
	registry for classes that implement IMoaAgent, and 
	also can handle a format specified in the DataObject 
	passed as an argument. Note that this method attempts 
	to determine this list based solely on the information 
	in the registry; this allows for faster use, but may 
	be inaccurate in some cases.

	pSourceData provides the source DataObject. (See 
	documentation on MoaDataObjects, below, for more 
	info.)

	numReceptors/pReceptors is an optional list of 
	Receptor interface IDs you can use to filter the list 
	returned. If pReceptors is not NULL, it is assumed to 
	be an array of numReceptors interface IDs; only 
	readers that can provide data to these Receptors will 
	be returned. If you pass 0/NULL for these two 
	arguments, all readers will be returned.

	ppEnum is an IMoaEnumMixAgentInfo, which is used to 
	enumerate through the information about each reader 
	found. It belongs to the caller, who must release it. 
	You may pass NULL for ppEnum , in which case a result 
	of kMoaErr_NoErr indicates that there exist readers 
	that can provide any of the listed Receptors.

	Note that there may well be multiple agents that 
	implement a given reader, all of whom believe they can 
	operate on the given data. In this case, you may want 
	to enumerate all the readers returned and present a 
	dialog for the user to choose an appropriate one, or 
	simply choose one arbitrarily.

	If there are no readers returned (either none are 
	found or none match the pReceptors), 
	kMoaMixErr_NoSuchReader is returned.
*/

	MoaDeclareMethod(StreamToFile, (THIS_
		moa_in PIMoaStream				pStream,
		moa_in ConstPMoaSystemFileSpec	pFileSpec,
		moa_in MoaFileType				fileCreator,
		moa_in MoaFileType				fileType
	))
/*	Description

	A utility method used to write the entire contents of 
	a stream to a file. If the file exists, it will be 
	deleted and replaced with the contents of the stream. 
	If fileType and fileCreator are not zero, they will be 
	used to set the type/creator on Mac systems.
*/

	MoaDeclareMethod(WriteToFileSpec, (THIS_
		moa_in PIMoaWriter pWriter,
		moa_in ConstPMoaSystemFileSpec pFileSpec
	))
/*	Description

	This is a convenience method used to synchronously 
	write from a writer to a file. It essentially replaces 
	this code:


	pDataObjSvc->FileSpecToStorageMedium(pFileSpec, FALSE, &storage);
	pWriter->WriteToMoaStorageMedium(&storage);
	pDataObjSvc->ReleaseMoaStorageMedium(&storage);
*/
};
typedef IMoaAgentServices * PIMoaAgentServices;
DEFINE_GUID(IID_IMoaAgentServices, 0xA4E7A8D2L, 0x4B54, 0x11CF, 0x83, 0x04, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

/* -------------------------------------------------------------------------- */

/*	returns:
		kMoaErr_NoErr if the writer should be added
		kMoaStatus_False if this writer shouldn't be added, but
			the enumeration should continue
		anything else to terminate the enumeration
*/
#ifdef MOA_STD_METHOD_PROCPTR
	typedef MOA_STD_METHOD_PROCPTR(MoaError, MoaAddReadersFilterProc)(
		moa_in PIMoaWriter pWriter,
		moa_in MixFormatParam writerFormat,
		moa_in PMoaVoid refCon
	);
#else
	typedef STDPROCPTR(STDMETHODCALLTYPE, MoaError, MoaAddReadersFilterProc)(
		moa_in PIMoaWriter pWriter,
		moa_in MixFormatParam writerFormat,
		moa_in PMoaVoid refCon
	);
#endif

#undef INTERFACE
#define INTERFACE IMoaDataObjectServices
DECLARE_INTERFACE_(IMoaDataObjectServices, IMoaUnknown)
/*	Description

	This interface is used to create and release 
	DataObjects; it is implemented on the class 
	CMoaMixServices.
*/
{
	STD_IUNKNOWN_METHODS

	MoaDeclareMethod(CreateEmptyDataObject, (THIS_
		moa_out PIMoaDataObject * ppData
	))
/*	Description

	Creates a new, empty MoaDataObject. It contains no 
	formats or storage mediums and is expected to be 
	released by the caller.
*/

	MoaDeclareMethod(AddPtrToDataObject, (THIS_
		moa_in PIMoaDataObject pData,
		moa_in ConstPMoaFormatEtc pFormatIn,
		moa_in PMoaVoid p,
		moa_in MoaLong numBytes
	))
/*	Description

	Adds a format to the given DataObject; the data format 
	is specified by pFormatIn, and the storage medium is 
	specified by the given pointer. The DataObject must 
	have been created with CreateEmptyDataObject. numBytes 
	of data are copied from the pointer p into a private 
	buffer owned by the DataObject.
*/

	MoaDeclareMethod(AddFileToDataObject, (THIS_
		moa_in PIMoaDataObject pData,
		moa_in ConstPMoaFormatEtc pFormatIn,
		moa_in ConstPMoaSystemFileSpec pFileSpec,
		moa_in MoaBoolParam isTempFile
	))
/*	Description

	Adds a format to the given DataObject; the data format 
	is specified by pFormatIn, and the storage medium is 
	specified by the given file. If isTempFile is true, 
	then the file will be deleted when the DataObject is 
	released; if isTempFile is false, the file will not be 
	altered. The DataObject must have been created with 
	CreateEmptyDataObject. Note that the given file is not 
	copied into the DataObject, but just added as a 
	reference by filespec; thus, if the file is deleted or 
	renamed before the DataObject is used, misbehavior may 
	ensue.

	If you pass NULL for pFormatIn, 
	IMoaFormatServices::FindFormatsForFile will be used to 
	determine the file's format(s) (if the file can be 
	interpreted in multiple formats, they are all added to 
	the DataObject). Thus, normally you should always pass 
	NULL for pFormatIn unless you already have specific 
	information about the file's format.

	However, if you pass NULL for pFormatIn and the file 
	is unrecognized (ie, cannot be identified as any known 
	format), the file will not be added, and 
	kMoaStatus_False will be returned.
*/

	MoaDeclareMethod(AddMoaStreamToDataObject, (THIS_
		moa_in PIMoaDataObject pData,
		moa_in ConstPMoaFormatEtc pFormatIn,
		moa_in PIMoaStream pStream
	))
/*	Description

	Adds a format to the given DataObject; the data format 
	is specified by pFormatIn, and the storage medium is 
	specified by the given MoaStream. You must pass a 
	value for pFormatIn; you cannot pass NULL.
*/

	MoaDeclareMethod(AddWriterToDataObject, (THIS_
		moa_in PIMoaDataObject pData,
		moa_in ConstPMoaFormatEtc pFormatIn,
		moa_in PIMoaWriter pWriter
	))
/*	Description

	Adds a format to the given DataObject; the data format 
	is specified by pFormatIn, and the storage medium is 
	specified by the given writer. In this case, the 
	output of the given writer is treated as the given 
	data source! This is a sort of cool effect that allows 
	you to "pipe" a writer into another reader (or 
	whatever).

	You won't normally need to call this method; it is 
	called by AddReadersToDataObject, which is more 
	generally useful to a client application. 

	Note that it is assumed that pFormatIn matches the 
	actual format produced by the writer; no checking is 
	done.
*/

	MoaDeclareMethod(AddReadersToDataObject, (THIS_
		moa_in PIMoaDataObject pData,
		moa_in MoaLong numReaders,
		moa_in PIMoaReader * pReaderArray,
		moa_in MoaAddReadersFilterProc pFilterProc,
		moa_in PMoaVoid refCon
	))
/*	Description

	OK, this method is a little hard to follow, so bear 
	with me. The idea is this: you have a list of readers. 
	You want to find the writers that can accept those 
	readers as input, create each writer, and call 
	AddWriterToDataObject(). The net result: all the 
	formats that can be generated from the list of readers 
	are added to given dataobject. (You'll typically only 
	pass a single reader, but you could pass multiple.)

	Why would you want to do this? Let's say you have a 
	document that you want placed on the clipboard. You 
	don't want to convert it manually to each clipboard 
	type. However, you can send the data in this document 
	into IMoaReceptorText and IMoaReceptorPixels. Let's 
	also say that there are currently writers for PICT, 
	JPEG, BMP, and GIF that take IMoaReceptorPixels as an 
	input, and writers for ascii and unicode text that 
	take IMoaReceptorText as input. All you need to do is 
	call AddReadersToDataObject() with your readers as 
	input, then SetClipboard with the resulting 
	DataObject: voila! A rich clipboard export. This is a 
	really cool capability that allows for really rich 
	data export in a very simple manner.

	Now for the details:

	numReaders/pReaderArray is the array of readers to use 
	as input.

	pFilterProc/refCon are optional parameters you can use 
	to filter the addition of a specific writer. If you 
	pass a non-NULL proc here, your proc will be called 
	for each writer generated, along with the writer's 
	format. You may choose to accept the writer (return 
	kMoaErr_NoErr), reject the writer (return 
	kMoaStatus_False), or terminate the enumeration 
	(return anything else). You can also examine and/or 
	set the writer's options dictionary.
*/

	MoaDeclareMethod(DataObjectToMoaStream, (THIS_
		moa_in PIMoaDataObject pData,
		moa_in ConstPMoaFormatEtc pFormatEtc,
		moa_out PIMoaStream * ppStream
	))
/*	Description

	A convenience routine that can used with any 
	DataObject (not just those created by 
	CreateEmptyDataObject). It's often desirable to access 
	any given format in a DataObject via a stream, but not 
	all formats may provide the data to you in a streaming 
	form. Moreover, there isn't even a DataObject storage 
	medium type for returning a MoaStream! This function 
	attempts to "wrap" a stream around most well-known 
	formats (specifically, HGlobal and File, and IStream 
	on Windows systems) so that you can deal with a 
	MoaStream in all cases (well, almost all). 
*/

	MoaDeclareMethod(StorageMediumToMoaStream, (THIS_
		moa_in ConstPMoaStorageMedium pMedium,
		moa_out PIMoaStream * ppStream
	))
/*	Description

	Similar to DataObjectToMoaStream, but takes a 
	MoaStorageMedium as input rather than a 
	DataObject/FormatEtc pair. You shouldn't generally
	need to use this.
*/

	MoaDeclareMethod(ReleaseMoaStorageMedium, (THIS_
		moa_inout PMoaStorageMedium pStorage
	))
/*	Description

	Analogous to the Windows routine, ReleaseStgMedium() 
	(in fact, it just calls that directly on Windows 
	systems). It knows how to correctly release the 
	storage medium(s) used by a given MoaStorageMedium. 
	You should always call this rather than manipulating 
	the fields directly.
*/

	MoaDeclareMethod(FileSpecToStorageMedium, (THIS_
		moa_in ConstPMoaSystemFileSpec pFileSpec,
		moa_in MoaBoolParam isTempFile,
		moa_out PMoaStorageMedium pStorage
	))
/*	Description

	This is a convenience routine for converting a 
	MoaSystemFileSpec into a MoaStorageMedium. You may 
	want to use this to create a storage medium to pass to 
	IMoaWriter::WriteToStorageMedium. It's handy because: 
	(1) MoaStorageMediums require pathnames, but 
	MoaSystemFileSpecs aren't pathnames on all systems, 
	and (2) it easily allows for the "don't-delete-this-
	file-when-releasing-the-storage-medium" option.

	pFileSpec is the filespec to jam into the storage 
	medium. isTempFile controls file life: if TRUE, the 
	file is deleted (!) when the storage medium is 
	released; if FALSE, the file is not deleted.
*/

	MoaDeclareMethod(ExpandDraggedFiles, (THIS_
		moa_in PIMoaDataObject pDataIn,
		moa_out PIMoaDataObject * ppDataOut
	))
/*	Description

	This is a handy utility routine used primarily within 
	IMoaDropTarget::DragEnter and IMoaDropTarget::Drop.

	Here's the problem: say your drop target is interested 
	only in text data, so it examines the DataObject for 
	kMoaCfFormat_Text. If the user drags text from the 
	scrapbook, this is what will show up. But if the user 
	drags a text file, the format will be 
	kMoaCfFormat_DraggedFiles!

	This routine makes a new DataObject, adds all the data 
	in the old DataObject, then "expands" any dragged file 
	data by calling AddFileToDataObject() repeatedly. 
	Dragged files that aren't recognized as any known 
	format aren't added. (Note: the actual implementation 
	just stores a reference to the original DataObject in 
	the new DataObject, so no rendering of data is done 
	until your code calls GetData. In other words, this is 
	fairly efficient.)

	The moral: most DropTargets should unconditionally 
	call this method first thing, unless they specifically 
	want to deal with files as files (rather than as the 
	data contained in the files).
*/

	MoaDeclareMethod(PathNameToFileSpec, (THIS_
		moa_in ConstPMoaPathnameChar pPathName,
		moa_in MoaLong bufLen,
		moa_out PMoaSystemFileSpec pFileSpec
	))
/*	Description

	A convenience method for converting the lpszFileName 
	field of a MoaStorageMedium from a pathname to a 
	MoaSystemFileSpec.
*/

	MoaDeclareMethod(PathNameToRealPathName, (THIS_
		moa_in ConstPMoaPathnameChar pPathName,
		moa_in MoaLong bufLen,
		moa_out PMoaChar pRealPathName
	))
/*	Description

	A convenience method for converting the lpszFileName 
	field of a MoaStorageMedium from a string of 
	MoaPathnameChar to a string of MoaChar. This is mainly 
	useful under Win32, where MoaPathnameChar is unicode.
*/
};
typedef IMoaDataObjectServices * PIMoaDataObjectServices;
DEFINE_GUID(IID_IMoaDataObjectServices, 0x2253DDA6L, 0xCA88, 0x11CF, 0xAD, 0x29, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

/* -------------------------------------------------------------------------- */

typedef struct _MoaDragRegRef * MoaDragRegRef;

#undef INTERFACE
#define INTERFACE IMoaDragServices
DECLARE_INTERFACE_(IMoaDragServices, IMoaUnknown)
/*	Description

	IMoaDragServices is an interface that you can use to 
	simplify handling of Drag-n-Drop and clipboard; it 
	provides a (mostly) cross-platform interface onto both 
	of these. It is implemented on the class 
	CMoaMixServices.

	The interface is actually almost identical to the OLE 
	standard for these; this was done partly because it 
	was easier to layer this on top of Macintosh Drag Mgr 
	and clipboard than vice versa, and partly because it 
	seems to fit the rest of MIX more cleanly. 

	Note that you don't have to use these drag or 
	clipboard services to use MIX in conjunction with your 
	drag or clipboard; you can still just obtain a 
	DataObject directly (either from the OS or from 
	IMoaMacServices) in your existing code.
*/
{
	STD_IUNKNOWN_METHODS

	MoaDeclareMethod(RegisterDrag, (THIS_
		moa_in SysWindow pWindow,
		moa_in PIMoaDropTarget pDrop,
		moa_out MoaDragRegRef * pRegisterRef
	))
/*	Description

	Used to register a drop target. pWnd is the window 
	that will own the drop target. pDrop is the drop 
	target itself. *pRegisterRef is a cookie you'll need 
	to use to unregister the drag later on. All drag 
	operations into the given window will be handled by 
	the DropTarget you pass in. Note that you're only 
	allowed one drop target per window.
*/

	MoaDeclareMethod(UnRegisterDrag, (THIS_
		moa_in MoaDragRegRef registerRef
	))
/*	Description

	Used to unregister a drop target.
*/

	MoaDeclareMethod(DoDrag, (THIS_
		moa_in PIMoaDataObject pData,
		moa_in PIMoaDropSource pDropSource,
		moa_in MoaLong effectsPermitted,
		moa_in PMoaVoid pMacEvent,
		moa_in ConstPMoaRect pRect,
		moa_out MoaLong * pEffectUsed
	))
/*	Description

	Used to initiate a drag from one of your windows. 
	pData is the source data of the drag. pDropSource is 
	an IMoaDropSource used to control some feedback. 
	effectsPermitted is a mask of the kMoaDropEffect_xxx 
	flags used to describe the type of drag operations 
	being done. pMacEvent is the Macintosh event 
	corresponding to the mousedown that began the drag 
	(pass NULL on non-Mac systems). pRect is an optional 
	rectangle describing the bounding rectangle of the 
	object being dragged (in global coordinates); pass 
	NULL if you don't care. *pEffectUsed returns the 
	effect used by the recipient of the drop 
	(kMoaDropEffect_None if no drop took place). 
	kMoaStatus_DoDrop will be returned if the drop 
	actually took place; kMoaStatus_CancelDrop will be 
	returned if the user canceled the drag.
*/

	MoaDeclareMethod(GetFileFromDraggedFiles, (THIS_
		moa_in ConstPMoaStorageMedium pStorage,
		moa_in MoaLong index,
		moa_in MoaLong fileSpecBufLen,
		moa_out PMoaSystemFileSpec pFileSpec,
		moa_out PMoaFileType pFileType
	))
/*	Description

	There is a special format known as the DraggedFiles 
	format; this is presented when file(s) are dropped 
	into a target. (This corresponds to the HDROP format 
	under Windows and to a list of HFS flavors on the 
	Macintosh.) You shouldn't examine the data in this 
	format directly, but rather, pass the storage medium 
	to this method, along with an index (0..n-1) of the 
	file spec you want. The file spec and file type will 
	be returned.
*/

	MoaDeclareMethod(GetNumDraggedFiles, (THIS_
		moa_in ConstPMoaStorageMedium pStorage,
		moa_out MoaLong * pNum
	))
/*	Description

	Return the number of files in a DraggedFiles storage 
	medium.
*/

	MoaDeclareMethod(GetClipboard, (THIS_
		moa_out PIMoaDataObject * ppData
	))
/*	Description

	Returns the current clipboard as a DataObject (which 
	belongs to the caller, who must Release it). Note that 
	this is "lazily" allocated on Macintosh systems, so 
	you should be careful to use the DataObject before the 
	clipboard may change, or unpredictable results may 
	occur.
*/

	MoaDeclareMethod(SetClipboard, (THIS_
		moa_in PIMoaDataObject pData
	))
/*	Description

	Places the given DataObject on the clipboard. On 
	Macintosh systems, this actually copies the data 
	having a Macintosh type onto the clipboard.
*/
};
typedef IMoaDragServices * PIMoaDragServices;
DEFINE_GUID(IID_IMoaDragServices, 0x20804544L, 0x606D, 0x11CF, 0x99, 0x89, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

/* -------------------------------------------------------------------------- */
#undef INTERFACE
#define INTERFACE IMoaDictServices
DECLARE_INTERFACE_(IMoaDictServices, IMoaUnknown)
/*	Description

	This provides several utility functions for dealing 
	with MoaDicts. It is implemented on the class 
	CMoaMixServices.
*/
{
	STD_IUNKNOWN_METHODS

	MoaDeclareMethod(CreateDict, (THIS_
		moa_out PIMoaDict * ppDict
	))
/*	Description

	Creates a new, empty MoaDict. You'll need  to create 
	dicts to pass as arguments to some other service 
	interfaces. The caller owns the dict and must 
	Release() it when done with it. You can fill in the 
	MoaDict by using its methods (e.g., Put()). For more 
	information, see the documentation on IMoaDict in the 
	Moa Concepts & Reference document.
*/

	MoaDeclareMethod(CalcFlatDictSize, (THIS_
		moa_in PIMoaDict pDict,
		moa_out MoaLong * pFlatDictLen
	))
/*	Description

	Used to calculate how big a buffer is needed to 
	flatten a dict.
*/

	MoaDeclareMethod(FlattenDict, (THIS_
		moa_in PIMoaDict pDict,
		moa_out PMoaVoid pFlatDict
	))
/*	Description

	Flattens the given dictionary into the given section 
	of memory. It is assumed that the buffer passed to 
	FlattenDict is at least as long as the value returned 
	by CalcFlatDictSize. FlattenDict doesn't alter or 
	Release() the source dictionary. 

	Note: the "flat" version is byte-swapped; a dict 
	flattened on Motorola platforms can be unflattened on 
	Intel platforms. However, note that keys of 
	DictType_Bytes are not (and cannot) be swapped 
	correctly, so you should use caution when flattening 
	dictionaries for cross-platform transport if they 
	contain any keys of this type.
*/

	MoaDeclareMethod(UnflattenDict, (THIS_
		moa_in PMoaVoid pFlatDict,
		moa_out PIMoaDict * ppDict
	))
/*	Description

	Used to create a new dict from a flattened dictionary. 
	The created dictionary is owned by the caller of 
	UnflattenDict, who must release it when done.
*/
};
typedef IMoaDictServices * PIMoaDictServices;
DEFINE_GUID(IID_IMoaDictServices, 0x45F459C4L, 0x4B54, 0x11CF, 0x92, 0x44, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

/* -------------------------------------------------------------------------- */
#undef INTERFACE
#define INTERFACE IMoaTempStreamServices
DECLARE_INTERFACE_(IMoaTempStreamServices, IMoaUnknown)
/*	Description

	This service allows you to create temporary
	streams as intermediate steps in reading or writing.

	This service was first available in MIX Services v1.02
	(and not v1.0), so be sure to check for errors when
	attempting to acquire it.
*/
{
	STD_IUNKNOWN_METHODS

	MoaDeclareMethod(CreateTempStream, (THIS_
		moa_in MoaLong maxSizeToWrite,
		moa_in MoaBoolParam readAllowed,
		moa_in MoaBoolParam writeAllowed,
		moa_out PIMoaStream * ppStream
	))
/*	Description
	
	Creates a temporary stream that may reside in a file or a 
	memory handle. It is initially empty and closed. 

	readAllowed and writeAllowed indicate how the stream can 
	be opened. 
	
	maxSizeToWrite indicates the maximum size that will be written; 
	this number determines whether a file or memory handle 
	is used (small values use handles, larger ones use files,
	which the threshhold being implementation-dependent). 
	passing zero is legal and means "i don't know how large
	it will be" (in which case a file is generally used). 
	Note also that if you pass a nonzero value, you won't
	be allowed to write past that position.
*/

	MoaDeclareMethod(CreateMemStream, (THIS_
		moa_in MoaHandle hMemForStream,
		moa_in MoaBoolParam streamOwnsHandle,
		moa_in MoaBoolParam readAllowed,
		moa_in MoaBoolParam writeAllowed,
		moa_in MoaBoolParam resizeAllowed,
		moa_out PIMoaStream * ppStream
	))

/*	Description
	
	Creates a streaming interface for the given MoaHandle. 
	(if hMemForStream is NULL, a new empty handle will be 
	allocated.) The stream is initially closed. 
	
	readAllowed and writeAllowed indicate how the stream 
	can be opened. 
	
	if streamOwnsHandle is TRUE, then the stream assumes 
	ownership of the handle passed in, and will free 
	it when the stream is destroyed. if streamOwnsHandle 
	is FALSE, the stream does not assume ownership, and 
	the caller is responsible for ensuring that the handle 
	is eventually disposed.
	
	if resizeAllowed is TRUE, then the handle will be resized 
	as necessary to accomodate write requests. if 
	resizeAllowed is FALSE, then the handle won't be 
	resized (thus you can't write past the handle's
	initial size).
*/
};
typedef IMoaTempStreamServices * PIMoaTempStreamServices;
DEFINE_GUID(IID_IMoaTempStreamServices, 0x0D191616L, 0x421C, 0x11D0, 0x92, 0x0F, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

/* -------------------------------------------------------------------------- */
#undef INTERFACE
#define INTERFACE IMoaMixMiscServices
DECLARE_INTERFACE_(IMoaMixMiscServices, IMoaUnknown)
/*	Description

	This is a catch-all of miscellaneous MIX utility functions.

	This service was first available in MIX Services v1.02
	(and not v1.0), so be sure to check for errors when
	attempting to acquire it.
*/
{
	STD_IUNKNOWN_METHODS

	MoaDeclareMethod(ResetCachedMixInfo, (THIS))
/*	Description

	MIX Services does some caching of information
	about readers, writers, and formats; this caching
	is normally done when CMixServices is first used
	during a session.
	
	If your application uses Moa(Un)RegisterDiskXtra()
	to open or close individual Xtras, this cached information
	could be out of date. This call forces MIX to discard
	its cached information and rescan all Xtras to
	rebuild this information. This is not exceptionally slow,
	but neither is it fast, so you should use it judiciously;
	basically, any application that uses MIX should
	probably call this after calling Moa(Un)RegisterDiskXtra().
	
	Note: it isn't necessary to call this during the 
	normal MOA startup sequence, only if you load/unload
	individual Xtras during runtime.
*/
};
typedef IMoaMixMiscServices * PIMoaMixMiscServices;
DEFINE_GUID(IID_IMoaMixMiscServices, 0xAEEF313CL, 0x4221, 0x11D0, 0xA8, 0x5B, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

/* -------------------------------------------------------------------------- */

typedef struct MoaPipeServerID {
	MoaUlong hi;
	MoaUlong lo;
} MoaPipeServerID;
typedef MoaPipeServerID * PMoaPipeServerID;
typedef const MoaPipeServerID * ConstPMoaPipeServerID;
typedef struct _MoaLeggerPipeRef * MoaLeggerPipeRef;

#undef INTERFACE
#define INTERFACE IMoaLeggerPipeServices
DECLARE_INTERFACE_(IMoaLeggerPipeServices, IMoaUnknown)
/*	Description

	MIX provides a handy technique for IAC that allows for 
	simple asynchronous cross-app communication with a 
	(fairly) cross-platform API. The basis of this is a 
	"LeggerPipe": a special implementation of the 
	IMoaLegger interface that simply transmits its calls 
	across processes. (This is done using RPC under 
	Windows and AppleEvents on the Macintosh). 
	IMoaLeggerPipeServices is used by clients to acquire 
	LeggerPipes, and by servers to install LeggerPipes. It 
	is implemented on the class CMoaMixServices. (The 
	IMoaLegger interface is described elsewhere.)

	Note that although a LeggerPipe is just an IMoaLegger 
	interface, it isn't semantically identical to an 
	IMoaLegger: a LeggerPipe is purely a conduit used 
	internally by a Legger, to communicate between the 
	client (i.e., the app requesting the edit be 
	performed) and the server (i.e., the app performing 
	the edit). Typically a Legger will implement stubs on 
	both the client and server side: the client side will 
	simply creates a LeggerPipe to the server side (using 
	CreateLeggerPipe), and the server side will install a 
	LeggerPipe (using InstallLeggerPipe) to listen for 
	messages from the client. 

	Note that most readers and writers won't need to use 
	LeggerPipes (though I guess they could if they wanted 
	to); LeggerPipes are more useful to leggers.

	Note that data pipes are provided as a utility for 
	agents. The client of an reader, writer, or legger 
	should never have to deal with data pipes; data pipes 
	should always be handled "under the surface" in order 
	to provide transparency to the reader's client.

	Note that under MacOS, you must obtain the 
	ProcessSerialNumber of the destination app (you will 
	probably use the MIX utility method 
	IMoaMacServices::LaunchApp to do this, documented 
	elsewhere in this document). In practice, this means 
	just a few lines of ifdef'ed code.
*/
{
	STD_IUNKNOWN_METHODS

	MoaDeclareMethod(CreateLeggerPipe, (THIS_
		moa_in ConstPMoaPipeServerID pDstID,
		moa_in ConstPMoaClassID pClassID,
		moa_out PIMoaLegger * ppLegPipe
	))
/*	Description

	Used by a client to acquire a LeggerPipe to another 
	application; it creates the LeggerPipe returns it to 
	you.

	pDstID is the destination process ID of the other 
	application; it's currently only used under MacOS, 
	where it's a pointer to a ProcessSerialNumber (you can 
	acquire this via system calls, or from the LaunchApp() 
	method of the IMoaMacServices interface). It's unused 
	under other operating systems (you can pass NULL).

	pClassID is the MOA class ID of the LeggerPipe in the 
	other application. (This sounds confusing? Not really. 
	If you know you want to create a data pipe to 
	FreeHand, you just have to find the GUID FreeHand uses 
	for its data pipe.)

	The new pipe is returned in *ppLeggerPipe. It belongs 
	to the caller, who must release it.
*/

	MoaDeclareMethod(InstallLeggerPipe, (THIS_
		moa_in ConstPMoaClassID pMoaLeggerPipeClassID,
		moa_out MoaLeggerPipeRef * pRef
	))
/*	Description

	Used by a server to install a LeggerPipe for clients 
	to use. (Under MacOS, it sets up AppleEvent listeners; 
	under Windows, it enters keys in the system registry.) 
	Clients can't obtain a LeggerPipe from a server until 
	the server has called this; thus, this should 
	typically be called at app launch time. An app can 
	install as many LeggerPipes as it likes; each must 
	have a unique class ID, however.

	pMoaLeggerPipeClassID is the MoaID of the Moa class 
	that implements your rendition of IMoaLegger.

	pRefNum returns a long integer used with 
	UnInstallLeggerPipe.
*/

	MoaDeclareMethod(UnInstallLeggerPipe, (THIS_
		moa_out MoaLeggerPipeRef ref
	))
/*	Description

	Used to uninstall a LeggerPipe. Normally you should 
	only call this at app termination time. You must pass 
	the refNum returned by InstallLeggerPipe.
*/
};
typedef IMoaLeggerPipeServices * PIMoaLeggerPipeServices;
DEFINE_GUID(IID_IMoaLeggerPipeServices, 0x73D76BD4L, 0x7353, 0x11CF, 0xB6, 0x3B, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* ---------------------------- Mac-Specific Stuff ------------------------ */
/* -------------------------------------------------------------------------- */

#undef INTERFACE
#define INTERFACE IMoaMacServices
DECLARE_INTERFACE_(IMoaMacServices, IMoaUnknown)
/*	Description

	This interface contains utilities that are MacOS-
	specific. This interface is contained in 
	CMoaMixServices, along with the other services 
	interfaces. This interface is only defined under 
	MacOS.
*/
{
	STD_IUNKNOWN_METHODS

	MoaDeclareMethod(LaunchApp, (THIS_
		moa_in MoaLong applSignature,
		moa_in MoaBoolParam bringToFront,
		moa_out PMoaVoid pPID	/* ProcessSerialNumber */
	))
/*	Description

	Used in conjunction with creating a LeggerPipe (q.v.). 
	This ensures that the app with the given application 
	signature is running. If it is not running, it 
	attempts to launch it. The app will be launched with 
	its default memory partition; if there is not enough 
	memory for that, it will attempt to launch it in its 
	minimum memory partition.

	If bringToFront is TRUE, the target app is made the 
	frontmost process; if FALSE, its layering is 
	unaffected. (Note that newly launched apps may be 
	brought to the front by the system, though.)

	The MoaPipeServerID (i.e., ProcessSerialNumber) is 
	returned in pPID.

	If no such app can be found, kMoaMixErr_AppNotFound is 
	returned. If the app is present but there isn't enough 
	memory to launch it, kMoaMixErr_NotEnoughMemForApp is 
	returned.

	Note that it is not necessary to use this call in 
	order to get a MoaPipeServerID; you are free to use 
	the Launch trap directly, and use the Process Manager 
	(or whatever) to get a ProcessSerialNumber for use 
	with LeggerPipes. This method is merely provided for 
	your convenience.
*/

	MoaDeclareMethod(AddMacScrapToDataObject, (THIS_
		moa_in PIMoaDataObject pData
	))
/*	Description

	Used to add the data on the Macintosh clipboard to a 
	given DataObject. The DataObject must have been 
	created with 
	IMoaDataObjectServices::CreateEmptyDataObject. All of 
	the data types on the clipboard are added. Note that 
	they are added in a "lazy" fashion; the data is not 
	actually copied into the DataObject until 
	DataObject::GetData is called. Thus, if the clipboard 
	is changed in the meantime, the DataObject may get out 
	of sync.
*/

	MoaDeclareMethod(AddMacDragToDataObject, (THIS_
		moa_in PIMoaDataObject pData,
		moa_in PMoaVoid dragRef,
		moa_in PMoaVoid itemRef
	))
/*	Description

	Used to add the data from a Macintosh Drag Manager 
	item to a given DataObject. The DataObject must have 
	been created with 
	IMoaDataObjectServices::CreateEmptyDataObject. All of 
	the flavors in the given drag/item are added. Note 
	that the data continues to live in the DragReference 
	rather than the DataObject; if the DragReference is 
	changed prior to calling DataObject::GetData, 
	incorrect data may be retrieved.
*/

	MoaDeclareMethod(AddMacHandleToDataObject, (THIS_
		moa_in PIMoaDataObject pData,
		moa_in ConstPMoaFormatEtc pFormatIn,
		moa_in PMoaVoid h
	))
/*	Description

	Adds a format to the given DataObject; the data format 
	is specified by pFormatIn, and the storage medium is 
	specified by the Macintosh handle h. The DataObject 
	must have been created with 
	IMoaDataObjectServices::CreateEmptyDataObject. If no 
	error is returned, the DataObject owns the handle and 
	will free it; if an error is returned, the caller 
	still owns the handle and must free it.
*/
};
typedef IMoaMacServices * PIMoaMacServices;
DEFINE_GUID(IID_IMoaMacServices, 0x2AED6176L, 0xCA88, 0x11CF, 0xAD, 0x29, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

#undef INTERFACE

#if defined(__cplusplus)

#if defined(CINTERFACE)
	#define MakeGetSvc(svc) \
		inline MoaError Get##svc(PIMoaCallback pCbk, P##svc * ppSvc) \
			{ return pCbk->lpVtbl->MoaCreateInstance(pCbk, &CLSID_CMix, &IID_##svc, (PPMoaVoid)ppSvc); }
#else
	#define MakeGetSvc(svc) \
		inline MoaError Get##svc(PIMoaCallback pCbk, P##svc * ppSvc) \
			{ return pCbk->MoaCreateInstance(&CLSID_CMix, &IID_##svc, (PPMoaVoid)ppSvc); }
#endif

	MakeGetSvc(IMoaLegServices)
	MakeGetSvc(IMoaFormatServices)
	MakeGetSvc(IMoaAgentServices)
	MakeGetSvc(IMoaDataObjectServices)
	MakeGetSvc(IMoaDragServices)
	MakeGetSvc(IMoaDictServices)
	MakeGetSvc(IMoaLeggerPipeServices)
	MakeGetSvc(IMoaMacServices)
	MakeGetSvc(IMoaTempStreamServices)
	MakeGetSvc(IMoaMixMiscServices)

	#undef MakeGetSvc

#endif


#endif
