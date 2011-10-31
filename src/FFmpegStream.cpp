/*
 * FFMpegStream.cpp
 *
 *  Created on: 22 Sep 2011
 *      Author: mediabag
 */

#include "FFmpegStream.hpp"

using namespace std;

template <class Decoded>
FFmpegStream<Decoded>::FFmpegStream(AVStream* avs, AVFormatContext* fc, bool findCodec,
		string streamName) {
	logger = Logger::getInstance(LOG4CPLUS_TEXT(streamName));
	avStream = *avs;
	formatContext = *fc;
	if (findCodec) {
		codec = *(avcodec_find_decoder(avs->codec->codec_id));
		int open = avcodec_open(avStream.codec, &codec);
		if (open < 0) {
			throw new TranscodeException("Failed to open codec", open);
		}
	}
	bps = av_get_bits_per_sample_format(avStream.codec->sample_fmt) >> 3;
	initPts();
}

template <class Decoded>
FFmpegStream<Decoded>::~FFmpegStream() {

}
template <class Decoded>
void FFmpegStream<Decoded>::put(AVPacket pkt) {

}

template <class Decoded>
void FFmpegStream<Decoded>::adjustTimeStamps(AVPacket pkt) {
	if (pkt.dts != AV_NOPTS_VALUE)
		pkt.dts += av_rescale_q(0, AV_TIME_BASE_Q, avStream.time_base);
	if (pkt.pts != AV_NOPTS_VALUE)
		pkt.pts += av_rescale_q(0, AV_TIME_BASE_Q, avStream.time_base);

	//input file scaling
//	if (pkt.stream_index < nb_input_files_ts_scale[file_index]
//			&& input_files_ts_scale[file_index][pkt.stream_index]) {
//		if (pkt.pts != AV_NOPTS_VALUE)
//			pkt.pts *= input_files_ts_scale[file_index][pkt.stream_index];
//		if (pkt.dts != AV_NOPTS_VALUE)
//			pkt.dts *= input_files_ts_scale[file_index][pkt.stream_index];
//	}

	if (pkt.dts != AV_NOPTS_VALUE && nextPts != AV_NOPTS_VALUE
			&& (formatContext.iformat->flags & AVFMT_TS_DISCONT)) {
		int64_t pkt_dts = av_rescale_q(pkt.dts, avStream.time_base,
				AV_TIME_BASE_Q);
		int64_t delta = pkt_dts - nextPts;
		if ((FFABS(delta) > 1LL * dtsDeltaThreshold * AV_TIME_BASE
				|| pkt_dts + 1 < pts)) { // && !copy_ts - invesitgate copy timestamps
			//input_files_ts_offset[ist->file_index] -= delta;
			//ffmpeg.c 2891 add this back in
			LOG4CPLUS_WARN(logger, "Timestamp discontinuity " << delta);
			pkt.dts -= av_rescale_q(delta, AV_TIME_BASE_Q, avStream.time_base);
			if (pkt.pts != AV_NOPTS_VALUE)
				pkt.pts -= av_rescale_q(delta, AV_TIME_BASE_Q,
						avStream.time_base);
		}
	}
}

template <class Decoded>
void FFmpegStream<Decoded>::initPts() {
	pts = avStream.avg_frame_rate.num ?
			-avStream.codec->has_b_frames * AV_TIME_BASE
					/ av_q2d(avStream.avg_frame_rate) :
			0;

	nextPts = AV_NOPTS_VALUE;
	start = 1;
}

FFmpegVideoStream::FFmpegVideoStream(AVStream* avs, AVFormatContext* ctx) :
		FFmpegStream(avs, ctx, true, "FFmpegVideoStream") {

}

FFmpegAudioStream::FFmpegAudioStream(AVStream* avs, AVFormatContext* ctx) :
		FFmpegStream(avs, ctx, true, "FFmpegAudioStream") {
	sampleSize = 0;
	samples = NULL;
}

FFmpegAudioStream::~FFmpegAudioStream() {
	if (samples != NULL) {
		av_free(samples);
	}
}

void FFmpegAudioStream::put(AVPacket pkt) {
	adjustTimeStamps(pkt);
	checkAndAllocateSampleBuffer(pkt);

	int decodedDataSize = sampleSize;
	/* XXX: could avoid copy if PCM 16 bits with same
	 endianness as CPU */
	int dataSize = avcodec_decode_audio3(avStream.codec, samples,
			&decodedDataSize, &pkt);
	if (dataSize < 0) {
		string msg;
		msg = "Failed to decode audio dts=";
		msg += avStream.cur_dts;
		msg += " pts=";
		msg += avStream.pts.val;
		msg += " - ";
		throw new TranscodeException(msg, dataSize);
	}
	pkt.data += dataSize;
	pkt.size -= dataSize;

	/* Some bug in mpeg audio decoder gives */
	/* decoded_data_size < 0, it seems they are overflows */
	if (decodedDataSize > 0) {
		// do this later
		//		ist->next_pts += ((int64_t) AV_TIME_BASE / bps * decoded_data_size)
		//				/ (ist->st->codec->sample_rate * ist->st->codec->channels);
		LOG4CPLUS_DEBUG(logger,
				"Got audio dts=" << pkt.dts << " pts=" << pkt.pts);
	}
}

void FFmpegAudioStream::checkAndAllocateSampleBuffer(AVPacket pkt) {
	int max = FFMAX(pkt.size*sizeof(*samples), AVCODEC_MAX_AUDIO_FRAME_SIZE);
	if (sampleSize < max) {
		sampleSize = max;
		if (samples != NULL) {
			av_free(samples);
		}
		samples = (int16_t*) av_malloc(sampleSize);
	}
}

void FFmpegVideoStream::put(AVPacket pkt) {
	adjustTimeStamps(pkt);
	AVFrame pFrame;
	int gotPicture = 0;

	if (pkt.size > 0) {
		int decodedDataSize = (avStream.codec->width * avStream.codec->height
				* 3) / 2;
		/* XXX: allocate picture correctly */
		avcodec_get_frame_defaults(&pFrame);

		int ret = avcodec_decode_video2(avStream.codec, &pFrame, &gotPicture,
				&pkt);
		avStream.quality = pFrame.quality;

		if (ret < 0) {
			string msg;
			msg = "Failed to decode video dts=";
			msg += avStream.cur_dts;
			msg += " pts=";
			msg += avStream.pts.val;
			msg += " - ";
			throw new TranscodeException(msg, ret);
		}
		if (gotPicture) {
			// deal with the picture
			LOG4CPLUS_DEBUG(logger,
					"Got picture dts=" << pkt.dts << " pts=" << pkt.pts);
			AVPicture p = (AVPicture&) pFrame;
			Picture pict(p, avStream.codec->pix_fmt, avStream.codec->width, avStream.codec->height, pkt.pts, pkt.dts);

		}
		// deal with this
		//	if (avStream.codec->time_base.num != 0) {
		//		int ticks = ist->st->parser ? ist->st->parser->repeat_pict + 1
		//				: ist->st->codec->ticks_per_frame;
		//		ist->next_pts += ((int64_t) AV_TIME_BASE
		//				* ist->st->codec->time_base.num * ticks)
		//				/ ist->st->codec->time_base.den;
		//	}
	}
}

template <class Decoded>
FFmpegStream<Decoded> FFmpegStreamFactory<Decoded>::createStream(AVStream *stream,
		AVFormatContext* fc) {
	switch (stream->codec->codec_type) {
	case AVMEDIA_TYPE_VIDEO:
		LOG4CPLUS_WARN(logger, "Creating video stream");
		return FFmpegVideoStream(stream, fc);
	case AVMEDIA_TYPE_AUDIO:
		LOG4CPLUS_WARN(logger, "Creating audio stream");
		return FFmpegAudioStream(stream, fc);
	case AVMEDIA_TYPE_DATA:
	case AVMEDIA_TYPE_NB:
	case AVMEDIA_TYPE_SUBTITLE:
	case AVMEDIA_TYPE_UNKNOWN:
	case AVMEDIA_TYPE_ATTACHMENT:
	default:
		LOG4CPLUS_DEBUG(logger,
				"Creating unknown stream " << stream->codec->codec_type);
		return FFmpegStream<Unit>(stream, fc, false, "FFmpegStream");
	}
}
