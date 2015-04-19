#pragma once
#include<LibXtra\_XtraRegister.h>

//Basic
class _Basic :public _BaseXValue{
	using SELF = _Basic;

public:
	explicit _Basic(_MOAFactory* mobj);
	~_Basic();

public:
	static void get_interface(std::stringstream& ss);

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
	HRESULT readfile(MoaMmCallInfo& callPtr);
	HRESULT writefile(MoaMmCallInfo& callPtr);
};
