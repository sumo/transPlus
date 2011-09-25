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
	}
	bps = av_get_bits_per_sample_format(avStream.codec->sample_fmt) >> 3;
}

FFmpegStream::~FFmpegStream() {

}
string FFmpegStream::avStrError(int errorCode) {
	char *s = new char[1024];
	av_strerror(errorCode, s, 1024);
	string errorMessage(s);
	delete s;
	return errorMessage;
}

FFmpegStream FFmpegStream::operator <<(const AVPacket pkt) {
	return *this;
}

FFmpegVideoStream::FFmpegVideoStream(AVStream* avs) :
	FFmpegStream(avs, true) {

}

FFmpegAudioStream::FFmpegAudioStream(AVStream* avs) :
	FFmpegStream(avs, true) {
	sampleSize = 0;
}

FFmpegStream FFmpegAudioStream::operator <<(const AVPacket inPkt) {
	AVPacket pkt = inPkt;
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
		msg += avStrError(dataSize);
		throw new TranscodeException(msg);
	}
	pkt.data += dataSize;
	pkt.size -= dataSize;

	/* Some bug in mpeg audio decoder gives */
	/* decoded_data_size < 0, it seems they are overflows */
	if (!decodedDataSize <= 0) {

		cout << "Sample is " << *samples;
		// do this later
		//		ist->next_pts += ((int64_t) AV_TIME_BASE / bps * decoded_data_size)
		//				/ (ist->st->codec->sample_rate * ist->st->codec->channels);
	}
}

void FFmpegAudioStream::checkAndAllocateSampleBuffer(const AVPacket pkt) {
	int max = FFMAX(pkt.size*sizeof(*samples), AVCODEC_MAX_AUDIO_FRAME_SIZE);
	if (sampleSize < max) {
		sampleSize = max;
		av_realloc(samples, sampleSize);
	}
}

FFmpegStream FFmpegVideoStream::operator <<(const AVPacket inPkt) {
	AVFrame pFrame;
	AVPacket pkt = inPkt;
	int gotPicture = 0;

	int decodedDataSize = (avStream.codec->width * avStream.codec->height * 3)
			/ 2;
	/* XXX: allocate picture correctly */
	avcodec_get_frame_defaults(&pFrame);

	int ret = avcodec_decode_video2(avStream.codec, &pFrame, &gotPicture, &pkt);
	avStream.quality = pFrame.quality;
	if (ret < 0) {
		string msg;
		msg = "Failed to decode video dts=";
		msg += avStream.cur_dts;
		msg += " pts=";
		msg += avStream.pts.val;
		msg += " - ";
		msg += avStrError(ret);
		throw new TranscodeException(msg);
	}
	if (gotPicture) {
		// deal with the picture
		cout << "Got picture dts=" << avStream.cur_dts << " pts="
				<< avStream.pts.val << endl;
	}
	// deal with this
	//	if (avStream.codec->time_base.num != 0) {
	//		int ticks = ist->st->parser ? ist->st->parser->repeat_pict + 1
	//				: ist->st->codec->ticks_per_frame;
	//		ist->next_pts += ((int64_t) AV_TIME_BASE
	//				* ist->st->codec->time_base.num * ticks)
	//				/ ist->st->codec->time_base.den;
	//	}
	pkt.size = 0;
	return *this;
}

FFmpegStream FFmpegStreamFactory::createStream(AVStream *stream) {
	switch (stream->codec->codec_type) {
	case AVMEDIA_TYPE_VIDEO:
		cout << "Creating video stream" << endl;
		return FFmpegVideoStream(stream);
	case AVMEDIA_TYPE_AUDIO:
		cout << "Creating audio stream" << endl;
		return FFmpegAudioStream(stream);
	case AVMEDIA_TYPE_DATA:
	case AVMEDIA_TYPE_NB:
	case AVMEDIA_TYPE_SUBTITLE:
	case AVMEDIA_TYPE_UNKNOWN:
		cout << "Creating unknown stream " << stream->codec->codec_type << endl;
		return FFmpegStream(stream, false);
	}
}
