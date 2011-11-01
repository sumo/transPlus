/*
 * DecodedDataObserver.h
 *
 *  Created on: 31 Oct 2011
 *      Author: mediabag
 */

#ifndef FFMPEGDECODER_H_
#define FFMPEGDECODER_H_
#include "fixstdint.hpp"
#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}
#include "StreamReader.hpp"
#include "FFmpegStream.hpp"
#include "TranscodeException.hpp"

using namespace log4cplus;
class FFmpegDecoder {
	StreamReader& streamReader;
	AVInputFormat *fmt;
	const Logger& logger;
	map<string, string> probeInfo;
	AVFormatContext *formatContext;
	vector<FFmpegStream> ffStreams;
public:
	FFmpegDecoder(StreamReader);
	virtual ~FFmpegDecoder();
	void probe();
	void extractStreams();
	void runDecodeLoop();

};

#endif /* FFMPEGDECODER_H_ */
