/*
 * FFmpegAudioStream.cpp
 *
 *  Created on: 3 Nov 2011
 *      Author: mediabag
 */

#include "FFmpegAudioStream.hpp"

FFmpegAudioStream::FFmpegAudioStream(AVStream* avs, AVFormatContext* ctx) :
		FFmpegDataGenerator<Sound>(avs, ctx, true, "FFmpegAudioStream") {
	sampleSize = 0;
	samples = NULL;
}

FFmpegAudioStream::~FFmpegAudioStream() {
	if (samples != NULL) {
		av_free(samples);
	}
}

bool FFmpegAudioStream::isValid(Sound s) {
	return &s != &NoSound;
}

Sound FFmpegAudioStream::putImpl(AVPacket pkt) {
	adjustTimeStamps(pkt);
	checkAndAllocateSampleBuffer(pkt);

	int decodedDataSize = sampleSize;
	/* XXX: could avoid copy if PCM 16 bits with same
	 endianness as CPU */
	int dataSize = avcodec_decode_audio3(avStream->codec, samples,
			&decodedDataSize, &pkt);
	if (dataSize < 0) {
		string msg;
		msg = "Failed to decode audio dts=";
		msg += avStream->cur_dts;
		msg += " pts=";
		msg += avStream->pts.val;
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
	return NoSound;
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

