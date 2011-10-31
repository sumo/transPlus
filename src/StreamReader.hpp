/*
 * StreamReader.h
 *
 *  Created on: 22 Sep 2011
 *      Author: mediabag
 */

#ifndef STREAMREADER_H_
#include <iostream>
#include "fixstdint.hpp"
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
extern "C" {
#include <libavformat/avio.h>
}
#define STREAMREADER_H_

using namespace std;
using namespace log4cplus;

class StreamReader {
	istream& ios;
	Logger logger;
public:
	StreamReader(istream& ios);
	virtual ~StreamReader();

	int read(uint8_t *buf, int buf_size);
	int64_t seek(int64_t offset, int whence);

	static int readFunction(void* opaque, uint8_t *buf, int buf_size);
	static int64_t seekFunction(void* opaque, int64_t offset, int whence);
};

#endif /* STREAMREADER_H_ */
