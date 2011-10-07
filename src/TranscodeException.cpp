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

TranscodeException::TranscodeException(string &m, int avErrorCode) {
	msg = m;
	if (avErrorCode < 0) {
		msg += avStrError(avErrorCode);
	}

}

TranscodeException::TranscodeException(int avErrorCode) {
	append(avErrorCode);
}

TranscodeException::TranscodeException() {
	msg = "A transcode exception occured";
}

TranscodeException::TranscodeException(const char* cpmsg, int avErrorCode) {
	msg = string(cpmsg);
	append(avErrorCode);
}

TranscodeException::~TranscodeException() throw () {

}

void TranscodeException::append(int avErrorCode) {
	if (avErrorCode < 0) {
		msg += avStrError(avErrorCode);
		msg += "(";
		msg += avErrorCode;
		msg += ")";
	}
}

const char* TranscodeException::what() const throw () {
	return msg.c_str();
}

string TranscodeException::avStrError(int errorCode) {
	char *s = new char[1024];
	av_strerror(errorCode, s, 1024);
	string errorMessage(s);
	delete s;
	return errorMessage;
}
