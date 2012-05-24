/*
 * VideoEvent.hpp
 *
 *  Created on: 23 May 2012
 *      Author: mediabag
 */

#ifndef VIDEOEVENT_HPP_
#define VIDEOEVENT_HPP_

#include "../DecodedDataObserver.hpp"
#include "../Picture.hpp"
#include "../streams/FFmpegAudioStream.hpp"

class VideoEvent: public Event<Picture> {
	FFmpegVideoStream stream;
public:
	VideoEvent(FFmpegVideoStream& str, PacketPtr p) :
			stream(str), Event(p) {
	};
	virtual ~VideoEvent();
	virtual shared_ptr<Picture> decode() {
		return stream.decode(packet);
	}


};


#endif /* VIDEOEVENT_HPP_ */
