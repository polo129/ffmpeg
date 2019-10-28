#pragma once
#include <stdio.h>
#define __STDC_CONSTANT_MACROS

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
	 int decodeAudio(AVCodecContext *dec_ctx, AVFrame *frame, AVPacket *pkt);
	 int decodeVideo(AVCodecContext *dec_ctx, AVFrame *frame, AVPacket *pkt);
	 void decode();

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
	AVFrame *pFrame, *pFrameRGB;
	uint8_t *frameBuffer;
	int numBytes, pwidth, pheight;
	int iVideo, iAudio;
};

