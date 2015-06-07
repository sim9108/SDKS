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

ImageCrop::ImageCrop(BYTE* data, int width, int height, int pixel_bytes, int image_row_bytes, int pitch_bytes)
:
data_{ data }, 
width_{ width }, height_{ height },
pitch_bytes_{ pitch_bytes },image_row_bytes_{ image_row_bytes },pixel_bytes_{ pixel_bytes }
{}

MoaLong ImageCrop::width() const{
	return this->width_;
}

MoaLong ImageCrop::height() const{
	return this->height_;
}

MoaLong ImageCrop::image_row_bytes() const{
	return this->image_row_bytes_;
}

MoaLong ImageCrop::pitch_bytes() const{
	return this->pitch_bytes_;
}

MoaLong ImageCrop::pixel_bytes() const{
	return this->pixel_bytes_;
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
	RECT full_rect;
	SetRect(&full_rect, 0, 0, this->width() - 1, this->height() - 1);

	RECT result_rect;
	::IntersectRect(&result_rect, &rect, &full_rect);
	BYTE* data = this->data_
		+ this->pitch_bytes_*(this->height_ - result_rect.bottom - 1)
		+ result_rect.left *  this->pixel_bytes_;

	int width = result_rect.right - result_rect.left + 1;
	int height = result_rect.bottom - result_rect.top + 1;
	int pixel_bytes = this->pixel_bytes_;
	int image_row_bytes = pixel_bytes*width;
	int pitch_bytes = this->pitch_bytes_;

	return ImageCrop(data, width, height, pixel_bytes, image_row_bytes, pitch_bytes);
}


ImageLock::operator ImageCrop(){
	RECT full_rect;
	SetRect(&full_rect, 0, 0, this->width() - 1, this->height() - 1);
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