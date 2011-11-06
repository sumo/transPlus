/*
 * Picture.h
 *
 *  Created on: 31 Oct 2011
 *      Author: mediabag
 */

#ifndef PICTURE_H_
#define PICTURE_H_
#include "fixstdint.hpp"
#include <queue>
#include "TranscodeException.hpp"
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

using namespace std;

class Picture {
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

class Sound {
	uint8_t *data;
public:
	Sound(uint8_t *, int);
	virtual ~Sound();
};
#endif /* PICTURE_H_ */
