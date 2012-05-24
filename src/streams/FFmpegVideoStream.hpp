#ifndef FFMPEGVIDEOSTREAM_H_
#define FFMPEGVIDEOSTREAM_H_

#include "FFmpegStream.hpp"

class Picture: public DecodedData {
	AVFrame picture;
	int64_t pts;
	int64_t dts;
	bool allocated;
public:
	Picture(AVPicture* sourcePic, PixelFormat pixFormat, int width, int height,
			int64_t pts, int64_t dts);
	virtual ~Picture();
	AVPicture* getPicture();
};

class FFmpegVideoStream: public FFmpegStream {
protected:
	shared_ptr<Picture> decode(PacketPtr);
public:
	FFmpegVideoStream(AVStream* avs, AVFormatContext* afc) :
			FFmpegStream(avs, afc, "FFmpegVideoStream") {
	}
	;
	virtual ~FFmpegVideoStream();
	virtual StreamType getType() {
		return VIDEO;
	}
};


static shared_ptr<Picture> NoPicture(new Picture(NULL, PIX_FMT_NONE, 0, 0, 0, 0));

#endif
