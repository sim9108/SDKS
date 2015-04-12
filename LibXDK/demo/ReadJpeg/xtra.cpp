#include "xtra.h"
#include "Src/_Basic.h"

//FACTORY
FACTORY::MAP_FN
FACTORY::map_ = {
	{ "jpeg", { "Jpeg", &make_Basic } },
};

FACTORY::FACTORY(_MOAFactory* obj)
:obj_{ obj }, utils_(obj_)
{
}

void
FACTORY::help(MoaUtils& utils, MoaMmCallInfo& callPtr){
	
	std::set<std::string> help_set;
	for (auto& item : map_){
		help_set.insert(item.second.first);
	}

	utils.make_list(callPtr);
	for (auto& item : help_set){
		MoaMmValue v1;
		utils.write(item, v1);
		utils.append(callPtr, v1);
		utils.ValueRelease(v1);
	}
}

IMoaMmXValue*
FACTORY::operator()(MoaMmCallInfo& callPtr)
{
	std::string cmd;
	utils_.read(callPtr, 0, cmd);

	for (auto& item : cmd){
		item = std::tolower(item);
	}

	auto iter = map_.find(cmd);
	if (map_.end() == iter) return nullptr;
	return (this->*map_[cmd].second)(callPtr);
}

IMoaMmXValue*
FACTORY::make_Basic(MoaMmCallInfo& callPtr){
	return new _Basic(this->obj_);
}

//MyRegister
MyRegister::MyRegister(_MOAFactory* mobj)
:_XtraRegister(mobj)
{
}

HRESULT  __stdcall
MyRegister::Register(PIMoaCache pCache, PIMoaXtraEntryDict pXtraDict){
	this->register_script_(pCache, pXtraDict, __uuidof(FACTORY), "xtra ReadJpeg\n+new object me,string xtraname,*\n+help object me\n");
	return kMoaErr_NoErr;
}


void rawXtraInfo(XTRA_INFO*& info, size_t& size){
	static XTRA_INFO XTRAS_LIST[]={
		{ &__uuidof(MyRegister), Build_Interface<MyRegister>, &IID_IMoaRegister },
		{ &__uuidof(FACTORY), Build_Interface< Build_XValue_FACTORY<FACTORY>  >, &IID_IMoaMmXScript },
	};
	
	info=XTRAS_LIST;
	size =sizeof(XTRAS_LIST)/sizeof(XTRA_INFO);
}