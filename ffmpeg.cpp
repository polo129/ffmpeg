#include "ffmpeg.h"



ffmpeg::ffmpeg()
{
}


ffmpeg::~ffmpeg()
{
}

void ffmpeg::initffmpeg()
{
	av_register_all();
	avcodec_register_all();
	avdevice_register_all();
	avformat_network_init();
}

int ffmpeg::OpenUrl(char * filename)
{
	int rec = 0;
	AVFormatContext * inContext = NULL, *outContext = NULL;
	AVCodec *audioCodec = NULL, *videoCodec = NULL;
	AVCodecContext *audioContext = NULL, *videoContext = NULL;
	AVOutputFormat * outFormat = NULL;
	AVInputFormat * inFormat = NULL;
	SwsContext *swsContext = NULL;
	int audioStream = -1, videoStream = -1;

	rec = avformat_open_input(&inContext, filename, NULL, NULL);
	if (rec != 0)
		return rec;

	rec = avformat_find_stream_info(inContext, NULL);
	if (rec != 0)
		return rec;

	for (unsigned int s = 0; s < inContext->nb_streams; ++s)
	{
		av_dump_format(inContext, s, filename, FALSE);
		if (inContext->streams[s]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO && audioStream < 0)
		{
			audioStream = s;
		}
		else if (inContext->streams[s]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && videoStream < 0)
		{
			videoStream = s;
		}
	}

	if (audioStream < 0 && videoStream < 0) 
		return -1;

	if (audioStream > 0)
	{

	}
	
	if (videoStream > 0)
	{

	}
	return rec;
}
