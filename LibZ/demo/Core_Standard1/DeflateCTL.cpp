#include "DeflateCTL.h"
#include <iostream>


DeflateCTL::DeflateCTL()
{
	std::memset(&this->stream_, 0, sizeof(z_stream));
	deflateInit(&this->stream_, Z_DEFAULT_COMPRESSION);
}


DeflateCTL::~DeflateCTL()
{
	deflateEnd(&this->stream_);
}

DeflateCTL&
DeflateCTL::operator <<(const std::vector<Bytef>& input){
	this->stream_.avail_in = input.size();
	this->stream_.next_in = const_cast<Bytef*>( input.data() );

	const int BUF = 1024;
	Bytef out[BUF];

	do{
		this->stream_.avail_out = BUF;
		this->stream_.next_out = out;
		deflate(&this->stream_, Z_NO_FLUSH);
		int writesize = BUF - this->stream_.avail_out;

		for (int i = 0; i < writesize; ++i){
			this->result_.push_back(out[i]);
		}

	} while (0 == this->stream_.avail_out);

	return  *this;
}

DeflateCTL&
DeflateCTL::operator <<(const std::string& input){
	this->stream_.avail_in = input.size();
	this->stream_.next_in =
		reinterpret_cast<Bytef*>(
		 const_cast<char*>(input.data())
		);

	const int BUF = 1024;
	Bytef out[BUF];

	do{
		this->stream_.avail_out = BUF;
		this->stream_.next_out = out;
		deflate(&this->stream_, Z_NO_FLUSH);
		int writesize = BUF - this->stream_.avail_out;

		for (int i = 0; i < writesize; ++i){
			this->result_.push_back(out[i]);
		}

	} while (0 == this->stream_.avail_out);

	return  *this;
}


DeflateCTL&
DeflateCTL::endl(DeflateCTL& obj){
	// this-> == obj
	obj.stream_.avail_in = 0;
	obj.stream_.next_in = nullptr;

	const int BUF = 1024;
	Bytef out[BUF];

	do{
		obj.stream_.avail_out = BUF;
		obj.stream_.next_out = out;
		deflate(&obj.stream_, Z_FINISH);
		int writesize = BUF - obj.stream_.avail_out;

		for (int i = 0; i < writesize; ++i){
			obj.result_.push_back(out[i]);
		}

	} while (0 == obj.stream_.avail_out);
	return obj;
}


DeflateCTL&
DeflateCTL::operator <<( std::ifstream& input){
	const int BUF = 1024;
	Bytef in[BUF];
	Bytef out[BUF];
	int ret;
	do{
		input.read(reinterpret_cast<char*>(in), BUF);
		int readsize = (int)input.gcount();

		this->stream_.avail_in = readsize;
		this->stream_.next_in = in;

		do{
			this->stream_.avail_out = BUF;
			this->stream_.next_out = out;
			ret = deflate(&this->stream_, Z_NO_FLUSH);

			int writesize = BUF - this->stream_.avail_out;
			for (int i = 0; i < writesize; ++i){
				this->result_.push_back(out[i]);
			}
		} while (0 == this->stream_.avail_out);
	} while ( !input.eof());

	return  *this;
}

void
DeflateCTL::writefile(const std::string& fname){
	std::ofstream fp(fname, std::ios_base::binary);
	fp.write( 
		reinterpret_cast<char*>( this->result_.data() ),		
		this->result_.size());
}