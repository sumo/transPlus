/*
 * FFMpegStream.cpp
 *
 *  Created on: 22 Sep 2011
 *      Author: mediabag
 */

#include "FFmpegStream.hpp"

using namespace std;

FFmpegStream::FFmpegStream(AVStream* avs, bool findCodec) {
	avStream = *avs;
	if (findCodec) {
		codec = *(avcodec_find_decoder(avs->codec->codec_id));
		int open = avcodec_open(avStream.codec, &codec);
		if (open < 0) {
			throw new TranscodeException("Failed to open codec", open);
		}
	}
	bps = av_get_bits_per_sample_format(avStream.codec->sample_fmt) >> 3;
}

FFmpegStream::~FFmpegStream() {

}

FFmpegStream FFmpegStream::operator <<(AVPacket pkt) {
	return *this;
}

FFmpegVideoStream::FFmpegVideoStream(AVStream* avs) :
	FFmpegStream(avs, true) {

}

FFmpegAudioStream::FFmpegAudioStream(AVStream* avs) :
	FFmpegStream(avs, true) {
	sampleSize = 0;
	av_free(samples);
}

FFmpegStream FFmpegAudioStream::operator <<(AVPacket pkt) {
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
		cout << "Got audio dts=" << pkt.dts << " pts=" << pkt.pts << endl;
	}
	return *this;
}

void FFmpegAudioStream::checkAndAllocateSampleBuffer(AVPacket pkt) {
	int max = FFMAX(pkt.size*sizeof(*samples), AVCODEC_MAX_AUDIO_FRAME_SIZE);
	if (sampleSize < max) {
		sampleSize = max;
		av_free(samples);
		samples = (int16_t*) av_malloc(sampleSize);
	}
}

FFmpegStream FFmpegVideoStream::operator <<(AVPacket pkt) {
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
			cout << "Got picture dts=" << pkt.dts << " pts=" << pkt.pts << endl;
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
	return *this;
}

FFmpegStream* FFmpegStreamFactory::createStream(AVStream *stream) {
	switch (stream->codec->codec_type) {
	case AVMEDIA_TYPE_VIDEO:
		cout << "Creating video stream" << endl;
		return new FFmpegVideoStream(stream);
	case AVMEDIA_TYPE_AUDIO:
		cout << "Creating audio stream" << endl;
		return new FFmpegAudioStream(stream);
	case AVMEDIA_TYPE_DATA:
	case AVMEDIA_TYPE_NB:
	case AVMEDIA_TYPE_SUBTITLE:
	case AVMEDIA_TYPE_UNKNOWN:
	case AVMEDIA_TYPE_ATTACHMENT:
	default:
		cout << "Creating unknown stream " << stream->codec->codec_type << endl;
		return new FFmpegStream(stream, false);
	}
}
