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

template <class Decoded>
class FFmpegStream {

protected:
	AVStream avStream;
	AVCodec codec;
	AVFormatContext formatContext;
	int bps;
	int nextPts;
	bool start;
	double pts;
	Logger logger;
	queue<DecodedDataObserver<Decoded> > observers;

public:
	static const float dtsDeltaThreshold = 10;
	FFmpegStream(AVStream*, AVFormatContext*, bool, string);
	virtual ~FFmpegStream();
	virtual void put(AVPacket);
	virtual void initPts();
	virtual void adjustTimeStamps(AVPacket);
	void notifyObservers(Decoded);
	void addObserver(DecodedDataObserver<Decoded>);

};

class FFmpegVideoStream: public FFmpegStream<Picture> {
public:
	FFmpegVideoStream(AVStream*, AVFormatContext*);
	virtual void put(AVPacket);
};

class FFmpegAudioStream: public FFmpegStream<Sound> {
	int sampleSize;
	int16_t *samples;
	void checkAndAllocateSampleBuffer(AVPacket);
public:
	FFmpegAudioStream(AVStream*, AVFormatContext*);
	virtual void put(AVPacket);
	virtual ~FFmpegAudioStream();
};

struct Unit {
};

template <class Decoded>
class FFmpegStreamFactory {
	Logger logger;
public:
	FFmpegStream<Decoded> createStream(AVStream*, AVFormatContext*);
	FFmpegStreamFactory() {
		logger = Logger::getInstance("FFmpegStreamFactory");
	}
};

#endif /* FFMPEGSTREAM_H_ */
