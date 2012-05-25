/*
 * FFMpegStream.h
 *
 *  Created on: 22 Sep 2011
 *      Author: mediabag
 */

#ifndef FFMPEGSTREAM_H_
#define FFMPEGSTREAM_H_
#include "../fixstdint.hpp"
#include <iostream>
#include <exception>
#include <queue>
#include "../TranscodeException.hpp"
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <boost/utility.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/shared_ptr.hpp>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

using namespace log4cplus;
using namespace std;
using namespace boost;

enum StreamType {
	AUDIO, VIDEO, UNKNOWN
};

typedef shared_ptr<AVPacket> PacketPtr;

class FFmpegStream {

protected:
	AVStream* avStream;
	AVCodec* codec;
	AVFormatContext* formatContext;
	int bps;
	int nextPts;
	bool start;
	double pts;
	int streamIndex;
	Logger logger;

public:
	static const float dtsDeltaThreshold = 10;

	FFmpegStream(AVStream* avs, AVFormatContext* fc, string streamName,
			int sIdx);
	void adjustTimeStamps(AVPacket pkt);
	virtual StreamType getType() {
		return UNKNOWN;
	}
	virtual string getCodec() {
		return string(codec->name);
	}
	virtual ~FFmpegStream() {
	}
	template<class DecodedDataType>
	shared_ptr<DecodedDataType> decode(PacketPtr);

	int getStreamIndex() {
		return streamIndex;
	}
};

#endif /* FFMPEGSTREAM_H_ */
