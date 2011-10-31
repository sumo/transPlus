/*
 * Picture.cpp
 *
 *  Created on: 31 Oct 2011
 *      Author: mediabag
 */

#include "Picture.hpp"

Picture::Picture(AVPicture sourcePic, PixelFormat pixFormat, int width,
		int height, int64_t pts, int64_t dts) :
		pts(pts), dts(dts) {
	avpicture_alloc(&dstPic, pixFormat, width, height);
	av_picture_copy(&dstPic, &sourcePic, pixFormat, width, height);
}

Picture::~Picture() {
	avpicture_free(&dstPic);
}

Sound::Sound(uint8_t *src, int size) {
	data = new uint8_t(size);
	memcpy(data, src, size);
}

Sound::~Sound() {
	delete data;
}
