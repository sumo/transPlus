/*
 * FFMpegStream.h
 *
 *  Created on: 22 Sep 2011
 *      Author: mediabag
 */

#include "fixstdint.hpp"
#include <iostream>
#include <exception>
#include "TranscodeException.hpp"
#ifndef FFMPEGSTREAM_H_
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}
#define FFMPEGSTREAM_H_

class FFmpegStream {

protected:
	AVStream avStream;
	AVCodec codec;
	AVFormatContext formatContext;
	int bps;
	int nextPts;
	bool start;
	double pts;
public:
	static const float dtsDeltaThreshold = 10;
	FFmpegStream(AVStream*, AVFormatContext*, bool);
	virtual ~FFmpegStream();
	virtual FFmpegStream operator <<(AVPacket);
	virtual void initPts();
	virtual void adjustTimeStamps(AVPacket);

};

class FFmpegVideoStream: public FFmpegStream {
public:
	FFmpegVideoStream(AVStream*, AVFormatContext*);
	virtual FFmpegStream operator <<(AVPacket);
};

class FFmpegAudioStream: public FFmpegStream {
	int sampleSize;
	int16_t *samples;
	void checkAndAllocateSampleBuffer(AVPacket);
public:
	FFmpegAudioStream(AVStream*, AVFormatContext*);
	virtual FFmpegStream operator <<(AVPacket);
};

class FFmpegStreamFactory {
public:
	static FFmpegStream* createStream(AVStream*, AVFormatContext*);
};
#endif /* FFMPEGSTREAM_H_ */
