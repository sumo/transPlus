#ifndef FFMPEGVIDEOSTREAM_H_
#define FFMPEGVIDEOSTREAM_H_

#include "FFmpegStream.hpp"
#include "../Picture.hpp"

class FFmpegVideoStream: public FFmpegDataGenerator<Picture> {
protected:
	virtual Picture putImpl(AVPacket);
	virtual bool isValid(Picture);

public:
	FFmpegVideoStream(AVStream*, AVFormatContext*);
	virtual ~FFmpegVideoStream();
	virtual StreamType getType() {
		return VIDEO;
	}

};

static Picture NoPicture(NULL, PIX_FMT_NONE, 1, 1, 0, 0);

#endif
