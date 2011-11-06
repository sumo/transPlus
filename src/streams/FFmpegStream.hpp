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
#include "../DecodedDataObserver.hpp"
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <boost/utility.hpp>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

using namespace log4cplus;
using namespace std;

enum StreamType {
	AUDIO, VIDEO, UNKNOWN
};

class FFmpegStream  {

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

	virtual void put(AVPacket);
	virtual void initPts();
	virtual void adjustTimeStamps(AVPacket);
	virtual StreamType getType() {
		return UNKNOWN;
	}
	virtual string getCodec();
	virtual ~FFmpegStream();
};

template<class DecodedData>
class FFmpegDataGenerator: public FFmpegStream {
	vector<DecodedDataObserver<DecodedData> > observers;
protected:
	virtual DecodedData* putImpl(AVPacket)=0;
	virtual bool isValid(DecodedData*)=0;
	void notifyObservers(DecodedData data) {
		typedef typename vector<DecodedDataObserver<DecodedData> >::iterator Itr;
		Itr it;
		for (it = observers.begin(); it < observers.end(); it++) {
			(*it).dataDecoded(data);
		}
	}
public:
	FFmpegDataGenerator(AVStream* avs, AVFormatContext* ctx, bool findCodec,
			string streamName) :
			FFmpegStream(avs, ctx, findCodec, streamName) {
	}

	void put(AVPacket avp) {
		DecodedData* data = putImpl(avp);
		if (isValid(data)) {
			notifyObservers(*data);
			delete data;
		}
	}
	virtual ~FFmpegDataGenerator() {

	}
	void addObserver(DecodedDataObserver<DecodedData> obs) {
		observers.push_back(obs);
	}
};

#endif /* FFMPEGSTREAM_H_ */
