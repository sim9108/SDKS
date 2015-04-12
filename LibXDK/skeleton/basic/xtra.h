#pragma once
#include<LibXtra\_XtraRegister.h>

//FACTORY
class 
__declspec(uuid("1F51A16E-82BE-48F1-8806-B3D39BBCCC86"))
FACTORY{
private:
	_MOAFactory* obj_;
	MoaUtils utils_;

	typedef IMoaMmXValue* (FACTORY::*FN)(MoaMmCallInfo& callPtr);
	typedef std::pair<std::string, FN> PAIR_FN;
	typedef std::unordered_map<std::string, PAIR_FN> MAP_FN;
	static MAP_FN map_;

public:
	explicit FACTORY(_MOAFactory* obj);
	IMoaMmXValue* operator()(MoaMmCallInfo& callPtr);

	static void help(MoaUtils& utils, MoaMmCallInfo& callPtr);
private:
	IMoaMmXValue* make_Basic(MoaMmCallInfo& callPtr);
};


//MyRegister
class __declspec(uuid("94109C08-D5CF-4C0C-BDE0-B5B8D9557596"))
MyRegister :public _XtraRegister{
public:
	explicit MyRegister(_MOAFactory* mobj);

private:
	virtual __declspec(nothrow) HRESULT  __stdcall Register(PIMoaCache pCache, PIMoaXtraEntryDict pXtraDict);
};