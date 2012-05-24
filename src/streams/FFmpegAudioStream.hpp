#ifndef FFMPEGAUDIOSTREAM_H_
#define FFMPEGAUDIOSTREAM_H_

#include "FFmpegStream.hpp"

class Sound : public DecodedData {
	uint8_t *data;
public:
	Sound(uint8_t *, int);
	virtual ~Sound();
};

class FFmpegAudioStream: public FFmpegStream {
	int sampleSize;
	int16_t *samples;
	void checkAndAllocateSampleBuffer(AVPacket);
protected:
	shared_ptr<Sound> decode(PacketPtr);
public:
	FFmpegAudioStream(AVStream*, AVFormatContext*);
	virtual ~FFmpegAudioStream();
	virtual StreamType getType() {
		return AUDIO;
	}
};



static shared_ptr<Sound> NoSound(new Sound(0, 0));
#endif
