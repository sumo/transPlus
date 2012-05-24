/*
 * Helper.cpp
 *
 *  Created on: 23 May 2012
 *      Author: mediabag
 */

#ifndef HELPER_CPP_
#define HELPER_CPP_
#include "FFmpeg.hpp"
#include <map>
#include <string>
#include <set>
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>

using namespace std;
using namespace log4cplus;

class CodecList {
	typedef map<CodecID, string> CodecNameType;
	typedef map<string, CodecID> CodecResolverType;
	Logger logger;

	CodecNameType codecNames;
	CodecResolverType codecs;
public:
	CodecList() {
		logger = Logger::getInstance(LOG4CPLUS_TEXT("CodecList"));
		AVCodec *ac = av_codec_next(NULL);
		while (ac) {
			if (ac->long_name) {
				codecNames[ac->id] = string(ac->long_name);
			}
			codecs[ac->name] = ac->id;
			ac = av_codec_next(ac);
		}

		CodecNameType::const_iterator end = codecNames.end();
		for (CodecNameType::const_iterator it = codecNames.begin(); it != end;
				++it) {
			LOG4CPLUS_DEBUG(logger, it->first << ": " << it->second);
		}
	}

	CodecID resolveCodec(string name);
	set<string> getCodecNames();
};

class ContainerList {
	typedef map<string, string> FormatMapType;
	Logger logger;

	FormatMapType formats;
public:
	ContainerList() {
		logger = Logger::getInstance(LOG4CPLUS_TEXT("ContainerList"));
		AVOutputFormat *af = av_oformat_next(NULL);
		while (af) {
			if (af->long_name) {
				formats[af->name] = string(af->long_name);
			}
			af = av_oformat_next(af);
		}

		FormatMapType::const_iterator end = formats.end();
		for (FormatMapType::const_iterator it = formats.begin(); it != end;
				++it) {
			LOG4CPLUS_DEBUG(logger, it->first << ": " << it->second);
		}
	}

	string getContainer(string name);

	map<string, string> getContainers();
};




#endif /* HELPER_CPP_ */
