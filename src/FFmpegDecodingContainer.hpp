/*
 * DecodedDataObserver.h
 *
 *  Created on: 31 Oct 2011
 *      Author: mediabag
 */

#ifndef FFMPEGDECODER_H_
#define FFMPEGDECODER_H_
#include "FFmpegContainer.hpp"

using namespace log4cplus;
using namespace std;

class FFmpegDecodingContainer : public FFmpegContainer {
	StreamReader& streamReader;
	AVInputFormat *fmt;
	map<string, string> probeInfo;
	AVIOContext *context;
	unsigned char* buffer;
	void probeFormat();
	void probeStreams();
public:
	FFmpegDecodingContainer(StreamReader);
	virtual ~FFmpegDecodingContainer();
	map<string, string> getFormat() {
			return probeInfo;
	};
	void runDecodeLoop();
};

#endif /* FFMPEGDECODER_H_ */
