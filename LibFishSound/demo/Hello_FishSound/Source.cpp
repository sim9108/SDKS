#include <oggz\oggz.h>
#include <ogg\ogg.h>
#include<fishsound\fishsound.h>
#include <iostream>

//Ogg Flac, speex-->wav
//test.exe test.ogg test.wav

long decode_serailno = -1;

int decoded_float(FishSound* fsound, float** pcm, long frames, void* user_data)
{
	return 0;
}

int read_packet(OGGZ * oggz, oggz_packet * op, long serialno,
	void * user_data){

	FishSound* fsound = static_cast<FishSound*>(user_data);

	if (decode_serailno == -1 && op -> op.b_o_s && op -> op.bytes >= 8){
		if (fish_sound_identify(op->op.packet, op->op.bytes) != FISH_SOUND_UNKNOWN){
			decode_serailno = serialno;
		}
	}

	if (serialno == decode_serailno){
		fish_sound_prepare_truncation(fsound, static_cast<long>(op->op.granulepos), op->op.e_o_s);
		fish_sound_decode(fsound, op->op.packet, op->op.bytes);
	}

	std::cout << "ok good" << std::endl;
	return 0;
}

int main(int argc, char** argv)
{
	if (argc < 3){
		std::cout << "parm error" << std::endl;
		return 0;
	}

	char* infilename = argv[1];
	char* outfilename = argv[2];

	FishSound* fsound = fish_sound_new(FISH_SOUND_DECODE, nullptr);//!!  1
	fish_sound_set_interleave(fsound, 1);
	fish_sound_set_decoded_float_ilv(fsound, decoded_float, nullptr);
	//////////////////////////////////////
	OGGZ* oggz = oggz_open(infilename, OGGZ_READ);//!!  2
	if (!oggz){
		std::cout << "File not found" << std::endl;
		return 0;
	}
	oggz_set_read_callback(oggz, -1, read_packet, fsound);
	while (oggz_read(oggz, 1024) > 0); // 읽어드림

	oggz_close(oggz);//!!  2
	//////////////////////////////////////

	fish_sound_delete(fsound);
	return 0;
}