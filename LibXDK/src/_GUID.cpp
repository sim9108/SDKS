#include <LibXtra/_GUID.h>


// _GUID_fn
_GUID_fn::MAP_FN
_GUID_fn::map_ = {
		{ "settext", { "settext(data:UTF8STRING):bool", setString } },
};

_GUID_fn::MAP_GET_FN
_GUID_fn::get_map_ = {
	{ "text", { "text:UTF8STRING", get_string } },
};


_GUID_fn::MAP_SET_FN
_GUID_fn::set_map_ = {
	{ "text", { "text:UTF8STRING", set_string } },
};

HRESULT
_GUID_fn::CallHandler(std::string& cmd, _BaseXValue& obj, DATA_TYPE& data, MoaMmCallInfo& callPtr){
	auto iter = map_.find(cmd);
	if (map_.end() == iter) return kMoaMmErr_FunctionNotFound;
	return map_[cmd].second(obj, data, callPtr);
}

HRESULT
_GUID_fn::GetProp(std::string& cmd, _BaseXValue& obj, DATA_TYPE& data, GetRef& ref){
	auto iter = get_map_.find(cmd);
	if (get_map_.end() == iter) return kMoaMmErr_PropertyNotFound;
	return get_map_[cmd].second(obj, data, ref);
}

HRESULT
_GUID_fn::SetProp(std::string& cmd, _BaseXValue& obj, DATA_TYPE& data, SetRef& ref){
	auto iter = set_map_.find(cmd);
	if (set_map_.end() == iter) return kMoaMmErr_PropertyNotFound;
	return set_map_[cmd].second(obj, data, ref);
}

void
_GUID_fn::get_interface(std::stringstream& ss){
	ss << "-- GUID\n";
	HELPTEXT(ss, map_, set_map_, get_map_);
}
/////

HRESULT
_GUID_fn::get_string(_BaseXValue& obj, DATA_TYPE& data, GetRef& ref){
	obj.utils_.write_string(data, ref);
	return kMoaErr_NoErr;
}


HRESULT
_GUID_fn::set_string(_BaseXValue& obj, DATA_TYPE& data, SetRef& ref){
	std::wstring str;
	bool bret = obj.utils_.read(ref, str);
	if (!bret) return kMoaErr_BadParam;
	HRESULT hr = CLSIDFromString(str.c_str(), &data);
	return kMoaErr_NoErr;
}

HRESULT
_GUID_fn::setString(_BaseXValue& obj, DATA_TYPE& data, MoaMmCallInfo& callPtr){
	std::wstring str;
	bool bret = obj.utils_.read(callPtr,0, str);
	if (!bret) return kMoaErr_BadParam;
	HRESULT hr = CLSIDFromString(str.c_str(), &data);
	obj.utils_.write(hr, callPtr);
	return kMoaErr_NoErr;
}


// _GUID
__GUID::MAP_FN
__GUID::map_ = {
	{ "=", { "=GUID_PTR:boolean", &is_equal } },
	{ "create", { "Create():GUID", &Create } },
};

__GUID::MAP_GET_FN
__GUID::get_map_ = {
	{ "interface", { "interface:UTF8STRING", &get_interface } },
	{ "ptr", { "ptr:LONG_PTR", &get_ptr } },	
	{ "pptr", { "pptr:LONG_PPTR", &get_pptr } },
};


__GUID::MAP_SET_FN
__GUID::set_map_ = {
};

__GUID::__GUID(_MOAFactory* mobj, bool is_destroy)
	:_BaseXValue(mobj, "GUID"), data_{ nullptr }, is_destroy_{ is_destroy }
{
	if (!this->is_destroy_) return;
	data_ = new _TYPE;
	std::memset(data_, 0, sizeof(_TYPE));
}

__GUID::__GUID(_MOAFactory* mobj, const PTR_TYPE outptr)
: _BaseXValue(mobj, "GUID"), data_{ outptr }, is_destroy_{ false }
{
}


__GUID::__GUID(_MOAFactory* mobj, const _TYPE& outobj)
: _BaseXValue(mobj, "GUID"), data_{ new _TYPE }, is_destroy_{true}
{
	std::memcpy(this->data_, &outobj, sizeof(_TYPE));
}

__GUID::~__GUID(){
	if (!this->is_destroy_) return;
	delete this->data_;
	this->data_ = nullptr;
}

__GUID::_TYPE&
__GUID::type(){
	return *this->data_;
}

__GUID::PTR_TYPE&
__GUID::ptr(){
	return this->data_;
}

__GUID::PPTR_TYPE
__GUID::pptr(){
	return &this->data_;
}


__GUID::operator PPTR_TYPE(){
	return &this->data_;
}

__GUID::operator PTR_TYPE(){
	return this->data_;
}

HRESULT __stdcall
__GUID::CallHandler(PMoaMmCallInfo callPtr){
	std::string cmd;
	this->utils_.SymbolToString(callPtr->methodSelector, cmd);

	auto iter = map_.find(cmd);
	if (map_.end() != iter){
		return (this->*map_[cmd].second)(*callPtr);
	}

	return FN_TYPE::CallHandler(cmd, *this, *this->data_, *callPtr);
}

HRESULT __stdcall
__GUID::GetProp(ConstPMoaMmValue selfRef, MoaMmSymbol propName, MoaLong indexCount, ConstPMoaMmValue pIndexValues, PMoaMmValue pResult){
	std::string cmd;
	this->utils_.SymbolToString(propName, cmd);
	GetRef ref{ *selfRef, propName, indexCount, *pIndexValues, *pResult };

	auto iter = get_map_.find(cmd);
	if (get_map_.end() != iter){
		return (this->*get_map_[cmd].second)(ref);
	}

	return FN_TYPE::GetProp(cmd, *this, *this->data_, ref);
}

HRESULT __stdcall
__GUID::SetProp(MoaMmSymbol propName, MoaLong indexCount, ConstPMoaMmValue pIndexValues, ConstPMoaMmValue pNewValue){
	std::string cmd;
	this->utils_.SymbolToString(propName, cmd);
	SetRef ref{ propName, indexCount, *pIndexValues, *pNewValue };
	auto iter = set_map_.find(cmd);
	if (set_map_.end() != iter){
		return (this->*set_map_[cmd].second)(ref);
	}

	return FN_TYPE::SetProp(cmd, *this, *this->data_, ref);
}

void
__GUID::get_interface(std::stringstream& ss){
	FN_TYPE::get_interface(ss);

	ss << "-- GUID Extent\n";
	HELPTEXT(ss, map_, set_map_, get_map_);
}

HRESULT
__GUID::get_interface(GetRef& ref){
	std::stringstream ss;
	get_interface(ss);
	this->utils_.write(ss, ref);
	return kMoaErr_NoErr;
}

HRESULT
__GUID::get_ptr(GetRef& ref){
	LONG_PTR  ptr = reinterpret_cast<LONG_PTR>(this->data_);
	this->utils_.write(ptr, ref);
	return kMoaErr_NoErr;
}

HRESULT
__GUID::get_pptr(GetRef& ref){
	LONG_PTR  ptr = reinterpret_cast<LONG_PTR>(&this->data_);
	this->utils_.write(ptr, ref);
	return kMoaErr_NoErr;
}

HRESULT
__GUID::is_equal(MoaMmCallInfo& callPtr){
	MoaLong ptr{};
	bool bret = this->utils_.read(callPtr, 0, ptr);
	if (!bret) return kMoaErr_BadParam;
	_GUID* pptr = reinterpret_cast<_GUID*>(ptr);
	if (nullptr == pptr) return kMoaErr_BadParam;
	this->utils_.write((*this->data_ == *pptr), callPtr);
	return kMoaErr_NoErr;
}

HRESULT
__GUID::Create(MoaMmCallInfo& callPtr){
	SMART_COM mcom;
	GUID guid;
	HRESULT hr = ::CoCreateGuid(&guid);
	
	__GUID* ix = new __GUID(this->moa_obj_, guid);
	this->utils_.write(ix, callPtr);
	ix->Release();
	return kMoaErr_NoErr;
}