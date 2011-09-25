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
	virtual FFmpegStream operator <<(const AVPacket);
	string avStrError(int errorCode);

};

class FFmpegVideoStream: public FFmpegStream {
public:
	FFmpegVideoStream(AVStream*);
	FFmpegStream operator <<(const AVPacket);
};

class FFmpegAudioStream: public FFmpegStream {
	unsigned int sampleSize;
	short *samples;
	void checkAndAllocateSampleBuffer(const AVPacket);
public:
	FFmpegAudioStream(AVStream*);
	FFmpegStream operator <<(const AVPacket);
};

class FFmpegStreamFactory {
public:
	static FFmpegStream createStream(AVStream*);
};
#endif /* FFMPEGSTREAM_H_ */
