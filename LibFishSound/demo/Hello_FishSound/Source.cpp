#include <oggz\oggz.h>
#include <ogg\ogg.h>
#include<fishsound\fishsound.h>
#include <iostream>
#include <sndfile.h>

static char * infilename, *outfilename;
static int begun = 0;
static FishSoundInfo fsinfo;
static SNDFILE * sndfile;

long decode_serialno = -1;

static int
open_output(int samplerate, int channels){
	SF_INFO sfinfo;

	sfinfo.samplerate = samplerate;
	sfinfo.channels = channels;
	sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
	sndfile = sf_open(outfilename, SFM_WRITE, &sfinfo);
	return 0;
}

int decoded_float(FishSound* fsound, float** pcm, long frames, void* user_data){
	if (!begun) {
		fish_sound_command(fsound, FISH_SOUND_GET_INFO, &fsinfo,
			sizeof (FishSoundInfo));
		open_output(fsinfo.samplerate, fsinfo.channels);
		begun = 1;
	}

	sf_writef_float(sndfile, (float *)pcm, frames);
	return 0;
}

int read_packet(OGGZ * oggz, oggz_packet * opm, long serialno,void * user_data){
	FishSound * fsound = (FishSound *)user_data;
	ogg_packet* op = &opm->op;

	if (decode_serialno == -1 && op->b_o_s && op->bytes >= 8) {
		if (fish_sound_identify(op->packet, op->bytes) != FISH_SOUND_UNKNOWN)
			decode_serialno = serialno;
	}

	if (serialno == decode_serialno) {
		fish_sound_prepare_truncation(fsound, op->granulepos, op->e_o_s);
		fish_sound_decode(fsound, op->packet, op->bytes);
	}

	return 0;
}

int main(int argc, char** argv){
	if (argc < 3){
		std::cout << "parm error" << std::endl;
		return 0;
	}

	infilename = argv[1];
	outfilename = argv[2];

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

	sf_close(sndfile);
	return 0;
}