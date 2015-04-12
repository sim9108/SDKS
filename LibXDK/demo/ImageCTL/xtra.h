#pragma once
#include<LibXtra\_XtraRegister.h>

//FACTORY[Guid("AB70502E-6EE2-476A-B3A6-D26BD4C3B18C")]
class 
__declspec(uuid("AB70502E-6EE2-476A-B3A6-D26BD4C3B18C"))
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


//MyRegister [Guid("6F9A4680-DD3E-4159-965B-01B03D031C7D")]
class __declspec(uuid("6F9A4680-DD3E-4159-965B-01B03D031C7D"))
MyRegister :public _XtraRegister{
public:
	explicit MyRegister(_MOAFactory* mobj);

private:
	virtual __declspec(nothrow) HRESULT  __stdcall Register(PIMoaCache pCache, PIMoaXtraEntryDict pXtraDict);
};