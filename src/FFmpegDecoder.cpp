/*
 * FFmpegDecoder.cpp
 *
 *  Created on: 1 Nov 2011
 *      Author: mediabag
 */

#include "FFmpegDecoder.hpp"

FFmpegDecoder::FFmpegDecoder(StreamReader sr) :
		streamReader(sr), logger(
				Logger::getInstance(LOG4CPLUS_TEXT("FFmpegDecoder"))), streamsExtracted(
				false), formatProbed(false) {
}

FFmpegDecoder::~FFmpegDecoder() {
	if (formatContext != NULL) {
		av_close_input_stream(formatContext);
		avformat_free_context(formatContext);
	}
	if (context != NULL) {
		av_free(context);
	}
	if (buffer != NULL) {
		delete[] buffer;
	}
}

map<string, string> FFmpegDecoder::getFormat() {
	if (!formatProbed) {
		u_int8_t* buffer = new u_int8_t[BUFFER_SIZE];
		streamReader.rewind();
		streamReader.read(buffer, 4096);
		streamReader.rewind();
		AVProbeData *probeData = new AVProbeData;
		probeData->buf = buffer;
		probeData->buf_size = 4096;
		probeData->filename = "";
		fmt = av_probe_input_format(probeData, 1);
		delete[] buffer;
		delete probeData;

		if (fmt != NULL) {
			LOG4CPLUS_DEBUG(
					logger,
					"Probe successful: " << fmt->name << ": " << fmt->long_name);
			probeInfo[string(fmt->name)] = string(fmt->long_name);
			//fmt->flags |= AVFMT_NOFILE;
			formatProbed = true;
		} else {
			throw TranscodeException("Failed to probe format");
		}
	}
	return probeInfo;
}

boost::ptr_vector<FFmpegStream>* FFmpegDecoder::getStreams() {
	if (!streamsExtracted) {
		buffer = new unsigned char[BUFFER_SIZE];
		context = avio_alloc_context(buffer, 4096, 0,
				(void*) &streamReader, StreamReader::readFunction, NULL,
				StreamReader::seekFunction);
		context->buf_end=context->buf_ptr;
		formatContext = avformat_alloc_context();
		formatContext->pb = context;
		int i = avformat_open_input(&formatContext, "", fmt, NULL);
		if (i < 0) {
			throw TranscodeException(i);
		}

		LOG4CPLUS_DEBUG(logger,
				"Format: " << string(formatContext->iformat->name));
		AVStream** avStreams = formatContext->streams;
		for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
			AVStream *stream = avStreams[i];
			switch (stream->codec->codec_type) {
			case AVMEDIA_TYPE_VIDEO:
				LOG4CPLUS_DEBUG(logger, "Creating video stream");
				ffStreams.push_back(
						new FFmpegVideoStream(stream, formatContext));
				break;
			case AVMEDIA_TYPE_AUDIO:
				LOG4CPLUS_DEBUG(logger, "Creating audio stream");
				ffStreams.push_back(
						new FFmpegAudioStream(stream, formatContext));
				break;
			case AVMEDIA_TYPE_DATA:
			case AVMEDIA_TYPE_NB:
			case AVMEDIA_TYPE_SUBTITLE:
			case AVMEDIA_TYPE_UNKNOWN:
			case AVMEDIA_TYPE_ATTACHMENT:
			default:
				LOG4CPLUS_DEBUG(
						logger,
						"Creating unknown stream " << stream->codec->codec_type);
				ffStreams.push_back(
						new FFmpegStream(stream, formatContext, false,
								"FFmpegStream"));
				break;
			}
		}

		LOG4CPLUS_INFO(logger, "Found " << ffStreams.size() << " streams");
		streamsExtracted = true;
	}
	return &ffStreams;
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
				FFmpegStream& ffStream = ffStreams[streamIdx];
				ffStream.put(pkt);
			} else {
				LOG4CPLUS_DEBUG(logger, "Ignoring new stream " << streamIdx);
			}

		}
		av_free_packet(&pkt);
	}
}

