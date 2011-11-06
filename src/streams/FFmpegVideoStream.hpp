#ifndef FFMPEGVIDEOSTREAM_H_
#define FFMPEGVIDEOSTREAM_H_

#include "FFmpegStream.hpp"
#include "../Picture.hpp"

class FFmpegVideoStream: public FFmpegDataGenerator<Picture> {
protected:
	virtual Picture* putImpl(AVPacket);
	virtual bool isValid(Picture*);

public:
	FFmpegVideoStream(AVStream*, AVFormatContext*);
	virtual ~FFmpegVideoStream();
	virtual StreamType getType() {
		return VIDEO;
	}

};

static Picture* NoPicture = new Picture(NULL, PIX_FMT_NONE, 0, 0, 0, 0);

#endif
