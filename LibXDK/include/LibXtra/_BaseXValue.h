#pragma once
#include<LibXtra\LibXtra.h>

class _BaseXValue : public IMoaMmXValue
{
public:
	_MOAFactory* moa_obj_;
	MoaUtils utils_;

protected:
	std::string xtra_name_;

private:
	MoaLong 				refcnt_; 		/* refcount for THIS INTERFACE */

public:
	explicit _BaseXValue(_MOAFactory* mobj, const char* title = "base xtra");
	virtual  ~_BaseXValue(void);
	
	virtual __declspec(nothrow) HRESULT  __stdcall QueryInterface (ConstPMoaInterfaceID pInterfaceID, PPMoaVoid ppvObj);
	virtual __declspec(nothrow) MoaUlong __stdcall AddRef ();
	virtual __declspec(nothrow) MoaUlong __stdcall Release(); 

	
	//IMoaMmXValue
	virtual __declspec(nothrow) HRESULT __stdcall SetData(PMoaVoid pDataum);
	virtual __declspec(nothrow) HRESULT __stdcall GetPropCount(MoaLong* pCount);
	virtual __declspec(nothrow) HRESULT __stdcall GetPropDescriptionByIndex(MoaLong	index,PMoaMmValueDescription pDescription);
	virtual __declspec(nothrow) HRESULT __stdcall GetCount(MoaMmSymbol propName,MoaLong* pCount);
	virtual __declspec(nothrow) HRESULT __stdcall GetProp(ConstPMoaMmValue selfRef,	MoaMmSymbol propName,MoaLong indexCount, ConstPMoaMmValue pIndexValues, PMoaMmValue pResult);
	virtual __declspec(nothrow) HRESULT __stdcall SetProp(MoaMmSymbol propName,	MoaLong indexCount,	ConstPMoaMmValue pIndexValues, ConstPMoaMmValue pNewValue);
	virtual __declspec(nothrow) HRESULT __stdcall AccessPropRef(ConstPMoaMmValue selfRef, MoaMmSymbol propName, MoaLong indexCount,	ConstPMoaMmValue pIndexValues, PMoaMmValue pResult);
	virtual __declspec(nothrow) HRESULT __stdcall GetContents(ConstPMoaMmValue selfRef, PMoaMmValue pResult);
	virtual __declspec(nothrow) HRESULT __stdcall SetContents(PMoaMmValue pNewValue);
	virtual __declspec(nothrow) HRESULT __stdcall SetContentsBefore(PMoaMmValue	pNewValue);
	virtual __declspec(nothrow) HRESULT __stdcall SetContentsAfter(PMoaMmValue	pNewValue);

	/*
	return kMoaMmErr_FunctionNotFound;
	*/
	virtual __declspec(nothrow) HRESULT __stdcall CallHandler(PMoaMmCallInfo callPtr)=0;
	virtual __declspec(nothrow) HRESULT __stdcall Ilk(PMoaMmValue pArgument, PMoaMmValue pResult);
	virtual __declspec(nothrow) HRESULT __stdcall StringRep(PMoaMmValue pStringVal);
	virtual __declspec(nothrow) HRESULT __stdcall SymbolRep(PMoaMmSymbol pSymbol);
	virtual __declspec(nothrow) HRESULT __stdcall IntegerRep(PMoaLong pIntVal);
	virtual __declspec(nothrow) HRESULT __stdcall StreamOut(PIMoaStream2 pStream);
	virtual __declspec(nothrow) HRESULT __stdcall StreamIn(PIMoaStream2 pStream);
};

#include <LibXtra/_GUID.h>

// ImageLock
class ImageLock{
	_BaseXValue& mobj_;
	BYTE* data_;
	MoaMmImageInfo info_;
	MoaMmValue mv_;
	bool ready_;

public:
	explicit ImageLock(_BaseXValue& mobj, MoaMmCallInfo& callPtr, int idx);
	explicit ImageLock(_BaseXValue& mobj, MoaMmValue& mv);

	~ImageLock();

	BYTE* data() const;
	MoaLong width() const;
	MoaLong height() const;
	MoaLong total_depth() const;
	MoaLong alpha_depth() const;
	MoaBool is_cartesian() const;
	MoaLong row_bytes()  const;

	void reset();

	template<typename T>
	T* rptr(int rows){
		if (rows >= this->height()) rows = 0;
		return  reinterpret_cast<T*>(this->data_ + this->info_.iRowBytes*(this->height()-rows-1));
	}

	template<typename T>
	T* rptr(int rows, int cols){
		if (rows >= this->height()) rows = 0;
		if (cols >= this->width()) cols = 0;

		return reinterpret_cast<T*>(this->data_ + this->info_.iRowBytes*(this->height() - rows - 1) + cols* info_.iTotalDepth / 8);
	}


	template<typename T>
	T* ptr(int rows){
		if (rows >= this->height()) rows = 0;
		return  reinterpret_cast<T*>(this->data_ + this->info_.iRowBytes*rows);
	}

	template<typename T>
	T* ptr(int rows, int cols){
		if (rows >= this->height()) rows = 0;
		if (cols >= this->width()) cols = 0;

		return reinterpret_cast<T*>(this->data_ + this->info_.iRowBytes*rows + cols* info_.iTotalDepth/8);
	}

	bool operator !() const {
		return !ready_;
	}
};

struct BGRPIXEL4{
	MoaByte   blue;
	MoaByte   green;
	MoaByte   red;
	MoaByte   alpha;
};

struct PIXEL4{
	MoaByte   red;
	MoaByte   green;
	MoaByte   blue;
	MoaByte   alpha;
};

struct BGRPIXEL3{
	MoaByte   blue;
	MoaByte   green;
	MoaByte   red;
};

struct PIXEL3{
	MoaByte   red;
	MoaByte   green;
	MoaByte   blue;
};

template<typename T>
BYTE saturate_cast(T&& a){
	return (a > 255) ? 255 : static_cast<BYTE>(a);
}

//SMART_COM
class SMART_COM{
public:
	SMART_COM(){
		CoInitialize(NULL);
	}
		
	~SMART_COM(){
		CoUninitialize();
	}
};
