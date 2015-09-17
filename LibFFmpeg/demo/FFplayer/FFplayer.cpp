#include "stdafx.h"
#include <iostream>

extern "C"{
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libswscale/swscale.h>
	#include <SDL.h>
	#include <SDL_thread.h>
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

	av_dump_format(pFormatCtx, 0, argv[1], 0);

	unsigned int videoStream = -1;
	for (unsigned int i = 0; i < pFormatCtx->nb_streams; ++i){
		if (AVMEDIA_TYPE_VIDEO != pFormatCtx->streams[i]->codec->codec_type) continue;
		videoStream = i;
		break;
	}
	if (-1 == videoStream) return 0;

	AVCodecContext *pCodecCtx = pFormatCtx->streams[videoStream]->codec;
	AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if (!pCodec) return 0;

	AVDictionary* optionsDict = nullptr;
	if (avcodec_open2(pCodecCtx, pCodec, &optionsDict) < 0) return 0;

	AVFrame *pFrame = avcodec_alloc_frame();
	AVFrame *pFrameYUV = avcodec_alloc_frame();
		

	//SDL_Window* screen = SDL_CreateWindow("My Game Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, pCodecCtx->width/2, pCodecCtx->height/2, SDL_WINDOW_OPENGL);
	SDL_Window* screen = SDL_CreateWindow("My Game Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 3840, 1620, SDL_WINDOW_OPENGL);
	if (!screen) return 0;

	SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1, 0);	
	SDL_Texture *bmp = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, pCodecCtx->width, pCodecCtx->height);
	SwsContext *sws_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, PIX_FMT_YUV420P, SWS_BILINEAR, nullptr, nullptr, nullptr);

	int numBytes = avpicture_get_size(PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);
	uint8_t* buffer = (uint8_t *)av_malloc(numBytes*sizeof(uint8_t));
	avpicture_fill((AVPicture *)pFrameYUV, buffer, PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);
	
	SDL_Rect rect = { 0, 0, pCodecCtx->width, pCodecCtx->height };
	//SDL_Rect drect = { 0, 0, pCodecCtx->width/2, pCodecCtx->height/2 };
	SDL_Rect drect = { 0, 0, 3840, 1620 };
	AVPacket  packet;
	int frameFinished{};
	SDL_Event event;

	while (av_read_frame(pFormatCtx, &packet) >= 0) {
		do{
			if (packet.stream_index != videoStream) break;			
			avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);
			if (!frameFinished) break;

			sws_scale(
				sws_ctx, (uint8_t const * const *)pFrame->data, pFrame->linesize, 0,
				pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize
				);

			SDL_UpdateTexture(bmp, &rect, pFrameYUV->data[0], pFrameYUV->linesize[0]);
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, bmp, &rect, &drect);
			SDL_RenderPresent(renderer);
			//SDL_Delay(20);
		} while (false);
				
		av_free_packet(&packet);
	
		SDL_PollEvent(&event);
		switch (event.type) {
		case SDL_QUIT:
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

	avcodec_close(pCodecCtx);

	avformat_close_input(&pFormatCtx);
	return 0;
}

