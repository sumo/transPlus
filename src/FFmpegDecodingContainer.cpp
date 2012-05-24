/*
 * FFmpegDecoder.cpp
 *
 *  Created on: 1 Nov 2011
 *      Author: mediabag
 */

#include "FFmpegDecodingContainer.hpp"

FFmpegDecodingContainer::FFmpegDecodingContainer(StreamReader sr) :
		FFmpegContainer(Logger::getInstance(LOG4CPLUS_TEXT("FFmpegDecoder"))), streamReader(
				sr) {
	probeFormat();
	probeStreams();
}

FFmpegDecodingContainer::~FFmpegDecodingContainer() {
	if (formatContext != NULL) {
		av_close_input_stream(formatContext);
	}
	if (buffer != NULL) {
		delete[] buffer;
	}
}

void FFmpegDecodingContainer::probeFormat() {
	u_int8_t* buffer = new u_int8_t[BUF_SIZE_ADJUSTED];
	streamReader.rewind();
	streamReader.read(buffer, BUF_SIZE);
	streamReader.rewind();
	AVProbeData *probeData = new AVProbeData;
	probeData->buf = buffer;
	probeData->buf_size = BUF_SIZE;
	probeData->filename = "";
	fmt = av_probe_input_format(probeData, 1);
	delete[] buffer;
	delete probeData;

	if (fmt != NULL) {
		LOG4CPLUS_DEBUG( logger,
				"Probe successful: " << fmt->name << ": " << fmt->long_name);
		probeInfo[string(fmt->name)] = string(fmt->long_name);
		//fmt->flags |= AVFMT_NOFILE;
	} else {
		throw TranscodeException("Failed to probe format");
	}
}

void FFmpegDecodingContainer::probeStreams() {
	buffer = new unsigned char[BUF_SIZE_ADJUSTED];
	context = avio_alloc_context(buffer, BUF_SIZE, 0, (void*) &streamReader,
			StreamReader::readFunction, NULL, StreamReader::seekFunction);
	context->buf_end = context->buf_ptr;
	formatContext = avformat_alloc_context();
	formatContext->pb = context;
	int i = avformat_open_input(&formatContext, "", fmt, NULL);
	if (i < 0) {
		throw TranscodeException(i);
	}

	LOG4CPLUS_DEBUG(logger, "Format: " << string(formatContext->iformat->name));
	AVStream** avStreams = formatContext->streams;
	for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
		AVStream *stream = avStreams[i];
		switch (stream->codec->codec_type) {
		case AVMEDIA_TYPE_VIDEO:
			LOG4CPLUS_DEBUG(logger, "Creating video stream");
			ffStreams.push_back(new FFmpegVideoStream(stream, formatContext));
			break;
		case AVMEDIA_TYPE_AUDIO:
			LOG4CPLUS_DEBUG(logger, "Creating audio stream");
			ffStreams.push_back(new FFmpegAudioStream(stream, formatContext));
			break;
		case AVMEDIA_TYPE_DATA:
		case AVMEDIA_TYPE_NB:
		case AVMEDIA_TYPE_SUBTITLE:
		case AVMEDIA_TYPE_UNKNOWN:
		case AVMEDIA_TYPE_ATTACHMENT:
		default:
			LOG4CPLUS_DEBUG( logger,
					"Unknown stream " << stream->codec->codec_type << " has tobe ignored");
			break;
		}
	}

	LOG4CPLUS_INFO(logger, "Found " << ffStreams.size() << " streams");
}

void FFmpegDecodingContainer::runDecodeLoop() {
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
				FFmpegStream& ffStream = ffStreams[streamIdx];

			} else {
				LOG4CPLUS_DEBUG(logger, "Ignoring new stream " << streamIdx);
			}

		}
		av_free_packet(&pkt);
	}
}

