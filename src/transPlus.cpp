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
#include "FFmpegDecodingContainer.hpp"
#include "FFmpegEncodingContainer.hpp"
#include "streams/StreamFactory.hpp"

using namespace std;
using namespace boost;

int main(int argc, char* argv[]) {
	if(argc < 2) {
		cout << "Usage: transPlus <filename>";
		exit(255);
	}
	BasicConfigurator::doConfigure();
	avcodec_register_all();
	av_register_all();
	string filename(argv[1]);
	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("Main"));
	LOG4CPLUS_INFO(logger, avcodec_configuration());
	CodecList codecs = StreamFactory::getCodecList();
	ContainerList containers = FFmpegEncodingContainer::getContainerList();

	LOG4CPLUS_INFO(logger, "Configured with " << codecs.getCodecNames().size() << " codecs and " << containers.getContainers().size() << " containers");
	LOG4CPLUS_INFO(logger, "Using file name " << filename);
	ifstream src;
	src.open(filename.c_str(), ios::binary);
	StreamReader reader(src);
	FFmpegDecodingContainer decoder(reader);
	map<string, string> m = decoder.getFormat();
	LOG4CPLUS_INFO(logger,
			"Format of content stream is " << (*m.begin()).second);
	ptr_vector<ObservableDecodingStream>& streams = decoder.getStreams();

	ptr_vector<ObservableDecodingStream>::iterator it;
	for (it = streams.begin(); it < streams.end(); it++) {
		LOG4CPLUS_INFO(logger, "Found stream " << it->getType() << " of codec " << it->getCodec());
	}
	decoder.runDecodeLoop();
}
