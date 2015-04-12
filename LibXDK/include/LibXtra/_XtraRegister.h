#pragma once
#include<LibXtra\LibXtra.h>
#include<LibXtra\_BaseXValue.h>
 
class _XtraRegister :public IMoaRegister
{
private:
	MoaLong 				refcnt_; 		/* refcount for THIS INTERFACE */
	virtual __declspec(nothrow) HRESULT  __stdcall QueryInterface (ConstPMoaInterfaceID pInterfaceID, PPMoaVoid ppvObj);
	virtual __declspec(nothrow) MoaUlong __stdcall AddRef ();
	virtual __declspec(nothrow) MoaUlong __stdcall Release(); 

	/*
		this->register_script_(pCache, pXtraDict, __uuidof(_BaseXValue), "xtra XtraName\nnew object me\n");
		return kMoaErr_NoErr;
	*/
	virtual __declspec(nothrow) HRESULT  __stdcall Register ( PIMoaCache pCache,PIMoaXtraEntryDict pXtraDict )=0;

public:
	explicit _XtraRegister(_MOAFactory* mobj);
	virtual ~_XtraRegister(void);

protected:
	MoaError register_script_(PIMoaCache& pCache, PIMoaDict& pXtraDict,const GUID& clsid, const char* msgtable);
};

