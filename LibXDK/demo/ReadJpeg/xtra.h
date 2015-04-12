#pragma once
#include<LibXtra\_XtraRegister.h>

//FACTORY<Guid("737EDACA-B24D-4F88-B1B9-485360F2C9A8")>
class 
__declspec(uuid("737EDACA-B24D-4F88-B1B9-485360F2C9A8"))
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


//MyRegister<Guid("B14EB8B4-0AB3-4A71-8379-889927817124")>
class __declspec(uuid("B14EB8B4-0AB3-4A71-8379-889927817124"))
MyRegister :public _XtraRegister{
public:
	explicit MyRegister(_MOAFactory* mobj);

private:
	virtual __declspec(nothrow) HRESULT  __stdcall Register(PIMoaCache pCache, PIMoaXtraEntryDict pXtraDict);
};