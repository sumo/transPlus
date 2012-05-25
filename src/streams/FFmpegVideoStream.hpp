#ifndef FFMPEGVIDEOSTREAM_H_
#define FFMPEGVIDEOSTREAM_H_

#include "FFmpegStream.hpp"

class Picture {
	shared_ptr<AVFrame> picture;
public:
	Picture(shared_ptr<AVFrame> sourcePic) :
			picture(sourcePic) {
	}
	Picture(AVFrame* sourcePic) :
			picture(sourcePic) {
	}
	virtual ~Picture() {

	}
	shared_ptr<AVFrame> getFrame() {
		return picture;
	}
};

class FFmpegVideoStream: public FFmpegStream {
public:
	FFmpegVideoStream(AVStream* avs, AVFormatContext* afc, int streamIndex) :
			FFmpegStream(avs, afc, "FFmpegVideoStream", streamIndex) {
	}
	virtual ~FFmpegVideoStream() {

	}
	virtual StreamType getType() {
		return VIDEO;
	}
	shared_ptr<Picture> decode(PacketPtr);
};

static shared_ptr<Picture> NoPicture(new Picture(shared_ptr<AVFrame>()));

#endif
