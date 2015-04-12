
/*==================================================================================================\
| ADOBE SYSTEMS INCORPORATED																		|
| Copyright 2006 Ð 2008 Adobe Macromedia Software LLC												|
| All Rights Reserved																				|
|																									|
| NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the		|
| terms of the Adobe license agreement accompanying it.  If you have received this file from a		|
| source other than Adobe, then your use, modification, or distribution of it requires the prior	|
| written permission of Adobe.																		|
\==================================================================================================*/

#ifndef _moaByteArray_
#define _moaByteArray_

/*Define ByteArray CLSID*/
// {A5120015-7596-44bd-A450-92C23E2FDAFB}

DEFINE_GUID(CLSID(CByteArray), 0xa5120015, 0x7596, 0x44bd, 0xa4, 0x50, 0x92, 0xc2, 0x3e, 0x2f, 0xda, 0xfb);

/*
ByteArray class implements two interfaces IMoaMmXValue (For Lingo) and IMoaStream2 (for reading and writing).
IMoaMmxValue can be converted to MoaMmValue using IMoaMmXValueUtils and passed on to a lingo handler. Similarly ByteArray (in form of MoaMmValue) 
passed from lingo can be cnverted back to IMoaMmXValue in an Xtra. Use query interface to get IMoaStream2 interface from Xtra.


*/


#endif // _moabytearray_