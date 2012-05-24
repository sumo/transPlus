/*
 * AudioEvent.hpp
 *
 *  Created on: 23 May 2012
 *      Author: mediabag
 */

#ifndef AUDIOEVENT_HPP_
#define AUDIOEVENT_HPP_

#include "../DecodedDataObserver.hpp"
#include "../Picture.hpp"
#include "../streams/FFmpegAudioStream.hpp"

class AudioEvent: public Event<Sound> {
	FFmpegAudioStream stream;
public:
	AudioEvent(FFmpegAudioStream& str, PacketPtr p) :
			stream(str), Event(p) {
	};
	virtual ~AudioEvent();
	virtual shared_ptr<Sound> decode() {
		return stream.decode(packet);
	}


};

#endif /* AUDIOEVENT_HPP_ */
