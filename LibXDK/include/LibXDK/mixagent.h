/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_mixagent
#define _H_mixagent

#ifndef _H_moastdif
	#include "moastdif.h"
#endif
#ifndef _H_mixdatob
	#include "mixdatob.h"
#endif

/* -------------------------------------------------------------------------- */

typedef MoaLong 				MoaFileType;
typedef MoaFileType *			PMoaFileType;
typedef const MoaFileType * 	ConstPMoaFileType;
typedef MoaLong 				MoaScrapType;
typedef MoaScrapType *			PMoaScrapType;
typedef const MoaScrapType *	ConstPMoaScrapType;

/* -------------------------------------------------------------------------- */

#undef INTERFACE
#define INTERFACE IMoaReceptor
DECLARE_INTERFACE_(IMoaReceptor, IMoaUnknown)
/*	Description
	
	This is an abstract interface that should never be instantiated; however, 
	all Receptor interfaces should descend from it. 
*/
{
	STD_IUNKNOWN_METHODS

	MoaDeclareMethod(SetRefCon, (THIS_
		moa_in PMoaVoid refCon
	))

/*	Description

	Sets the refCon. Your Receptor may save the refCon in an instance 
	variable for use during future calls. Typically, the instantiator 
	of the receptor will pass an implementation-dependent value here for 
	use by the receptor.
*/

	MoaDeclareMethod(SetStatus, (THIS_
		moa_in MoaError status
	))
/*	Description

	Sends a "status code" to the receptor to indicate why a reception is 
	being terminated. 

	Imagine that a Reader or Legger is pushing data into a Receptor, and 
	then an error occurs inside the Reader/Legger. The Reader/Legger can 
	call SetStatus for the Receptor to notify the Receptor of the cause 
	of the error.

	For Readers, using this method is generally just a convenience; the 
	Receptor might be interested in knowing what error occurred, but the 
	same error will probably be returned from IMoaReader::Read().
	
	For Leggers, however, this method is much more useful. Leggers may not 
	be able to immediately return an error code from IMoaLegger::RequestLegData 
	(due to IAC delays); in these cases, they send the error code to 
	IMoaReceptor::SetStatus.

	As such, some of the errors you should watch for are:
	
	kMoaErr_NoErr					
		meaningless in this context; just ignore it.
	kMoaMixErr_UserCanceled 		
		user canceled the operation.
	kMoaErr_OutOfMem				
		duh.
	kMoaMixErr_AppNotFound		
		Leggers send this if their editing app can't be located.
	kMoaMixErr_NotEnoughMemForApp 
		Leggers send this if their editing app can be found, but there 
		isn't enough free memory to launch it.
	kMoaErr_BadParam
		Some of the params you passed to Read or RequestLegData were bad. 
	kMoaMixErr_BadDataSourceType 
		the source data object doesn't have any formats that the 
		Reader/Legger can understand; or, Reader/Legger requires a source 
		data object and you passed NULL.
	kMoaMixErr_NotMyType
		the destination format(s) can't be provided by the Reader/Legger.
	kMoaStatus_IntermediateData
		This is a "special case" that's generally only sent by Leggers. 
		This means, "the next time I send you data, it's an intermediate 
		update, not the final data, so continue the editing session". 
		For instance: if an IMoaReceptorDataObject received this code, it 
		should interpret that as meaning that the next call to its 
		ReceiveData will be an "intermediate" update.

*/
};
typedef IMoaReceptor * PIMoaReceptor;
/* no IID -- it's an abstract base interface */

/* -------------------------------------------------------------------------- */

#undef INTERFACE
#define INTERFACE IMoaReceptorText
DECLARE_INTERFACE_(IMoaReceptorText, IMoaReceptor)
/*	Description

	This interface allows simple, single-byte access to text.
*/
{
	STD_IUNKNOWN_METHODS

	MoaDeclareMethod(SetRefCon, (THIS_
		moa_in PMoaVoid refCon
	))

	MoaDeclareMethod(SetStatus, (THIS_
		moa_in MoaError status
	))

	MoaDeclareMethod(Begin, (THIS_
		moa_in MoaLong totalLength,
		moa_in MoaLong firstSelectedChar,
		moa_in MoaLong numSelectedChars 
	))
/*	Description

	Called first. 
	totalLength is the maximum length of text to be provided. The actual 
	length may be less. If totalLength is -1, the totalLength is unknown.

	firstSelectedChar/numSelectedChars indicate the selection range. If there 
	is no selection range, these will be -1.
*/

	MoaDeclareMethod(AppendText, (THIS_
		moa_in ConstPMoaChar pText,
		moa_in MoaLong textLen
	))
/*	Description

	This call appends text to the end of the text already sent. 
	No line endings (CR, LF, or CR/LF) should ever be sent thru this method.
*/

	MoaDeclareMethod(AppendLineEnd, (THIS_
		moa_in ConstPMoaChar pText,
		moa_in MoaLong textLen
	))
/*	Description

	Similar to AppendText, but only CR, LF, or CR/LF should be sent. 
	The idea is that line runs are sent thru AppendText, and line 
	ending are sent thru AppendLineEnd.
*/

	MoaDeclareMethod(End, (THIS))
/*	Description

	Called at the end.
*/
};
typedef IMoaReceptorText * PIMoaReceptorText;
DEFINE_GUID(IID_IMoaReceptorText, 0x5D27DA10L, 0x661D, 0x11CF, 0xA1, 0x64, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

/* -------------------------------------------------------------------------- */

#undef INTERFACE
#define INTERFACE IMoaReceptorUnicode
DECLARE_INTERFACE_(IMoaReceptorUnicode, IMoaReceptor)
/*	Description
	
	Similar to IMoaReceptorText, but all text is in Unicode characters. 
	Note that all indices are unichar offsets, not byte offsets.
*/
/*	See also
		IMoaReceptorText
*/
{

	STD_IUNKNOWN_METHODS

	MoaDeclareMethod(SetRefCon, (THIS_
		moa_in PMoaVoid refCon
	))

	MoaDeclareMethod(SetStatus, (THIS_
		moa_in MoaError status
	))

	MoaDeclareMethod(Begin, (THIS_
		moa_in MoaLong totalLength,
		moa_in MoaLong firstSelectedChar,
		moa_in MoaLong numSelectedChars 
	))
/*	Description

	Called first. 
	
	totalLength is the maximum length of text to be provided. The actual 
	length may be less. If totalLength is -1, the totalLength is unknown.

	firstSelectedChar/numSelectedChars indicate the selection range. If 
	there is no selection range, these will be -1.
*/

	MoaDeclareMethod(AppendUnicode, (THIS_
		moa_in ConstPMoaUnichar pUni,
		moa_in MoaLong unicodeLen
	))
/*	Description

	This call appends text to the end of the text already sent. No line 
	endings (CR, LF, or CR/LF) should ever be sent thru this method.
*/

	MoaDeclareMethod(AppendLineEnd, (THIS_
		moa_in ConstPMoaUnichar pUni,
		moa_in MoaLong unicodeLen
	))
/*	Description

	Similar to AppendText, but only CR, LF, or CR/LF should be sent. 
	The idea is that line runs are sent thru AppendText, and line ending 
	are sent thru AppendLineEnd.
*/

	MoaDeclareMethod(End, (THIS))
/*	Description

	Called at the end.
*/
};
typedef IMoaReceptorUnicode * PIMoaReceptorUnicode;
DEFINE_GUID(IID_IMoaReceptorUnicode, 0x95C7B35EL, 0x661D, 0x11CF, 0xB4, 0x20, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

/* -------------------------------------------------------------------------- */

enum {
	kMixAnchorBounds_xAxis = 0x0001,
	kMixAnchorBounds_yAxis = 0x0002,
	kMixAnchorBounds_zAxis = 0x0004,
	kMixAnchorBounds_tAxis = 0x0008
};

typedef struct MixAnchorBounds {
	MoaLong flags;
	MoaDouble xMin, xMax;
	MoaDouble yMin, yMax;
	MoaDouble zMin, zMax;
	MoaDouble tMin, tMax;
} MixAnchorBounds;
typedef MixAnchorBounds * PMixAnchorBounds;
typedef const MixAnchorBounds * ConstPMixAnchorBounds;

typedef struct MoaXyPoint {
	MoaDouble x;
	MoaDouble y;
} MoaXyPoint;

typedef PIMoaDict PIMoaAnchorDict;

#define kAnchorRegKey_FrameStart			"Anchor_FrameStart"
#define kAnchorRegKey_FrameCount			"Anchor_FrameCount"
#define kAnchorRegKey_FrameDuration 		"Anchor_FrameDuration"
#define kAnchorRegKey_TimeStart 			"Anchor_TimeStart"
#define kAnchorRegKey_TimeDuration			"Anchor_TimeDuration"
#define kAnchorRegKey_Point 				"Anchor_Point"
#define kAnchorRegKey_Line					"Anchor_Line"
#define kAnchorRegKey_Quadrilateral 		"Anchor_Quadrilateral"
#define kAnchorRegKey_SpatialResolution 	"Anchor_SpatialResolution"
#define kAnchorRegKey_Label 				"Anchor_Label"
#define kAnchorRegKey_Id					"Anchor_Id"

#undef INTERFACE
#define INTERFACE IMoaReceptorAnchor
DECLARE_INTERFACE_(IMoaReceptorAnchor, IMoaReceptor)
/*	Description

	An anchor is a named spatial and/or temporal location. For instance, it 
	could be a cue point in SoundEdit, a registration mark in FreeHand or 
	E3D, or a movie frame in Director. IMoaReceptorAnchor is an interface 
	for accessing these in a fairly transparent manner.

	A single anchor is described using a MoaDict. This allows for easily 
	extensible data types in an anchor. A custom type, PIMoaAnchorDict, 
	will be defined to refer to a dictionary intended to contain anchor 
	information; however, it is not functionally distinct from any other 
	kind of MoaDict (it's just semantic reinforcement).

	Some standard keys are described below. Note that not all will be 
	present in all cases. For instance, FreeHand is unlikely to provide 
	anchors with Time or Duration keys.

	Key Name:	"Anchor_FrameStart"
	Data type:	kMoaDictType_Long
	Semantic:	"Start Frame" of the Anchor
	
	Key Name:	"Anchor_FrameCount"
	Data type:	kMoaDictType_Long
	Semantic:	"Frame Count" of the Anchor
	
	Key Name:	"Anchor_FrameDuration"
	Data Type:	kMoaDictType_Double
	Semantic:	duration of each frame, in seconds
	
	Key Name:	"Anchor_TimeStart"
	Data type:	kMoaDictType_Double
	Semantic:	"Start Time" of the Anchor, in seconds
	
	Key Name:	"Anchor_TimeDuration"
	Data type:	kMoaDictType_Double
	Semantic:	"Duration" of the Anchor, in seconds
	
	Key Name:	"Anchor_Point"
	Data Type:	kMoaDictType_Bytes (MoaXyPoint)
	Semantic:	"Location" of the Anchor, in inches
	
	Key Name:	"Anchor_Line"
	Data Type:	kMoaDictType_Bytes (array of 2 MoaXyPoint)
	Semantic:	"Location" of the Anchor, in inches
	
	Key Name:	"Anchor_Quadrilateral"
	Data Type:	kMoaDictType_Bytes (array of 4 MoaXyPoint)
	Semantic:	"Location" of the Anchor, in inches
	
	Key Name:	"Anchor_SpatialResolution"
	Data Type:	kMoaDictType_Double
	Semantic:	Preferred spatial resolution of the location(s), in pixels 
		per inch. (Note that this only makes sense in the case of data that 
		might be interpreted in rasterized form, as a "hint" to the client 
		about fitting the anchor to the drawn version; this key should be 
		omitted for data that does not apply.)
	
	Key Name:	"Anchor_Label"
	Data type:	kMoaDictType_CString
	Semantic:	User-visible label of this Anchor.

	Key Name:	"Anchor_Id"
	Data type:	kMoaDictType_Long
	Semantic:	Internal ID of this Anchor.

	Note that none of these keys are required (in fact, an empty dict is a 
	"valid" anchor dict), though this isn't very useful; in practice, 
	most anchor dicts ought to have a Label key, in addition to some others, 
	in order to be meaningful.

	There are two ways of specifying time-based data; either using a frame 
	metaphor, or a continuous-time metaphor. Both may be supplied, in which 
	case the client can choose which is more useful. The client should be 
	prepared for time-based data to be presented in either form. Note that 
	"frames" are always presented in integral form; there is no concept of 
	a sub-frame. Clients should use the continuous-time metaphor to access 
	subframes.

	Both continuous-time and spatial keys operate in a well-defined unit 
	space, namely, seconds and inches. This is done merely in the interest of 
	simplicity so as to avoid having to deal with variable units; since 
	doubles are used for all of these, there's plenty of precision for 
	conversion to other units.

	The above keys are not intended to be exhaustive; other keys are, 
	of course, possible and permitted.
*/
{
	STD_IUNKNOWN_METHODS

	MoaDeclareMethod(SetRefCon, (THIS_
		moa_in PMoaVoid refCon
	))

	MoaDeclareMethod(SetStatus, (THIS_
		moa_in MoaError status
	))

	MoaDeclareMethod(Begin, (THIS_
		moa_in ConstPMixAnchorBounds pMaxBounds,
		moa_out PMixAnchorBounds pRequestedBounds
	))
/*	Description
	
	Called first thing. 
	
	pMaxBounds, passed in to you, indicates the bounding spacetime area of the 
	anchors, and establishes the frame of reference of the anchors. The 
	flags field indicates which of the min/max fields are valid. For instance, 
	if the caller is presenting 2D graphics with an x and y range of 
	x = (-1000, 1000), y = (-2000, 2000), the record would look like:
		
	flags = kMixAnchorBounds_xAxis | kMixAnchorBounds_yAxis;
	xMin = -1000.0;
	xMax = 1000.0;
	yMin = -2000.0;
	yMax = 2000.0;
	(other fields are undefined)
	
	*pRequestedBounds is returned as the sub-area of *pMaxBounds that the 
	receptor is interested in. In the case above, the receptor might only 
	be interested in the subsection from -100 to 0; it would fill in an 
	appropriate subarea and return it. (If all the anchors are desired, 
	ie, no subarea, the receptor should copy *pMaxBounds into 
	*pRequestedBounds.)

	Note that the caller of the receptor doesn't have to filter out all 
	the anchors that aren't in the requested bounds; it can still pass all 
	of the anchors if it desires. The requested bounds is intended to used 
	as a hint that the caller can use to optimize its calls (perhaps it can 
	avoid unnecessary calculations or disk accesses).
*/

	MoaDeclareMethod(ReceiveAnchor, (THIS_ 
		moa_in PIMoaAnchorDict pAnchor
	))
/*	Description

	Is called repeatedly to present the anchors to the Receptor. The dict 
	belongs to your caller, so don't Release() it. If you want to hang on 
	to a dict, you should AddRef() it.
*/

	MoaDeclareMethod(End, (THIS))
/*	Description

	Called last thing.
*/
};
typedef IMoaReceptorAnchor * PIMoaReceptorAnchor;
DEFINE_GUID(IID_IMoaReceptorAnchor, 0x4DFF9690L, 0x7356, 0x11CF, 0xA7, 0x86, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

/* -------------------------------------------------------------------------- */

#undef INTERFACE
#define INTERFACE IMoaReceptorDataObject
DECLARE_INTERFACE_(IMoaReceptorDataObject, IMoaReceptor)
/*	Description

	This is a sort of pseudo-Receptor used mostly by Leggers. 
	
	Note that this interface might also be useful for a Reader or 
	Writer if they wanted to get or send the data in a "raw" form.
*/
{
	STD_IUNKNOWN_METHODS

	MoaDeclareMethod(SetRefCon, (THIS_
		moa_in PMoaVoid refCon
	))

	MoaDeclareMethod(SetStatus, (THIS_
		moa_in MoaError status
	))

	MoaDeclareMethod(ReceiveData, (THIS_ 
		moa_in PIMoaDataObject pData
	))
/*	Description

	Receives the source data object. This belongs to the caller and 
	shouldn't be Released; if you need to keep it (though I don't know why 
	you would), you need to AddRef it. Note that some callers may pass 
	NULL here, so be sure to check for that (and return an error, 
	if appropriate).
*/
	MoaDeclareMethod(InquireReceivedData, (THIS_ 
		moa_out PIMoaDataObject * ppData
	))	
/*	Description

	This is a utility routine provided for your use; MIX will never call this 
	routine. This method is primarily useful if you want to poll your 
	Receptor to see if it has received data.

	You should return the most recently received data in the given parameters. 
	Your implementation of this method should AddRef() the data object you 
	return; thus, the caller is responsible for Release()ing it when done.

	If you don't need this method, don't implement it. If you don't 
	implement this routine, you should return kMoaErr_NotImplemented.
*/
/*	Return
		kMoaStatus_False				nothing received yet
		kMoaErr_NoErr					final data received
		kMoaStatus_IntermediateData 	intermediate data received
		kMoaErr_NotImplemented			this method not implemented
		some other error				reception was aborted
*/
};
typedef IMoaReceptorDataObject * PIMoaReceptorDataObject;
DEFINE_GUID(IID_IMoaReceptorDataObject, 0x95862682L, 0x7356, 0x11CF, 0x88, 0xCA, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

/* -------------------------------------------------------------------------- */

#undef INTERFACE
#define INTERFACE IMoaAgent
DECLARE_INTERFACE_(IMoaAgent, IMoaUnknown)
/*	Description

	Description

	All Agent (i.e., Reader/Writer/Legger) Xtras must provide a class 
	that registers the IMoaAgent interface. This interface
	doesn't actually do anything, in that it has no methods
	beyond those in IMoaUnknown; however, it is used to provide
	certain registry keys used for that Agent.

	kAgentRegKey_Name is a C string. This is a human-readable name 
	for the agent. This may be presented to the user if multiple 
	Agents can read the same data, or in a Save As dialog for Writers.

	kAgentRegKey_Format is the data format name, as it would be 
	passed to IMoaFormatServices::RegisterFormat, e.g., "FreeHand4". 
	Note that this is not a user-visible string.

	kAgentRegKey_FileTypes is a list of four-byte identifiers used to 
	identify the Macintosh file types that we assume consitute the 
	Agent's format. Example: "FH50TEXTBINA".

	kAgentRegKey_MacScrapTypes is a list of four-byte identifiers used to 
	identify the Macintosh clipboard types that we assume consitute the 
	Agent's format. Example: "FH50TEXTBINA".

	kAgentRegKey_FileCreator is a single four-byte identifier used to 
	indicate the preferred Mac file creator for this format. Writers 
	(or more likely, the app calling the Writer) may use this field 
	to set the file creator when creating a new file. You should not 
	use the file creator to filter file open dialogs; it should be 
	used only when creating new files. MIX doesn't use this tag 
	internally; all it does is fill in the 
	MoaFormatTypeInfo::fileCreator field to the value of this tag (if 
	present). This is an optional tag; if not present, 
	MoaFormatTypeInfo::fileCreator will be set to zero (in which 
	case the app may want to use '????' or some similar creator).

	kAgentRegKey_FileExts is a file extension list is used to identify 
	file formats that we can assume constitute the Agent's format. 
	This list should be valid for all platforms, not just Windows. 
	It is a restricted form of the Windows file-filter format, 
	like so: "*.fh;*.fht;*.foo". Each entry must be "*", 
	".", 1 to 3 chars, ";" (except there must be no trailing ";").

	kAgentRegKey_MimeType is the format's MIME type. This is a 
	single character string.

	It's important to note that a single agent handles exactly one data 
	format. This may seem overly restrictive, but is done to simplify the 
	programming model. If you have several similar formats that could be 
	handled by a single agent, you can implement several "stub" agents that 
	use the same code base within the same Xtra; alternately, you can 
	register the same class multiple times with different format information 
	each time.

	Do I Really Need An Agent?
	--------------------------
	
	OK, now for the rest of the story. (Warning: this is confusing.) You can 
	create a Reader or Writer without actually implementing an Agent 
	interface. 

	If your IMoaReader or IMoaWriter interface is registered with the 
	kAgentRegKey_Format key defined in its registry dict, the information in 
	the IMoaReader/IMoaWriter registry dict will override information in the 
	IMoaAgent registry dict. In fact, this allows you to omit the IMoaAgent 
	entirely.

	Why would you want to do this? The main reason would be to allow for 
	writing "adapter" readers/writers (e.g., an adapter that converts a 
	Photoshop filter to a MIX Receptor). Your code can dynamically register 
	an IMoaReader for each adapter, with the appropriate data format 
	information in the IMoaReader registry dict itself (along with possible 
	private data the IMoaReader's internal use).

	So now you're wondering, why do I need an IMoaAgent at all? The answer 
	is, you don't actually ever *need* one, but if you are implementing a 
	Reader and Writer for the same data type, you can make more efficient use 
	of memory by putting the data format information into an Agent (rather 
	than redundantly into both the Reader and Writer).
*/
{
	STD_IUNKNOWN_METHODS
};
typedef IMoaAgent * PIMoaAgent;

DEFINE_GUID(IID_IMoaAgent, 0x0FA45856L, 0x33C1, 0x11CF, 0xB9, 0x6E, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

#define kAgentRegKey_Name			"Name"			/* type: kMoaDictType_CString */
#define kAgentRegKey_Format 		"Format"		/* type: kMoaDictType_CString */
#define kAgentRegKey_FileCreator	"FileCreator"	/* type: kMoaDictType_CString */
#define kAgentRegKey_FileTypes		"FileTypes" 	/* type: kMoaDictType_CString */
#define kAgentRegKey_MacScrapTypes	"MacScrapTypes" /* type: kMoaDictType_CString */
#define kAgentRegKey_FileExts		"FileExts"		/* type: kMoaDictType_CString */
#define kAgentRegKey_MimeType		"MimeType"		/* type: kMoaDictType_CString */

/* -------------------------------------------------------------------------- */

#undef INTERFACE
#define INTERFACE IMoaReader
DECLARE_INTERFACE_(IMoaReader, IMoaUnknown)
/*	Description

	Description

	An Agent (i.e., a class supporting IMoaAgent) should also support 
	IMoaReader if it wishes to read the Agent's format.

	Readers use a Receptor to communicate their data. The caller of a 
	Reader doesn't poll for data directly; rather, the caller provides the 
	Reader with a Receptor. A Receptor is an interface called by the Reader 
	to receive data. A Reader is usually capable of dealing with several 
	different Receptors. 

	Registering Info About Your Reader
	----------------------------------

	If a Reader has no Agent, it should put the Agent type information into 
	the Reader registry (see "Do I Really Need An Agent?" for more 
	information). In addition to possible Agent data, readers need 
	additional type information.

	Readers should declare whether or not the readers has a UI. This is 
	declared in the key kReaderRegKey_HasUI, which is a Boolean.

	Readers should declare whether or not the reader should be presented
	in any UI context. This is declared in the key kReaderRegKey_Hidden,
	which is a Boolean.

	Readers must declare the interface IDs of Receptors to which they can 
	send data in the key kReaderRegKey_UnderstoodReceptors, which is
	just an array of MoaInterfaceID (use the "Bytes" dict data type).

	Selectors
	---------

	Readers use the concept of "Selectors" to allow a Reader to refer to 
	a subset of a given data source. A Selector is simply a MoaDict 
	describing the class of data subset desired (e.g., channel, frame, 
	range, etc.), and additional data specific to the given data (e.g., 
	which channel, the frame range, the range start and end, etc.).

	Every Selector dict must include the kMoaSelectorKey_SelectorID key,
	which is a MoaID identifying the class of data specified by the selector. 
	Additional keys may be present in the dict to further specify the data. 
	For instance, SLCTID_ImageChannel refers to a particular channel of an 
	image; a separate integer key in the dict may be used to define which 
	channel.

	Each reader may provide a list of "user" selectors; that is, selectors 
	that have a user-visible name. A client can ask a reader for its list of 
	user-selectors and present this information to the user, who may choose 
	one in order to restrict the data operated on. Note that readers may 
	also support selectors that aren't in the user-selector list.

	For instance: a reader for FreeHand data might list a user-selector for 
	every page, with the names being "Page 1", "Page 2", "Page 3", etc. This 
	reader may also support selectors for each object (!), but not as user 
	selectors (to avoid overwhelming the user interface).

	The client using the FreeHand agent could allow the user to view the 
	list of selectors provided by the agent, and choose the one to operate 
	on (assuming the user wanted to operate on a single page).

	Note that we don't allow for selecting multiple selectors (i.e., you 
	can't select a page range in the above example). 

	There is also the concept of "Well-Known" Selectors; not all agents 
	can or should support all (or even any) of them, but agents should 
	strive to use these where they make sense.

		MoaID: SLCTID_Page
		Dict Contents: kMoaDictKey_PageNum (MoaLong)

		MoaID: SLCTID_AlphaChannel
		Dict Contents: kMoaDictKey_ChannelIndex (MoaLong, 0...n-1)
*/
{
	STD_IUNKNOWN_METHODS

	MoaDeclareMethod(CountUnderstoodReceptorIDs, (THIS_
		moa_out MoaLong * pNumReceptors
	))
/*	Description

	Provides the number of Receptors that can be returned by
	GetNthUnderstoodReceptorID.
*/

	MoaDeclareMethod(GetNthUnderstoodReceptorID, (THIS_
		moa_in MoaLong index,
		moa_out MoaInterfaceID * ppReceptorIID
	))
/*	Description

	Returns the Receptors that the Reader can provide data thru. 
	These should be provided in the order preferred by the Reader, 
	i.e., in order from most accurate to least accurate. 
	
	index varies from 0 ... (CountUnderstoodReceptorIDs - 1).
*/
	
	MoaDeclareMethod(SetReaderDataSource, (THIS_ 
		moa_in PIMoaDataObject pSourceData
	))

/*	Description

	Sets the source of the data used by the reader to be the given 
	DataObject. The agent is expected to AddRef the object internally (and 
	Release it when the agent is destroyed).
*/
/*	Return

	kMoaErr_NotMyType	DataObject doesn't contain the correct format of data
*/
	MoaDeclareMethod(SetReaderOptions, (THIS_
		moa_in PIMoaDict pOptionsDict
	))

/*	Description

	Set the options to use for future reads. The reader should retain this 
	dictionary internally. If this method is never called, the Reader should
	use a default set of options should be used.
*/
	MoaDeclareMethod(GetReaderOptions, (THIS_
		moa_out PIMoaDict * ppOptionsDict
	))

/*	Description

	Set the current set of options used for reads. If SetReaderOptions has 
	never been called, this method should return a dictionary identical to
	that returned by GetDefaultReadOptions. The options dict belongs to 
	the caller, who must Release it.
*/
	MoaDeclareMethod(GetDefaultReaderOptions, (THIS_
		moa_out PIMoaDict * ppOptionsDict
	))
/*	Description

	Returns a dict fully populated with all the options supported by the 
	reader. This can be used by the caller to determine the keys understood 
	by the reader. If the reader has no options, it should return an empty 
	dict (i.e., one containing no entries). Note that the values of the 
	default options might vary depending on the source data, but the same 
	keys should always be present. The options dict belongs to the caller, 
	who must Release it. Note that this doesn't set the options (you must 
	call SetReaderOptions to do that).
*/

	MoaDeclareMethod(GetReaderOptionsFromUser, (THIS_
		moa_in SysWindow pParentWindow,
		moa_out PIMoaDict * ppOptionsDict
	))
/*	Description

	Use a dialog to return a set of reader options. Note that this doesn't 
	set the options (you must call SetReaderOptions to do that). The options 
	dict belongs to the caller, who must Release it. pParentWindow is the 
	(optional) parent window for the options dialog (HWND or WindowPtr).
*/
/*	Return
	
	kMoaErr_BadParam		Reader has no UI
	kMoaErr_UserCanceled	User canceled the dialog
*/

	MoaDeclareMethod(CanRead, (THIS_
		moa_in ConstPMoaStorageMedium pStorage,
		moa_in MoaFileType fileType
	))
/*	Description

	This method may be called to determine if a given reader can read from 
	the given storage medium. It returns:

		kMoaErr_NoErr if the given storage medium can definitely be 
	identified as a readable medium of the proper data type. (Note that the 
	read may of course fail later due to lack of memory, disk space, garbled 
	data, etc.)

		kMoaMixStatus_ProbablyCanRead if the given storage medium appears 
	likely (but not certainly) to be a readable medium of the proper data 
	type. Some data types are hard to identify with certainty (e.g., Mac 
	PICT) so this can be returned when you think it is "probably" the right 
	type.

		kMoaMixStatus_DontKnowCanRead if you just can't tell if the given 
	storage medium is a readable medium of the proper data type. 

		kMoaStatus_False if the given storage medium is definitely not a 
	readable medium of the proper data type. 

		(some other error) is another error occurs.

	The storage medium belongs to the caller; this method should NOT release 
	it.

	Note that this method should not examine file types/extensions, but 
	only the contents of the file (or stream, etc.). (So why is the fileType 
	passed in to this method? Historical reasons. Just ignore it.)
*/
/*	Return

	kMoaErr_NoErr					storage medium is definitely readable.
	kMoaMixStatus_ProbablyCanRead	storage medium is probably readable.
	kMoaMixStatus_DontKnowCanRead	cannot tell with any certainty if readable or not.
	kMoaStatus_False				storage medium is definitely not readable.
	(some other error)				error occurred trying to determine (e.g., i/o error)
*/

	MoaDeclareMethod(CountUserSelectors, (THIS_
		moa_out MoaLong * pNumSelectors
	))
/*	Description

	Return the number of user-selectors. This returns the number of distinct 
	user-selectors the given reader can provide for the data source. 
	(Selectors without a user-visible name aren't included in the count.)
*/
/*	Return
	
	kMoaErr_BadParam	Reader doesn't support selectors.
*/

	MoaDeclareMethod(GetNthUserSelector, (THIS_
		moa_in MoaLong index,
		moa_out PMoaChar pSelectorName,
		moa_out PIMoaDict * ppSelectorData
	))
/*	Description

	Returns the nth user-selector. The selector data dict belongs to the 
	caller, who must release it. Note that this doesn't set the current 
	selector; you must call SetCurrentSelector() to do that.
*/
/*	Return
	
	kMoaErr_BadParam	Reader doesn't support selectors, or index out of range.
*/

	MoaDeclareMethod(SetCurrentSelector, (THIS_
		moa_in PIMoaDict pSelectorData
	))
/*	Description

	Used to set a given selector for an agent. You can construct the 
	selector dict yourself (if the selector is well-known), or pass a dict 
	returned by GetNthUserSelector.

	Passing NULL for pSelector is equivalent to restoring the default state 
	of "all the data". It is legal to call SetSelector multiple times for 
	the same reader; each call replaces previous calls to that interface.
*/
/*	Return
	
	kMoaErr_BadParam	Reader doesn't support selectors, or this selector not understood.
*/

	MoaDeclareMethod(Read, (THIS_
		moa_in PIMoaReceptor pReceptor,
		moa_in ConstPMoaInterfaceID pReceptorID
	))
/*	Description

	Read the data. The data is fed to the Receptor specified by pReceptor, 
	which is assumed to actually be an interface corresponding to 
	pReceptorID (since IMoaReceptor is an abstract interface class that's 
	never actually instantiated). 
*/
};
typedef IMoaReader * PIMoaReader;
DEFINE_GUID(IID_IMoaReader, 0x573C95EAL, 0x8C6B, 0x11CF, 0x8B, 0x95, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

#define kReaderRegKey_HasUI 				"HasUI" 				/* kMoaDictType_Bool */
#define kReaderRegKey_Hidden				"Hidden"				/* kMoaDictType_Bool */
#define kReaderRegKey_UnderstoodReceptors	"UnderstoodReceptors"	/* type: kMoaDictType_Bytes */
#define kReaderRegKey_WillScanForHeader 	"WillScanForHeader" 	/* kMoaDictType_Bool */

/* -------------------------------------------------------------------------- */

#undef INTERFACE
#define INTERFACE IMoaStreamReader
DECLARE_INTERFACE_(IMoaStreamReader, IMoaUnknown)
/* -------------------------------------------------------------------------- */
/*	Description

	This is a stripped down IMoaReader with some new capabilites.

	First, the Read method takes a PIMoaStream, rather than expecting to
	have a PIMoaDataObject supplied via SetReaderDataSource.

	Second, it can handle multiple selectors simultaneously. This is useful
	for sounds which get a playable stream via an IMoaReceptorSoundStream
	and at the same time get the cuepoints from with an IMoaReceptorAnchor.
*/
{
	STD_IUNKNOWN_METHODS

	MoaDeclareMethod(AddReceptor, (THIS_
		moa_in PIMoaReceptor pReceptor,
		moa_in ConstPMoaInterfaceID pReceptorID
	))
/*	Description

	Adds a receptor to the Stream Reader. This must be called at least once.
*/
/*	Return

	kMoaMixErr_UnknownReceptor		Stream Reader can't handle this type of receptor.
	kMoaMixErr_DuplicateReceptor	This receptor ID has already been added.
*/

	MoaDeclareMethod(CountAddedReceptorIDs, (THIS_
		moa_out MoaLong * pNumReceptors
	))
/*	Description

	Provides the number of Receptors that have been added with AddReceptor.
*/

	MoaDeclareMethod(GetNthAddedReceptorID, (THIS_
		moa_in MoaLong index,
		moa_out MoaInterfaceID * ppReceptorIID
	))
/*	Description

	Returns the Receptors ID that have been added to the Stream Reader.
	
	index varies from 0 ... (CountAddedReceptorIDs - 1).
*/
	
	MoaDeclareMethod(ReadStream, (THIS_
		moa_in PIMoaStream pStream
	))
/*	Description

	Read the data. The data is fed to the added receptors. 
*/
/*	Return

	kMoaMixErr_NoReceptor			Stream Reader does not have any added receptors.
*/
};
typedef IMoaStreamReader *PIMoaStreamReader;
DEFINE_GUID(IID_IMoaStreamReader, 0xfc0eadbc, 0x3ba2, 0x11d3, 0x88, 0x6f, 0x00, 0x90, 0x27, 0x72, 0x04, 0xfa);

/* -------------------------------------------------------------------------- */

#undef INTERFACE
#define INTERFACE IMoaWriter
DECLARE_INTERFACE_(IMoaWriter, IMoaUnknown)
/*	Description

	A given Agent can declare the IMoaWriter interface to indicate that it 
	can write the Agent's format (as opposed to just reading it). While 
	Readers take DataObjects as their source material, Writers take Readers 
	as source material! (Wacky, eh?)

	Why is this? Well, mainly because DataObjects are inappropriate as 
	writer source material. Writers are typically used by an application to 
	write a particular data format, and applications don't want have to 
	provide the data in a particular data format (in a DataObject) before 
	writing it. What we really want to do is provide the reader with an 
	abstracted way of accessing the data to be written that's appropriate to 
	the data in question, without imposing requirements on the data 
	representation.

	For example, you may write a JPEG writer that can accept an 
	IMoaReader as a "data source"; an application would then 
	have to implement this Reader onto its own internal document structure 
	in order to use this Writer. But, every application that did so would be 
	able to take advantage of this Writer.

	Alternatively, a Writer can accept a DataObject directly (more or less; 
	see IMoaReceptorDataObject) if it suspects that there is a format it can 
	access directly to write more efficiently. Most Writers probably won't 
	access DataObjects directly, but it's there if they need to. (For example, 
	the JPEG writer mentioned above might also accept DataObjects 
	containing Macintosh PICT data and use the Translation 
	Manager to do the conversion.)

	Writer Options
	--------------
	
	Every Writer is assumed to have a set of options for how it will write 
	its format. For instance, a JPEG writer might want to set the resolution 
	and "lossy" factor. Writers are expected to store these options in a 
	MoaDict.

	A Writer must be able to provide a caller with a "default" set of 
	options. It may also optionally present a user interface (i.e., modal 
	dialog) to set the options. In both cases, the options aren't used 
	directly, but returned to the caller, who may modify them before actual 
	use.

	Why is this? Well, for one, it allows the caller to infer something 
	about the nature of the Writer. Usually, the caller may not care what 
	Writer is being used; all he knows is that the user requested a given 
	Writer be used to save his document. However, the caller may examine the 
	options dictionary for well-known keys to determine information about 
	the Writer.

	It also allows the caller to control what is being written (overriding 
	user choice) and to save/restore Writer settings.

	Registering Your Writer
	-----------------------
	
	If a Writer has no Agent, it should put the Agent type information into 
	the Writer registry (see "Do I Really Need An Agent?" for more 
	information). In addition the info in the IMoaAgent interface, writers 
	need additional type information.

	Writers should declare whether or not the writer has a UI in the key
	kWriterRegKey_HasUI (which is type Boolean).

	Writers must declare the interface IDs of receptors they can provide,
	using the key kWriterRegKey_ProvidedReceptors. This is an array
	of MoaInterfaceID (inserted into the dictionary using kMoaDictType_Bytes).

	Your Writer can list IID_IMoaReceptorDataObject under ProvidedReceptors 
	if it wants to receive DataObjects directly; if so, it should also list 
	the acceptable formats under the key kWriterRegKey_InputFormats. This is 
	in the form of format names (as would be passed to 
	IMoaFormatServices::RegisterFormat); if there are multiple formats, they 
	should be separated with a semicolon (but no trailing semicolon).
*/
{
	STD_IUNKNOWN_METHODS

	MoaDeclareMethod(CountProvidedReceptorIDs, (THIS_
		moa_out MoaLong * pNumReceptors
	))
/*	Description

	Provides the number of Receptors that can be returned by
	GetNthProvidedReceptorID.
*/

	MoaDeclareMethod(GetNthProvidedReceptorID, (THIS_
		moa_in MoaLong index,
		moa_out MoaInterfaceID * ppReceptorIID
	))
/*	Description

	Returns the Receptors that the Writer can provide to receive data. 
	These should be provided in the order preferred by the Writer, 
	i.e., in order from most accurate to least accurate. 
	
	index varies from 0 ... (CountUnderstoodReceptorIDs - 1).
*/

	MoaDeclareMethod(SetWriterDataSource, (THIS_
		moa_in PIMoaReader pSourceReader
	))
/*	Description

	Sets the data source to be used by the writer. The Writer is passed a 
	Reader. Your Writer should examine the Receptors that the Reader can 
	use; if your Writer can't provide any of those Receptors to the Reader, 
	you should return kMoaMixErr_BadDataSourceType. Be sure to AddRef()/
	Release() any Readers you keep around.
*/
/*	Return
	
	kMoaMixErr_BadDataSourceType	Writer can't write this data source
*/

	MoaDeclareMethod(SetWriterOptions, (THIS_
		moa_in PIMoaDict pOptionsDict
	))
/*	Description

	Set the options to use for future writes. The Writer should retain this 
	dictionary internally. If this method is never called, the default 
	options should be used.
*/

	MoaDeclareMethod(GetWriterOptions, (THIS_
		moa_out PIMoaDict * ppOptionsDict
	))
/*	Description

	Get the current set of options used for writes. If SetWriterOptions has 
	never been called, this should be return a dictionary identical to one
	returned by GetDefaultWriteOptions. The options dict belongs to the 
	caller, who must Release it.
*/

	MoaDeclareMethod(GetDefaultWriterOptions, (THIS_
		moa_out PIMoaDict * ppOptionsDict
	))
/*	Description

	Returns a dict fully populated with all the options supported by the 
	writer. This can be used by the caller to determine the keys understood 
	by the Writer. If the Writer has no options, it should return an empty 
	dict (i.e., one containing no entries). Note that the values of the 
	default options might vary depending on the source data, but the same 
	keys should always be present. The options dict belongs to the caller, 
	who must Release it. Note that this doesn't set the options (you must 
	call SetWriterOptions to do that).
*/

	MoaDeclareMethod(GetWriterOptionsFromUser, (THIS_
		moa_in SysWindow pParentWindow,
		moa_out PIMoaDict * ppOptionsDict
	))
/*	Description

	Use a dialog to return a set of writer options. Note that this doesn't 
	set the options (you must call SetWriterOptions to do that). The options 
	dict belongs to the caller, who must Release it. pParentWindow is the 
	(optional) parent window for the options dialog (HWND or WindowPtr).
*/
/*	Return
	
	kMoaErr_BadParam		Reader has no UI
	kMoaErr_UserCanceled	User canceled the dialog
*/

	MoaDeclareMethod(GetMaxWriteSize, (THIS_
		moa_out MoaLong * pMaxSize
	))
/*	Description

	Sets *pMaxSize to the maximum size of the data that would be written 
	by the current options and data source. If the maximum size is unknown or 
	unbounded, set *pMaxSize to -1. If you do return a maximum size, you must 
	not exceed it (though you can write less). This is intended to be a 
	reasonable-size estimate for chunk allocation, so if you cannot give a 
	reasonable size estimate, it may be better to always return -1.
*/

	MoaDeclareMethod(WriteToMoaStorageMedium, (THIS_
		moa_in ConstPMoaStorageMedium pDestStorage
	))
/*	Description

	Write the data source to the given storage medium. All Writers must 
	support this method; if a Writer doesn't want or need to special-case 
	various storage media, it can use 
	IMoaDataObjectServices::StorageMediumToMoaStream to convert the storage 
	medium into a MoaStream.
*/

	MoaDeclareMethod(WriteToMoaStream, (THIS_
		moa_in PIMoaStream pDestStream
	))
/*	Description

	Write the data source to the given MoaStream. You should assume the 
	stream is open for writing and already set to the correct position 
	(which might not be position 0).
*/
};
typedef IMoaWriter * PIMoaWriter;
DEFINE_GUID(IID_IMoaWriter, 0xB8B0D2C4L, 0x7356, 0x11CF, 0x89, 0x5C, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

#define kWriterRegKey_HasUI 				"HasUI" 			/* kMoaDictType_Bool */
#define kWriterRegKey_ProvidedReceptors 	"ProvidedReceptors" /* kMoaDictType_Bytes */
#define kWriterRegKey_InputFormats			"InputFormats"		/* kMoaDictType_CString */

/* -------------------------------------------------------------------------- */

enum {
	kLegOptions_EditRequire = 0x0001,		/* if T, user must be allowed to edit */
	kLegOptions_EditForbid	= 0x0002,		/* if T, user must not be allowed to edit */
	kLegOptions_WantIntermediateData = 0x0004	/* if F, server doesn't need to send
		intermediate updates. */
};

#undef INTERFACE
#define INTERFACE IMoaLegger
DECLARE_INTERFACE_(IMoaLegger, IMoaUnknown)
/*	Description

	To leg is a verb, meaning "launch an external editor, 
	ask it to edit some data, and get the modified data" 
	(Launch Edit Get == LEG... get it? Ummm... ok. It's a 
	lame name.).

	Moa Services provides a way to do this with 
	applications that don't use MIX; this basically 
	launches the other app, tells it to open the given 
	file, then monitors the file mod date until it has 
	changed. (See IMoaLegServices for more details.)

	However, an agent can provide a specific interface 
	(i.e., IMoaLegger) to enhance the "leg experience", 
	by providing for a tighter UI coupling, or by 
	eliminating unnecessary data translations.
*/
{
	STD_IUNKNOWN_METHODS

	MoaDeclareMethod(CountUnderstoodReceptorIDs, (THIS_
		moa_out MoaLong * pNumReceptors
	))
/*	Description

	Provides the number of Receptors that can be returned by
	GetNthUnderstoodReceptorID.
*/

	MoaDeclareMethod(GetNthUnderstoodReceptorID, (THIS_
		moa_in MoaLong index,
		moa_out MoaInterfaceID * ppReceptorIID
	))
/*	Description

	Returns the Receptors that the Legger can provide data thru. 
	These should be provided in the order preferred by the Legger, 
	i.e., in order from most accurate to least accurate. 
	
	index varies from 0 ... (CountUnderstoodReceptorIDs - 1).

	Note: Leggers currently are restricted to using 
	IMoaReceptorDataObject! This may be relaxed in the 
	future, however, so be sure to correctly implement 
	these methods: CountUnderstoodReceptorIDs should 
	return "1", and GetNthUnderstoodReceptorID should 
	return IID_IMoaReceptorDataObject if index is 0, and 
	kMoaErr_BadParam otherwise.
*/

	MoaDeclareMethod(GetLeggerName, (THIS_
		moa_in MoaLong nameBufLen,
		moa_out PMoaChar pNameBuf
	))
/*	Description

	Returns the name of the application in a user-visible 
	form.
*/

	MoaDeclareMethod(SetHostNames, (THIS_
		moa_in ConstPMoaChar pAppName,
		moa_in ConstPMoaChar pMediaName
	))
/*	Description

	This is useful for when the data to be edited is 
	written to a temp file, which can be very short and 
	cryptic on some systems. This call allows the client 
	to tell the legger the name of itself, and of the 
	media being edited, in a longer and more descriptive 
	manner.
*/

	MoaDeclareMethod(SetSelector, (THIS_
		moa_in PIMoaDict pSelectorDict
	))
/*	Description

	OK, you're probably confused here -- I know, 
	IMoaReader has a similar method. These selectors are 
	exactly the same in format as the ones you pass to a 
	reader. Here, they are intended to serve as a "hint" 
	to the legger as to the sort of data you want edited 
	and returned. 

	For instance, say you have a complete xRes file, and 
	want to edit only the first alpha channel. By passing 
	a selector for just that channel here, the legger may 
	be able to optimize its performance (e.g., pass only 
	that channel back and forth, rather than the whole 
	file).

	Note that the data returned must still match the 
	requested format, regardless of the selector used.

	As with readers, if the selector is not understood by 
	the legger, kMoaErr_BadParam is returned.
*/

	MoaDeclareMethod(RequestLegData, (THIS_
		moa_in PIMoaDataObject pDataIn,
		moa_in MoaLong legOptions,
		moa_in MoaLong numFormats,
		moa_in ConstPMixFormat pFormatsWeWant,
		moa_in SysWindow pParentWindow,
		moa_in ConstPMoaRect pSrcRect,
		moa_in ConstPMoaChar pTempFileNameBase,
		moa_in PIMoaReceptor pReceptor,
		moa_in ConstPMoaInterfaceID pReceptorID
	))
/*	Description

	This is the meat of this interface. You call 
	RequestLegData with source data in the form of a 
	DataObject, and specify the format of data you want 
	returned; the edited data is sent to the Receptor that 
	you pass in.

	Note: Leggers currently are restricted to using 
	IMoaReceptorDataObject. This may be relaxed in the 
	future.

	legOptions specifies various options. If 
	kLegOptions_EditRequire is set, the user must be 
	allowed to edit the data; if kLegOptions_EditForbid is 
	set, the user must not be allowed to edit the data (or 
	interact in any way, ie, the app must be "background 
	only"). If kLegOptions_WantIntermediateData is set, it 
	means that your Receptor would like to receive 
	possible "intermediate" updates to the edited data. 
	(Note that you may or may not get intermediate updates 
	whether or not you set this bit; its intent is as a 
	"hint" to the server that it need not send 
	intermediate updates if the bit is clear.)

	numFormats/pFormatsWeWant is the format(s) of data 
	desired to be returned. If the count is zero, it means 
	that the return format should be the same as the 
	source format. If RequestLegData can't return at least 
	one of these types, it should return 
	kMoaMixErr_NotMyType.

	pParentWindow is the parent window containing the 
	source data (if any); you should always pass a valid 
	value, even if there is no source window. Similarly, 
	pSrcRect is the rectangle containing the source data 
	in the parent window; if there is no source data or 
	source window, you should pass the entire content area 
	of the window being passed. You may not pass NULL for 
	either of these parameters.

	pTempFileNameBase is used when it is necessary to 
	write the pDataIn data object toa temporary file; this 
	is a localized string used to form temporary file 
	names. You should be sure to leave at least two 
	characters' worth of space for a unique number; thus, 
	on 8.3 filename systems, this parameter should be no 
	longer than 6 characters. The suggested form (in 
	English, anyway) for this parm is <appname> <temp> 
	(Thus, Director might use "DRTEMP" on 8.3 systems and 
	"Director Temp " on Mac/Win95; MIX would append a two-
	digit number afterwards to get "DRTEMP1" or "Director 
	Temp 1").

	pReceptor/pReceptorID is an IMoaReceptor that you 
	construct to "listen" for the updates; it will be 
	called at a later date with the updated data. Note 
	that pReceptor is assumed to actually be an interface 
	corresponding to pReceptorID (since IMoaReceptor is an 
	abstract interface class that's never actually 
	instantiated). Example: 


	PIMoaReceptorDataObject pRDO = GetMyReceptor();
	pLeg->RequestLegData(... (PIMoaReceptor)pRDO, &IID_IMoaReceptorDataObject);

	A typical app will use IMoaLegger like so:

	-- call RequestLegData with the source data to	be edited.
	-- if that succeeds, put up a modal dialog with "Done" and "Cancel".
	-- Wait for your Receptor to receive some data (or an error):
		-- kMoaStatus_IntermediateData (data has been returned, 
			but editing session isn't done; don't close the dialog yet)
		-- kMoaErr_NoErr (data has been returned, and editing session is done)
		-- kMoaMixErr_UserCanceled (user canceled the operation)
		-- kMoaErr_OutOfMem (nuff said)
		-- kMoaMixErr_NotEnoughMemForApp (not enough memory to launch editor)
		-- kMoaErr_BadParam (legOptions were bad)
		-- kMoaMixErr_BadDataSourceType (source DataObject has no format I can process)
		-- kMoaMixErr_NotMyType (destination format can't be provided)
	-- when the user clicks Done or Cancel, or the 
		listener returns one of the above codes, 
		take down the dialog and update your 
		document with the data that was returned 
		(or show an error message).

	A "synthesized" IMoaLegger (ie, one constructed by 
	IMoaLegServices to work with a non-MIX app) will do 
	something like this:

	-- Launch the editing app
	-- Write the source data object to a file (if it's not already in one)
	-- Send the editing app an "Open this file" event
	-- Repeatedly poll the modification date of the file and 
		check for the application to quit
	-- When the file changes, send an intermediate 
		update to the client, 
	-- When the user clicks "done" or "cancel", or the 
		application quits, put the file in a data object, 
		and return it to the caller

	Notes:

	-- In all cases, kMoaMixErr_UserCancel should be 
	returned if the user cancels the operation.

	-- Don't forget to register the IMoaLegger interface 
	for your agent. There are currently no extra keys to 
	register with the interface.

	-- Some error conditions won't be returned from 
	RequestLegData, but will instead be sent to your 
	Receptor. This is due to the nature of the IAC 
	involved; on some systems (e.g., Macintosh) the server 
	won't receive the request immediately, and thus 
	RequestLegData can't return an error immediately. The 
	moral: check for errors returned by 
	IMoaLegger::RequestLegData, and passed into 
	IMoaReceptor::SetStatus.
*/
/*	Return
	
	kMoaErr_UserCanceled	User canceled
*/

	MoaDeclareMethod(LeggerStopped, (THIS_
		moa_in MoaError status
	))
/*	Description

	This is a method that the client can use to tell the 
	server that the leg connection is being terminated. 
	The value of "status" should be kMoaErr_NoErr if the 
	user has requested a normal termination (e.g., clicked 
	"Done" after data has been received) or 
	kMoaMixErr_UserCanceled if the operation is a cancel 
	(e.g., clicked "cancel").

	Note that the server will normally respond in the same 
	way regardless of the value of "status": that is, 
	close any document window(s) associated with this leg 
	connection without any user prompting.
*/
};
typedef IMoaLegger * PIMoaLegger;
DEFINE_GUID(IID_IMoaLegger, 0x77BA09F8L, 0xCA8A, 0x11CF, 0x80, 0x3D, 0x08, 0x00, 0x07, 0x16, 0x0D, 0xC3);

/* -------------------------------------------------------------------------- */
#undef INTERFACE
#define INTERFACE IMoaLegger2
DECLARE_INTERFACE_(IMoaLegger2, IMoaLegger)
/*	Description

	IMoaLegger2 is a superset of IMoaLegger that
	is provided by some (but not all) leggers. If you
	need to use the extra methods provided by IMoaLegger2,
	acquire an IMoaLegger in the usual way, call
	QueryInterface on it, and *if* IMoaLegger2 is supported,
	you can use the extra calls. DO NOT ASSUME THAT ALL
	LEGGERS SUPPORT IMoaLegger2.
*/
{
	STD_IUNKNOWN_METHODS

	MoaDeclareMethod(CountUnderstoodReceptorIDs, (THIS_
		moa_out MoaLong * pNumReceptors
	))
/*	Description

	Provides the number of Receptors that can be returned by
	GetNthUnderstoodReceptorID.
*/

	MoaDeclareMethod(GetNthUnderstoodReceptorID, (THIS_
		moa_in MoaLong index,
		moa_out MoaInterfaceID * ppReceptorIID
	))
/*	Description

	Returns the Receptors that the Legger can provide data thru. 
	These should be provided in the order preferred by the Legger, 
	i.e., in order from most accurate to least accurate. 
	
	index varies from 0 ... (CountUnderstoodReceptorIDs - 1).

	Note: Leggers currently are restricted to using 
	IMoaReceptorDataObject! This may be relaxed in the 
	future, however, so be sure to correctly implement 
	these methods: CountUnderstoodReceptorIDs should 
	return "1", and GetNthUnderstoodReceptorID should 
	return IID_IMoaReceptorDataObject if index is 0, and 
	kMoaErr_BadParam otherwise.
*/

	MoaDeclareMethod(GetLeggerName, (THIS_
		moa_in MoaLong nameBufLen,
		moa_out PMoaChar pNameBuf
	))
/*	Description

	Returns the name of the application in a user-visible 
	form.
*/

	MoaDeclareMethod(SetHostNames, (THIS_
		moa_in ConstPMoaChar pAppName,
		moa_in ConstPMoaChar pMediaName
	))
/*	Description

	This is useful for when the data to be edited is 
	written to a temp file, which can be very short and 
	cryptic on some systems. This call allows the client 
	to tell the legger the name of itself, and of the 
	media being edited, in a longer and more descriptive 
	manner.
*/

	MoaDeclareMethod(SetSelector, (THIS_
		moa_in PIMoaDict pSelectorDict
	))
/*	Description

	OK, you're probably confused here -- I know, 
	IMoaReader has a similar method. These selectors are 
	exactly the same in format as the ones you pass to a 
	reader. Here, they are intended to serve as a "hint" 
	to the legger as to the sort of data you want edited 
	and returned. 

	For instance, say you have a complete xRes file, and 
	want to edit only the first alpha channel. By passing 
	a selector for just that channel here, the legger may 
	be able to optimize its performance (e.g., pass only 
	that channel back and forth, rather than the whole 
	file).

	Note that the data returned must still match the 
	requested format, regardless of the selector used.

	As with readers, if the selector is not understood by 
	the legger, kMoaErr_BadParam is returned.
*/

	MoaDeclareMethod(RequestLegData, (THIS_
		moa_in PIMoaDataObject pDataIn,
		moa_in MoaLong legOptions,
		moa_in MoaLong numFormats,
		moa_in ConstPMixFormat pFormatsWeWant,
		moa_in SysWindow pParentWindow,
		moa_in ConstPMoaRect pSrcRect,
		moa_in ConstPMoaChar pTempFileNameBase,
		moa_in PIMoaReceptor pReceptor,
		moa_in ConstPMoaInterfaceID pReceptorID
	))
/*	Description

	This is the meat of this interface. You call 
	RequestLegData with source data in the form of a 
	DataObject, and specify the format of data you want 
	returned; the edited data is sent to the Receptor that 
	you pass in.

	Note: Leggers currently are restricted to using 
	IMoaReceptorDataObject. This may be relaxed in the 
	future.

	legOptions specifies various options. If 
	kLegOptions_EditRequire is set, the user must be 
	allowed to edit the data; if kLegOptions_EditForbid is 
	set, the user must not be allowed to edit the data (or 
	interact in any way, ie, the app must be "background 
	only"). If kLegOptions_WantIntermediateData is set, it 
	means that your Receptor would like to receive 
	possible "intermediate" updates to the edited data. 
	(Note that you may or may not get intermediate updates 
	whether or not you set this bit; its intent is as a 
	"hint" to the server that it need not send 
	intermediate updates if the bit is clear.)

	numFormats/pFormatsWeWant is the format(s) of data 
	desired to be returned. If the count is zero, it means 
	that the return format should be the same as the 
	source format. If RequestLegData can't return at least 
	one of these types, it should return 
	kMoaMixErr_NotMyType.

	pParentWindow is the parent window containing the 
	source data (if any); you should always pass a valid 
	value, even if there is no source window. Similarly, 
	pSrcRect is the rectangle containing the source data 
	in the parent window; if there is no source data or 
	source window, you should pass the entire content area 
	of the window being passed. You may not pass NULL for 
	either of these parameters.

	pTempFileNameBase is used when it is necessary to 
	write the pDataIn data object toa temporary file; this 
	is a localized string used to form temporary file 
	names. You should be sure to leave at least two 
	characters' worth of space for a unique number; thus, 
	on 8.3 filename systems, this parameter should be no 
	longer than 6 characters. The suggested form (in 
	English, anyway) for this parm is <appname> <temp> 
	(Thus, Director might use "DRTEMP" on 8.3 systems and 
	"Director Temp " on Mac/Win95; MIX would append a two-
	digit number afterwards to get "DRTEMP1" or "Director 
	Temp 1").

	pReceptor/pReceptorID is an IMoaReceptor that you 
	construct to "listen" for the updates; it will be 
	called at a later date with the updated data. Note 
	that pReceptor is assumed to actually be an interface 
	corresponding to pReceptorID (since IMoaReceptor is an 
	abstract interface class that's never actually 
	instantiated). Example: 


	PIMoaReceptorDataObject pRDO = GetMyReceptor();
	pLeg->RequestLegData(... (PIMoaReceptor)pRDO, &IID_IMoaReceptorDataObject);

	A typical app will use IMoaLegger like so:

	-- call RequestLegData with the source data to	be edited.
	-- if that succeeds, put up a modal dialog with "Done" and "Cancel".
	-- Wait for your Receptor to receive some data (or an error):
		-- kMoaStatus_IntermediateData (data has been returned, 
			but editing session isn't done; don't close the dialog yet)
		-- kMoaErr_NoErr (data has been returned, and editing session is done)
		-- kMoaMixErr_UserCanceled (user canceled the operation)
		-- kMoaErr_OutOfMem (nuff said)
		-- kMoaMixErr_NotEnoughMemForApp (not enough memory to launch editor)
		-- kMoaErr_BadParam (legOptions were bad)
		-- kMoaMixErr_BadDataSourceType (source DataObject has no format I can process)
		-- kMoaMixErr_NotMyType (destination format can't be provided)
	-- when the user clicks Done or Cancel, or the 
		listener returns one of the above codes, 
		take down the dialog and update your 
		document with the data that was returned 
		(or show an error message).

	A "synthesized" IMoaLegger (ie, one constructed by 
	IMoaLegServices to work with a non-MIX app) will do 
	something like this:

	-- Launch the editing app
	-- Write the source data object to a file (if it's not already in one)
	-- Send the editing app an "Open this file" event
	-- Repeatedly poll the modification date of the file and 
		check for the application to quit
	-- When the file changes, send an intermediate 
		update to the client, 
	-- When the user clicks "done" or "cancel", or the 
		application quits, put the file in a data object, 
		and return it to the caller

	Notes:

	-- In all cases, kMoaMixErr_UserCancel should be 
	returned if the user cancels the operation.

	-- Don't forget to register the IMoaLegger interface 
	for your agent. There are currently no extra keys to 
	register with the interface.

	-- Some error conditions won't be returned from 
	RequestLegData, but will instead be sent to your 
	Receptor. This is due to the nature of the IAC 
	involved; on some systems (e.g., Macintosh) the server 
	won't receive the request immediately, and thus 
	RequestLegData can't return an error immediately. The 
	moral: check for errors returned by 
	IMoaLegger::RequestLegData, and passed into 
	IMoaReceptor::SetStatus.
*/
/*	Return
	
	kMoaErr_UserCanceled	User canceled
*/

	MoaDeclareMethod(LeggerStopped, (THIS_
		moa_in MoaError status
	))
/*	Description

	This is a method that the client can use to tell the 
	server that the leg connection is being terminated. 
	The value of "status" should be kMoaErr_NoErr if the 
	user has requested a normal termination (e.g., clicked 
	"Done" after data has been received) or 
	kMoaMixErr_UserCanceled if the operation is a cancel 
	(e.g., clicked "cancel").

	Note that the server will normally respond in the same 
	way regardless of the value of "status": that is, 
	close any document window(s) associated with this leg 
	connection without any user prompting.
*/

	/* methods new to IMoaLegger2 */
	MoaDeclareMethod(ActivateLegger, (THIS))
/*	Description

	Calling this function activates the Legger by
	making it the frontmost application and activating
	the appropriate editing window. This call is useful
	if you support asynchronous legging and want to
	allow the user to easily re-focus the editor
	on an item being edited by double-clicking on it again.
*/
};
typedef IMoaLegger2 * PIMoaLegger2;
DEFINE_GUID(IID_IMoaLegger2, 0xe6e5de60, 0x415e, 0x11d0, 0x84, 0xd3, 0x0, 0xa0, 0x24, 0x86, 0x58, 0x76);

/* -------------------------------------------------------------------------- */

#endif
