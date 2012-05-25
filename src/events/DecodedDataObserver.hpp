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
#include <boost/bind.hpp>
#include "../FFmpeg.hpp"
#include "../streams/FFmpegStream.hpp"
#include "Event.hpp"

using namespace std;
using namespace boost;


class DecodedDataObserver {
public:
	DecodedDataObserver();
	virtual ~DecodedDataObserver();
public:
	virtual void dataDecoded(shared_ptr<AudioEvent> event);
	virtual void dataDecoded(shared_ptr<VideoEvent> event);
};

#endif /* DECODEDDATAOBSERVER_HPP_ */
