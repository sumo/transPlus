/*
 * TranscodeException.h
 *
 *  Created on: 25 Sep 2011
 *      Author: mediabag
 */

#ifndef TRANSCODEEXCEPTION_H_
#include "fixstdint.hpp"
extern "C" {
#include <libavutil/avutil.h>
}
#define TRANSCODEEXCEPTION_H_

#include <exception>
#include <sstream>

using namespace std;
class TranscodeException: public std::exception {
	stringstream msg;
	void append(int avErrorCode);
public:
	TranscodeException();
	TranscodeException(const TranscodeException&);
	TranscodeException(string &msg);
	TranscodeException(int avErrorCode);
	TranscodeException(string &msg, int avErrorCode);
	TranscodeException(const char*, int avErrorCode);
	TranscodeException(const char*);
	virtual const char* what() const throw();
	virtual ~TranscodeException() throw();
};

#endif /* TRANSCODEEXCEPTION_H_ */
