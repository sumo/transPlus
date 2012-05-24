/*
 * FFmpegContainer.cpp
 *
 *  Created on: 22 May 2012
 *      Author: mediabag
 */

#include "FFmpegContainer.hpp"

void checkForError(int code) {
	if (code < 0) {
		char* s = new char[1024];
		av_strerror(code, s, 1024);
		TranscodeException* t = new TranscodeException(s, code);
		delete s;
		throw t;
	}
}


