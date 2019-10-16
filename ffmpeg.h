#pragma once

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

	static void initffmpeg();
	static int OpenUrl(char *filename);
	

};

