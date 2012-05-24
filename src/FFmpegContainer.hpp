/*
 * FFmpegProcessor.hpp
 *
 *  Created on: 21 May 2012
 *      Author: mediabag
 */

#ifndef FFMPEGPROCESSOR_HPP_
#define FFMPEGPROCESSOR_HPP_
#include "FFmpeg.hpp"
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <boost/ptr_container/ptr_vector.hpp>
#include "streams/FFmpegStream.hpp"
#include "streams/FFmpegAudioStream.hpp"
#include "streams/FFmpegVideoStream.hpp"
#include "streams/StreamReader.hpp"

using namespace log4cplus;
using namespace std;

class FFmpegContainer {
protected:
	const Logger logger;
	AVFormatContext *formatContext;
	AVIOContext *context;
	boost::ptr_vector<FFmpegStream> ffStreams;
	const static int BUF_SIZE = 4096;
	const static int BUF_SIZE_ADJUSTED = 4096 + AVPROBE_PADDING_SIZE;
	FFmpegContainer(const Logger& l) :
			logger(l) {
		formatContext = NULL;
		context = NULL;
	};
	virtual ~FFmpegContainer() {
		if (formatContext != NULL) {
			avformat_free_context(formatContext);
		}
		if (context != NULL) {
			av_free(context);
		}
	}
public:
	boost::ptr_vector<FFmpegStream>& getStreams() {
		return ffStreams;
	}
};

void checkForError(int code);
#endif /* FFMPEGPROCESSOR_HPP_ */
