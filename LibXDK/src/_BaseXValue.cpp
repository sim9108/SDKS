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

//ImageCrop

ImageCrop::ImageCrop(ImageLock& org_image, RECT& roi_rect, RECT& intersect_rect)
:org_image_{ org_image },
roi_rect_( roi_rect ),
intersect_rect_(intersect_rect)
{
	width_ = roi_rect_.right - roi_rect_.left + 1;
	height_ = roi_rect_.bottom - roi_rect_.top + 1;
}

const RECT& ImageCrop::intersect_rect() const{
	return this->intersect_rect_;
}

MoaLong ImageCrop::width() const{
	return this->width_;
}

MoaLong ImageCrop::height() const{
	return this->height_;
}

// ImageLock
ImageLock::ImageLock(_BaseXValue& mobj, MoaMmValue& mv)
:mobj_(mobj), ready_{false}
{
	auto mtype = mobj.utils_.is_type(mv);
	if (kMoaMmValueType_Other != mtype) return;
		
	ready_ = true;
	this->mv_ = mv;
	this->raw_update_info();
	this->mobj_.utils_.LockPixels(&this->mv_, (PMoaVoid*)&this->data_);
}


ImageLock::ImageLock(_BaseXValue& mobj, MoaMmCallInfo& callPtr, int idx)
:mobj_(mobj), ready_{ false }
{
	auto mtype = mobj.utils_.is_type(callPtr, idx);
	if (kMoaMmValueType_Other != mtype) return;

	ready_ = true;
	this->mv_ = this->mobj_.utils_.get_index(callPtr, idx);
	this->raw_update_info();
	this->mobj_.utils_.LockPixels(&this->mv_, (PMoaVoid*)&this->data_);
}

void
ImageLock::raw_update_info(){
	MoaMmImageInfo info;
	mobj_.utils_.GetImageInfo(&this->mv_, &info);
	this->width_ = info.iWidth;
	this->height_ = info.iHeight;
	this->image_row_bytes_ = info.iRowBytes;
	this->pitch_bytes_ = info.iRowBytes;
	this->pixel_bytes_ = info.iTotalDepth / 8;
	this->alpha_pixel_bytes_ = info.iAlphaDepth / 8;
}


ImageCrop
ImageLock::get_crop(RECT& rect){
	RECT full_rect = { 0, 0, this->width() - 1, this->height() - 1 };
	RECT intersect_rect;
	::IntersectRect(&intersect_rect, &full_rect, &rect);

	intersect_rect.left -= rect.left;
	intersect_rect.right -= rect.left;
	intersect_rect.top -= rect.top;
	intersect_rect.bottom -= rect.top;
	return ImageCrop(*this, rect, intersect_rect);
}


ImageLock::operator ImageCrop(){
	RECT full_rect = { 0, 0, this->width() - 1, this->height() - 1 };
	return this->get_crop(full_rect);
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
	width_ = height_ = image_row_bytes_ = pitch_bytes_ = pixel_bytes_ = alpha_pixel_bytes_ = 0;
}

BYTE*
ImageLock::data()  const{
	return this->data_;
}

MoaLong
ImageLock::width()  const{
	return this->width_;
}

MoaLong
ImageLock::height() const {
	return this->height_;
}

MoaLong
ImageLock::total_depth()  const{
	return this->pixel_bytes_;
}

MoaLong
ImageLock::row_bytes() const{
	return this->image_row_bytes_;
}