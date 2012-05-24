/*
 * StreamFactory.cpp
 *
 *  Created on: 21 May 2012
 *      Author: mediabag
 */

#include "StreamFactory.hpp"


FFmpegVideoStream StreamFactory::makeVideoStream(
		FFmpegEncodingContainer container, int streamId, bool copy, int frameWidth, int frameHeight, float frameAspectRatio) {
	AVStream *st = st = av_new_stream(container.oc, streamId);
	if (!st) {
		throw new TranscodeException("Could not allocate copy stream");
	}
	st->stream_copy = 1;
	st->codec->codec_type = AVMEDIA_TYPE_VIDEO;
	st->codec->sample_aspect_ratio = st->sample_aspect_ratio = av_d2q(
			frameAspectRatio * frameHeight / frameWidth, 255);
	return FFmpegVideoStream(st, container.oc);
}

//FFmpegVideoStream StreamFactory::makeVideoStream(string codec,
//		int id, AVRational frameRate, int frameWidth, int frameHeight,
//		PixelFormat framePixFmt) {
//
//}

