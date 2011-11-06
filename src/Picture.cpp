/*
 * Picture.cpp
 *
 *  Created on: 31 Oct 2011
 *      Author: mediabag
 */

#include "Picture.hpp"

Picture::Picture(AVPicture* sourcePic, PixelFormat pixFormat, int width,
		int height, int64_t pts, int64_t dts) :
		pts(pts), dts(dts), allocated(false) {
	if (width > 0 && height > 0) {
		avcodec_get_frame_defaults(&picture);
		av_picture_copy((AVPicture*)&picture, sourcePic, pixFormat, width, height);
		allocated = true;
	}
}

Picture::~Picture() {
	if (allocated) {
		avpicture_free((AVPicture*)&picture);
	}
}

Sound::Sound(uint8_t *src, int size) {
	data = new uint8_t(size);
	memcpy(data, src, size);
}

Sound::~Sound() {
	delete data;
}
