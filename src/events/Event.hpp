/*
 * Event.hpp
 *
 *  Created on: 25 May 2012
 *      Author: mediabag
 */

#ifndef EVENT_HPP_
#define EVENT_HPP_
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include "../FFmpeg.hpp"
#include "../streams/FFmpegAudioStream.hpp"
#include "../streams/FFmpegVideoStream.hpp"

template<class DecodedDataType, class StreamType>
class Event {
protected:
	PacketPtr packet;
	StreamType stream;
public:
	Event(AVPacket p, StreamType& fs) :
			packet(&p, boost::bind(&Event::destroy, this, _1)), stream(fs) {
	}

	virtual shared_ptr<DecodedDataType> decode() {
		return stream.decode(packet);
	}
	virtual ~Event() {
	}
	FFmpegStream& getStream() {
		return stream;
	}
	void destroy(AVPacket* pkt) {
		av_free_packet(pkt);
	}
	PacketPtr getPacket() {
		return packet;
	}
};

typedef Event<Picture, FFmpegVideoStream> VideoEvent;
typedef Event<Sound, FFmpegAudioStream> AudioEvent;

#endif /* EVENT_HPP_ */
