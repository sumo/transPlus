/*
 * FFMpegStream.cpp
 *
 *  Created on: 22 Sep 2011
 *      Author: mediabag
 */

#include "FFmpegStream.hpp"

using namespace std;

FFmpegStream::FFmpegStream(AVStream* avs, AVFormatContext* fc, bool findCodec,
		string streamName):avStream(avs), formatContext(fc), logger(Logger::getInstance(LOG4CPLUS_TEXT(streamName))) {
	if (findCodec) {
		codec = avcodec_find_decoder(avs->codec->codec_id);
		int open = avcodec_open(avStream->codec, codec);
		if (open < 0) {
			throw new TranscodeException("Failed to open codec", open);
		}
	}
	bps = av_get_bits_per_sample_format(avStream->codec->sample_fmt) >> 3;
	initPts();
}

FFmpegStream::~FFmpegStream() {

}

void FFmpegStream::put(AVPacket pkt) {

}

void FFmpegStream::adjustTimeStamps(AVPacket pkt) {
	if (pkt.dts != AV_NOPTS_VALUE)
		pkt.dts += av_rescale_q(0, AV_TIME_BASE_Q, avStream->time_base);
	if (pkt.pts != AV_NOPTS_VALUE)
		pkt.pts += av_rescale_q(0, AV_TIME_BASE_Q, avStream->time_base);

	//input file scaling
//	if (pkt.stream_index < nb_input_files_ts_scale[file_index]
//			&& input_files_ts_scale[file_index][pkt.stream_index]) {
//		if (pkt.pts != AV_NOPTS_VALUE)
//			pkt.pts *= input_files_ts_scale[file_index][pkt.stream_index];
//		if (pkt.dts != AV_NOPTS_VALUE)
//			pkt.dts *= input_files_ts_scale[file_index][pkt.stream_index];
//	}

	if (pkt.dts != AV_NOPTS_VALUE && nextPts != AV_NOPTS_VALUE
			&& (formatContext->iformat->flags & AVFMT_TS_DISCONT)) {
		int64_t pkt_dts = av_rescale_q(pkt.dts, avStream->time_base,
				AV_TIME_BASE_Q);
		int64_t delta = pkt_dts - nextPts;
		if ((FFABS(delta) > 1LL * dtsDeltaThreshold * AV_TIME_BASE
				|| pkt_dts + 1 < pts)) { // && !copy_ts - invesitgate copy timestamps
			//input_files_ts_offset[ist->file_index] -= delta;
			//ffmpeg.c 2891 add this back in
			LOG4CPLUS_WARN(logger, "Timestamp discontinuity " << delta);
			pkt.dts -= av_rescale_q(delta, AV_TIME_BASE_Q, avStream->time_base);
			if (pkt.pts != AV_NOPTS_VALUE)
				pkt.pts -= av_rescale_q(delta, AV_TIME_BASE_Q,
						avStream->time_base);
		}
	}
}

void FFmpegStream::initPts() {
	pts = avStream->avg_frame_rate.num ?
			-avStream->codec->has_b_frames * AV_TIME_BASE
					/ av_q2d(avStream->avg_frame_rate) :
			0;

	nextPts = AV_NOPTS_VALUE;
	start = 1;
}

string FFmpegStream::getCodec() {
	return string(codec->long_name);
}
