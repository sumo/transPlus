/*
 * TranscodeException.h
 *
 *  Created on: 25 Sep 2011
 *      Author: mediabag
 */

#ifndef TRANSCODEEXCEPTION_H_
#define TRANSCODEEXCEPTION_H_

#include <exception>
#include <string>

using namespace std;
class TranscodeException: public std::exception {
	string msg;
public:
	TranscodeException();
	TranscodeException(string &msg);
	virtual const char* what() const throw();
	virtual ~TranscodeException() throw();
};

#endif /* TRANSCODEEXCEPTION_H_ */
