/*
 * FFMpegStream.h
 *
 *  Created on: 22 Sep 2011
 *      Author: mediabag
 */

#ifndef FFMPEGSTREAM_H_
#define FFMPEGSTREAM_H_
#include "fixstdint.hpp"
#include <iostream>
#include <exception>
#include <queue>
#include "TranscodeException.hpp"
#include "Picture.hpp"
#include "DecodedDataObserver.hpp"
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

using namespace log4cplus;

class FFmpegStream {

protected:
	AVStream* avStream;
	AVCodec* codec;
	AVFormatContext* formatContext;
	int bps;
	int nextPts;
	bool start;
	double pts;
	Logger logger;

public:
	static const float dtsDeltaThreshold = 10;
	FFmpegStream(AVStream*, AVFormatContext*, bool, string);
	virtual ~FFmpegStream();
	virtual void put(AVPacket);
	virtual void initPts();
	virtual void adjustTimeStamps(AVPacket);
};

template<class DecodedData>
class FFmpegStreamNotifier: public DecodedDataNotifier<DecodedData> {
	virtual DecodedData putImpl(AVPacket);
	virtual bool isValid(DecodedData);
public:
	void put(AVPacket);
	virtual ~FFmpegStreamNotifier();
};

class FFmpegVideoStream: public FFmpegStream, FFmpegStreamNotifier<Picture> {
	virtual Picture putImpl(AVPacket);
	virtual bool isValid(Picture);
public:
	FFmpegVideoStream(AVStream*, AVFormatContext*);

};

class FFmpegAudioStream: public FFmpegStream, FFmpegStreamNotifier<Sound> {
	int sampleSize;
	int16_t *samples;
	void checkAndAllocateSampleBuffer(AVPacket);
	virtual Sound putImpl(AVPacket);
	virtual bool isValid(Sound);
public:
	FFmpegAudioStream(AVStream*, AVFormatContext*);

	virtual ~FFmpegAudioStream();
};

static Picture NoPicture(NULL, PIX_FMT_NONE, 0, 0, 0, 0);
static Sound NoSound(0, 0);

#endif /* FFMPEGSTREAM_H_ */
