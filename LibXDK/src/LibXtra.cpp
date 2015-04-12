#define INITGUID 
#include <LibXtra\LibXtra.h>
#include <LibXtra/_GUID.h>

///BASIC
MoaLong DllgXtraInterfaceCount = 0; 
HINSTANCE g_hInst;

#pragma comment(linker, "/ENTRY:DllEntryPoint")

extern "C"  {
	void __cdecl __security_init_cookie(void);
	BOOL WINAPI _DllEntryPoint(HINSTANCE, ULONG, __inout_opt LPVOID);

	extern "C"
		DECLSPEC_NOINLINE
		BOOL
		WINAPI
		DllEntryPoint(
		HINSTANCE hInstance,
		ULONG ulReason,
		__inout_opt LPVOID pv
		)
	{
		if (ulReason == DLL_PROCESS_ATTACH) {
			// Must happen before any other code is executed.  Thankfully - it's re-entrant
			__security_init_cookie();
		}
		return _DllEntryPoint(hInstance, ulReason, pv);
	}

	DECLSPEC_NOINLINE
		BOOL WINAPI _DllEntryPoint(
		HINSTANCE hInstance,
		ULONG ulReason,
		__inout_opt LPVOID pv
		)
	{
		switch (ulReason)
		{
		case DLL_PROCESS_ATTACH:
			g_hInst = hInstance;
			break;

		case DLL_PROCESS_DETACH:
			break;
		}

		return TRUE;
	}

	/// Basic
	MoaError __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, PPMoaVoid ppv) { 
		return kMoaErr_BadClass; 
	} 

	MoaError __stdcall DllCanUnloadNow(void) { 
		return ((DllgXtraInterfaceCount == 0 ? 0 : kMoaErr_XtraInUse)); 
	} 

	// 전체 엑스트라 정보
	MoaError __stdcall DllGetClassInfo(PIMoaCalloc pCalloc, ConstPMoaClassInterfaceInfo * ppClassInfo) { 
		size_t len(0);
		XTRA_INFO* info=NULL;
		rawXtraInfo(info, len);

		size_t bufsize=2*sizeof(MoaClassInterfaceInfo)*(len+1);

		*ppClassInfo = (ConstPMoaClassInterfaceInfo)pCalloc->NRAlloc( bufsize ); 
		if (!*ppClassInfo) return (kMoaErr_OutOfMem); 

		MoaClassInterfaceInfo* cur = const_cast<MoaClassInterfaceInfo*>(*ppClassInfo);
		memset(cur,0, bufsize);

		for( size_t i=0;i<len;++i){
			//CLASS INFO
			cur->classID = *info[i].classID_;
			cur->classVersion = XTRA_CLASS_VERSION;
			cur->interfaceID=IID_IMoaClassFactory;
			cur->interfaceVersion=1;

			//INTERFACE INFO
			++cur;
			cur->classID=*info[i].classID_;
			cur->classVersion= XTRA_CLASS_VERSION;
			cur->interfaceID = *info[i].interfaceID_;
			cur->interfaceVersion = XTRA_CLASS_VERSION;

			//NEXT
			++cur;
		}

		return (kMoaErr_NoErr); 
	}

	//클래스 정보를 반환한다.
	MoaError __stdcall DllGetClassForm(ConstPMoaClassID pClassID, MoaLong * pObjSize, MoaCreatedProc * pCreateProc, MoaDestroyedProc * pDestroyProc) { 
		size_t len(0);
		XTRA_INFO* info=NULL;
		rawXtraInfo(info, len);

		for( size_t i=0;i<len;++i){
			if( *info[i].classID_ != *pClassID) continue;
			*pObjSize = sizeof(_MOAFactory);
			*pCreateProc=NULL;
			*pDestroyProc=NULL;
			return kMoaErr_NoErr;
		}
		return kMoaErr_BadClass;
	} 

	//인터페이스를 반환한다.
	MoaError __stdcall DllGetInterface(PMoaVoid pObj, ConstPMoaClassID pClassID, ConstPMoaInterfaceID pInterfaceID, PPMoaVoid ppv) { 
		size_t len(0);
		XTRA_INFO* info=NULL;
		rawXtraInfo(info, len);

		for( size_t i=0;i<len;++i){
			if (*info[i].classID_ != *pClassID) continue;
			if (*info[i].interfaceID_ != *pInterfaceID) continue;
			return info[i].fn_(pObj, ppv);
		}

		return kMoaErr_BadInterface;
	}
}

/*
Text Encoding
*/

std::wstring 
string_to_wstring(const std::string& str, const std::locale& loc){
	return ansi_to_wchar(str, loc);
}

std::string
string_to_utf8(const std::string& str, const std::locale& loc){
	std::wstring wstr = ansi_to_wchar(str, loc);
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	return myconv.to_bytes(wstr);
}

std::wstring 
ansi_to_wchar(const std::string& str, const std::locale& loc){
	typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
	codecvt_t const& codecvt = std::use_facet<codecvt_t>(loc);		
	std::wstring temp(str.size(), (wchar_t)0);

	const char* in_next = str.c_str();
	wchar_t* out_next = &temp[0];
	
	std::mbstate_t state = std::mbstate_t();
	std::codecvt_base::result r = codecvt.in(state,
		str.c_str(), str.c_str() + str.size(), in_next,
		&temp[0], &temp[0] + temp.size(), out_next);
	if (r == std::codecvt_base::error) return L"";

	std::size_t len = out_next - &temp[0];
	temp.resize(len);
	return temp;
}

std::string 
wchar_to_ansi(const std::wstring& wstr, const std::locale& loc){
	typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
	codecvt_t const& codecvt = std::use_facet<codecvt_t>(loc);
		
	std::string temp((wstr.size()) * codecvt.max_length(), (char)0);

	wchar_t const* in_next = wstr.c_str();
	char* out_next = &temp[0];

	std::mbstate_t state = std::mbstate_t();
	std::codecvt_base::result r = codecvt.out(state,
		wstr.c_str(), wstr.c_str() + wstr.size(), in_next,
		&temp[0], &temp[0] + temp.size(), out_next);	
	if (r == std::codecvt_base::error) return "";

	std::size_t len = out_next - &temp[0];
	temp.resize(len);
	return temp;
}

std::string
utf8_to_string(const std::string& utf8str, const std::locale& loc)
{
	// UTF-8 to wstring
	std::wstring_convert<std::codecvt_utf8<wchar_t>> wconv;
	std::wstring wstr = wconv.from_bytes(utf8str);
	return wchar_to_ansi(wstr, loc);
}

std::string
wstring_to_string(const std::wstring& wstr, const std::locale& loc){
	return wchar_to_ansi(wstr, loc);
}

std::wstring
utf8_to_wstring(const std::string& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> wconv;
	return wconv.from_bytes(str);
}

std::string 
wstring_to_utf8(const std::wstring& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	return myconv.to_bytes(str);
}

/*
MoaUtils
*/
MoaUtils::MoaUtils(_MOAFactory* obj)
	:obj_{ obj }, 
	value_{ this->obj_->pCallback }, 
	list_{ this->obj_->pCallback }, 
	xvalue_{ this->obj_->pCallback }, 
	color_{ this->obj_->pCallback },
	utils2_{ this->obj_->pCallback },
	image_{ this->obj_->pCallback }
{
	obj_->AddRef();
}


MoaUtils::~MoaUtils(void)
{
	obj_->Release();
}
//////////////////////////////

HRESULT
MoaUtils::QueryInterface(ConstPMoaInterfaceID pInterfaceID, PPMoaVoid ppvObj)
{
	return this->obj_->pCallback->QueryInterface(pInterfaceID, ppvObj);
}

HRESULT
MoaUtils::MoaCreateInstance(ConstPMoaClassID pClassID, ConstPMoaInterfaceID pInterfaceID, PPMoaVoid ppvObj)
{
	return this->obj_->pCallback->MoaCreateInstance(pClassID, pInterfaceID, ppvObj);
}

struct IMoaCache *
	MoaUtils::MoaGetCache()
{
		return this->obj_->pCallback->MoaGetCache();
	}

XtraResourceCookie
MoaUtils::MoaBeginUsingResources(MoaFileRef fileRef, XtraResourceCookie * pSaveCookie)
{
	return this->obj_->pCallback->MoaBeginUsingResources(fileRef, pSaveCookie);
}

void
MoaUtils::MoaEndUsingResources(MoaFileRef fileRef, XtraResourceCookie saveCookie)
{
	return this->obj_->pCallback->MoaEndUsingResources(fileRef, saveCookie);
}

HRESULT
MoaUtils::SymbolToString(MoaMmSymbol symbol, PMoaChar pStringBuf, MoaLong bufLen)
{
	return value_->SymbolToString(symbol, pStringBuf, bufLen);
}

HRESULT
MoaUtils::SymbolToString(MoaMmSymbol& symbol, std::string& str)
{
	char buf[1024] = {};
	MoaLong bufLen{ 1024 };
	HRESULT hr = this->SymbolToString(symbol, buf, bufLen);
	str = buf;

	std::locale loc("");
	for (auto& item : str){
		item = std::tolower(item, loc);
	}
	return hr;
}


MoaMmValueType 
MoaUtils::is_type(const MoaMmValue& pValue){
	MoaMmValueType type{};
	value_->ValueType(&pValue, &type);
	return type;
}

HRESULT
MoaUtils::ValueToPropReference(ConstPMoaMmValue pValue, PIMoaMmPropertyReference *ppPropRef){
	MOAQIPtr<IMoaMmPropReferenceValue, &IID_IMoaMmPropReferenceValue> p(this->obj_->pCallback);
	return p->ValueToPropReference(pValue, ppPropRef);
}

HRESULT
MoaUtils::PropReferenceToValue(PIMoaMmPropertyReference pPropRef, PMoaMmValue pValue){
	MOAQIPtr<IMoaMmPropReferenceValue, &IID_IMoaMmPropReferenceValue> p(this->obj_->pCallback);
	return p->PropReferenceToValue(pPropRef, pValue);
}


HRESULT
MoaUtils::ValueAddRef(PMoaMmValue  pValue)
{
	return value_->ValueAddRef(pValue);
}

void
MoaUtils::ValueRelease(MoaMmValue& value)
{
	value_->ValueRelease(&value);
}

void
MoaUtils::ValueRelease(MoaMmCallInfo& callPtr)
{
	value_->ValueRelease(&callPtr.resultValue);
}

void
MoaUtils::ValueRelease(GetRef& ref)
{
	value_->ValueRelease(&ref.result_);
}



///
HRESULT
MoaUtils::PrintMessage(ConstPMoaChar pMsg)
{
	return utils2_->PrintMessage(pMsg);
}

HRESULT
MoaUtils::PrintMessage1(ConstPMoaChar pMsg, MoaLong arg1)
{
	return utils2_->PrintMessage1(pMsg, arg1);
}

HRESULT
MoaUtils::PrintMessage2(ConstPMoaChar pMsg, MoaLong arg1, MoaLong arg2)
{
	return utils2_->PrintMessage2(pMsg, arg1, arg2);
}

HRESULT
MoaUtils::PrintMessage3(ConstPMoaChar pMsg, MoaLong arg1, MoaLong arg2, MoaLong arg3)
{
	return utils2_->PrintMessage3(pMsg, arg1, arg2, arg3);
}


HRESULT
MoaUtils::PrintMessage4(ConstPMoaChar pMsg, MoaLong arg1, MoaLong arg2, MoaLong arg3, MoaLong arg4)
{
	return utils2_->PrintMessage4(pMsg, arg1, arg2, arg3, arg4);
}


HRESULT
MoaUtils::NewGC(ConstPMoaMmNativeGCInfo pNativeGCInfo, PIMoaMmGC * ppGC)
{
	return utils2_->NewGC(pNativeGCInfo, ppGC);
}

HRESULT
MoaUtils::RGBToIndex(ConstPMoaMmRGBTriple pRGBColor, PMoaMmColorIndex pIndex)
{
	return color_->RGBToIndex(pRGBColor, pIndex);
}

HRESULT
MoaUtils::IndexToRGB(MoaMmColorIndex index, PMoaMmRGBTriple pRGBColor)
{
	return color_->IndexToRGB(index, pRGBColor);
}


HRESULT
MoaUtils::GetActiveMovie(PIMoaDrMovie * ppIMoaDrMovie)
{
	MOAQIPtr<IMoaDrPlayer, &IID_IMoaDrPlayer> p(this->obj_->pCallback);
	return p->GetActiveMovie(ppIMoaDrMovie);
}

HRESULT
MoaUtils::GetMovieCount(MoaLong * pMovieCount)
{
	MOAQIPtr<IMoaDrPlayer, &IID_IMoaDrPlayer> p(this->obj_->pCallback);
	return p->GetMovieCount(pMovieCount);
}

HRESULT
MoaUtils::GetNthMovie(MoaLong movieIndex, PIMoaDrMovie * ppIMoaDrMovie)
{
	MOAQIPtr<IMoaDrPlayer, &IID_IMoaDrPlayer> p(this->obj_->pCallback);
	return p->GetNthMovie(movieIndex, ppIMoaDrMovie);
}

HRESULT
MoaUtils::GetCastMemTypeCount(MoaLong * pTypeCount)
{
	MOAQIPtr<IMoaDrPlayer, &IID_IMoaDrPlayer> p(this->obj_->pCallback);
	return p->GetCastMemTypeCount(pTypeCount);
}

HRESULT
MoaUtils::GetNthCastMemTypeSymbol(MoaLong typeIndex, PMoaMmSymbol pSymbol)
{
	MOAQIPtr<IMoaDrPlayer, &IID_IMoaDrPlayer> p(this->obj_->pCallback);
	return p->GetNthCastMemTypeSymbol(typeIndex, pSymbol);
}

HRESULT
MoaUtils::SetGlobalVarValue(MoaMmSymbol globalVarName, PMoaMmValue pValue)
{
	MOAQIPtr<IMoaDrPlayer, &IID_IMoaDrPlayer> p(this->obj_->pCallback);
	return p->SetGlobalVarValue(globalVarName, pValue);
}

HRESULT
MoaUtils::GetGlobalVarValue(MoaMmSymbol globalVarName, PMoaMmValue pValue)
{
	MOAQIPtr<IMoaDrPlayer, &IID_IMoaDrPlayer> p(this->obj_->pCallback);
	return p->GetGlobalVarValue(globalVarName, pValue);
}

HRESULT
MoaUtils::GetCastMemTypeDisplayName(MoaMmSymbol typeSymbol, PMoaChar pName, MoaLong maxLen)
{
	MOAQIPtr<IMoaDrPlayer, &IID_IMoaDrPlayer> p(this->obj_->pCallback);
	return p->GetCastMemTypeDisplayName(typeSymbol, pName, maxLen);
}

HRESULT
MoaUtils::CallHandler(MoaMmSymbol name, MoaLong nArgs, PMoaMmValue pArgs, PMoaMmValue pResult)
{
	MOAQIPtr<IMoaDrPlayer, &IID_IMoaDrPlayer> p(this->obj_->pCallback);
	return p->CallHandler(name, nArgs, pArgs, pResult);
}

HRESULT
MoaUtils::ResolveFileName(ConstPMoaChar pFileName, PMoaChar pPathName, MoaLong maxLen)
{
	MOAQIPtr<IMoaDrPlayer, &IID_IMoaDrPlayer> p(this->obj_->pCallback);
	return p->ResolveFileName(pFileName, pPathName, maxLen);
}

HRESULT
MoaUtils::GetProp(MoaMmSymbol symbol, PMoaMmValue pPropValue)
{
	MOAQIPtr<IMoaDrPlayer, &IID_IMoaDrPlayer> p(this->obj_->pCallback);
	return p->GetProp(symbol, pPropValue);
}

HRESULT
MoaUtils::SetProp(MoaMmSymbol symbol, ConstPMoaMmValue pPropValue)
{
	MOAQIPtr<IMoaDrPlayer, &IID_IMoaDrPlayer> p(this->obj_->pCallback);
	return p->SetProp(symbol, pPropValue);
}

void
MoaUtils::make_list(MoaMmValue& listValue)
{
	list_->NewListValue(&listValue);
}

void
MoaUtils::make_list(MoaMmCallInfo& callPtr)
{
	this->make_list(callPtr.resultValue);
}

void
MoaUtils::make_list(GetRef& ref)
{
	this->make_list(ref.result_);
}


void
MoaUtils::make_prop_list(MoaMmValue& value)
{
	list_->NewPropListValue(&value);
}

void
MoaUtils::make_prop_list(MoaMmCallInfo& callPtr)
{
	this->make_prop_list(callPtr.resultValue);
}

void
MoaUtils::make_prop_list(GetRef& ref)
{
	this->make_prop_list(ref.result_);
}

///

MoaMmSymbol 
MoaUtils::make_symbol(std::string& value){
	MoaMmSymbol s1;
	value_->StringToSymbol(value.c_str(), &s1);
	return s1;
}

MoaMmSymbol
MoaUtils::make_symbol(const char* value){
	MoaMmSymbol s1;
	value_->StringToSymbol(value, &s1);
	return s1; 
}


PMoaVoid
MoaUtils::NRAlloc(MoaUlong cb)
{
	return this->obj_->pCalloc->NRAlloc(cb);
}

void
MoaUtils::NRFree(PMoaVoid pv)
{
	return this->obj_->pCalloc->NRFree(pv);
}


MoaError
MoaUtils::ValueToCMRef(ConstPMoaMmValue pValue, PMoaDrCMRef pCMRef)
{
	MOAQIPtr<IMoaDrUtils, &IID_IMoaDrUtils> p(this->obj_->pCallback);
	return p->ValueToCMRef(pValue, pCMRef);
}

MoaError
MoaUtils::NewMediaInfo(MoaMmSymbol labelSymbol, MoaMmSymbol formatSymbol, PMoaVoid mediaData, MoaLong options, PMoaVoid aux, PMoaDrMediaInfo pMediaInfo)
{
	MOAQIPtr<IMoaDrUtils, &IID_IMoaDrUtils> p(this->obj_->pCallback);
	return p->NewMediaInfo(labelSymbol, formatSymbol, mediaData, options, aux, pMediaInfo);
}

MoaError
MoaUtils::MediaRelease(PMoaDrMediaInfo pMediaInfo)
{
	MOAQIPtr<IMoaDrUtils, &IID_IMoaDrUtils> p(this->obj_->pCallback);
	return p->MediaRelease(pMediaInfo);
}

MoaError
MoaUtils::NewScoreAccess(PIMoaDrScoreAccess * ppScore)
{
	MOAQIPtr<IMoaDrUtils, &IID_IMoaDrUtils> p(this->obj_->pCallback);
	return p->NewScoreAccess(ppScore);
}

MoaError
MoaUtils::RegisterNotificationClient(PIMoaDrNotificationClient pNotificationClient, MoaDrNotifyType notifyType, PMoaVoid refCon)
{
	MOAQIPtr<IMoaDrUtils, &IID_IMoaDrUtils> p(this->obj_->pCallback);
	return p->RegisterNotificationClient(pNotificationClient, notifyType, refCon);
}

MoaError
MoaUtils::UnregisterNotificationClient(PIMoaDrNotificationClient pNotificationClient, MoaDrNotifyType notifyType, PMoaVoid refCon)
{
	MOAQIPtr<IMoaDrUtils, &IID_IMoaDrUtils> p(this->obj_->pCallback);
	return p->UnregisterNotificationClient(pNotificationClient, notifyType, refCon);
}

MoaError
MoaUtils::ValueToMedia(ConstPMoaMmValue pValue, MoaHandle *phMediaData)
{
	MOAQIPtr<IMoaDrMediaValue, &IID_IMoaDrMediaValue> p(this->obj_->pCallback);
	return p->ValueToMedia(pValue, phMediaData);
}

MoaError
MoaUtils::MediaToValue(const MoaHandle hMediaData, PMoaMmValue pValue)
{
	MOAQIPtr<IMoaDrMediaValue, &IID_IMoaDrMediaValue> p(this->obj_->pCallback);
	return p->MediaToValue(hMediaData, pValue);
}


MoaError
MoaUtils::ValueToPicture(ConstPMoaMmValue pValue, MoaHandle *phPictureData)
{
	MOAQIPtr<IMoaDrMediaValue, &IID_IMoaDrMediaValue> p(this->obj_->pCallback);
	return p->ValueToPicture(pValue, phPictureData);
}


MoaError
MoaUtils::PictureToValue(const MoaHandle hPictureData, PMoaMmValue pValue)
{
	MOAQIPtr<IMoaDrMediaValue, &IID_IMoaDrMediaValue> p(this->obj_->pCallback);
	return p->PictureToValue(hPictureData, pValue);
}

MoaError
MoaUtils::CMRefToValue(ConstPMoaDrCMRef pCMRef, PMoaMmValue pValue)
{
	MOAQIPtr<IMoaDrValue, &IID_IMoaDrValue> p(this->obj_->pCallback);
	return p->CMRefToValue(pCMRef, pValue);
}

MoaError
MoaUtils::ValueToSpriteRef(ConstPMoaMmValue pValue, PMoaDrSpriteRef pSRef)
{
	MOAQIPtr<IMoaDrValue, &IID_IMoaDrValue> p(this->obj_->pCallback);
	return p->ValueToSpriteRef(pValue, pSRef);
}

MoaError
MoaUtils::SpriteRefToValue(ConstPMoaDrSpriteRef pSRef, PMoaMmValue pValue)
{
	MOAQIPtr<IMoaDrValue, &IID_IMoaDrValue> p(this->obj_->pCallback);
	return p->SpriteRefToValue(pSRef, pValue);
}


MoaError
MoaUtils::ValueToCastLibRef(ConstPMoaMmValue pValue, PMoaDrCastLibRef pCLRef)
{
	MOAQIPtr<IMoaDrValue, &IID_IMoaDrValue> p(this->obj_->pCallback);
	return p->ValueToCastLibRef(pValue, pCLRef);
}


MoaError
MoaUtils::CastLibRefToValue(ConstPMoaDrCastLibRef pCLRef, PMoaMmValue pValue)
{
	MOAQIPtr<IMoaDrValue, &IID_IMoaDrValue> p(this->obj_->pCallback);
	return p->CastLibRefToValue(pCLRef, pValue);
}

MoaError
MoaUtils::ValueToSoundSpriteRef(ConstPMoaMmValue pValue, PMoaDrSoundSpriteRef pSRef)
{
	MOAQIPtr<IMoaDrValue, &IID_IMoaDrValue> p(this->obj_->pCallback);
	return p->ValueToSoundSpriteRef(pValue, pSRef);
}

MoaError
MoaUtils::SoundSpriteRefToValue(ConstPMoaDrSoundSpriteRef pSRef, PMoaMmValue pValue)
{
	MOAQIPtr<IMoaDrValue, &IID_IMoaDrValue> p(this->obj_->pCallback);
	return p->SoundSpriteRefToValue(pSRef, pValue);
}

HWND
MoaUtils::GetHWNDth(long nTh){
	MoaLong count;
	GetMovieCount(&count);
	if (nTh>count) nTh = count;
	if (nTh<0) nTh = 1;

	MoaMmNativeGCInfo mGCInfo;

	MOAQIPtr<IMoaDrMovie, &IID_IMoaDrMovie> pDrMovie;
	MOAQIPtr<IMoaMmGC, &IID_IMoaMmGC> pMmGC;

	GetNthMovie(nTh, &pDrMovie);
	pDrMovie->GetStageWindowGC(&pMmGC);
	pMmGC->GetNativeGCInfo(&mGCInfo);
	return (HWND)mGCInfo.data.win_window.hWND;
}

HWND
MoaUtils::GetHWNDActive(){
	MoaMmNativeGCInfo mGCInfo;

	MOAQIPtr<IMoaDrMovie, &IID_IMoaDrMovie> pDrMovie;
	MOAQIPtr<IMoaMmGC, &IID_IMoaMmGC> pMmGC;

	GetActiveMovie(&pDrMovie);
	pDrMovie->GetStageWindowGC(&pMmGC);
	pMmGC->GetNativeGCInfo(&mGCInfo);
	return (HWND)mGCInfo.data.win_window.hWND;
}

MoaError
MoaUtils::ValueToMoa3dVector( ConstPMoaMmValue pValue, Moa3dVector pVector)
{
	MOAQIPtr<IMoa3dVectorValueUtils, &IID_IMoa3dVectorValueUtils> p(this->obj_->pCallback);
	return p->ValueToMoa3dVector(pValue, pVector);
}

MoaError
MoaUtils::Moa3dVectorToValue( Moa3dVector pVector, PMoaMmValue pValue)
{
	MOAQIPtr<IMoa3dVectorValueUtils, &IID_IMoa3dVectorValueUtils> p(this->obj_->pCallback);
	return p->Moa3dVectorToValue(pVector, pValue);
}

MoaError
MoaUtils::ValueToMoa3dVectorPtr( ConstPMoaMmValue pValue, Moa3dVector **ppVector)
{
	MOAQIPtr<IMoa3dVectorValueUtils, &IID_IMoa3dVectorValueUtils> p(this->obj_->pCallback);
	return p->ValueToMoa3dVectorPtr(pValue, ppVector);
}

MoaError
MoaUtils::ValueReleaseVectorPtr( ConstPMoaMmValue pValue, Moa3dVector **ppVector)
{
	MOAQIPtr<IMoa3dVectorValueUtils, &IID_IMoa3dVectorValueUtils> p(this->obj_->pCallback);
	return p->ValueReleaseVectorPtr(pValue, ppVector);
}

MoaError
MoaUtils::ValueIsAVector( ConstPMoaMmValue pValue)
{
	MOAQIPtr<IMoa3dVectorValueUtils, &IID_IMoa3dVectorValueUtils> p(this->obj_->pCallback);
	return p->ValueIsAVector(pValue);
}

MoaError
MoaUtils::ValueToMoa3dMatrix( ConstPMoaMmValue pValue, Moa3dMatrix pMatrix)
{
	MOAQIPtr<IMoa3dVectorValueUtils, &IID_IMoa3dVectorValueUtils> p(this->obj_->pCallback);
	return p->ValueToMoa3dMatrix(pValue, pMatrix);
}

MoaError
MoaUtils::Moa3dMatrixToValue( Moa3dMatrix pMatrix, PMoaMmValue pValue)
{
	MOAQIPtr<IMoa3dVectorValueUtils, &IID_IMoa3dVectorValueUtils> p(this->obj_->pCallback);
	return p->Moa3dMatrixToValue(pMatrix, pValue);
}

MoaError
MoaUtils::ValueIsATransform( ConstPMoaMmValue pValue)
{
	MOAQIPtr<IMoa3dVectorValueUtils, &IID_IMoa3dVectorValueUtils> p(this->obj_->pCallback);
	return p->ValueIsATransform(pValue);
}

MoaError
MoaUtils::PushXtraContext(PIMoaDrMovie movie, PDrContextState pDrContextState){
	MOAQIPtr<IMoaDrMovieContext, &IID_IMoaDrMovieContext> p(movie);
	return p->PushXtraContext(pDrContextState);
}

MoaError
MoaUtils::PopXtraContext(PIMoaDrMovie movie, PDrContextState pDrContextState){
	MOAQIPtr<IMoaDrMovieContext, &IID_IMoaDrMovieContext> p(movie);
	return p->PopXtraContext(pDrContextState);
}
MoaError
MoaUtils::ReleaseExclusiveThread(PIMoaDrMovie movie, PDrContextState pDrContextState){
	MOAQIPtr<IMoaDrMovieContext, &IID_IMoaDrMovieContext> p(movie);
	return p->ReleaseExclusiveThread(pDrContextState);
}

MoaError
MoaUtils::ReacquireExclusiveThread(PIMoaDrMovie movie, PDrContextState pDrContextState){
	MOAQIPtr<IMoaDrMovieContext, &IID_IMoaDrMovieContext> p(movie);
	return p->ReacquireExclusiveThread(pDrContextState);
}

MoaError
MoaUtils::NewImage(MoaLong iWidth, MoaLong iHeight, MoaLong iBitDepth, MoaLong iAlphaDepth, PMoaMmValue pPaletteRef, PMoaMmValue pResultImage)
{
	return image_->NewImage(iWidth, iHeight, iBitDepth, iAlphaDepth, pPaletteRef, pResultImage);
}

MoaError
MoaUtils::SetAlpha( ConstPMoaMmValue pImage, ConstPMoaMmValue pAlphaImageOrAlphaValue)	{
	return image_->SetAlpha(pImage, pAlphaImageOrAlphaValue);
}

MoaError
MoaUtils::TrimWhiteSpace( ConstPMoaMmValue pImage, PMoaMmValue pResultImage){
	return image_->TrimWhiteSpace(pImage, pResultImage);
}

MoaError
MoaUtils::CopyPixels( ConstPMoaMmValue pImage, ConstPMoaMmValue pSourceImage, ConstPMoaMmValue pDestRectOrQuad, ConstPMoaMmValue pSourceRect, ConstPMoaMmValue pPropList){
	return image_->CopyPixels(pImage, pSourceImage, pDestRectOrQuad, pSourceRect, pPropList);
}

MoaError
MoaUtils::Duplicate( ConstPMoaMmValue pImage, PMoaMmValue pResultImage){
	return image_->Duplicate(pImage, pResultImage);
}

MoaError
MoaUtils::Crop( ConstPMoaMmValue pImage, ConstPMoaMmValue pRect, PMoaMmValue pResultImage){
	return image_->Crop(pImage, pRect, pResultImage);
}

MoaError
MoaUtils::GetPixel( ConstPMoaMmValue pImage, MoaLong iX, MoaLong iY, PMoaMmValue pResultValue){
	return image_->GetPixel(pImage, iX, iY, pResultValue);
}


MoaError
MoaUtils::SetPixel( ConstPMoaMmValue pImage, MoaLong iX, MoaLong iY, ConstPMoaMmValue pPixelValue){
	return image_->SetPixel(pImage, iX, iY, pPixelValue);
}

MoaError
MoaUtils::GetPixelInteger( ConstPMoaMmValue pImage, MoaLong iX, MoaLong iY, PMoaLong piResult){
	return image_->GetPixelInteger(pImage, iX, iY, piResult);
}

MoaError
MoaUtils::SetPixelInteger( ConstPMoaMmValue pImage, MoaLong iX, MoaLong iY, MoaLong iPixelValue)	{
	return image_->SetPixelInteger(pImage, iX, iY, iPixelValue);
}

MoaError
MoaUtils::Draw( ConstPMoaMmValue pImage, ConstPMoaMmValue pRect, ConstPMoaMmValue pPropList) {
	return image_->Draw(pImage, pRect, pPropList);
}

MoaError
MoaUtils::Fill( ConstPMoaMmValue pImage, ConstPMoaMmValue pRect, ConstPMoaMmValue pPropList){
	return image_->Fill(pImage, pRect, pPropList);
}

MoaError
MoaUtils::CreateMatte( ConstPMoaMmValue pImage, PMoaMmValue pResultImage){
	return image_->CreateMatte(pImage, pResultImage);
}

MoaError
MoaUtils::CreateMask( ConstPMoaMmValue pImage, PMoaMmValue pResultImage){
	return image_->CreateMask(pImage, pResultImage);
}

MoaError
MoaUtils::ExtractAlpha( ConstPMoaMmValue pImage, PMoaMmValue pResultImage){
	return image_->ExtractAlpha(pImage, pResultImage);
}

MoaError
MoaUtils::LockPixels( ConstPMoaMmValue pImage, PMoaVoid *ppPixels){
	return image_->LockPixels(pImage, ppPixels);
}

MoaError
MoaUtils::UnlockPixels( ConstPMoaMmValue pImage){
	return image_->UnlockPixels(pImage);
}

MoaError
MoaUtils::GetImageInfo( ConstPMoaMmValue pImage, PMoaMmImageInfo pImageInfo){
	return image_->GetImageInfo(pImage, pImageInfo);
}

MoaError
MoaUtils::NewImageFromPath( PIMoaPathName path, PMoaMmValue pResultImage){
	return image_->NewImageFromPath(path, pResultImage);
}

MoaError
MoaUtils::NewImageFromStream( PIMoaStream pStream, PPMoaVoid pProgressCookie, PMoaMmValue pResultImage) {
	return image_->NewImageFromStream(pStream, pProgressCookie, pResultImage);
}

MoaError
MoaUtils::UpdateImageFromStream( PIMoaStream pStream, PMoaVoid progressCookie, PMoaMmValue pUpdateImage){
	return image_->UpdateImageFromStream(pStream, progressCookie, pUpdateImage);
}

void DWORD_TO_RGBA(DWORD color, BYTE& r, BYTE& g, BYTE& b, BYTE& a) {
	r = (BYTE)((color & 0xFF000000) >> 24);
	g = (BYTE)((color & 0x00FF0000) >> 16);
	b = (BYTE)((color & 0x0000FF00) >> 8);
	a = (BYTE)((color & 0x000000FF));
}

void RGBA_TO_DWORD(DWORD& color, BYTE r, BYTE g, BYTE b, BYTE a) {
	color = (r << 24) | (g << 16) | (b << 8) | a;
}

DWORD RGB_TO_BGR(DWORD rgb) {
	int b, g, r;
	b = rgb & 0xFF;
	g = (rgb >> 8) & 0xFF;
	r = rgb >> 16;
	return (b << 16) | (g << 8) | r;
}

DWORD BGR_TO_RGB(DWORD bgr) {
	int r, g, b;
	r = bgr & 0xFF;
	g = (bgr >> 8) & 0xFF;
	b = bgr >> 16;
	return (r << 16) | (g << 8) | b;
}

MoaError
MoaUtils::ReleaseProgressCookie( PPMoaVoid pProgressCookie){
	return image_->ReleaseProgressCookie(pProgressCookie);
}

//get_index
MoaMmValue&
MoaUtils::get_index(const MoaMmCallInfo& callPtr, MoaLong index)
{
	return callPtr.pArgs[index + 1];
}

const MoaMmValue&
MoaUtils::get_index(const GetRef& ref, MoaLong index)
{
	const MoaMmValue* pargs = &ref.args_;
	return pargs[index];
}

const MoaMmValue&
MoaUtils::get_index(const SetRef& ref, MoaLong index)
{
	const MoaMmValue* pargs = &ref.args_;
	return pargs[index];
}


//GUID
void 
MoaUtils::write(const GUID& value, MoaMmValue& mvalue){
	__GUID* ix = new __GUID(this->obj_, (const __GUID::PTR_TYPE) &value);
	xvalue_->IXValueToValue(ix, &mvalue);
	ix->Release();
}