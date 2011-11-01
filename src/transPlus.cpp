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
#include "StreamReader.hpp"
#include "FFmpegStream.hpp"

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

	unsigned char* buffer = new unsigned char[4096 + AVPROBE_PADDING_SIZE];

	LOG4CPLUS_INFO(logger, "Using file name " << filename);
	ifstream src;
	src.open(filename.c_str(), ios::binary);
	src.read(reinterpret_cast<char*>(buffer), 4096);
	src.seekg(ios_base::beg);
	StreamReader reader(src);

}
