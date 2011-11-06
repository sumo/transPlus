/*
 * DecodedDataObserver.h
 *
 *  Created on: 31 Oct 2011
 *      Author: mediabag
 */

#ifndef FFMPEGDECODER_H_
#define FFMPEGDECODER_H_
#include "streams/FFmpegStream.hpp"
#include "streams/FFmpegAudioStream.hpp"
#include "streams/FFmpegVideoStream.hpp"
#include "streams/StreamReader.hpp"
#include "fixstdint.hpp"
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <boost/ptr_container/ptr_vector.hpp>

using namespace log4cplus;
using namespace std;
class FFmpegDecoder {
	StreamReader& streamReader;
	AVInputFormat *fmt;
	const Logger logger;
	map<string, string> probeInfo;
	AVFormatContext *formatContext;
	AVIOContext *context;
	boost::ptr_vector<FFmpegStream> ffStreams;
	bool streamsExtracted;
	bool formatProbed;
	const static int BUFFER_SIZE = 4096 + AVPROBE_PADDING_SIZE;
	unsigned char* buffer;
public:
	FFmpegDecoder(StreamReader);
	virtual ~FFmpegDecoder();
	map<string, string> getFormat();
	boost::ptr_vector<FFmpegStream>* getStreams();
	void runDecodeLoop();

};

#endif /* FFMPEGDECODER_H_ */
