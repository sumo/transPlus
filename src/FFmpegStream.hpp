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
	int bps;
public:
	FFmpegStream(AVStream*, bool);
	virtual ~FFmpegStream();
	virtual FFmpegStream operator <<(AVPacket);

};

class FFmpegVideoStream: public FFmpegStream {
public:
	FFmpegVideoStream(AVStream*);
	virtual FFmpegStream operator <<(AVPacket);
};

class FFmpegAudioStream: public FFmpegStream {
	int sampleSize;
	int16_t *samples;
	void checkAndAllocateSampleBuffer(AVPacket);
public:
	FFmpegAudioStream(AVStream*);
	virtual FFmpegStream operator <<(AVPacket);
};

class FFmpegStreamFactory {
public:
	static FFmpegStream* createStream(AVStream*);
};
#endif /* FFMPEGSTREAM_H_ */
