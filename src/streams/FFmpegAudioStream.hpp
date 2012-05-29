#ifndef FFMPEGAUDIOSTREAM_H_
#define FFMPEGAUDIOSTREAM_H_

#include "FFmpegStream.hpp"

class Sound {
	shared_ptr<uint8_t> data;
public:
	Sound(shared_ptr<uint8_t> dt) :
			data(dt) {
	}
	Sound(uint8_t* dt) :
			data(dt) {
	}
	virtual ~Sound() {
	}
	shared_ptr<uint8_t> getData() {
		return data;
	}
};

class FFmpegAudioStream: public FFmpegStream {
	shared_ptr<uint8_t> checkAndAllocateSampleBuffer(AVPacket);
public:
	FFmpegAudioStream(AVStream*, AVFormatContext*, int streamIndex);
	virtual ~FFmpegAudioStream() {

	}
	virtual StreamType getType() {
		return AUDIO;
	}
	shared_ptr<Sound> decode(PacketPtr);
	FFmpegAudioStream* clone() const {
		return new FFmpegAudioStream(*this);
	}
};

static shared_ptr<Sound> NoSound(new Sound(shared_ptr<uint8_t>()));
#endif
