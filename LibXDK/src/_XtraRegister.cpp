#include <LibXtra/_XtraRegister.h>

HRESULT 
__stdcall _XtraRegister::QueryInterface(ConstPMoaInterfaceID pInterfaceID, PPMoaVoid ppv)
{ 
	if(*pInterfaceID==IID_IMoaRegister){
		this->AddRef();
		*ppv =(IMoaRegister*)this;
		return S_OK;
	}

	if(*pInterfaceID==IID_IMoaUnknown){
		this->AddRef();
		*ppv =(IMoaUnknown*)this;
		return S_OK;
	}

	return E_NOINTERFACE;
} 
		 
MoaUlong 
__stdcall _XtraRegister::AddRef()
{ 
	return InterlockedIncrement(&this->refcnt_); 
} 
		 
MoaUlong 
__stdcall _XtraRegister::Release()
{ 
	MoaUlong result; 			 
	result = InterlockedDecrement(&this->refcnt_); 			
	if (!result) delete this; 			 	
	return result; 
}

_XtraRegister::_XtraRegister(_MOAFactory* mobj)
	:refcnt_(1)
{
	++DllgXtraInterfaceCount;
}

_XtraRegister::~_XtraRegister(void)
{
	--DllgXtraInterfaceCount;
}

/*
HRESULT  __stdcall 
_XtraRegister::Register(PIMoaCache pCache, PIMoaXtraEntryDict pXtraDict){
	this->register_script_(pCache, pXtraDict, __uuidof(_BaseXValue), "xtra XtraName\nnew object me\n");
	return kMoaErr_NoErr;
}
*/

MoaError 
_XtraRegister::register_script_(PIMoaCache& pCache, PIMoaDict& pXtraDict, const GUID& clsid, const char* msgtable){
	MoaError 		err = kMoaErr_NoErr;
	PIMoaDict 		pRegDict = NULL;
	if (err = pCache->AddRegistryEntry(pXtraDict, &clsid, &IID_IMoaMmXScript, &pRegDict)) {
		return err;
	}

	err = pRegDict->Put(kMoaDrDictType_MessageTable, msgtable, 0, kMoaDrDictKey_MessageTable);
	return err;
}



