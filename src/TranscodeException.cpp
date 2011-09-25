/*
 * TranscodeException.cpp
 *
 *  Created on: 25 Sep 2011
 *      Author: mediabag
 */

#include "TranscodeException.hpp"

TranscodeException::TranscodeException(string &m) {
	msg = m;

}

TranscodeException::TranscodeException() {
	msg = "A transcode exception occured";
}

TranscodeException::~TranscodeException() throw() {

}

const char* TranscodeException::what() const throw () {
	return msg.c_str();
}
