#pragma once
#include <stdio.h>
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavfilter/avfilter.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavutil/imgutils.h>
#include <libavutil/avstring.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>

#include <SDL2\include\SDL.h>
#include <SDL2\include\SDL_thread.h>
#include <SDL2\include\SDL_syswm.h>
#include <SDL2\include\SDL_render.h>
#include <SDL2\include\SDL_audio.h>
#ifdef __cplusplus
}
#endif

class ffmpeg
{
public:
	ffmpeg();
	~ffmpeg();

	 void initffmpeg();
	 int OpenUrl(char *filename);
	 int setupAudioCodex(int);
	 int setupVideoCodex(int);
private:
	int rec;
	AVFormatContext * inContext, *outContext;
	AVCodec *audioCodec, *videoCodec;
	AVCodecContext *audioContext, *videoContext;
	AVOutputFormat * outFormat;
	AVInputFormat * inFormat;
	SwsContext *swsContext;
	SwrContext *swrContext;
	int audioStream,videoStream;
};

