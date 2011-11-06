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
		int ret = avpicture_alloc((AVPicture*)&picture, pixFormat, width, height);
		if (ret < 0) {
			throw TranscodeException(ret);
		}
		av_picture_copy((AVPicture*)&picture, sourcePic, pixFormat, width, height);
		allocated = true;
	}
}

Picture::~Picture() {
	if (allocated) {
		av_free(picture.data[0]);
	}
}

Sound::Sound(uint8_t *src, int size) {
	data = new uint8_t(size);
	memcpy(data, src, size);
}

Sound::~Sound() {
	delete data;
}
