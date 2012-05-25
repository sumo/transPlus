/*
 * DecodedDataObserver.h
 *
 *  Created on: 31 Oct 2011
 *      Author: mediabag
 */

#ifndef FFMPEGDECODER_H_
#define FFMPEGDECODER_H_
#include "FFmpegContainer.hpp"
#include "events/DecodedDataObserver.hpp"

using namespace log4cplus;
using namespace std;

class FFmpegDecodingContainer: public FFmpegContainer {
	StreamReader& streamReader;
	AVInputFormat *fmt;
	map<string, string> probeInfo;
	AVIOContext *context;
	boost::ptr_vector<DecodedDataObserver> observers;
	unsigned char* buffer;
	void probeFormat();
	void probeStreams();

public:
	FFmpegDecodingContainer(StreamReader);
	virtual ~FFmpegDecodingContainer();
	map<string, string> getFormat() {
		return probeInfo;
	}
	template <class DecodedDataType, class StreamType>
	void notifyObservers(Event<DecodedDataType, StreamType>& event);
	void runDecodeLoop();
};

#endif /* FFMPEGDECODER_H_ */
