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
#include <boost/shared_ptr.hpp>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}
#include "StreamReader.hpp"

using namespace std;



int main(int argc, char* argv[]) {
	avcodec_register_all();
	av_register_all();
	cout << avcodec_configuration() << endl;
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
		cout << it->first << ": " << it->second << endl;
	}

	unsigned char* buffer = new unsigned char[4096 + AVPROBE_PADDING_SIZE];
	string filename(argv[1]);
	cout << "Using file name " << filename << endl;
	ifstream src;
	src.open(filename.c_str());
	src.read(reinterpret_cast<char*> (buffer), 4096);
	src.seekg(ios_base::beg);
	StreamReader reader(src);

	AVProbeData *probeData = new AVProbeData;
	probeData->buf = buffer;
	probeData->buf_size = 4096;
	AVInputFormat *fmt = av_probe_input_format(probeData, 1);
	cout << "Probe successful: " << fmt->name << ": " << fmt->long_name << endl;

	AVIOContext *context = avio_alloc_context(buffer, 4096, URL_RDONLY, (void*)&reader,
			StreamReader::readFunction, NULL, StreamReader::seekFunction);
	AVFormatContext *formatContext = new AVFormatContext;
	if (int i = av_open_input_stream(&formatContext, context, "", fmt, NULL) < 0) {
		char* s = new char[1024];
		av_strerror(i, s, 1024);
		cout << "Read failed: " << s << endl;
		delete s;
		return 1;
	}
	cout << "Format: " << formatContext->iformat->name << endl;
	AVStream** avStreams = formatContext->streams;
	cout << "Codec: " << avStreams[0]->codec->codec_name << endl;
	return 0;
}

