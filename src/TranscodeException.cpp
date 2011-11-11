/*
 * TranscodeException.cpp
 *
 *  Created on: 25 Sep 2011
 *      Author: mediabag
 */

#include "TranscodeException.hpp"

TranscodeException::TranscodeException(string &m):msg(m) {

}

TranscodeException::TranscodeException(string &m, int avErrorCode):msg(m) {
	if (avErrorCode < 0) {
		append(avErrorCode);
	}

}

TranscodeException::TranscodeException(int avErrorCode):msg() {
	append(avErrorCode);
}

TranscodeException::TranscodeException():msg("A transcode exception occured") {
}

TranscodeException::TranscodeException(const char* cpmsg, int avErrorCode):msg(cpmsg) {
	append(avErrorCode);
}

TranscodeException::TranscodeException(const char* cpmsg):msg() {
	msg << cpmsg;
}

TranscodeException::TranscodeException(const TranscodeException& c):msg() {
	msg << c.msg;
}

TranscodeException::~TranscodeException() throw () {

}

void TranscodeException::append(int avErrorCode) {
	if (avErrorCode < 0) {
		char *s = new char[1024];
		av_strerror(avErrorCode, s, 1024);
		msg << s << "(" << avErrorCode << ")";
		delete s;
	}
}

const char* TranscodeException::what() const throw () {
	return msg.str().c_str();
}
