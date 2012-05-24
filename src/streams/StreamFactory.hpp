/*
 * StreamFactory.hpp
 *
 *  Created on: 21 May 2012
 *      Author: mediabag
 */

#ifndef STREAMFACTORY_HPP_
#define STREAMFACTORY_HPP_
#include "FFmpegAudioStream.hpp"
#include "FFmpegVideoStream.hpp"
#include "../FFmpegEncodingContainer.hpp"
#include <set>
using namespace std;

class StreamFactory {
public:
	static FFmpegVideoStream makeVideoStream(FFmpegEncodingContainer container,
			int streamId, bool copy, int frameWidth, int frameHeight,
			float frameAspectRatio);
	static FFmpegVideoStream makeVideoStream(string codec, int id,
			AVRational frameRate, int frameWidth, int frameHeight,
			PixelFormat framePixFmt);
	static CodecList& getCodecList() {
		static CodecList codecList; // constructor runs once, when someone first needs it
		return codecList;
	}
};

#endif /* STREAMFACTORY_HPP_ */
