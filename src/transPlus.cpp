//============================================================================
// Name        : transPlus.cpp
// Author      : Sumit Raja
// Version     :
// Copyright   : BSD Licence
// Description : Hello World in C++, Ansi-style
//============================================================================

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
#include "streams/StreamReader.hpp"
#include "FFmpegDecoder.hpp"

using namespace std;

int main(int argc, char* argv[]) {
	BasicConfigurator::doConfigure();
	avcodec_register_all();
	av_register_all();
	string filename(argv[1]);
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("Main"));
	LOG4CPLUS_INFO(logger, avcodec_configuration());
	AVCodec *ac = av_codec_next(NULL);
	typedef map<CodecID, string> CodecMapType;
	CodecMapType codecs;
	while (ac) {
		if (ac->long_name) {
			codecs[ac->id] = string(ac->long_name);
		}
		ac = av_codec_next(ac);
	}

	CodecMapType::const_iterator end = codecs.end();
	for (CodecMapType::const_iterator it = codecs.begin(); it != end; ++it) {
		LOG4CPLUS_DEBUG(logger, it->first << ": " << it->second);
	}

	LOG4CPLUS_INFO(logger, "Using file name " << filename);
	ifstream src;
	src.open(filename.c_str(), ios::binary);
	StreamReader reader(src);
	FFmpegDecoder decoder(reader);
	map<string, string> m = decoder.getFormat();
	LOG4CPLUS_INFO(logger,
			"Format of content stream is " << (*m.begin()).second);
	boost::ptr_vector<FFmpegStream> *streams = decoder.getStreams();
	typedef typename boost::ptr_vector<FFmpegStream>::iterator Itr;
	Itr it;
	for (it = streams->begin(); it < streams->end(); it++) {
		LOG4CPLUS_INFO(logger, "Found stream " << it->getType() << " of codec " << it->getCodec());
	}
	decoder.runDecodeLoop();
}
