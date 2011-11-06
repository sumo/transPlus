#ifndef FFMPEGAUDIOSTREAM_H_
#define FFMPEGAUDIOSTREAM_H_

#include "FFmpegStream.hpp"
#include "../Picture.hpp"

class FFmpegAudioStream: public FFmpegDataGenerator<Sound> {
	int sampleSize;
	int16_t *samples;
	void checkAndAllocateSampleBuffer(AVPacket);
protected:
	virtual Sound* putImpl(AVPacket);
	virtual bool isValid(Sound*);
public:
	FFmpegAudioStream(AVStream*, AVFormatContext*);
	virtual ~FFmpegAudioStream();
	virtual StreamType getType() {
		return AUDIO;
	}
};

static Sound* NoSound = new Sound(0, 0);
#endif
