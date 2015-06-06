#include <LibXtra/_BaseXValue.h>

_BaseXValue::_BaseXValue(_MOAFactory* mobj, const char* title)
:refcnt_(1), moa_obj_{ mobj }, utils_{ moa_obj_ }, xtra_name_{ title }
{
	this->moa_obj_->AddRef();
	++DllgXtraInterfaceCount;
}


_BaseXValue::~_BaseXValue(void)
{
	this->moa_obj_->Release();
	--DllgXtraInterfaceCount;
}

HRESULT 
__stdcall _BaseXValue::QueryInterface(ConstPMoaInterfaceID pInterfaceID, PPMoaVoid ppv) 
{ 
	if (*pInterfaceID == IID_IMoaMmXValue){
		this->AddRef();
		*ppv = (IMoaMmXValue*)this;
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
__stdcall _BaseXValue::AddRef() { 
	return InterlockedIncrement(&this->refcnt_); 
} 
		 
MoaUlong 
__stdcall _BaseXValue::Release() { 
	MoaUlong result; 			 
	result = InterlockedDecrement(&this->refcnt_); 			
	if (!result) delete this; 			 	
	return result; 
}


HRESULT  __stdcall
_BaseXValue::SetData(PMoaVoid pDataum){
	return kMoaMmErr_AccessNotSupported;
}

HRESULT  __stdcall
_BaseXValue::GetPropCount(MoaLong* pCount){
	*pCount = 0;
	return kMoaMmErr_AccessNotSupported;
}

HRESULT  __stdcall
_BaseXValue::GetPropDescriptionByIndex(MoaLong index, PMoaMmValueDescription pDescription){
	return kMoaMmErr_AccessNotSupported;
}

HRESULT  __stdcall
_BaseXValue::GetCount(MoaMmSymbol propName, MoaLong* pCount){
	*pCount = 0;
	return kMoaMmErr_PropertyNotFound;
}

///!!!!
HRESULT  __stdcall
_BaseXValue::GetProp(ConstPMoaMmValue selfRef, MoaMmSymbol propName, MoaLong indexCount, ConstPMoaMmValue pIndexValues, PMoaMmValue	pResult){
	return kMoaMmErr_PropertyNotFound;
}

HRESULT  __stdcall
_BaseXValue::SetProp(MoaMmSymbol propName, MoaLong indexCount, ConstPMoaMmValue pIndexValues, ConstPMoaMmValue pNewValue){
	return kMoaMmErr_PropertyNotFound;
}
////!!!!!!

//!!!! important !!!
HRESULT  __stdcall
_BaseXValue::AccessPropRef(ConstPMoaMmValue selfRef, MoaMmSymbol propName, MoaLong indexCount, ConstPMoaMmValue pIndexValues, PMoaMmValue pResult){
	return this->GetProp(selfRef, propName, indexCount, pIndexValues, pResult);
}

HRESULT  __stdcall
_BaseXValue::GetContents(ConstPMoaMmValue selfRef, PMoaMmValue pResult){
	return kMoaMmErr_AccessNotSupported;
}

HRESULT  __stdcall
_BaseXValue::SetContents(PMoaMmValue pNewValue){
	return kMoaMmErr_AccessNotSupported;
}

HRESULT  __stdcall
_BaseXValue::SetContentsBefore(PMoaMmValue pNewValue){
	return kMoaMmErr_AccessNotSupported;
}

HRESULT  __stdcall
_BaseXValue::SetContentsAfter(PMoaMmValue pNewValue){
	return kMoaMmErr_AccessNotSupported;
}


HRESULT  __stdcall
_BaseXValue::Ilk(PMoaMmValue pArgument, PMoaMmValue pResult){
	return kMoaMmErr_AccessNotSupported;
}

HRESULT  __stdcall
_BaseXValue::StringRep(PMoaMmValue pStringVal){
	std::stringstream ss;
	ss << "<Xtra child \"" << xtra_name_  <<"\" 0x"<<std::hex<<(void*) this << ">";
	this->utils_.write_string(ss, *pStringVal);
	return kMoaErr_NoErr;
}

HRESULT  __stdcall
_BaseXValue::SymbolRep(PMoaMmSymbol pSymbol){	
	*pSymbol = this->utils_.make_symbol(xtra_name_);
	return kMoaErr_NoErr;
}

HRESULT  __stdcall
_BaseXValue::IntegerRep(PMoaLong pIntVal){
	*pIntVal = 0;
	return kMoaMmErr_AccessNotSupported;
}

HRESULT  __stdcall
_BaseXValue::StreamOut(PIMoaStream2 pStream){
	return kMoaMmErr_AccessNotSupported;
}

HRESULT  __stdcall
_BaseXValue::StreamIn(PIMoaStream2 pStream){
	return kMoaMmErr_AccessNotSupported;
}



/*
ImageLock
*/

ImageLock::ImageLock(_BaseXValue& mobj, MoaMmValue& mv)
:mobj_(mobj), crop_rect_({ 0, 0, 0, 0 })
{
	auto mtype = mobj.utils_.is_type(mv);
	if (kMoaMmValueType_Other != mtype){
		ready_ = false;
		return;
	}

	ready_ = true;
	this->mv_ = mv;
	mobj_.utils_.GetImageInfo(&this->mv_, &this->info_);
	this->mobj_.utils_.LockPixels(&this->mv_, (PMoaVoid*)&this->data_);
	SetRect(&raw_rect_, 0, 0, this->width() - 1, this->height() - 1);
	SetRect(&crop_rect_, 0, 0, this->width() - 1, this->height() - 1);
}


ImageLock::ImageLock(_BaseXValue& mobj, MoaMmCallInfo& callPtr, int idx)
:mobj_(mobj), crop_rect_({ 0, 0, 0, 0 })
{
	auto mtype = mobj.utils_.is_type(callPtr, idx);
	if (kMoaMmValueType_Other != mtype){
		ready_ = false;
		return;
	}

	ready_ = true;
	this->mv_ = this->mobj_.utils_.get_index(callPtr, idx);
	mobj_.utils_.GetImageInfo(&this->mv_, &this->info_);
	this->mobj_.utils_.LockPixels(&this->mv_, (PMoaVoid*)&this->data_);
	SetRect(&raw_rect_, 0, 0, this->width() - 1, this->height() - 1);
	SetRect(&crop_rect_, 0, 0, this->width() - 1, this->height() - 1);
}

void 
ImageLock::set_crop_rect(RECT& rect){
	
	::IntersectRect(&this->crop_rect_, &rect, &raw_rect_);
}

ImageLock::~ImageLock(){
	this->reset();
}

void
ImageLock::reset(){
	if (!ready_) return;
	this->mobj_.utils_.UnlockPixels(&this->mv_);
	ready_ = false;
	this->data_ = nullptr;
	std::memset(&this->info_, 0, sizeof(this->info_));
}

BYTE*
ImageLock::data()  const{
	return this->data_;
}

MoaLong
ImageLock::width()  const{
	return this->info_.iWidth;
}

MoaLong
ImageLock::height() const {
	return this->info_.iHeight;
}

MoaLong
ImageLock::total_depth()  const{
	return this->info_.iTotalDepth;
}

MoaLong
ImageLock::alpha_depth() const{
	return this->info_.iAlphaDepth;
}

MoaBool
ImageLock::is_cartesian() const{
	return this->info_.bCartesian;
}

MoaLong
ImageLock::row_bytes() const{
	return this->info_.iRowBytes;
}