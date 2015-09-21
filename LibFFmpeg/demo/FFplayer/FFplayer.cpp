#include "stdafx.h"
#include <iostream>

extern "C"{
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libswscale/swscale.h>
	#include <SDL.h>
	#include <SDL_thread.h>
}

#define SDL_AUDIO_BUFFER_SIZE 1024
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

int audio_decode_frame(AVCodecContext *aCodecCtx, uint8_t *audio_buf, int buf_size) {

	static AVPacket pkt;
	static uint8_t *audio_pkt_data = NULL;
	static int audio_pkt_size = 0;
	static AVFrame frame;

	int len1, data_size = 0;

	for (;;) {
		while (audio_pkt_size > 0) {
			int got_frame = 0;
			len1 = avcodec_decode_audio4(aCodecCtx, &frame, &got_frame, &pkt);
			if (len1 < 0) {
				/* if error, skip frame */
				audio_pkt_size = 0;
				break;
			}
			audio_pkt_data += len1;
			audio_pkt_size -= len1;
			data_size = 0;
			if (got_frame) {
				data_size = av_samples_get_buffer_size(NULL,
					aCodecCtx->channels,
					frame.nb_samples,
					aCodecCtx->sample_fmt,
					1);
				memcpy(audio_buf, frame.data[0], data_size);
			}
			if (data_size <= 0) {
				/* No data yet, get more frames */
				continue;
			}
			/* We have data, return it and come back for more later */
			return data_size;
		}
		if (pkt.data)
			av_free_packet(&pkt);

		if (quit) {
			return -1;
		}

		if (packet_queue_get(&audioq, &pkt, 1) < 0) {
			return -1;
		}
		audio_pkt_data = pkt.data;
		audio_pkt_size = pkt.size;
	}
}

void audio_callback(void *userdata, Uint8 *stream, int len) {

	AVCodecContext *aCodecCtx = (AVCodecContext *)userdata;
	int len1, audio_size;

	static uint8_t audio_buf[(MAX_AUDIO_FRAME_SIZE * 3) / 2];
	static unsigned int audio_buf_size = 0;
	static unsigned int audio_buf_index = 0;

	while (len > 0) {
		if (audio_buf_index >= audio_buf_size) {
			/* We have already sent all our data; get more */
			audio_size = audio_decode_frame(aCodecCtx, audio_buf, sizeof(audio_buf));
			if (audio_size < 0) {
				/* If error, output silence */
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
		memcpy(stream, (uint8_t *)audio_buf + audio_buf_index, len1);
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

	avcodec_register_all();
	av_register_all();
	AVFormatContext *pFormatCtx = nullptr;
	if (avformat_open_input(&pFormatCtx, argv[1], nullptr, nullptr) != 0) return 0;
	if (avformat_find_stream_info(pFormatCtx, nullptr) < 0) return 0;

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

	av_dump_format(pFormatCtx, 0, argv[1], 0);

	AVCodecContext *vCodecCtx = pFormatCtx->streams[videoStream]->codec;
	AVCodec *vCodec = avcodec_find_decoder(vCodecCtx->codec_id);
	if (!vCodec) return 0;

	AVCodecContext *aCodecCtx = pFormatCtx->streams[audioStream]->codec;
	AVCodec *aCodec = avcodec_find_decoder(aCodecCtx->codec_id);
	if (!aCodec) return 0;
	
	AVDictionary* voptionsDict = nullptr;
	if (avcodec_open2(vCodecCtx, vCodec, &voptionsDict) < 0) return 0;

	SDL_AudioSpec   wanted_spec, spec;
	wanted_spec.freq = aCodecCtx->sample_rate;
	wanted_spec.format = AUDIO_S16SYS;
	wanted_spec.channels = aCodecCtx->channels;
	wanted_spec.silence = 0;
	wanted_spec.samples = SDL_AUDIO_BUFFER_SIZE;
	wanted_spec.callback = audio_callback;
	wanted_spec.userdata = aCodecCtx;

	if (SDL_OpenAudio(&wanted_spec, &spec) < 0) return 0;

	AVDictionary* aoptionsDict = nullptr;
	if (avcodec_open2(aCodecCtx, aCodec, &aoptionsDict) < 0) return 0;

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
			av_free_packet(&packet);
		} while (false);

		do{
			if (packet.stream_index != audioStream) break;
			packet_queue_put(&audioq, &packet);
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

	av_free(pFrame);
	av_free(pFrameYUV);
	av_free(buffer);

	avcodec_close(vCodecCtx);
	avcodec_close(aCodecCtx);

	avformat_close_input(&pFormatCtx);
	return 0;
}

