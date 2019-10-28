#include "ffmpeg.h"

ffmpeg::ffmpeg()
{
}


ffmpeg::~ffmpeg()
{
	if (frameBuffer)
		av_free(frameBuffer);
	if (pFrame)
		av_frame_free(&pFrame);
	if (pFrameRGB)
		av_frame_free(&pFrameRGB);
	if (videoContext)
		avcodec_close(videoContext);
	if (audioContext)
		avcodec_close(audioContext);
	if (swsContext)
		sws_freeContext(swsContext);
	if (swrContext)
		swr_free(&swrContext);
	if (inContext)
		avformat_close_input(&inContext);
	if (outContext)
		avformat_free_context(outContext);
}

void ffmpeg::initffmpeg()
{
	av_register_all();
	avcodec_register_all();
	avdevice_register_all();
	avformat_network_init();
	rec = 0;
	inContext = NULL;
	outContext = NULL;
	audioCodec = NULL;
	videoCodec = NULL;
	audioContext = NULL;
	videoContext = NULL;
	outFormat = NULL;
	inFormat = NULL;
	swsContext = NULL;
	swrContext = NULL;
	audioStream = -1;
	videoStream = -1;
	pFrame = NULL;
	pFrameRGB = NULL;
}

int ffmpeg::OpenUrl(char * filename)
{

	// file I/O context: demuxers read a media file and split it into chunks of data (packets)
	rec = avformat_open_input(&inContext, filename, NULL, NULL);
	if (rec != 0)
		return rec;

	// read packets of a media file to get stream information
	rec = avformat_find_stream_info(inContext, NULL);
	if (rec != 0)
		return rec;

	for (unsigned int s = 0; s < inContext->nb_streams; ++s)
	{
		//show information metadata
		av_dump_format(inContext, s, filename, false);

		// find video stream
		if (inContext->streams[s]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO && audioStream < 0)
		{
			audioStream = s;
		}

		// find audio stream
		else if (inContext->streams[s]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && videoStream < 0)
		{
			videoStream = s;
		}
	}

	if (audioStream < 0 && videoStream < 0) 
		return -1;

	if (audioStream >= 0)
	{
		setupAudioCodex(audioStream);
	}
	
	if (videoStream >= 0)
	{
		setupVideoCodex(videoStream);
	}

	return rec;
}

int ffmpeg::setupAudioCodex(int audioStream)
{
	// retrieve audio codec
	audioCodec = avcodec_find_decoder(inContext->streams[audioStream]->codecpar->codec_id);
	if (!audioCodec)
		return -1;

	// retrieve audio codec context
	audioContext = avcodec_alloc_context3(audioCodec);
	if (!audioContext) 
		return -1;

	//copy codec context
	rec = avcodec_parameters_to_context(audioContext, inContext->streams[audioStream]->codecpar);
	if (rec != 0)
		return rec;

	//open codec context
	rec = avcodec_open2(audioContext, audioCodec, NULL);
	if (rec != 0)
		return rec;

	//struct setup for audio
	swrContext = swr_alloc();
	if (!swrContext)
		return -1;

	//setup audio parameter
	av_opt_set_channel_layout(swrContext, "in_channel_layout", audioContext->channel_layout, 0);
	av_opt_set_channel_layout(swrContext, "out_channel_layout", audioContext->channel_layout, 0);
	av_opt_set_int(swrContext, "in_sample_rate", audioContext->sample_rate, 0);
	av_opt_set_int(swrContext, "out_sample_rate", audioContext->sample_rate, 0);
	av_opt_set_sample_fmt(swrContext, "in_sample_fmt", audioContext->sample_fmt, 0);
	av_opt_set_sample_fmt(swrContext, "out_sample_fmt", AV_SAMPLE_FMT_FLT, 0);

	rec = swr_init(swrContext);
	if (rec != 0)
		return rec;

	if (audioCodec)
	{

	}
	return 0;
}

int ffmpeg::setupVideoCodex(int videoStream)
{
	// retrieve video codec
	videoCodec = avcodec_find_decoder(inContext->streams[videoStream]->codecpar->codec_id);
	if (!videoCodec)
		return -1;

	// retrieve video codec context
	videoContext = avcodec_alloc_context3(videoCodec);
	if (!videoContext)
		return -1;

	//copy codec context
	rec = avcodec_parameters_to_context(videoContext, inContext->streams[videoStream]->codecpar);
	if (rec != 0)
		return rec;

	//open codec context
	rec = avcodec_open2(videoContext, videoCodec, NULL);
	if (rec != 0)
		return rec;

	//struct setup for video 
	//setup SWSContext to convert the image data
	swsContext = sws_getContext(videoContext->width,
		videoContext->height,
		videoContext->pix_fmt,
		videoContext->width,
		videoContext->height,
		AV_PIX_FMT_RGB24,
		SWS_BILINEAR,
		NULL,
		NULL,
		NULL);

	if (!swsContext)
		return -1;
	if (videoCodec)
	{
		pwidth = inContext->streams[videoStream]->codecpar->width;
		pheight = inContext->streams[videoStream]->codecpar->height;

		pFrame = av_frame_alloc();
		if (!pFrame)
			return -1;

		pFrameRGB = av_frame_alloc();
		if (!pFrameRGB)
			return -1;

		numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, pwidth, pheight, 8);

		if (numBytes < 1)
		{
			rec = numBytes;
			return rec;
		}

		frameBuffer = (uint8_t*)av_malloc(numBytes);
		if (!frameBuffer)
			return -1;

		rec = av_image_fill_arrays(&pFrameRGB->data[0], &pFrameRGB->linesize[0], frameBuffer, AV_PIX_FMT_RGB24, pwidth, pheight, 1);
		if (rec < 0)
			return rec;
	}
	return 0;
}

int ffmpeg::decodeAudio(AVCodecContext * dec_ctx, AVFrame * frame, AVPacket * pkt)
{
		rec = avcodec_send_packet(dec_ctx, pkt);
		if (rec)
			return rec;

		while (!avcodec_receive_frame(dec_ctx, pFrame))
		{
			printf(
				"audio #### Frame \n"
			);
		}
	return 0;
}

int ffmpeg::decodeVideo(AVCodecContext * dec_ctx, AVFrame * frame, AVPacket * pkt)
{
	rec = avcodec_send_packet(dec_ctx, pkt);
	if (rec < 0)
	{
		printf("Error sending packet for decoding.\n");
		return -1;
	}

	while (rec >= 0)
	{
		rec = avcodec_receive_frame(dec_ctx, pFrame);

		if (rec == AVERROR(EAGAIN) || rec == AVERROR_EOF)
		{
			return -1;
		}
		else if (rec < 0)
		{
			printf("Error while decoding.\n");
			return -1;
		}

		sws_scale(swsContext,
			pFrame->data,
			pFrame->linesize,
			0,
			dec_ctx->height,
			pFrameRGB->data,
			pFrameRGB->linesize);


			printf(
				"Frame %c (%d) pts %d dts %d key_frame %d [coded_picture_number %d, display_picture_number %d, %dx%d]\n",
				av_get_picture_type_char(pFrame->pict_type),
				dec_ctx->frame_number,
				pFrame->pts,
				pFrame->pkt_dts,
				pFrame->key_frame,
				pFrame->coded_picture_number,
				pFrame->display_picture_number,
				dec_ctx->width,
				dec_ctx->height
			);
	
	}
	return 0;
}

void ffmpeg::decode()
{
	AVPacket packet;
	while (av_read_frame(inContext, &packet) >= 0)
	{
		if (packet.stream_index == videoStream)
		{
			decodeVideo(videoContext, pFrame, &packet);
		}
		else if (packet.stream_index == audioStream)
		{
			decodeAudio(audioContext, pFrame, &packet);
		}
	}
}

