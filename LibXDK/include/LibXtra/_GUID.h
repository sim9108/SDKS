#pragma once
#include<LibXtra\_XtraRegister.h>

// _GUID_fn
class _GUID_fn{
	using DATA_TYPE = _GUID;
public:
	static HRESULT CallHandler(std::string& cmd, _BaseXValue& obj, DATA_TYPE& data, MoaMmCallInfo& callPtr);
	static HRESULT GetProp(std::string& cmd, _BaseXValue& obj, DATA_TYPE& data, GetRef& ref);
	static HRESULT SetProp(std::string& cmd, _BaseXValue& obj, DATA_TYPE& data, SetRef& ref);

	static void get_interface(std::stringstream& ss);

private:
	typedef HRESULT(*FN)(_BaseXValue& obj, DATA_TYPE& data, MoaMmCallInfo& callPtr);
	typedef std::pair<std::string, FN> FN_PAIR;
	typedef std::unordered_map<std::string, FN_PAIR> MAP_FN;
	static MAP_FN map_;

	//GetProp
	typedef HRESULT(*GET_FN)(_BaseXValue& obj, DATA_TYPE& data, GetRef& ref);
	typedef std::pair<std::string, GET_FN> GET_FN_PAIR;
	typedef std::unordered_map<std::string, GET_FN_PAIR> MAP_GET_FN;
	static MAP_GET_FN get_map_;

	//SetProp
	typedef HRESULT(*SET_FN)(_BaseXValue& obj, DATA_TYPE& data, SetRef& ref);
	typedef std::pair<std::string, SET_FN> SET_FN_PAIR;
	typedef std::unordered_map<std::string, SET_FN_PAIR> MAP_SET_FN;
	static MAP_SET_FN set_map_;

	//Handler
	static HRESULT get_string(_BaseXValue& obj, DATA_TYPE& data, GetRef& ref);
	static HRESULT set_string(_BaseXValue& obj, DATA_TYPE& data, SetRef& ref);

	static HRESULT setString(_BaseXValue& obj, DATA_TYPE& data, MoaMmCallInfo& callPtr);
};

// __GUID
class __GUID :public _BaseXValue
{

	using FN_TYPE = _GUID_fn;
	using SELF = __GUID;

public:
	using _TYPE = GUID;
	using PTR_TYPE = _TYPE*;
	using PPTR_TYPE = _TYPE**;
	using DATA_TYPE = PTR_TYPE;

private:

	DATA_TYPE data_;
	bool is_destroy_;
public:
	explicit __GUID(_MOAFactory* mobj, bool is_destroy);
	explicit __GUID(_MOAFactory* mobj, const PTR_TYPE outptr);
	explicit __GUID(_MOAFactory* mobj, const _TYPE& outobj);
	
	~__GUID();

	static void get_interface(std::stringstream& ss);
	_TYPE& type();
	PTR_TYPE& ptr();
	PPTR_TYPE pptr();

	operator PTR_TYPE();
	operator PPTR_TYPE();

private:
	virtual __declspec(nothrow) HRESULT __stdcall CallHandler(PMoaMmCallInfo callPtr);
	virtual __declspec(nothrow) HRESULT __stdcall GetProp(ConstPMoaMmValue selfRef, MoaMmSymbol propName, MoaLong indexCount, ConstPMoaMmValue pIndexValues, PMoaMmValue pResult);
	virtual __declspec(nothrow) HRESULT __stdcall SetProp(MoaMmSymbol propName, MoaLong indexCount, ConstPMoaMmValue pIndexValues, ConstPMoaMmValue pNewValue);

	typedef HRESULT(SELF::*FN)(MoaMmCallInfo& callPtr);
	typedef std::pair<std::string, FN> FN_PAIR;
	typedef std::unordered_map<std::string, FN_PAIR> MAP_FN;
	static MAP_FN map_;

	//GetProp
	typedef HRESULT(SELF::*GET_FN)(GetRef& ref);
	typedef std::pair<std::string, GET_FN> GET_FN_PAIR;
	typedef std::unordered_map<std::string, GET_FN_PAIR> MAP_GET_FN;
	static MAP_GET_FN get_map_;

	//SetProp
	typedef HRESULT(SELF::*SET_FN)(SetRef& ref);
	typedef std::pair<std::string, SET_FN> SET_FN_PAIR;
	typedef std::unordered_map<std::string, SET_FN_PAIR> MAP_SET_FN;
	static MAP_SET_FN set_map_;

	//Handler
	HRESULT get_interface(GetRef& ref);
	HRESULT get_ptr(GetRef& ref);
	HRESULT get_pptr(GetRef& ref);
	HRESULT is_equal(MoaMmCallInfo& callPtr);
	HRESULT Create(MoaMmCallInfo& callPtr);
};