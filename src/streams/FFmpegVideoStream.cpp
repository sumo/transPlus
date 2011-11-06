/*
 * FFmpegAudioStream.cpp
 *
 *  Created on: 3 Nov 2011
 *      Author: mediabag
 */

#include "FFmpegVideoStream.hpp"

FFmpegVideoStream::FFmpegVideoStream(AVStream* avs, AVFormatContext* ctx) :
	FFmpegDataGenerator<Picture>(avs, ctx, true, "FFmpegVideoStream") {

}

FFmpegVideoStream::~FFmpegVideoStream() {
}

bool FFmpegVideoStream::isValid(Picture* s) {
	return s != NoPicture;
}

Picture* FFmpegVideoStream::putImpl(AVPacket pkt) {
	adjustTimeStamps(pkt);
	AVFrame pFrame;
	int gotPicture = 0;

	if (pkt.size > 0) {
		int decodedDataSize = (avStream->codec->width * avStream->codec->height
				* 3) / 2;
		/* XXX: allocate picture correctly */
		avcodec_get_frame_defaults(&pFrame);

		int ret = avcodec_decode_video2(avStream->codec, &pFrame, &gotPicture,
				&pkt);
		avStream->quality = pFrame.quality;

		if (ret < 0) {
			string msg;
			msg = "Failed to decode video dts=";
			msg += avStream->cur_dts;
			msg += " pts=";
			msg += avStream->pts.val;
			msg += " - ";
			throw new TranscodeException(msg, ret);
		}
		if (gotPicture) {
			// deal with the picture
			LOG4CPLUS_DEBUG(logger,
					"Got picture dts=" << pkt.dts << " pts=" << pkt.pts);

			Picture* pict = new Picture((AVPicture*) &pFrame, avStream->codec->pix_fmt,
					avStream->codec->width, avStream->codec->height, pkt.pts,
					pkt.dts);
			return pict;
		}
		// deal with this
		//	if (avStream->codec->time_base.num != 0) {
		//		int ticks = ist->st->parser ? ist->st->parser->repeat_pict + 1
		//				: ist->st->codec->ticks_per_frame;
		//		ist->next_pts += ((int64_t) AV_TIME_BASE
		//				* ist->st->codec->time_base.num * ticks)
		//				/ ist->st->codec->time_base.den;
		//	}
	}
	return NoPicture;
}
