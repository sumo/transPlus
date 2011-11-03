/*
 * StreamReader.cpp
 *
 *  Created on: 22 Sep 2011
 *      Author: mediabag
 */

#include "StreamReader.hpp"

StreamReader::StreamReader(istream& input) :
		ios(input), logger(Logger::getInstance(LOG4CPLUS_TEXT("StreamReader"))) {
}

StreamReader::~StreamReader() {

}

void StreamReader::rewind() {
	ios.seekg(ios_base::beg);
}

int StreamReader::read(uint8_t *buf, int buf_size) {
	if (ios.eof()) {
		LOG4CPLUS_DEBUG(logger, "Read complete");
		return -1;
	} else {
		ios.read(reinterpret_cast<char*>(buf), buf_size);
		int outcome = ios.fail() ? ios.gcount() : buf_size;
		LOG4CPLUS_DEBUG(logger,
				"Read asked " << buf_size << " read " << outcome);
		if (ios.fail() || ios.eof()) {
			ios.clear(std::ios_base::failbit);
			ios.clear(std::ios_base::eofbit);
		}
		return outcome;
	}
}

int64_t StreamReader::seek(int64_t offset, int whence) {
	LOG4CPLUS_DEBUG(logger, "Seek: " << offset << " whence:");
	switch (whence) {
	case AVSEEK_SIZE:
		LOG4CPLUS_DEBUG(logger, "AVSEEK_SIZE");
		return -1;
	case SEEK_SET:
		LOG4CPLUS_DEBUG(logger, "SEEK_SET");
		if (offset < 0) {
			return -1;
		} else {
			ios.seekg(offset, ios_base::beg);
			LOG4CPLUS_DEBUG(
					logger,
					"Seek resulted in " << (ios.fail() || ios.eof() ? -1 : (int64_t) (ios.tellg())));
			return ios.fail() || ios.eof() ? -1 : (int64_t) ios.tellg();
		}
	case SEEK_CUR:
		LOG4CPLUS_DEBUG(logger, "SEEK_CUR to " << ios.tellg() + offset);
		ios.seekg(ios.tellg() + offset);
		return ios.fail() || ios.eof() ? -1 : (int64_t) ios.tellg();
	case SEEK_END:
		LOG4CPLUS_DEBUG(logger, "SEEK_END");
		ios.seekg(offset, ios_base::end);
		LOG4CPLUS_DEBUG(
				logger,
				"Seek resulted in " << (ios.fail() ? -39 : (int64_t) (ios.tellg())));
		return ios.fail() || ios.eof() ? -1 : (int64_t) (ios.tellg());
	}LOG4CPLUS_DEBUG(logger, whence);
	return -1;
}

int StreamReader::readFunction(void* opaque, uint8_t *buf, int buf_size) {
	StreamReader *me = (StreamReader*) opaque;
	return me->read(buf, buf_size);
}

int64_t StreamReader::seekFunction(void* opaque, int64_t offset, int whence) {
	StreamReader *me = (StreamReader*) opaque;
	return me->seek(offset, whence);
}
