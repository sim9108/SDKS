#pragma once

#ifdef WIN32_LEAN_AND_MEAN
#else
#define WIN32_LEAN_AND_MEAN
#endif

#include <LibXDK\moaxtra.h>
#include <LibXDK\mmiservc.h>
#include <LibXDK\mmixscrp.h>
#include <LibXDK\driservc.h>
#include <LibXDK\Mui.h>
#include <LibXDK\moafile2.h>
#include <LibXDK\MoaTry.h>
#include <LibXDK\mmiDate.h>
#include <LibXDK\mmiClr.h>
#include <LibXDK\mmipref.h>
#include <LibXDK\mivect3d.h>
#include <LibXDK\mmiimage.h>
#include <LibXDK\mmiutil.h>
#include <LibXDK\mmixval.h>
#include <LibXDK\xclassver.h>
#include <tchar.h>
#include <strsafe.h>

#include <string>
#include <codecvt>
#include <locale>

#include <vector>
#include <set>
#include <deque>
#include <stack>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#include <thread>
#include <filesystem>
#include <chrono>
#include <mutex>

#include <sstream>
#include <iomanip>
#include <iterator>
#include <fstream>
#include <algorithm>
#include <cctype>

#include <iostream>
#include <atomic>
#include <functional>

#include <limits>

#include <Winuser.h>
#include <future>

#pragma comment( linker, "/export:DllGetClassObject=_DllGetClassObject@12,PRIVATE" )
#pragma comment( linker, "/export:DllCanUnloadNow=_DllCanUnloadNow@0,PRIVATE" )
#pragma comment( linker, "/export:DllGetClassInfo=_DllGetClassInfo@8,PRIVATE" )
#pragma comment( linker, "/export:DllGetInterface=_DllGetInterface@16,PRIVATE" )
#pragma comment( linker, "/export:DllGetClassForm=_DllGetClassForm@16,PRIVATE" )

#define MACRO_STRING(x) #x
#define WINAPI_NAME(x) ""MACRO_STRING(x)""

extern MoaLong DllgXtraInterfaceCount;
extern HINSTANCE g_hInst;

template<typename FN>
class DLL_Loader
{
private:
	HMODULE module_;
	FARPROC fn_;

public:
	DLL_Loader(const TCHAR* dllname, const char* fname)
		:module_{ LoadLibrary(dllname) }, fn_{ GetProcAddress(module_, fname) }
	{

	}

	~DLL_Loader(){
		if (!module_) return;

		FreeLibrary(module_);
		module_ = nullptr;
	}

	bool operator!() const {
		return (nullptr == fn_);
	}

	operator FN*() const{
		return reinterpret_cast<FN*>(this->fn_);
	}
};

template <typename FN, typename GE, typename SE>
void HELPTEXT(std::stringstream& ss, FN& fn, GE& ge, SE& se){
	std::set<std::string> m1;
	for (auto& item : fn){
		m1.insert(item.second.first);
	}

	std::set<std::string> m2;
	for (auto& item : ge){
		m2.insert(item.second.first);
	}

	std::set<std::string> m3;
	for (auto& item : se){
		m3.insert(item.second.first);
	}

	for (auto& item : m1){
		ss << item << "\n";
	}

	for (auto& item : m2){
		ss << "+" << item<< "\n";
	}

	for (auto& item : m3){
		ss << "-" << item << "\n";
	}
}

struct GetRef{
	const MoaMmValue& self_;
	MoaMmSymbol& name_;
	MoaLong& count_;
	const MoaMmValue& args_;
	MoaMmValue& result_;
};

struct SetRef{
	MoaMmSymbol& name_;
	MoaLong& count_;
	const MoaMmValue& args_;
	const MoaMmValue& value_;
};


struct _MOAFactory : public IMoaUnknown 
{
	MoaXtraMagicCookie		magicCookie; 	/* for internal use */ 
	PIMoaCallback			pCallback; 		/* callbacks, or NULL */ 
	PIMoaCalloc 			pCalloc; 		/* IMoaCalloc we used for ourself */ 
	ConstPMoaClassID	 	pClassID; 		/* ptr to our class id */ 
	MoaLong					refCount; 		/* our refcount */ 
};


template<typename T>
struct move_on_copy
{
	move_on_copy(T&& aValue) : value(move(aValue)) {}
	move_on_copy(const move_on_copy& other) : value(move(other.value)) {}

	T& Value()
	{
		return value;
	}

	const T& Value() const
	{
		return value;
	}

private:
	mutable T value;
	move_on_copy& operator=(move_on_copy&& aValue) = delete; // not needed here
	move_on_copy& operator=(const move_on_copy& aValue) = delete; // not needed here
};

template<typename T>
move_on_copy<T> make_move_on_copy(T&& aValue)
{
	return move_on_copy<T>(move(aValue));
}

template <class T, const IID* piid = &__uuidof(T)>
class MOAQIPtr
{
private:
	T* t_;

public:
	MOAQIPtr()
		:t_(NULL)
	{
		t_ = NULL;
	}

	MOAQIPtr(T* t)
		:t_(t)
	{
		t_->AddRef();
	}

	MOAQIPtr(IMoaUnknown* t)
		:t_(NULL)
	{
		t_ = NULL;
		t->QueryInterface(piid, (void**)&t_);
	}

	~MOAQIPtr()
	{
		if (!t_) return;
		t_->Release();
		t_ = NULL;
	}

	operator IMoaUnknown*(){
		return (IMoaUnknown*)t_;
	}

	operator T*() {
		return t_;
	}

	void Release() {
		if (t_ == NULL) return;
		T* pTemp = t_;
		t_ = NULL;
		pTemp->Release();

	}

	void Attach(T* p2){
		if (t_) t_->Release();
		t_ = p2;
	}

	void CopyFrom(T* p2){
		if (t_) t_->Release();
		t_ = p2;
		t_->AddRef();
	}

	T* Detach() {
		T* pt = p;
		p = NULL;
		return pt;
	}

	bool operator!() const {
		return (t_ == NULL);
	}

	T* operator->() {
		return t_;
	}

	T** operator&() throw(){
		return &t_;
	}

	void forcereset(){
		t_ = NULL;
	}
};



std::string string_to_utf8(const std::string& str, const std::locale& loc /*= std::locale("")*/);
std::wstring string_to_wstring(const std::string& str, const std::locale& loc /*= std::locale("")*/);

std::string utf8_to_string(const std::string& utf8str, const std::locale& loc /*= std::locale("")*/);
std::wstring utf8_to_wstring(const std::string& str);

std::string wstring_to_utf8(const std::wstring& str);
std::string wstring_to_string(const std::wstring& str, const std::locale& loc /*= std::locale("")*/);

std::string wchar_to_ansi(const std::wstring& wstr, const std::locale& loc /*= std::locale("")*/);
std::wstring ansi_to_wchar(const std::string& str, const std::locale& loc /*= std::locale("")*/);

#define DSTDMETHOD(method)        COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE method
#define DSTDMETHOD_(type,method)  COM_DECLSPEC_NOTHROW type STDMETHODCALLTYPE method

class MoaUtils
{
private:
	_MOAFactory* obj_;
	MOAQIPtr<IMoaMmValue, &IID_IMoaMmValue> value_;
	MOAQIPtr<IMoaMmList, &IID_IMoaMmList> list_;
	MOAQIPtr<IMoaMmXValueUtils, &IID_IMoaMmXValueUtils> xvalue_;
	MOAQIPtr<IMoaMmColor, &IID_IMoaMmColor> color_;
	MOAQIPtr<IMoaMmUtils2, &IID_IMoaMmUtils2> utils2_;
	MOAQIPtr<IMoaMmImage, &IID_IMoaMmImage> image_;

public:
	explicit MoaUtils(_MOAFactory* obj);
	~MoaUtils(void);
	
public:
	MoaMmValue& get_index(const MoaMmCallInfo& callPtr, MoaLong index);
	const MoaMmValue& get_index(const GetRef& ref, MoaLong index);
	const MoaMmValue& get_index(const SetRef& ref, MoaLong index);

	const MoaLong get_count(const MoaMmCallInfo& callPtr){ return callPtr.nargs - 1; }
	const MoaLong get_count(const GetRef& ref){ return ref.count_; }
	const MoaLong get_count(const SetRef& ref){ return ref.count_; }

public:
	//IUnknown
	HRESULT QueryInterface(ConstPMoaInterfaceID pInterfaceID, PPMoaVoid ppvObj);
	HRESULT MoaCreateInstance(ConstPMoaClassID pClassID, ConstPMoaInterfaceID pInterfaceID, PPMoaVoid ppvObj);
	HRESULT ValueAddRef(PMoaMmValue  pValue);
	
	DSTDMETHOD_(struct IMoaCache *, MoaGetCache) (THIS);
	DSTDMETHOD_(XtraResourceCookie, MoaBeginUsingResources) ( MoaFileRef fileRef, XtraResourceCookie * pSaveCookie);
	DSTDMETHOD_(void, MoaEndUsingResources) ( MoaFileRef fileRef, XtraResourceCookie saveCookie);
			
	DSTDMETHOD(SymbolToString) ( MoaMmSymbol symbol, PMoaChar pStringBuf, MoaLong bufLen);
	DSTDMETHOD(SymbolToString) ( MoaMmSymbol& symbol, std::string& str);

	//IMoaMmUtils2 Interface	
	DSTDMETHOD(PrintMessage) (  ConstPMoaChar pMsg);
	DSTDMETHOD(PrintMessage1) (  ConstPMoaChar pMsg, MoaLong arg1);
	DSTDMETHOD(PrintMessage2) ( ConstPMoaChar pMsg, MoaLong arg1, MoaLong arg2);
	DSTDMETHOD(PrintMessage3) ( ConstPMoaChar pMsg, MoaLong arg1, MoaLong arg2, MoaLong arg3);
	DSTDMETHOD(PrintMessage4) ( ConstPMoaChar  pMsg, MoaLong arg1, MoaLong arg2, MoaLong arg3, MoaLong arg4);
	DSTDMETHOD(NewGC) (  ConstPMoaMmNativeGCInfo pNativeGCInfo, PIMoaMmGC * ppGC);
	
	//IMoaDrPlayer Interface	
	DSTDMETHOD(GetProp)( MoaMmSymbol symbol, PMoaMmValue pPropValue);
	DSTDMETHOD(SetProp)( MoaMmSymbol symbol, ConstPMoaMmValue pPropValue);
	DSTDMETHOD(GetActiveMovie)( PIMoaDrMovie * ppIMoaDrMovie);
	DSTDMETHOD(GetMovieCount)( 	MoaLong * pMovieCount);
	DSTDMETHOD(GetNthMovie)( MoaLong movieIndex, PIMoaDrMovie * ppIMoaDrMovie);
	DSTDMETHOD(GetCastMemTypeCount)( MoaLong * pTypeCount);
	DSTDMETHOD(GetNthCastMemTypeSymbol)( MoaLong typeIndex, PMoaMmSymbol pSymbol);
	DSTDMETHOD(SetGlobalVarValue)( 	MoaMmSymbol globalVarName, PMoaMmValue pValue);
	DSTDMETHOD(GetGlobalVarValue)( 	MoaMmSymbol globalVarName, PMoaMmValue pValue);
	DSTDMETHOD(GetCastMemTypeDisplayName)( 	MoaMmSymbol typeSymbol, PMoaChar pName, MoaLong maxLen);
	DSTDMETHOD(CallHandler) ( 	MoaMmSymbol name, MoaLong nArgs, PMoaMmValue pArgs, PMoaMmValue pResult);
	DSTDMETHOD(ResolveFileName)( ConstPMoaChar pFileName, PMoaChar pPathName, MoaLong maxLen);

	//IMoaMmColor
	DSTDMETHOD(RGBToIndex) (ConstPMoaMmRGBTriple  pRGBColor, PMoaMmColorIndex pIndex);
	DSTDMETHOD(IndexToRGB) (MoaMmColorIndex index, PMoaMmRGBTriple pRGBColor);

	//IMoaCalloc InterfaceValueRelease
	DSTDMETHOD_(PMoaVoid, NRAlloc) (MoaUlong cb);
	DSTDMETHOD_(void, NRFree) (PMoaVoid pv);

	//IMoaDrUtils Interface
	STDMETHOD(ValueToCMRef)(ConstPMoaMmValue pValue, PMoaDrCMRef pCMRef);
	STDMETHOD(NewMediaInfo)(MoaMmSymbol labelSymbol, MoaMmSymbol formatSymbol, PMoaVoid mediaData, MoaLong options, PMoaVoid aux, PMoaDrMediaInfo pMediaInfo);
	STDMETHOD(MediaRelease)(PMoaDrMediaInfo pMediaInfo);
	STDMETHOD(NewScoreAccess)(PIMoaDrScoreAccess * ppScore);
	STDMETHOD(RegisterNotificationClient)(PIMoaDrNotificationClient pNotificationClient, MoaDrNotifyType notifyType, PMoaVoid refCon);
	STDMETHOD(UnregisterNotificationClient)(PIMoaDrNotificationClient pNotificationClient, MoaDrNotifyType notifyType, PMoaVoid refCon);

	//IMoaDrMediaValue
	STDMETHOD(ValueToMedia)(ConstPMoaMmValue pValue, MoaHandle *phMediaData);
	STDMETHOD(MediaToValue)(const MoaHandle hMediaData, PMoaMmValue pValue);
	STDMETHOD(ValueToPicture)(ConstPMoaMmValue pValue, MoaHandle *phPictureData);
	STDMETHOD(PictureToValue)(const MoaHandle hPictureData, PMoaMmValue pValue);

	//IMoaDrValue
	STDMETHOD(CMRefToValue)(ConstPMoaDrCMRef pCMRef, PMoaMmValue pValue);
	STDMETHOD(ValueToSpriteRef)(ConstPMoaMmValue pValue, PMoaDrSpriteRef pSRef);
	STDMETHOD(SpriteRefToValue)(ConstPMoaDrSpriteRef pSRef, PMoaMmValue pValue);
	STDMETHOD(ValueToCastLibRef)(ConstPMoaMmValue pValue, PMoaDrCastLibRef pCLRef);
	STDMETHOD(CastLibRefToValue)(ConstPMoaDrCastLibRef pCLRef, PMoaMmValue pValue);
	STDMETHOD(ValueToSoundSpriteRef)(ConstPMoaMmValue pValue, PMoaDrSoundSpriteRef pSRef);
	STDMETHOD(SoundSpriteRefToValue)(ConstPMoaDrSoundSpriteRef pSRef, PMoaMmValue pValue);

	//IMoaMmPropReferenceValue
	STDMETHOD(ValueToPropReference) (ConstPMoaMmValue pValue, PIMoaMmPropertyReference *ppPropRef);
	STDMETHOD(PropReferenceToValue) (PIMoaMmPropertyReference	pPropRef, PMoaMmValue pValue);

	//IMoa3dVectorValueUtils
	STDMETHOD(ValueToMoa3dVector) (ConstPMoaMmValue pValue, Moa3dVector pVector);
	STDMETHOD(Moa3dVectorToValue) (Moa3dVector pVector, PMoaMmValue pValue);
	STDMETHOD(ValueToMoa3dVectorPtr) (ConstPMoaMmValue pValue, Moa3dVector **ppVector);
	STDMETHOD(ValueReleaseVectorPtr) (ConstPMoaMmValue pValue, Moa3dVector **ppVector);
	STDMETHOD(ValueIsAVector) (ConstPMoaMmValue pValue);
	STDMETHOD(ValueToMoa3dMatrix) (ConstPMoaMmValue pValue, Moa3dMatrix pMatrix);
	STDMETHOD(Moa3dMatrixToValue) (Moa3dMatrix pMatrix, PMoaMmValue pValue);
	STDMETHOD(ValueIsATransform) (ConstPMoaMmValue pValue);

	//IMoaDrMovieContext
	STDMETHOD(PushXtraContext) (PIMoaDrMovie movie, PDrContextState pDrContextState);
	STDMETHOD(PopXtraContext) (PIMoaDrMovie movie, PDrContextState pDrContextState);
	STDMETHOD(ReleaseExclusiveThread) (PIMoaDrMovie movie, PDrContextState pDrContextState);
	STDMETHOD(ReacquireExclusiveThread) (PIMoaDrMovie movie, PDrContextState pDrContextState);

	//IMoaMmImage
	STDMETHOD(CopyPixels) (ConstPMoaMmValue pImage, ConstPMoaMmValue pSourceImage, ConstPMoaMmValue pDestRectOrQuad, ConstPMoaMmValue pSourceRect, ConstPMoaMmValue pPropList);
	STDMETHOD(NewImage) (MoaLong iWidth, MoaLong iHeight, MoaLong iBitDepth, MoaLong iAlphaDepth, PMoaMmValue pPaletteRef, PMoaMmValue pResultImage);
	STDMETHOD(SetAlpha) (ConstPMoaMmValue pImage, ConstPMoaMmValue pAlphaImageOrAlphaValue);
	STDMETHOD(TrimWhiteSpace)	 (ConstPMoaMmValue pImage, PMoaMmValue pResultImage);
	STDMETHOD(Duplicate) (ConstPMoaMmValue pImage, PMoaMmValue pResultImage);
	STDMETHOD(Crop)  (ConstPMoaMmValue pImage, ConstPMoaMmValue pRect, PMoaMmValue pResultImage);
	STDMETHOD(GetPixel) (ConstPMoaMmValue pImage, MoaLong iX, MoaLong iY, PMoaMmValue pResultValue);
	STDMETHOD(SetPixel) (ConstPMoaMmValue pImage, MoaLong iX, MoaLong iY, ConstPMoaMmValue pPixelValue);
	STDMETHOD(GetPixelInteger)	(ConstPMoaMmValue pImage, MoaLong iX, MoaLong iY, PMoaLong piResult);
	STDMETHOD(SetPixelInteger)	(ConstPMoaMmValue pImage, MoaLong iX, MoaLong iY, MoaLong iPixelValue);
	STDMETHOD(Draw) (ConstPMoaMmValue pImage, ConstPMoaMmValue pRect, ConstPMoaMmValue pPropList);
	STDMETHOD(Fill)  (ConstPMoaMmValue pImage, ConstPMoaMmValue pRect, ConstPMoaMmValue pPropList);
	STDMETHOD(CreateMatte) (ConstPMoaMmValue pImage, PMoaMmValue pResultImage);
	STDMETHOD(CreateMask) (ConstPMoaMmValue pImage, PMoaMmValue pResultImage);
	STDMETHOD(ExtractAlpha) 	(ConstPMoaMmValue pImage, PMoaMmValue pResultImage);
	STDMETHOD(LockPixels) (ConstPMoaMmValue pImage, PMoaVoid *ppPixels);
	STDMETHOD(UnlockPixels) (ConstPMoaMmValue pImage);
	STDMETHOD(GetImageInfo) (ConstPMoaMmValue pImage, PMoaMmImageInfo pImageInfo);
	STDMETHOD(NewImageFromPath) (PIMoaPathName path, PMoaMmValue pResultImage);
	STDMETHOD(NewImageFromStream) (PIMoaStream pStream, PPMoaVoid pProgressCookie, PMoaMmValue pResultImage);
	STDMETHOD(UpdateImageFromStream) (PIMoaStream pStream, PMoaVoid progressCookie, PMoaMmValue pUpdateImage);
	STDMETHOD(ReleaseProgressCookie) (PPMoaVoid pProgressCookie);

	void ValueRelease(MoaMmValue& pValue);
	void ValueRelease(MoaMmCallInfo& callPtr);
	void ValueRelease(GetRef& ref);

	void make_list(MoaMmValue& listValue);
	void make_list(MoaMmCallInfo& callPtr);
	void make_list(GetRef& ref);
	
	void make_prop_list(MoaMmValue& value);
	void make_prop_list(MoaMmCallInfo& callPtr);
	void make_prop_list(GetRef& ref);
	
	//append
	void append(MoaMmValue& mvalue, MoaMmValue& value){
		list_->AppendValueToList(&mvalue, &value);
	}

	void append(MoaMmCallInfo& callPtr, MoaMmValue& value){
		this->append(callPtr.resultValue, value);
	}

	void append(GetRef& ref, MoaMmValue& value){
		this->append(ref.result_, value);
	}

	template <typename T>
	void append(MoaMmCallInfo& callPtr, T&& value){
		MoaMmValue v1;
		this->write(value, v1);
		list_->AppendValueToList(&callPtr.resultValue, &v1);
		this->ValueRelease(v1);
	}

	template <typename T>
	void append(GetRef& ref, T&& value){
		MoaMmValue v1;
		this->write(value, v1);
		list_->AppendValueToList(&ref.result_, &v1);
		this->ValueRelease(v1);
	}

	template <typename T>
	void append(MoaMmValue& mvalue, T&& value){
		MoaMmValue v1;
		this->write(value, v1);
		list_->AppendValueToList(&mvalue, &v1);
		this->ValueRelease(v1);
	}
			
	//append_string
	template<typename T >
	void append_string(MoaMmCallInfo& callPtr, T&& value){
		this->append_string(callPtr.resultValue, value);
	}

	template<typename T >
	void append_string(GetRef& ref, T&& value){
		this->append_string(ref.result_, value);
	}

	template<typename T >
	void append_string(MoaMmValue& mvalue, T&& value){
		MoaMmValue v1;
		this->write_string(value, v1);
		list_->AppendValueToList(&mvalue, &v1);
		this->ValueRelease(v1);
	}

	//append_symbol
	template<typename T >
	void append_symbol(MoaMmCallInfo& callPtr, T&& value){
		this->append_symbol(callPtr.resultValue, value);
	}

	template<typename T >
	void append_symbol(GetRef& ref, T&& value){
		this->append_symbol(ref.result_, value);
	}

	template<typename T >
	void append_symbol(MoaMmValue& mvalue, T&& value){
		MoaMmValue v1;
		this->append_symbol(value, v1);
		list_->AppendValueToList(&mvalue, &v1);
		this->ValueRelease(v1);
	}

	//append_color
	template<typename... T >
	void append_color(MoaMmCallInfo& callPtr, T&&... value){
		this->append_color(callPtr.resultValue, value&...);
	}

	template<typename... T >
	void append_color(GetRef& ref, T&&... value){
		this->append_color(ref.result_, value&...);
	}

	template<typename... T >
	void append_color(MoaMmValue& mvalue,T&&... value){
		MoaMmValue v1;
		this->write_color(value, v1);
		list_->AppendValueToList(&mvalue, &v1);
		this->ValueRelease(v1);
	}
				
	//append_prop_write
	template<typename STR, typename T >
	void append_prop(MoaMmCallInfo& callPtr, STR&& pElementProp, T&& value){
		this->append_prop(callPtr.resultValue, pElementProp, value);
	}

	template<typename STR, typename T >
	void append_prop(GetRef& ref, STR&& pElementProp, T&& value){
		this->append_prop(ref.result_, pElementProp, value);
	}

	void append_prop(MoaMmCallInfo& callPtr, const char* pElementProp, MoaMmValue& value){
		this->append_prop(callPtr.resultValue, pElementProp, value);
	}

	void append_prop(MoaMmCallInfo& callPtr, std::string& pElementProp, MoaMmValue& value){
		this->append_prop(callPtr.resultValue, pElementProp, value);
	}
	
	void append_prop(GetRef& ref, const char* pElementProp, MoaMmValue& value){
		this->append_prop(ref.result_, pElementProp, value);
	}

	void append_prop(GetRef& ref, std::string& pElementProp, MoaMmValue& value){
		this->append_prop(ref.result_, pElementProp, value);
	}


	void append_prop(MoaMmValue& mvalue, const char* pElementProp, MoaMmValue& value){
		MoaMmValue v2;
		this->write_symbol(pElementProp, v2);
		list_->AppendValueToPropList(&mvalue, &v2, &value);
		this->ValueRelease(v2);
	}

	void append_prop(MoaMmValue& mvalue, std::string& pElementProp, MoaMmValue& value){
		MoaMmValue v2;
		this->write_symbol(pElementProp, v2);
		list_->AppendValueToPropList(&mvalue, &v2, &value);
		this->ValueRelease(v2);
	}

	template<typename STR, typename T >
	void append_prop(MoaMmValue& mvalue, STR&& pElementProp, T&& value){
		MoaMmValue v1;
		this->write(value, v1);

		MoaMmValue v2;
		this->write_symbol(pElementProp, v2);
		list_->AppendValueToPropList(&mvalue, &v2, &v1);
		this->ValueRelease(v2);

		this->ValueRelease(v1);
	}

	//append_prop_string
	template<typename STR, typename T >
	void append_prop_string(MoaMmCallInfo& callPtr, STR&& pElementProp, T&& value){
		this->append_prop_string(callPtr.resultValue, pElementProp, value);
	}

	template<typename STR, typename T >
	void append_prop_string(GetRef& ref, STR&& pElementProp, T&& value){
		this->append_prop_string(ref.result_, pElementProp, value);
	}

	template<typename STR, typename T >
	void append_prop_string(MoaMmValue& mvalue, STR&& pElementProp, T&& value){
		MoaMmValue v1;
		this->write_string(value, v1);

		MoaMmValue v2;
		this->write_symbol(pElementProp, v2);
		list_->AppendValueToPropList(&mvalue, &v2, &v1);
		this->ValueRelease(v2);

		this->ValueRelease(v1);
	}

	//append_prop_symbol
	template<typename STR, typename T >
	void append_prop_symbol(MoaMmCallInfo& callPtr, STR&& pElementProp, T&& value){
		this->append_prop_symbol(callPtr.resultValue, pElementProp, value);
	}

	template<typename STR, typename T >
	void append_prop_symbol(GetRef& ref, STR&& pElementProp, T&& value){
		this->append_prop_symbol(ref.result_, pElementProp, value);
	}

	template<typename STR, typename T >
	void append_prop_symbol(MoaMmValue& mvalue, STR&& pElementProp, T&& value){
		MoaMmValue v1;
		this->write_symbol(value, v1);

		MoaMmValue v2;
		this->write_symbol(pElementProp, v2);
		list_->AppendValueToPropList(&mvalue, &v2, &v1);
		this->ValueRelease(v2);

		this->ValueRelease(v1);
	}
	
	//append_prop_color
	template<typename STR, typename... T >
	void append_prop_color(MoaMmCallInfo& callPtr, STR&& pElementProp, T&&... value){
		this->append_prop_color(callPtr.resultValue, pElementProp, value&...);
	}

	template<typename STR, typename... T >
	void append_prop_color(GetRef& ref, STR&& pElementProp, T&&... value){
		this->append_prop_color(ref.result_, pElementProp, value&...);
	}

	template<typename STR, typename... T >
	void append_prop_color(MoaMmValue& mvalue, STR&& pElementProp, T&&... value){
		MoaMmValue v1;
		this->write_color(value, v1);

		MoaMmValue v2;
		this->write_symbol(pElementProp, v2);
		list_->AppendValueToPropList(&mvalue, &v2, &v1);
		this->ValueRelease(v2);

		this->ValueRelease(v1);
	}
	
	
	//read_list
	template<typename T>
	bool read_list(const MoaMmCallInfo& callPtr, const int index, const int ldx, T&& value){
		if (callPtr.nargs<=index) return false;
		MoaMmValue& tempmv1 = get_index(callPtr, index);
		return this->read_list(tempmv1, ldx, value);
	}

	template<typename T>
	bool read_list(const GetRef& ref, const int index, const int ldx, T&& value){
		if (ref.count_<=index) return false;
		MoaMmValue& tempmv1 = get_index(ref, index);
		return this->read_list(tempmv1, ldx, value);
	}

	bool read_list(const MoaMmValue& ptr, const int index, MoaMmValue& value){
		HRESULT hr = this->list_->GetValueByIndex(&ptr, index+1, &value);
		if (FAILED(hr)) return false;
		return true;
	}

	template<typename T>
	bool read_list(const MoaMmValue& ptr, const int index, T&& value){
		MoaMmValue  pResultValue;
		HRESULT hr = this->list_->GetValueByIndex(&ptr, index+1, &pResultValue);	
		if (FAILED(hr)) return false;

		bool bret = this->read(pResultValue, value);
		this->ValueRelease(pResultValue);
		return bret;
	}

	//read_prop
	template<typename STR, typename T>
	bool read_prop(const MoaMmCallInfo& callPtr, const int index, STR&& pElementProp, T&& value){
		if (callPtr.nargs<=index) return false;
		MoaMmValue& tempmv1 = get_index(callPtr, index);
		return this->read_prop(tempmv1, pElementProp, value);
	}

	template<typename STR, typename T>
	bool read_prop(const SetRef& ref, STR&& pElementProp, T&& value){
		return this->read_prop(ref.value_, pElementProp, value);
	}

	template<typename STR, typename T>
	bool read_prop(const GetRef& ref, const int index, STR&& pElementProp, T&& value){
		if (ref.count_<=index) return false;
		MoaMmValue& tempmv1 = get_index(ref, index);
		return this->read_prop(tempmv1, pElementProp, value);
	}

	template<typename STR, typename T>
	bool read_prop(const MoaMmValue& ptr, STR&& pElementProp, T&& value){
		MoaMmValue v1;
		this->write_string(pElementProp, v1);

		MoaMmValue  pResultValue;
		HRESULT hr = list_->GetValueByProperty(&ptr, &v1, &pResultValue);
		this->ValueRelease(v1);
		if (FAILED(hr)) return false;

		bool bret = this->read(pResultValue, value);
		this->ValueRelease(pResultValue);
		return bret;
	}

	///

	HWND GetHWNDth(long nTh);
	HWND GetHWNDActive();

	MoaMmValueType is_type(const MoaMmValue& pValue);
	
	//write to symbol;
	MoaMmSymbol make_symbol(std::string& value);
	MoaMmSymbol make_symbol(const char* value);
	
	//write
	void write(IMoaMmXValue * value, MoaMmValue& mvalue){
		xvalue_->IXValueToValue(value, &mvalue);
	}

	void write(IMoaMmXValue * value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(IMoaMmXValue * value, GetRef& ref){
		this->write(value, ref.result_);
	}

	//GUID
	void write(const GUID& value, MoaMmValue& mvalue);
	void write(const GUID& value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const GUID& value, GetRef& ref){
		this->write(value, ref.result_);
	}


	//	
	void write(const MoaLong& value, MoaMmValue& mvalue){
		value_->IntegerToValue(value, &mvalue);
	}

	void write(const MoaLong&  value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const MoaLong&  value, GetRef& ref){
		this->write(value, ref.result_);
	}

	//
	void write(const MoaLong&& value, MoaMmValue& mvalue){
		value_->IntegerToValue(value, &mvalue);
	}

	void write(const MoaLong&&  value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const MoaLong&&  value, GetRef& ref){
		this->write(value, ref.result_);
	}

	//

	void write(const BYTE& value, MoaMmValue& mvalue){
		value_->IntegerToValue(value, &mvalue);
	}

	void write(const BYTE&  value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const BYTE&  value, GetRef& ref){
		this->write(value, ref.result_);
	}

	//

	void write(const BYTE&& value, MoaMmValue& mvalue){
		value_->IntegerToValue(value, &mvalue);
	}

	void write(const BYTE&&  value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const BYTE&&  value, GetRef& ref){
		this->write(value, ref.result_);
	}

	//

	void write(const bool& value, MoaMmValue& mvalue){
		MoaLong vfnum{ value };
		value_->IntegerToValue(vfnum, &mvalue);
	}

	void write(const bool&  value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const bool&  value, GetRef& ref){
		this->write(value, ref.result_);
	}
	//
	void write(const wchar_t*& value, MoaMmValue& mvalue){
		std::string u8 = wstring_to_utf8(value);
		value_->StringToValue(u8.c_str(), &mvalue);
	}

	void write(const wchar_t*& value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const wchar_t*& value, GetRef& ref){
		this->write(value, ref.result_);
	}

	void write(const wchar_t*&& value, MoaMmValue& mvalue){
		std::string u8 = wstring_to_utf8(value);
		value_->StringToValue(u8.c_str(), &mvalue);
	}

	void write(const wchar_t*&& value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const wchar_t*&& value, GetRef& ref){
		this->write(value, ref.result_);
	}
	//

	void write(wchar_t*& value, MoaMmValue& mvalue){
		std::string u8 = wstring_to_utf8(value);
		value_->StringToValue(u8.c_str(), &mvalue);
	}

	void write(wchar_t*& value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(wchar_t*& value, GetRef& ref){
		this->write(value, ref.result_);
	}

	void write(wchar_t*&& value, MoaMmValue& mvalue){
		std::string u8 = wstring_to_utf8(value);
		value_->StringToValue(u8.c_str(), &mvalue);
	}

	void write(wchar_t*&& value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(wchar_t*&& value, GetRef& ref){
		this->write(value, ref.result_);
	}

	//

	void write(const char*& value, MoaMmValue& mvalue){
		value_->StringToValue(value, &mvalue);
	}

	void write(const char*& value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const char*& value, GetRef& ref){
		this->write(value, ref.result_);
	}

	void write(const char*&& value, MoaMmValue& mvalue){
		value_->StringToValue(value, &mvalue);
	}

	void write(const char*&& value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const char*&& value, GetRef& ref){
		this->write(value, ref.result_);
	}
	//

	void write(const PMoaChar& value, MoaMmValue& mvalue){
		value_->StringToValue(value, &mvalue);
	}

	void write(const PMoaChar& value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const PMoaChar& value, GetRef& ref){
		this->write(value, ref.result_);
	}

	void write(const PMoaChar&& value, MoaMmValue& mvalue){
		value_->StringToValue(value, &mvalue);
	}

	void write(const PMoaChar&& value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const PMoaChar&& value, GetRef& ref){
		this->write(value, ref.result_);
	}
	//
	void write(PMoaChar& value, MoaMmValue& mvalue){
		value_->StringToValue(value, &mvalue);
	}

	void write(PMoaChar& value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(PMoaChar& value, GetRef& ref){
		this->write(value, ref.result_);
	}

	void write(PMoaChar&& value, MoaMmValue& mvalue){
		value_->StringToValue(value, &mvalue);
	}

	void write(PMoaChar&& value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(PMoaChar&& value, GetRef& ref){
		this->write(value, ref.result_);
	}
	//

	//

	void write(const bool&& value, MoaMmValue& mvalue){
		MoaLong vfnum{ value };
		value_->IntegerToValue(vfnum, &mvalue);
	}

	void write(const bool&& value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const bool&&  value, GetRef& ref){
		this->write(value, ref.result_);
	}

	//

	void write(const int& value, MoaMmValue& mvalue){
		MoaLong vfnum{ value };
		value_->IntegerToValue(vfnum, &mvalue);
	}

	void write(const int&  value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const int& value, GetRef& ref){
		this->write(value, ref.result_);
	}

	//

	void write(const int&& value, MoaMmValue& mvalue){
		MoaLong vfnum{ value };
		value_->IntegerToValue(vfnum, &mvalue);
	}

	void write(const int&&  value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const int&& value, GetRef& ref){
		this->write(value, ref.result_);
	}

	//

	void write(const std::size_t& value, MoaMmValue& mvalue){
		MoaLong vfnum =static_cast<MoaLong>(value);
		value_->IntegerToValue(vfnum, &mvalue);
	}

	void write(const std::size_t&  value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const std::size_t& value, GetRef& ref){
		this->write(value, ref.result_);
	}

	//

	void write(const std::size_t&& value, MoaMmValue& mvalue){
		MoaLong vfnum = static_cast<MoaLong>(value);
		value_->IntegerToValue(vfnum, &mvalue);
	}

	void write(const std::size_t&&  value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const std::size_t&& value, GetRef& ref){
		this->write(value, ref.result_);
	}

	//

	void write(const ULONG& value, MoaMmValue& mvalue){
		MoaLong vfnum = static_cast<MoaLong>(value);
		value_->IntegerToValue(vfnum, &mvalue);
	}
	void write(const ULONG&  value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const ULONG& value, GetRef& ref){
		this->write(value, ref.result_);
	}

	//

	void write(const ULONG&& value, MoaMmValue& mvalue){
		MoaLong vfnum = static_cast<MoaLong>(value);
		value_->IntegerToValue(vfnum, &mvalue);
	}

	void write(const ULONG&&  value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const ULONG&& value, GetRef& ref){
		this->write(value, ref.result_);
	}

	//

	void write(const MoaDouble& value, MoaMmValue& mvalue){
		value_->FloatToValue(value, &mvalue);
	}

	void write(const MoaDouble&  value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const MoaDouble& value, GetRef& ref){
		this->write(value, ref.result_);
	}

	//

	void write(const LONGLONG& value, MoaMmValue& mvalue){
		MoaDouble vfnum = static_cast<MoaDouble>(value);
		value_->FloatToValue(vfnum, &mvalue);
	}

	void write(const LONGLONG&  value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const LONGLONG& value, GetRef& ref){
		this->write(value, ref.result_);
	}

	//

	void write(const LONGLONG&& value, MoaMmValue& mvalue){
		MoaDouble vfnum = static_cast<MoaDouble>(value);
		value_->FloatToValue(vfnum, &mvalue);
	}

	void write(const LONGLONG&&  value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const LONGLONG&& value, GetRef& ref){
		this->write(value, ref.result_);
	}

	//

	void write(const ULONGLONG& value, MoaMmValue& mvalue){
		MoaDouble vfnum = static_cast<MoaDouble>(value);
		value_->FloatToValue(vfnum, &mvalue);
	}

	void write(const ULONGLONG&  value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const ULONGLONG& value, GetRef& ref){
		this->write(value, ref.result_);
	}

	//

	void write(const ULONGLONG&& value, MoaMmValue& mvalue){
		MoaDouble vfnum = static_cast<MoaDouble>(value);
		value_->FloatToValue(vfnum, &mvalue);
	}

	void write(const ULONGLONG&&  value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const ULONGLONG&& value, GetRef& ref){
		this->write(value, ref.result_);
	}

	//

	void write(const ULARGE_INTEGER& value, MoaMmValue& mvalue){
		MoaDouble vfnum = static_cast<MoaDouble>(value.QuadPart);
		value_->FloatToValue(vfnum, &mvalue);
	}

	void write(const ULARGE_INTEGER&  value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const ULARGE_INTEGER& value, GetRef& ref){
		this->write(value, ref.result_);
	}

	//

	void write(const ULARGE_INTEGER&& value, MoaMmValue& mvalue){
		MoaDouble vfnum = static_cast<MoaDouble>(value.QuadPart);
		value_->FloatToValue(vfnum, &mvalue);
	}

	void write(const ULARGE_INTEGER&&  value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const ULARGE_INTEGER&& value, GetRef& ref){
		this->write(value, ref.result_);
	}

	//

	void write(const LARGE_INTEGER& value, MoaMmValue& mvalue){
		MoaDouble vfnum = static_cast<MoaDouble>(value.QuadPart);
		value_->FloatToValue(vfnum, &mvalue);
	}

	void write(const LARGE_INTEGER&  value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const LARGE_INTEGER& value, GetRef& ref){
		this->write(value, ref.result_);
	}

	//

	void write(const LARGE_INTEGER&& value, MoaMmValue& mvalue){
		MoaDouble vfnum = static_cast<MoaDouble>(value.QuadPart);
		value_->FloatToValue(vfnum, &mvalue);
	}

	void write(const LARGE_INTEGER&&  value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const LARGE_INTEGER&& value, GetRef& ref){
		this->write(value, ref.result_);
	}

	//

	void write(const RECT& value, MoaMmValue& mvalue){
		MoaRect mrect;
		mrect.left = value.left;
		mrect.right = value.right;
		mrect.bottom = value.bottom;
		mrect.top = value.top;
		value_->RectToValue(&mrect, &mvalue);
	}

	void write(const RECT&  value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const RECT& value, GetRef& ref){
		this->write(value, ref.result_);
	}

	//

	void write(const RECT&& value, MoaMmValue& mvalue){
		MoaRect mrect;
		mrect.left = value.left;
		mrect.right = value.right;
		mrect.bottom = value.bottom;
		mrect.top = value.top;
		value_->RectToValue(&mrect, &mvalue);
	}

	void write(const RECT&& value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const RECT&& value, GetRef& ref){
		this->write(value, ref.result_);
	}

	//

	void write(const MoaRect& value, MoaMmValue& mvalue){
		value_->RectToValue(&value, &mvalue);
	}

	void write(const MoaRect& value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const MoaRect& value, GetRef& ref){
		this->write(value, ref.result_);
	}

	//

	void write(const MoaRect&& value, MoaMmValue& mvalue){
		value_->RectToValue(&value, &mvalue);
	}
	
	void write(const MoaRect&& value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const MoaRect&& value, GetRef& ref){
		this->write(value, ref.result_);
	}

	//

	void write(const POINT& value, MoaMmValue& mvalue){
		MoaPoint mpoint;
		mpoint.x = value.x;
		mpoint.y = value.y;
		value_->PointToValue(&mpoint, &mvalue);
	}

	void write(const POINT& value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const POINT& value, GetRef& ref){
		this->write(value, ref.result_);
	}

	//

	void write(const POINT&& value, MoaMmValue& mvalue){
		MoaPoint mpoint;
		mpoint.x = value.x;
		mpoint.y = value.y;
		value_->PointToValue(&mpoint, &mvalue);
	}

	void write(const POINT&& value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const POINT&& value, GetRef& ref){
		this->write(value, ref.result_);
	}

	//

	void write(const MoaPoint& value, MoaMmValue& mvalue){
		value_->PointToValue(&value, &mvalue);
	}

	void write(const MoaPoint& value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const MoaPoint& value, GetRef& ref){
		this->write(value, ref.result_);
	}

	//

	void write(const MoaPoint&& value, MoaMmValue& mvalue){
		value_->PointToValue(&value, &mvalue);
	}

	void write(const MoaPoint&& value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const MoaPoint&& value, GetRef& ref){
		this->write(value, ref.result_);
	}

	//

	void write(const MoaMmColorSpec& value, MoaMmValue& mvalue){
		color_->ColorToValue(&value, &mvalue);
	}

	void write(const MoaMmColorSpec& value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const MoaMmColorSpec& value, GetRef& ref){
		this->write(value, ref.result_);
	}

	//
	void write(const MoaMmColorSpec&& value, MoaMmValue& mvalue){
		color_->ColorToValue(&value, &mvalue);
	}

	void write(const MoaMmColorSpec&& value, MoaMmCallInfo& callPtr){
		this->write(value, callPtr.resultValue);
	}

	void write(const MoaMmColorSpec&& value, GetRef& ref){
		this->write(value, ref.result_);
	}
	
	//write_symbol
	void write_symbol(std::string& value, MoaMmValue& mvalue){
		MoaMmSymbol s1;
		value_->StringToSymbol(value.c_str(), &s1);
		value_->SymbolToValue(s1, &mvalue);
	}

	void write_symbol(std::string& value, MoaMmCallInfo& callPtr){
		this->write_symbol(value, callPtr.resultValue);
	}

	void write_symbol(std::string& value, GetRef& ref){
		this->write_symbol(value, ref.result_);
	}

	void write_symbol(std::stringstream& value, GetRef& ref){
		this->write_symbol(value.str(), ref.result_);
	}
	////
	void write_symbol(const  std::string& value, MoaMmValue& mvalue){
		MoaMmSymbol s1;
		value_->StringToSymbol(value.c_str(), &s1);
		value_->SymbolToValue(s1, &mvalue);
	}

	void write_symbol(const  std::string& value, MoaMmCallInfo& callPtr){
		this->write_symbol(value, callPtr.resultValue);
	}

	void write_symbol(const  std::string& value, GetRef& ref){
		this->write_symbol(value, ref.result_);
	}

	void write_symbol(const std::stringstream& value, GetRef& ref){
		this->write_symbol(value.str(), ref.result_);
	}

	//

	void write_symbol(std::string&& value, MoaMmValue& mvalue){
		MoaMmSymbol s1;
		value_->StringToSymbol(value.c_str(), &s1);
		value_->SymbolToValue(s1, &mvalue);
	}

	void write_symbol(std::string&& value, MoaMmCallInfo& callPtr){
		this->write_symbol(value, callPtr.resultValue);
	}

	void write_symbol(std::string&& value, GetRef& ref){
		this->write_symbol(value, ref.result_);
	}

	void write_symbol(std::stringstream&& value, GetRef& ref){
		this->write_symbol(value.str(), ref.result_);
	}
	//

	void write_symbol(const char*& value, MoaMmValue& mvalue){
		MoaMmSymbol s1;
		value_->StringToSymbol(value, &s1);
		value_->SymbolToValue(s1, &mvalue);
	}
		
	void write_symbol(const char*& value, MoaMmCallInfo& callPtr){
		this->write_symbol(value, callPtr.resultValue);
	}

	void write_symbol(const char*& value, GetRef& ref){
		this->write_symbol(value, ref.result_);
	}
	//

	//write_color

	void write_color(const COLORREF& color, MoaMmValue& mvalue){
		MoaMmColorSpec mcolor;
		mcolor.type = kMoaMmColorSpec_RGB;
		mcolor.color.rgb.red = GetRValue(color);
		mcolor.color.rgb.green = GetGValue(color);
		mcolor.color.rgb.blue = GetBValue(color);
		color_->ColorToValue(&mcolor, &mvalue);
	}

	void write_color(const COLORREF& value, MoaMmCallInfo& callPtr){
		this->write_color(value, callPtr.resultValue);
	}

	void write_color(const COLORREF& value, GetRef& ref){
		this->write_color(value, ref.result_);
	}
	//
	
	void write_color(const long& color, MoaMmValue& mvalue){
		MoaMmColorSpec mcolor;
		mcolor.type = kMoaMmColorSpec_RGB;
		mcolor.color.rgb.red = GetRValue(color);
		mcolor.color.rgb.green = GetGValue(color);
		mcolor.color.rgb.blue = GetBValue(color);
		color_->ColorToValue(&mcolor, &mvalue);
	}

	void write_color(const long& value, MoaMmCallInfo& callPtr){
		this->write_color(value, callPtr.resultValue);
	}

	void write_color(const long& value, GetRef& ref){
		this->write_color(value, ref.result_);
	}
	//


	void write_color(const BYTE r, const BYTE g, const BYTE b, MoaMmValue& mvalue){
		MoaMmColorSpec color;
		color.type = kMoaMmColorSpec_RGB;
		color.color.rgb.red = r;
		color.color.rgb.green = g;
		color.color.rgb.blue = b;
		color_->ColorToValue(&color, &mvalue);
	}

	void write_color(const BYTE r, const BYTE g, const BYTE b, MoaMmCallInfo& callPtr){
		this->write_color(r,g,b, callPtr.resultValue);
	}

	void write_color(const BYTE r, const BYTE g, const BYTE b, GetRef& ref){
		this->write_color(r, g, b, ref.result_);
	}
	//
		

	//write_string
	void write_string(const PMoaChar& value, MoaMmValue& mvalue){
		value_->StringToValue(value, &mvalue);
	}

	void write_string(const PMoaChar& value, MoaMmCallInfo& callPtr){
		this->write_string(value, callPtr.resultValue);
	}

	void write_string(const PMoaChar& value, GetRef& ref){
		this->write_string(value, ref.result_);
	}
	//

	void write_string(const wchar_t*& value, MoaMmValue& mvalue){
		std::string u8 = wstring_to_utf8(value);
		value_->StringToValue(u8.c_str(), &mvalue);
	}

	void write_string(const wchar_t*& value, MoaMmCallInfo& callPtr){
		this->write_string(value, callPtr.resultValue);
	}

	void write_string(const wchar_t*& value, GetRef& ref){
		this->write_string(value, ref.result_);
	}
	//

	void write_string(const std::string& value, MoaMmValue& mvalue){
		value_->StringToValue(value.c_str(), &mvalue);
	}

	void write_string(const std::string& value, MoaMmCallInfo& callPtr){
		value_->StringToValue(value.c_str(), &callPtr.resultValue);
	}

	void write_string(const std::string&  value, GetRef& ref){
		value_->StringToValue(value.c_str(), &ref.result_);
	}
	//
	void write(const std::string& value, MoaMmValue& mvalue){
		value_->StringToValue(value.c_str(), &mvalue);
	}

	void write(const std::string& value, MoaMmCallInfo& callPtr){
		value_->StringToValue(value.c_str(), &callPtr.resultValue);
	}

	void write(const std::string&  value, GetRef& ref){
		value_->StringToValue(value.c_str(), &ref.result_);
	}


	//

	void write_string(const char& value, MoaMmValue& mvalue){
		std::string str;
		str.push_back(value);
		value_->StringToValue(str.c_str(), &mvalue);
	}

	void write_string(const char&  value, MoaMmCallInfo& callPtr){
		this->write_string(value, callPtr.resultValue);
	}

	void write_string(const char&   value, GetRef& ref){
		this->write_string(value, ref.result_);
	}


	//

	void write_string(const _GUID& value, MoaMmValue& mvalue){
		LPOLESTR wszCLSID{ nullptr };
		StringFromCLSID(value, &wszCLSID);
		std::string u8 = wstring_to_utf8(wszCLSID);
		value_->StringToValue(u8.c_str(), &mvalue);
		CoTaskMemFree(wszCLSID);
	}

	void write_string(const _GUID& value, MoaMmCallInfo& callPtr){
		this->write_string(value, callPtr.resultValue);
	}

	void write_string(const _GUID& value, GetRef& ref){
		this->write_string(value, ref.result_);
	}
	/*

	void write(const _GUID& value, MoaMmValue& mvalue){
		LPOLESTR wszCLSID{ nullptr };
		StringFromCLSID(value, &wszCLSID);
		std::string u8 = wstring_to_utf8(wszCLSID);
		value_->StringToValue(u8.c_str(), &mvalue);
		CoTaskMemFree(wszCLSID);
	}

	void write(const _GUID& value, MoaMmCallInfo& callPtr){
		this->write_string(value, callPtr.resultValue);
	}

	void write(const _GUID& value, GetRef& ref){
		this->write_string(value, ref.result_);
	}
	*/

	void write_string(const wchar_t& value, MoaMmValue& mvalue){
		std::wstring str;
		str.push_back(value);
		std::string u8 = wstring_to_utf8(str);
		value_->StringToValue(u8.c_str(), &mvalue);
	}

	void write_string(const wchar_t& value, MoaMmCallInfo& callPtr){
		this->write_string(value, callPtr.resultValue);
	}

	void write_string(const wchar_t& value, GetRef& ref){
		this->write_string(value, ref.result_);
	}
	//

	void write_string(const std::wstring& value, MoaMmValue& mvalue){
		std::string u8 = wstring_to_utf8(value);
		value_->StringToValue(u8.c_str(), &mvalue);
	}

	void write_string(const std::wstring& value, MoaMmCallInfo& callPtr){
		this->write_string(value, callPtr.resultValue);
	}

	void write_string(const std::wstring& value, GetRef& ref){
		this->write_string(value, ref.result_);
	}
	//


	void write(const std::wstring& value, MoaMmValue& mvalue){
		std::string u8 = wstring_to_utf8(value);
		value_->StringToValue(u8.c_str(), &mvalue);
	}

	void write(const std::wstring& value, MoaMmCallInfo& callPtr){
		this->write_string(value, callPtr.resultValue);
	}

	void write(const std::wstring& value, GetRef& ref){
		this->write_string(value, ref.result_);
	}
	//

	void write_string(const std::stringstream& value, MoaMmValue& mvalue){
		value_->StringToValue(value.str().c_str(), &mvalue);
	}

	void write_string(const std::stringstream& value, MoaMmCallInfo& callPtr){
		this->write_string(value, callPtr.resultValue);
	}

	void write_string(const std::stringstream& value, GetRef& ref){
		this->write_string(value, ref.result_);
	}


	//
	void write(const std::stringstream& value, MoaMmValue& mvalue){
		value_->StringToValue(value.str().c_str(), &mvalue);
	}

	void write(const std::stringstream& value, MoaMmCallInfo& callPtr){
		this->write_string(value, callPtr.resultValue);
	}

	void write(const std::stringstream& value, GetRef& ref){
		this->write_string(value, ref.result_);
	}

	//

	void write_string(const std::wstringstream& value, MoaMmValue& mvalue){
		std::string u8 = wstring_to_utf8(value.str());
		value_->StringToValue(u8.c_str(), &mvalue);
	}

	void write_string(const std::wstringstream& value, MoaMmCallInfo& callPtr){
		this->write_string(value, callPtr.resultValue);
	}

	void write_string(const std::wstringstream& value, GetRef& ref){
		this->write_string(value, ref.result_);
	}
	//

	void write(const std::wstringstream& value, MoaMmValue& mvalue){
		std::string u8 = wstring_to_utf8(value.str());
		value_->StringToValue(u8.c_str(), &mvalue);
	}

	void write(const std::wstringstream& value, MoaMmCallInfo& callPtr){
		this->write_string(value, callPtr.resultValue);
	}

	void write(const std::wstringstream& value, GetRef& ref){
		this->write_string(value, ref.result_);
	}
	//

	void write_string(const MoaHandle& value, MoaMmValue& mvalue){
		value_->StringHandleToValue(value, &mvalue);
	}


	void write_string(const MoaHandle& value, MoaMmCallInfo& callPtr){
		this->write_string(value, callPtr.resultValue);
	}

	void write_string(const MoaHandle& value, GetRef& ref){
		this->write_string(value, ref.result_);
	}
	//
	

	//read
	bool read(const MoaMmValue& mValue, MoaLong& value){
		MoaMmValueType type = this->is_type(mValue);
		if (kMoaMmValueType_Integer != type) return false;
		value_->ValueToInteger(&mValue, &value);
		return true;
	}

	bool read(const GetRef& ref, const int index, MoaLong& value){
		if (ref.count_<=index) return false;
		const MoaMmValue& mvalue = this->get_index(ref, index);
		return this->read(mvalue, value);
	}

	bool read(const MoaMmCallInfo& callPtr, const int index, MoaLong& value){
		if (callPtr.nargs<=index) return false;
		const MoaMmValue& mvalue = this->get_index(callPtr, index);
		return this->read(mvalue, value);
	}

	bool read(const SetRef& ref, const int index, MoaLong& value){
		if (ref.count_ <= index) return false;
		const MoaMmValue& mvalue = this->get_index(ref, index);
		return this->read(mvalue, value);
	}

	bool read(const SetRef& ref, MoaLong& value){
		return this->read(ref.value_, value);
	}

	/*****/
	MoaMmValueType is_type(const MoaMmCallInfo& callPtr, const int index){
		if (callPtr.nargs <= index) return kMoaMmValueType_Void;
		const MoaMmValue& mvalue = this->get_index(callPtr, index);
		return this->is_type(mvalue);
	}

	MoaMmValueType is_type(const GetRef& ref, const int index){
		if (ref.count_ <= index) return kMoaMmValueType_Void;
		const MoaMmValue& mvalue = this->get_index(ref, index);
		return this->is_type(mvalue);
	}

	//		
	bool read(const MoaMmValue& mValue, int& value){
		MoaMmValueType type = this->is_type(mValue);
		if (kMoaMmValueType_Integer != type) return false;
		MoaLong temp{};
		value_->ValueToInteger(&mValue, &temp);
		value = temp;
		return true;
	}

	bool read(const GetRef& ref, const int index, int& value){
		if (ref.count_<=index) return false;
		const MoaMmValue& mvalue = this->get_index(ref, index);
		return this->read(mvalue, value);
	}

	bool read(const MoaMmCallInfo& callPtr, const int index, int& value){
		if (callPtr.nargs<=index) return false;
		const MoaMmValue& mvalue = this->get_index(callPtr, index);
		return this->read(mvalue, value);
	}

	bool read(const SetRef& ref, int& value){
		return this->read(ref.value_, value);
	}
	//	

	bool read(const MoaMmValue& mValue, MoaUlong& value){
		MoaMmValueType type = this->is_type(mValue);
		if (kMoaMmValueType_Integer != type) return false;
		MoaLong temp{};
		value_->ValueToInteger(&mValue, &temp);
		value = (temp > 0 ? temp : 0);
		return true;
	}

	bool read(const GetRef& ref, const int index, MoaUlong& value){
		if (ref.count_<=index) return false;
		const MoaMmValue& mvalue = this->get_index(ref, index);
		return this->read(mvalue, value);
	}

	bool read(const MoaMmCallInfo& callPtr, const int index, MoaUlong& value){
		if (callPtr.nargs<=index) return false;
		const MoaMmValue& mvalue = this->get_index(callPtr, index);
		return this->read(mvalue, value);
	}

	bool read(const SetRef& ref, MoaUlong& value){
		return this->read(ref.value_, value);
	}
	//

	bool read(const MoaMmValue& mValue, bool& value){
		MoaMmValueType type = this->is_type(mValue);
		if (kMoaMmValueType_Integer != type) return false;
		MoaLong temp{};
		value_->ValueToInteger(&mValue, &temp);
		value = (temp ? true : false);
		return true;
	}

	bool read(const GetRef& ref, const int index, bool& value){
		if (ref.count_<=index) return false;
		const MoaMmValue& mvalue = this->get_index(ref, index);
		return this->read(mvalue, value);
	}

	bool read(const MoaMmCallInfo& callPtr, const int index, bool& value){
		if (callPtr.nargs<=index) return false;
		const MoaMmValue& mvalue = this->get_index(callPtr, index);
		return this->read(mvalue, value);
	}

	bool read(const SetRef& ref, bool& value){
		return this->read(ref.value_, value);
	}
	//

	bool read(const MoaMmValue& mValue, MoaDouble& value){
		MoaMmValueType type = this->is_type(mValue);
		if (kMoaMmValueType_Integer != type && kMoaMmValueType_Float != type) return false;
		value_->ValueToFloat(&mValue, &value);
		return true;
	}

	bool read(const MoaMmValue& mValue, float& value){
		MoaMmValueType type = this->is_type(mValue);
		if (kMoaMmValueType_Integer != type && kMoaMmValueType_Float != type) return false;
		MoaDouble temp;
		value_->ValueToFloat(&mValue, &temp);
		value = static_cast<float>(temp);
		return true;
	}

	bool read(const GetRef& ref, const int index, MoaDouble& value){
		if (ref.count_<=index) return false;
		const MoaMmValue& mvalue = this->get_index(ref, index);
		return this->read(mvalue, value);
	}

	bool read(const GetRef& ref, const int index, float& value){
		if (ref.count_ <= index) return false;
		const MoaMmValue& mvalue = this->get_index(ref, index);
		return this->read(mvalue, value);
	}

	bool read(const MoaMmCallInfo& callPtr, const int index, MoaDouble& value){
		if (callPtr.nargs<=index) return false;
		const MoaMmValue& mvalue = this->get_index(callPtr, index);
		return this->read(mvalue, value);
	}

	bool read(const MoaMmCallInfo& callPtr, const int index, float& value){
		if (callPtr.nargs <= index) return false;
		const MoaMmValue& mvalue = this->get_index(callPtr, index);
		return this->read(mvalue, value);
	}

	bool read(const SetRef& ref, MoaDouble& value){
		return this->read(ref.value_, value);
	}

	bool read(const SetRef& ref, float& value){
		return this->read(ref.value_, value);
	}
	//

	bool read(const MoaMmValue& mValue, LONGLONG& value){
		MoaMmValueType type = this->is_type(mValue);
		if (kMoaMmValueType_Integer != type && kMoaMmValueType_Float != type) return false;
		MoaDouble temp{};
		value_->ValueToFloat(&mValue, &temp);
		value = static_cast<LONGLONG>(temp);
		return true;
	}

	bool read(const GetRef& ref, const int index, LONGLONG& value){
		if (ref.count_<=index) return false;
		const MoaMmValue& mvalue = this->get_index(ref, index);
		return this->read(mvalue, value);
	}

	bool read(const MoaMmCallInfo& callPtr, const int index, LONGLONG& value){
		if (callPtr.nargs<=index) return false;
		const MoaMmValue& mvalue = this->get_index(callPtr, index);
		return this->read(mvalue, value);
	}

	bool read(const SetRef& ref, LONGLONG& value){
		return this->read(ref.value_, value);
	}
	//

	bool read(const MoaMmValue& mValue, ULONGLONG& value){
		MoaMmValueType type = this->is_type(mValue);
		if (kMoaMmValueType_Integer != type && kMoaMmValueType_Float != type) return false;
		MoaDouble temp{};
		value_->ValueToFloat(&mValue, &temp);
		value = static_cast<ULONGLONG>(temp);
		return true;
	}

	bool read(const GetRef& ref, const int index, ULONGLONG& value){
		if (ref.count_<=index) return false;
		const MoaMmValue& mvalue = this->get_index(ref, index);
		return this->read(mvalue, value);
	}

	bool read(const MoaMmCallInfo& callPtr, const int index, ULONGLONG& value){
		if (callPtr.nargs<=index) return false;
		const MoaMmValue& mvalue = this->get_index(callPtr, index);
		return this->read(mvalue, value);
	}

	bool read(const SetRef& ref, ULONGLONG& value){
		return this->read(ref.value_, value);
	}
	//

	bool read(const MoaMmValue& mValue, ULARGE_INTEGER& value){
		return this->read(mValue, value.QuadPart);
	}

	bool read(const GetRef& ref, const int index, ULARGE_INTEGER& value){
		if (ref.count_<=index) return false;
		const MoaMmValue& mvalue = this->get_index(ref, index);
		return this->read(mvalue, value.QuadPart);
	}

	bool read(const MoaMmCallInfo& callPtr, const int index, ULARGE_INTEGER& value){
		if (callPtr.nargs<=index) return false;
		const MoaMmValue& mvalue = this->get_index(callPtr, index);
		return this->read(mvalue, value.QuadPart);
	}

	bool read(const SetRef& ref, ULARGE_INTEGER& value){
		return this->read(ref.value_, value.QuadPart);
	}
	//

	bool read(const MoaMmValue& mValue, LARGE_INTEGER& value){
		return this->read(mValue, value.QuadPart);
	}

	bool read(const GetRef& ref, const int index, LARGE_INTEGER& value){
		if (ref.count_<=index) return false;
		const MoaMmValue& mvalue = this->get_index(ref, index);
		return this->read(mvalue, value.QuadPart);
	}

	bool read(const MoaMmCallInfo& callPtr, const int index, LARGE_INTEGER& value){
		if (callPtr.nargs<=index) return false;
		const MoaMmValue& mvalue = this->get_index(callPtr, index);
		return this->read(mvalue, value.QuadPart);
	}

	bool read(const SetRef& ref, LARGE_INTEGER& value){
		return this->read(ref.value_, value.QuadPart);
	}
	//

	bool read(const MoaMmValue& mValue, RECT& value){
		MoaMmValueType type = this->is_type(mValue);
		if (kMoaMmValueType_Rect != type) return false;

		MoaRect mrect{};
		value_->ValueToRect(&mValue, &mrect);
		value.top = mrect.top;
		value.bottom = mrect.bottom;
		value.right = mrect.right;
		value.left = mrect.left;
		return true;
	}

	bool read(const GetRef& ref, const int index, RECT& value){
		if (ref.count_<=index) return false;
		const MoaMmValue& mvalue = this->get_index(ref, index);
		return this->read(mvalue, value);
	}

	bool read(const MoaMmCallInfo& callPtr, const int index, RECT& value){
		if (callPtr.nargs<=index) return false;
		const MoaMmValue& mvalue = this->get_index(callPtr, index);
		return this->read(mvalue, value);
	}

	bool read(const SetRef& ref, RECT& value){
		return this->read(ref.value_, value);
	}
	//

	bool read(const MoaMmValue& mValue, MoaRect& value){
		MoaMmValueType type = this->is_type(mValue);
		if (kMoaMmValueType_Rect != type) return false;
		value_->ValueToRect(&mValue, &value);
		return true;
	}

	bool read(const GetRef& ref, const int index, MoaRect& value){
		if (ref.count_<=index) return false;
		const MoaMmValue& mvalue = this->get_index(ref, index);
		return this->read(mvalue, value);
	}

	bool read(const MoaMmCallInfo& callPtr, const int index, MoaRect& value){
		if (callPtr.nargs<=index) return false;
		const MoaMmValue& mvalue = this->get_index(callPtr, index);
		return this->read(mvalue, value);
	}

	bool read(const SetRef& ref, MoaRect& value){
		return this->read(ref.value_, value);
	}
	//

	bool read(const MoaMmValue& mValue, POINT& value){
		MoaMmValueType type = this->is_type(mValue);
		if (kMoaMmValueType_Point != type) return false;
		MoaPoint mPoint{};
		value_->ValueToPoint(&mValue, &mPoint);
		value.x = mPoint.x;
		value.y = mPoint.y;
		return true;
	}

	bool read(const GetRef& ref, const int index, POINT& value){
		if (ref.count_<=index) return false;
		const MoaMmValue& mvalue = this->get_index(ref, index);
		return this->read(mvalue, value);
	}

	bool read(const MoaMmCallInfo& callPtr, const int index, POINT& value){
		if (callPtr.nargs<=index) return false;
		const MoaMmValue& mvalue = this->get_index(callPtr, index);
		return this->read(mvalue, value);
	}

	bool read(const SetRef& ref, POINT& value){
		return this->read(ref.value_, value);
	}
	//

	bool read(const MoaMmValue& mValue, MoaPoint& value){
		MoaMmValueType type = this->is_type(mValue);
		if (kMoaMmValueType_Point != type) return false;
		value_->ValueToPoint(&mValue, &value);
		return true;
	}

	bool read(const GetRef& ref, const int index, MoaPoint& value){
		if (ref.count_<=index) return false;
		const MoaMmValue& mvalue = this->get_index(ref, index);
		return this->read(mvalue, value);
	}

	bool read(const MoaMmCallInfo& callPtr, const int index, MoaPoint& value){
		if (callPtr.nargs<=index) return false;
		const MoaMmValue& mvalue = this->get_index(callPtr, index);
		return this->read(mvalue, value);
	}

	bool read(const SetRef& ref, MoaPoint& value){
		return this->read(ref.value_, value);
	}
	//

	bool read(const MoaMmValue& mValue, std::wstring& value){
		MoaMmValueType type = this->is_type(mValue);
		if (kMoaMmValueType_String != type) return false;

		MoaLong size{ 0 };
		const char* raw = nullptr;

		value_->ValueStringLength(&mValue, &size);
		value_->ValueToStringPtr(&mValue, &raw);
		value_->ValueReleaseStringPtr(&mValue);

		std::wstring m = utf8_to_wstring(std::string{ raw, static_cast<unsigned int>(size) });

		value.~basic_string();
		new(&value) std::wstring{ std::move(m) };
		return true;
	}

	bool read(const GetRef& ref, const int index, std::wstring& value){
		if (ref.count_<=index) return false;
		const MoaMmValue& mvalue = this->get_index(ref, index);
		return this->read(mvalue, value);
	}

	bool read(const MoaMmCallInfo& callPtr, const int index, std::wstring& value){
		if (callPtr.nargs<=index) return false;
		const MoaMmValue& mvalue = this->get_index(callPtr, index);
		return this->read(mvalue, value);
	}

	bool read(const SetRef& ref, std::wstring& value){
		return this->read(ref.value_, value);
	}
	//

	bool read(const MoaMmValue& mValue, std::string& value){
		MoaMmValueType type = this->is_type(mValue);
		if (kMoaMmValueType_String != type) return false;

		MoaLong size{ 0 };
		const char* raw = nullptr;

		value_->ValueStringLength(&mValue, &size);
		value_->ValueToStringPtr(&mValue, &raw);
		value_->ValueReleaseStringPtr(&mValue);
		
		value.~basic_string();
		new(&value) std::string{ raw, static_cast<unsigned int>(size) };
		return true;;
	}

	bool read(const GetRef& ref, const int index, std::string& value){
		if (ref.count_<=index) return false;
		const MoaMmValue& mvalue = this->get_index(ref, index);
		return this->read(mvalue, value);
	}

	bool read(const MoaMmCallInfo& callPtr, const int index, std::string& value){
		if (callPtr.nargs<=index) return false;
		const MoaMmValue& mvalue = this->get_index(callPtr, index);
		return this->read(mvalue, value);
	}

	bool read(const SetRef& ref, std::string& value){
		return this->read(ref.value_, value);
	}
	//

	bool read(const MoaMmValue& mValue, _GUID& value){
		std::wstring str;
		bool bret = this->read(mValue, str);
		if (!bret) return false;
		HRESULT hr = CLSIDFromString(str.c_str(), &value);
		return (NOERROR == hr);
	}

	bool read(const GetRef& ref, const int index, _GUID& value){
		if (ref.count_<=index) return false;
		const MoaMmValue& mvalue = this->get_index(ref, index);
		return this->read(mvalue, value);
	}

	bool read(const MoaMmCallInfo& callPtr, const int index, _GUID& value){
		if (callPtr.nargs<=index) return false;
		const MoaMmValue& mvalue = this->get_index(callPtr, index);
		return this->read(mvalue, value);
	}

	bool read(const SetRef& ref, _GUID& value){
		return this->read(ref.value_, value);
	}
	//

	bool read(const MoaMmValue& mValue, IMoaMmXValue*& value){
		if (!xvalue_->valueIsIXValue(const_cast<PMoaMmValue>(&mValue))) return false;
		xvalue_->ValueToIXValue(&mValue, &value);
		return true;
	}

	bool read(const GetRef& ref, const int index, IMoaMmXValue*& value){
		if (ref.count_<=index) return false;
		const MoaMmValue& mvalue = this->get_index(ref, index);
		return this->read(mvalue, value);
	}

	bool read(const MoaMmCallInfo& callPtr, const int index, IMoaMmXValue*& value){
		if (callPtr.nargs<=index) return false;
		const MoaMmValue& mvalue = this->get_index(callPtr, index);
		return this->read(mvalue, value);
	}

	bool read(const SetRef& ref, IMoaMmXValue*& value){
		return this->read(ref.value_, value);
	}
	//

	bool read(const MoaMmValue& mValue, wchar_t& value){
		MoaMmValueType type = this->is_type(mValue);
		if (kMoaMmValueType_String != type) return false;

		MoaLong size{ 0 };
		const char* raw = nullptr;

		value_->ValueStringLength(&mValue, &size);
		value_->ValueToStringPtr(&mValue, &raw);
		value_->ValueReleaseStringPtr(&mValue);
		if (size<1) return false;

		if (size > 1) size = 1;
		std::wstring m = utf8_to_wstring(std::string{ raw, static_cast<unsigned int>(size) });

		value = m[0];
		return true;
	}

	bool read(const GetRef& ref, const int index, wchar_t& value){
		if (ref.count_<=index) return false;
		const MoaMmValue& mvalue = this->get_index(ref, index);
		return this->read(mvalue, value);
	}

	bool read(const MoaMmCallInfo& callPtr, const int index, wchar_t& value){
		if (callPtr.nargs<=index) return false;
		const MoaMmValue& mvalue = this->get_index(callPtr, index);
		return this->read(mvalue, value);
	}

	bool read(const SetRef& ref, wchar_t& value){
		return this->read(ref.value_, value);
	}
	//

	bool read(const MoaMmValue& mValue, BYTE& value){
		MoaMmValueType type = this->is_type(mValue);
		if (kMoaMmValueType_Integer != type) return false;
		MoaLong result{};

		value_->ValueToInteger(&mValue, &result);
		if (result > 255 || result < 0) return false;
		value = static_cast<BYTE>(result);
		return true;
	}

	bool read(const GetRef& ref, const int index, BYTE& value){
		if (ref.count_<=index) return false;
		const MoaMmValue& mvalue = this->get_index(ref, index);
		return this->read(mvalue, value);
	}

	bool read(const MoaMmCallInfo& callPtr, const int index, BYTE& value){
		if (callPtr.nargs<=index) return false;
		const MoaMmValue& mvalue = this->get_index(callPtr, index);
		return this->read(mvalue, value);
	}

	bool read(const SetRef& ref, BYTE& value){
		return this->read(ref.value_, value);
	}
	//

	bool read(const MoaMmValue& mValue, char& value){
		MoaMmValueType type = this->is_type(mValue);
		if (kMoaMmValueType_String != type) return false;

		MoaLong size{ 0 };
		const char* raw = nullptr;

		value_->ValueStringLength(&mValue, &size);
		value_->ValueToStringPtr(&mValue, &raw);
		value_->ValueReleaseStringPtr(&mValue);
		if (size<1) return false;

		if (size > 1) size = 1;
		std::string m{ raw, static_cast<unsigned int>(size) };

		value = m[0];
		return true;
	}

	bool read(const GetRef& ref, const int index, char& value){
		if (ref.count_<=index) return false;
		const MoaMmValue& mvalue = this->get_index(ref, index);
		return this->read(mvalue, value);
	}

	bool read(const MoaMmCallInfo& callPtr, const int index, char& value){
		if (callPtr.nargs<=index) return false;
		const MoaMmValue& mvalue = this->get_index(callPtr, index);
		return this->read(mvalue, value);
	}

	bool read(const SetRef& ref, char& value){
		return this->read(ref.value_, value);
	}
	//

	//read_color
	bool read_color(const MoaMmValue& mValue, COLORREF& value){
		MoaMmValueType type = this->is_type(mValue);
		if (kMoaMmValueType_Color != type) return false;
		MoaMmColorSpec result{};
		color_->ValueToColor(&mValue, &result);
		value = RGB(result.color.rgb.red, result.color.rgb.green, result.color.rgb.blue);
		return true;
	}

	bool read_color(const GetRef& ref, const int index, COLORREF& value){
		if (ref.count_<=index) return false;
		const MoaMmValue& mvalue = this->get_index(ref, index);
		return this->read_color(mvalue, value);
	}

	bool read_color(const MoaMmCallInfo& callPtr, const int index, COLORREF& value){
		if (callPtr.nargs<=index) return false;
		const MoaMmValue& mvalue = this->get_index(callPtr, index);
		return this->read_color(mvalue, value);
	}

	bool read_color(const SetRef& ref, COLORREF& value){
		return this->read_color(ref.value_, value);
	}
	//
	
	bool read_color(const MoaMmValue& mValue, MoaMmColorSpec& value){
		MoaMmValueType type = this->is_type(mValue);
		if (kMoaMmValueType_Color != type) return false;
		color_->ValueToColor(&mValue, &value);
		return true;
	}

	bool read_color(const GetRef& ref, const int index, MoaMmColorSpec& value){
		if (ref.count_<=index) return false;
		const MoaMmValue& mvalue = this->get_index(ref, index);
		return this->read_color(mvalue, value);
	}

	bool read_color(const MoaMmCallInfo& callPtr, const int index, MoaMmColorSpec& value){
		if (callPtr.nargs<=index) return false;
		const MoaMmValue& mvalue = this->get_index(callPtr, index);
		return this->read_color(mvalue, value);
	}

	bool read_color(const SetRef& ref, MoaMmColorSpec& value){
		return this->read_color(ref.value_, value);
	}
	//

	//////////////////////////////////////////
	//read_string_ptr
	bool read_string_ptr(const MoaMmValue& mValue, const char*& value, MoaLong& size){
		MoaMmValueType type = this->is_type(mValue);
		if (kMoaMmValueType_String != type) return false;

		value_->ValueStringLength(&mValue, &size);
		value_->ValueToStringPtr(&mValue, &value);
		value_->ValueReleaseStringPtr(&mValue);
		return true;
	}

	bool read_string_ptr(const GetRef& ref, const int index, const char*& value, MoaLong& size){
		if (ref.count_<=index) return false;
		const MoaMmValue& mvalue = this->get_index(ref, index);
		return this->read_string_ptr(mvalue, value, size);
	}

	bool read_string_ptr(const MoaMmCallInfo& callPtr, const int index, const char*& value, MoaLong& size){
		if (callPtr.nargs<=index) return false;
		const MoaMmValue& mvalue = this->get_index(callPtr, index);
		return this->read_string_ptr(mvalue, value, size);
	}

	bool read_string_ptr(const SetRef& ref, const char*& value, MoaLong& size){
		return this->read_string_ptr(ref.value_, value, size);
	}


	//read_symbol

	bool read_symbol(const MoaMmValue& mValue, MoaMmSymbol& value){
		MoaMmValueType type = this->is_type(mValue);
		if (kMoaMmValueType_Symbol != type) return false;
		value_->ValueToSymbol(&mValue, &value);
		return true;
	}

	bool read_symbol(const GetRef& ref, const int index, MoaMmSymbol& value){
		if (ref.count_<=index) return false;
		const MoaMmValue& mvalue = this->get_index(ref, index);
		return this->read_symbol(mvalue, value);
	}

	bool read_symbol(const MoaMmCallInfo& callPtr, const int index, MoaMmSymbol& value){
		if (callPtr.nargs<=index) return false;
		const MoaMmValue& mvalue = this->get_index(callPtr, index);
		return this->read_symbol(mvalue, value);
	}

	bool read_symbol(const SetRef& ref, MoaMmSymbol& value){
		return this->read_symbol(ref.value_, value);
	}
	

	//read_count
	bool read_count(const MoaMmValue& mValue, MoaLong& value){
		MoaMmValueType type = this->is_type(mValue);
		if (kMoaMmValueType_List != type && kMoaMmValueType_PropList != type) return false;
		value = list_->CountElements(&mValue);
		return true;
	}
	
	bool read_count(const GetRef& ref, const int index, MoaLong& value){
		if (ref.count_<=index) return false;
		const MoaMmValue& mvalue = this->get_index(ref, index);
		return this->read_count(mvalue, value);
	}

	bool read_count(const MoaMmCallInfo& callPtr, const int index, MoaLong& value){
		if (callPtr.nargs<=index) return false;
		const MoaMmValue& mvalue = this->get_index(callPtr, index);
		return this->read_count(mvalue, value);
	}

	bool read_count(const SetRef& ref, MoaLong& value){
		return this->read_count(ref.value_, value);
	}
};


template<typename T>
STDMETHODIMP Build_Interface(PMoaVoid pObj, PPMoaVoid ppIF)
{ 
	T * pIF = new T((_MOAFactory*)pObj); 
			
	if (pIF){
		*ppIF = (PMoaVoid)pIF;
		return kMoaErr_NoErr;
	}		 	
	 
	return kMoaErr_OutOfMem; 
};

template<typename T>
class Build_XValue :public IMoaMmXScript
{
private:
	_MOAFactory* obj_;

	MoaLong 				refcnt_; 		/* refcount for THIS INTERFACE */
	virtual __declspec(nothrow) HRESULT  __stdcall QueryInterface(ConstPMoaInterfaceID pInterfaceID, PPMoaVoid ppv){
		if (*pInterfaceID == IID_IMoaMmXScript){
			this->AddRef();
			*ppv = (IMoaMmXScript*)this;
			return S_OK;
		}

		if (*pInterfaceID == IID_IMoaUnknown){
			this->AddRef();
			*ppv = (IMoaUnknown*)this;
			return S_OK;
		}

		return E_NOINTERFACE;
	}

	virtual __declspec(nothrow) MoaUlong __stdcall AddRef(){
		return InterlockedIncrement(&this->refcnt_);
	}

	virtual __declspec(nothrow) MoaUlong __stdcall Release(){
		MoaUlong result;
		result = InterlockedDecrement(&this->refcnt_);
		if (!result) delete this;
		return result;
	}


	//IMoaMmXScript
	virtual __declspec(nothrow) HRESULT  __stdcall Call(PMoaMmCallInfo callPtr){
		MoaError err = kMoaErr_NoErr;
		MOAQIPtr<IMoaMmXValueUtils, &IID_IMoaMmXValueUtils> p(obj_->pCallback);
		IMoaMmXValue* value = new T(obj_);
		p->IXValueToValue(value, &callPtr->resultValue);
		this->Release();
		return(err);
	}

public:
	explicit Build_XValue(_MOAFactory* mobj)
	:refcnt_(1), obj_(mobj)
	{
		obj_->AddRef();
	}

	~Build_XValue(){
		obj_->Release();
	}
};


template<typename FN>
class Build_XValue_FACTORY :public IMoaMmXScript
{
private:
	_MOAFactory* obj_;
	MoaUtils utils_;

	MoaLong 				refcnt_; 		/* refcount for THIS INTERFACE */
	virtual __declspec(nothrow) HRESULT  __stdcall QueryInterface(ConstPMoaInterfaceID pInterfaceID, PPMoaVoid ppv){
		if (*pInterfaceID == IID_IMoaMmXScript){
			this->AddRef();
			*ppv = (IMoaMmXScript*)this;
			return S_OK;
		}

		if (*pInterfaceID == IID_IMoaUnknown){
			this->AddRef();
			*ppv = (IMoaUnknown*)this;
			return S_OK;
		}

		return E_NOINTERFACE;
	}

	virtual __declspec(nothrow) MoaUlong __stdcall AddRef(){
		return InterlockedIncrement(&this->refcnt_);
	}

	virtual __declspec(nothrow) MoaUlong __stdcall Release(){
		MoaUlong result;
		result = InterlockedDecrement(&this->refcnt_);
		if (!result) delete this;
		return result;
	}


	//IMoaMmXScript
	virtual __declspec(nothrow) HRESULT  __stdcall Call(PMoaMmCallInfo callPtr){
		switch (callPtr->methodSelector){
			case 0://new
			{
					   FN fn(this->obj_);
					   IMoaMmXValue* value = fn(*callPtr);
					   if (!value) return kMoaErr_BadParam;

					   this->utils_.write(value, *callPtr);
					   value->Release();
					   return kMoaErr_NoErr;
			}

			case 1: //help
			{
				FN::help(this->utils_, *callPtr);
				return kMoaErr_NoErr;
			}

		}
		return(kMoaErr_BadParam);
	}

public:
	explicit Build_XValue_FACTORY(_MOAFactory* mobj)
		:refcnt_{ 1 }, obj_{ mobj }, utils_{ obj_ }
	{
		obj_->AddRef();
	}

	~Build_XValue_FACTORY(){
		obj_->Release();
	}
};


typedef HRESULT (__stdcall *BUID_FN)(PMoaVoid pObj, PPMoaVoid ppIF);

struct XTRA_INFO
{
	const GUID* classID_;
	BUID_FN fn_;
    const GUID* interfaceID_;	
};


extern MoaLong DllgXtraInterfaceCount;//!!!!!
void rawXtraInfo(XTRA_INFO*& info, size_t& size);

/*
void rawXtraInfo(XTRA_INFO*& info, size_t& size){
	const XTRA_INFO XTRAS_LIST[]={
		{&CLSID(CDrAccessRegister), Build_Interface<A>, &IID_IMoaRegister}
	};
	info=XTRAS_LIST;
	size =sizeof(XTRAS_LIST)/sizeof(XTRA_INFO);
};*/


