#include "stdafx.h"
#include <iostream>

extern "C"{
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libswscale/swscale.h>
	#include  <libswresample/swresample.h>
	#include <SDL.h>
	#include <SDL_thread.h>
}

#include <thread>

#if 0
#define MAX_AUDIO_FRAME_SIZE 192000 // 1 second of 48khz 32bit audio

//Output PCM
#define OUTPUT_PCM 0
//Use SDL
#define USE_SDL 1

//Buffer:
//|-----------|-------------|
//chunk-------pos---len-----|
static  Uint8  *audio_chunk;
static  Uint32  audio_len;
static  Uint8  *audio_pos;


void  fill_audio(void *udata, Uint8 *stream, int len) {
	//SDL 2.0
	SDL_memset(stream, 0, len);
	if (audio_len == 0)
		return;

	len = (len>audio_len ? audio_len : len);	

	SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
	audio_pos += len;
	audio_len -= len;
}
//-----------------

#ifdef main
#undef main 
#endif

int main(int argc, char* argv[])
{
	AVFormatContext	*pFormatCtx;
	int				i, audioStream;
	AVCodecContext	*pCodecCtx;
	AVCodec			*pCodec;
	AVPacket		*packet;
	uint8_t			*out_buffer;
	AVFrame			*pFrame;
	SDL_AudioSpec wanted_spec;
	int ret;
	uint32_t len = 0;
	int got_picture;
	int index = 0;
	int64_t in_channel_layout;
	struct SwrContext *au_convert_ctx;

	FILE *pFile = NULL;
	char*  url = argv[1];

	av_register_all();
	avformat_network_init();
	pFormatCtx = avformat_alloc_context();
	//Open
	if (avformat_open_input(&pFormatCtx, url, NULL, NULL) != 0) {
		printf("Couldn't open input stream.\n");
		return -1;
	}
	// Retrieve stream information
	if (avformat_find_stream_info(pFormatCtx, NULL)<0) {
		printf("Couldn't find stream information.\n");
		return -1;
	}
	// Dump valid information onto standard error
	av_dump_format(pFormatCtx, 0, url, false);

	// Find the first audio stream
	audioStream = -1;
	for (i = 0; i < pFormatCtx->nb_streams; i++)
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
			audioStream = i;
			break;
		}

	if (audioStream == -1) {
		printf("Didn't find a audio stream.\n");
		return -1;
	}

	// Get a pointer to the codec context for the audio stream
	pCodecCtx = pFormatCtx->streams[audioStream]->codec;

	// Find the decoder for the audio stream
	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if (pCodec == NULL) {
		printf("Codec not found.\n");
		return -1;
	}

	// Open codec
	if (avcodec_open2(pCodecCtx, pCodec, NULL)<0) {
		printf("Could not open codec.\n");
		return -1;
	}


#if OUTPUT_PCM
	pFile = fopen("output.pcm", "wb");
#endif

	packet = (AVPacket *)av_malloc(sizeof(AVPacket));
	av_init_packet(packet);

	//Out Audio Param
	uint64_t out_channel_layout = AV_CH_LAYOUT_STEREO;
	int out_nb_samples = pCodecCtx->frame_size;
	AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
	int out_sample_rate = pCodecCtx->sample_rate;
	int out_channels = av_get_channel_layout_nb_channels(out_channel_layout);
	//Out Buffer Size
	int out_buffer_size = av_samples_get_buffer_size(NULL, out_channels, out_nb_samples, out_sample_fmt, 1);

	out_buffer = (uint8_t *)av_malloc(MAX_AUDIO_FRAME_SIZE * 2);
	pFrame = av_frame_alloc();
	//SDL------------------
#if USE_SDL
	//Init
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
		printf("Could not initialize SDL - %s\n", SDL_GetError());
		return -1;
	}
	//SDL_AudioSpec
	wanted_spec.freq = out_sample_rate;
	wanted_spec.format = AUDIO_S16SYS;
	wanted_spec.channels = pCodecCtx->channels;
	wanted_spec.silence = 0;
	wanted_spec.samples = out_nb_samples;
	wanted_spec.callback = fill_audio;
	wanted_spec.userdata = pCodecCtx;

	if (SDL_OpenAudio(&wanted_spec, NULL)<0) {
		printf("can't open audio.\n");
		return -1;
	}
#endif

	//FIX:Some Codec's Context Information is missing
	in_channel_layout = av_get_default_channel_layout(pCodecCtx->channels);
	//Swr

	au_convert_ctx = swr_alloc();
	au_convert_ctx = swr_alloc_set_opts(au_convert_ctx, out_channel_layout, out_sample_fmt, out_sample_rate,
		in_channel_layout, pCodecCtx->sample_fmt, pCodecCtx->sample_rate, 0, NULL);
	swr_init(au_convert_ctx);

	//Play
	SDL_PauseAudio(0);

	while (av_read_frame(pFormatCtx, packet) >= 0) {
		if (packet->stream_index == audioStream) {
			ret = avcodec_decode_audio4(pCodecCtx, pFrame, &got_picture, packet);
			if (ret < 0) {
				printf("Error in decoding audio frame.\n");
				return -1;
			}
			if (got_picture > 0) {
				swr_convert(au_convert_ctx, &out_buffer, MAX_AUDIO_FRAME_SIZE, (const uint8_t **)pFrame->data, pFrame->nb_samples);
#if 1
				printf("index:%5d\t pts:%lld\t packet size:%d\n", index, packet->pts, packet->size);
#endif


#if OUTPUT_PCM
				//Write PCM
				fwrite(out_buffer, 1, out_buffer_size, pFile);
#endif
				index++;
			}

#if USE_SDL
			while (audio_len>0)//Wait until finish
				SDL_Delay(1);

			//Set audio buffer (PCM data)
			audio_chunk = (Uint8 *)out_buffer;
			//Audio buffer length
			audio_len = out_buffer_size;
			audio_pos = audio_chunk;

#endif
		}
		av_free_packet(packet);
	}

	swr_free(&au_convert_ctx);

#if USE_SDL
	SDL_CloseAudio();//Close SDL
	SDL_Quit();
#endif

#if OUTPUT_PCM
	fclose(pFile);
#endif
	av_free(out_buffer);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);

	return 0;
}

#else 

#define SDL_AUDIO_BUFFER_SIZE 8096
#define MAX_AUDIO_FRAME_SIZE 192000
int quit = 0;

typedef struct PacketQueue {
	AVPacketList *first_pkt, *last_pkt;
	int nb_packets;
	int size;
	SDL_mutex *mutex;
	SDL_cond *cond;
} PacketQueue;

PacketQueue audioq;

void packet_queue_init(PacketQueue *q) {
	memset(q, 0, sizeof(PacketQueue));
	q->mutex = SDL_CreateMutex();
	q->cond = SDL_CreateCond();
}
int packet_queue_put(PacketQueue *q, AVPacket *pkt) {

	AVPacketList *pkt1;
	if (av_dup_packet(pkt) < 0) {
		return -1;
	}
	pkt1 = (AVPacketList *)av_malloc(sizeof(AVPacketList));
	if (!pkt1)
		return -1;
	pkt1->pkt = *pkt;
	pkt1->next = NULL;


	SDL_LockMutex(q->mutex);

	if (!q->last_pkt)
		q->first_pkt = pkt1;
	else
		q->last_pkt->next = pkt1;
	q->last_pkt = pkt1;
	q->nb_packets++;
	q->size += pkt1->pkt.size;

	SDL_CondSignal(q->cond);
	SDL_UnlockMutex(q->mutex);
	return 0;
}
static int packet_queue_get(PacketQueue *q, AVPacket *pkt, int block)
{
	AVPacketList *pkt1;
	int ret;

	SDL_LockMutex(q->mutex);

	for (;;) {

		if (quit) {
			ret = -1;
			break;
		}

		pkt1 = q->first_pkt;
		if (pkt1) {
			q->first_pkt = pkt1->next;
			if (!q->first_pkt)
				q->last_pkt = NULL;
			q->nb_packets--;
			q->size -= pkt1->pkt.size;
			*pkt = pkt1->pkt;
			av_free(pkt1);
			ret = 1;
			break;
		}
		else if (!block) {
			ret = 0;
			break;
		}
		else {
			SDL_CondWait(q->cond, q->mutex);
		}
	}
	SDL_UnlockMutex(q->mutex);
	return ret;
}

struct SwrContext * au_convert_ctx_external = nullptr;

int audio_decode_frame(AVCodecContext *aCodecCtx, uint8_t *audio_buf, int buf_size) {

	AVPacket* pkt= (AVPacket *)av_malloc(sizeof(AVPacket));
	av_init_packet(pkt);

	int audio_pkt_size = 0;
	AVFrame* frame= av_frame_alloc();
	int len1, data_size = 0;

	for (;;) {
		while (audio_pkt_size > 0) {
			int got_frame = 0;
			len1 = avcodec_decode_audio4(aCodecCtx, frame, &got_frame, pkt);
			if (len1 < 0) {
				audio_pkt_size = 0;
				break;
			}

			if (got_frame > 0) {
				swr_convert(au_convert_ctx_external, &audio_buf, MAX_AUDIO_FRAME_SIZE, (const uint8_t **)frame->data, frame->nb_samples);
			}

			audio_pkt_size -= len1;
			data_size = buf_size;
			av_frame_free(&frame);
			return buf_size;
		}
		if (pkt->data)
			av_free_packet(pkt);

		if (quit) {
			av_frame_free(&frame);
			return -1;
		}

		if (packet_queue_get(&audioq, pkt, 1) < 0) {
			av_frame_free(&frame);
			return -1;
		}
		audio_pkt_size = pkt->size;
	}
}

void audio_callback(void *userdata, Uint8 *stream, int len) {

	std::cout << "Packet size:" << len << std::endl;
	AVCodecContext *aCodecCtx = (AVCodecContext *)userdata;
	int len1, audio_size;

	uint8_t audio_buf[MAX_AUDIO_FRAME_SIZE *  2];
	unsigned int audio_buf_size = 0;
	unsigned int audio_buf_index = 0;

	while (len > 0) {
		if (audio_buf_index >= audio_buf_size) {
			audio_size = audio_decode_frame(aCodecCtx, audio_buf, sizeof(audio_buf));
			if (audio_size < 0) {
				audio_buf_size = 1024; // arbitrary?
				memset(audio_buf, 0, audio_buf_size);
			}
			else {
				audio_buf_size = audio_size;
			}
			audio_buf_index = 0;
		}
		len1 = audio_buf_size - audio_buf_index;
		if (len1 > len)
			len1 = len;
		std::memcpy(stream, (uint8_t *)audio_buf + audio_buf_index, len1);
		SDL_Delay(1);
		len -= len1;
		stream += len1;
		audio_buf_index += len1;
	}
}

#ifdef main
#undef main 
#endif

int main(int argc, char** argv){
	if (argc < 2) {
		std::cout << "Usage: test <file>"<<std::endl;
		return 0;
	}
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) return 0;

	av_register_all();

	AVFormatContext *pFormatCtx = nullptr;
	if (avformat_open_input(&pFormatCtx, argv[1], nullptr, nullptr) != 0) return 0;


	if (avformat_find_stream_info(pFormatCtx, nullptr) < 0) return 0;

	av_dump_format(pFormatCtx, 0, argv[1], 0);

	unsigned int videoStream = -1;
	unsigned int audioStream = -1;
	for (unsigned int i = 0; i < pFormatCtx->nb_streams; ++i){
		if (AVMEDIA_TYPE_VIDEO == pFormatCtx->streams[i]->codec->codec_type) {
			videoStream = i;
			continue;
		}
		if (AVMEDIA_TYPE_AUDIO == pFormatCtx->streams[i]->codec->codec_type) {
			audioStream = i;
			continue;
		}
	}
	if (-1 == videoStream || -1 == audioStream) return 0;

	AVCodecContext *vCodecCtx = pFormatCtx->streams[videoStream]->codec;
	AVCodec *vCodec = avcodec_find_decoder(vCodecCtx->codec_id);
	if (!vCodec) return 0;

	AVCodecContext *aCodecCtx = pFormatCtx->streams[audioStream]->codec;
	AVCodec *aCodec = avcodec_find_decoder(aCodecCtx->codec_id);
	if (!aCodec) return 0;
	
	if (avcodec_open2(vCodecCtx, vCodec, nullptr) < 0) return 0;

	uint64_t out_channel_layout = AV_CH_LAYOUT_STEREO;
	int out_nb_samples = aCodecCtx->frame_size;
	AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
	int out_sample_rate = aCodecCtx->sample_rate;
	int out_channels = av_get_channel_layout_nb_channels(out_channel_layout);
	int out_buffer_size = av_samples_get_buffer_size(NULL, out_channels, out_nb_samples, out_sample_fmt, 1);


	SDL_AudioSpec   wanted_spec;
	wanted_spec.freq = out_sample_rate;
	wanted_spec.format = AUDIO_S16SYS;
	wanted_spec.channels = aCodecCtx->channels;
	wanted_spec.silence = 0;
	wanted_spec.samples = out_nb_samples;
	wanted_spec.callback = audio_callback;
	wanted_spec.userdata = aCodecCtx;

	if (SDL_OpenAudio(&wanted_spec, nullptr) < 0) return 0;
	if(avcodec_open2(aCodecCtx, aCodec, nullptr) < 0) return 0;

	// FIXED
	//FIX:Some Codec's Context Information is missing
	auto in_channel_layout = av_get_default_channel_layout(aCodecCtx->channels);
	//Swr

	auto au_convert_ctx = swr_alloc();

	au_convert_ctx = swr_alloc_set_opts(au_convert_ctx, out_channel_layout, out_sample_fmt, out_sample_rate,
		in_channel_layout, aCodecCtx->sample_fmt, aCodecCtx->sample_rate, 0, NULL);
	swr_init(au_convert_ctx);
	au_convert_ctx_external = au_convert_ctx;

	packet_queue_init(&audioq);
	SDL_PauseAudio(0);

	
	SDL_Window* screen = SDL_CreateWindow("My Game Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, vCodecCtx->width, vCodecCtx->height, SDL_WINDOW_OPENGL);
	if (!screen) return 0;

	SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1, 0);	
	SDL_Texture *bmp = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, vCodecCtx->width, vCodecCtx->height);
	SwsContext *sws_ctx = sws_getContext(vCodecCtx->width, vCodecCtx->height, vCodecCtx->pix_fmt, vCodecCtx->width, vCodecCtx->height, PIX_FMT_YUV420P, SWS_BILINEAR, nullptr, nullptr, nullptr);
	
	int numBytes = avpicture_get_size(PIX_FMT_YUV420P, vCodecCtx->width, vCodecCtx->height);
	uint8_t* buffer = (uint8_t *)av_malloc(numBytes*sizeof(uint8_t));
	AVFrame *pFrameYUV = av_frame_alloc();
	avpicture_fill((AVPicture *)pFrameYUV, buffer, PIX_FMT_YUV420P, vCodecCtx->width, vCodecCtx->height);
	
	SDL_Rect rect = { 0, 0, vCodecCtx->width, vCodecCtx->height };
	SDL_Rect drect = { 0, 0, vCodecCtx->width, vCodecCtx->height };

	int done{};
	SDL_Event event;
	AVPacket  packet; 
	av_init_packet(&packet);

	AVFrame *pFrame = av_frame_alloc();
	while (av_read_frame(pFormatCtx, &packet) >= 0) {
		do{
			if (packet.stream_index != videoStream) break;			
			avcodec_decode_video2(vCodecCtx, pFrame, &done, &packet);
			if (!done) break;

			sws_scale(
				sws_ctx, (const uint8_t* const *)pFrame->data, pFrame->linesize, 0,
				vCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize
				);

			SDL_UpdateTexture(bmp, &rect, pFrameYUV->data[0], pFrameYUV->linesize[0]);
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, bmp, &rect, &drect);
			SDL_RenderPresent(renderer);	

			SDL_Delay(50);

			av_free_packet(&packet);
			
		} while (false);

		do{
			if (packet.stream_index != audioStream) break;
			packet_queue_put(&audioq, &packet);
			SDL_Delay(1);
		} while (false);

		if (packet.stream_index != audioStream && packet.stream_index != videoStream){
			av_free_packet(&packet);
		}
						
		SDL_PollEvent(&event);
		switch (event.type) {
		case SDL_QUIT:
			quit = 1;
			SDL_Quit();
			exit(0);
			break;
		default:
			break;
		}

		
	}

	SDL_DestroyTexture(bmp);

	swr_free(&au_convert_ctx);

	av_free(pFrame);
	av_free(pFrameYUV);
	av_free(buffer);

	avcodec_close(vCodecCtx);
	avcodec_close(aCodecCtx);

	avformat_close_input(&pFormatCtx);
	return 0;
}

#endif