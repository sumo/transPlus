/*
 * DecodedDataObserver.hpp
 *
 *  Created on: 1 Nov 2011
 *      Author: mediabag
 */

#ifndef DECODEDDATAOBSERVER_HPP_
#define DECODEDDATAOBSERVER_HPP_
#include <vector>
#include <boost/shared_ptr.hpp>
#include "FFmpeg.hpp"

using namespace std;
using namespace boost;

typedef boost::shared_ptr<AVPacket> PacketPtr;

template <typename DecodedDataType>
class Event {
protected:
	PacketPtr packet;
public:
	Event(PacketPtr p) : packet(p) {
	};
	virtual shared_ptr<DecodedDataType> decode() = 0;
	virtual ~Event();
};

template <typename DecodedDataType>
class DecodedDataObserver {
public:
	DecodedDataObserver();
	virtual ~DecodedDataObserver();
public:
	virtual void dataDecoded(shared_ptr<Event<DecodedDataType> > event);
};

#endif /* DECODEDDATAOBSERVER_HPP_ */
