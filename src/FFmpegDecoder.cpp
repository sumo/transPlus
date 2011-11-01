/*
 * FFmpegDecoder.cpp
 *
 *  Created on: 1 Nov 2011
 *      Author: mediabag
 */

#include "FFmpegDecoder.hpp"

FFmpegDecoder::FFmpegDecoder(StreamReader sr) :
		streamReader(sr), logger(Logger::getInstance(LOG4CPLUS_TEXT("FFmpegDecoder"))) {
}

FFmpegDecoder::~FFmpegDecoder() {
	// TODO Auto-generated destructor stub
}

void FFmpegDecoder::probe() {
	unsigned char* buffer = new unsigned char[4096 + AVPROBE_PADDING_SIZE];
	AVProbeData *probeData = new AVProbeData;
	probeData->buf = buffer;
	probeData->buf_size = 4096;
	fmt = av_probe_input_format(probeData, 1);
	delete buffer;
	if (fmt != NULL) {
		LOG4CPLUS_INFO(logger,
				"Probe successful: " << fmt->name << ": " << fmt->long_name);
		probeInfo[string(fmt->name)] = string(fmt->long_name);
	} else {
		throw new TranscodeException("Failed to probe format");
	}
}

void FFmpegDecoder::extractStreams() {
	unsigned char* buffer = new unsigned char[4096 + AVPROBE_PADDING_SIZE];
	AVIOContext *context = avio_alloc_context(buffer, 4096, URL_RDONLY,
			(void*) &streamReader, StreamReader::readFunction, NULL,
			StreamReader::seekFunction);
	formatContext = new AVFormatContext;
	if (int i = av_open_input_stream(&formatContext, context, "", fmt, NULL) < 0) {
		throw new TranscodeException(i);
	}

	LOG4CPLUS_INFO(logger, "Format: " << formatContext->iformat->name);
	AVStream** avStreams = formatContext->streams;
	for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
		AVStream *stream = avStreams[i];
		switch (stream->codec->codec_type) {
		case AVMEDIA_TYPE_VIDEO:
			LOG4CPLUS_WARN(logger, "Creating video stream");
			ffStreams.push_back(FFmpegVideoStream(stream, formatContext));
			break;
		case AVMEDIA_TYPE_AUDIO:
			LOG4CPLUS_WARN(logger, "Creating audio stream");
			ffStreams.push_back(FFmpegAudioStream(stream, formatContext));
			break;
		case AVMEDIA_TYPE_DATA:
		case AVMEDIA_TYPE_NB:
		case AVMEDIA_TYPE_SUBTITLE:
		case AVMEDIA_TYPE_UNKNOWN:
		case AVMEDIA_TYPE_ATTACHMENT:
		default:
			LOG4CPLUS_DEBUG(logger,
					"Creating unknown stream " << stream->codec->codec_type);
			ffStreams.push_back(FFmpegStream(stream, formatContext, false, "FFmpegStream"));
			break;
		}
	}

	LOG4CPLUS_INFO(logger, "Found " << ffStreams.size() << " streams");

}

void FFmpegDecoder::runDecodeLoop() {
	bool eof = false;
	while (!eof) {
		AVPacket pkt;
		int ret = av_read_frame(formatContext, &pkt);
		if (ret < 0) {
			av_free_packet(&pkt);
			if (ret != -32) {
				char* s = new char[1024];
				av_strerror(ret, s, 1024);
				LOG4CPLUS_DEBUG(logger,
						"Read frame failed: " << s << "[" << ret << "]");
				delete s;
			}
			return;
		}
		if (ret == EAGAIN) {
			LOG4CPLUS_WARN(logger, "Stream " << pkt.stream_index << " EAGAIN");
		} else {
			int streamIdx = pkt.stream_index;
			if (streamIdx < ffStreams.size()) {
				FFmpegStream ffStream = ffStreams[streamIdx];
				ffStream.put(pkt);
			} else {
				LOG4CPLUS_DEBUG(logger, "Ignoring new stream " << streamIdx);
			}

		}
		av_free_packet(&pkt);
	}
}

