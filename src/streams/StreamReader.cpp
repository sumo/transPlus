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
		return EOF;
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
	int result = -1;
	switch (whence) {
	case AVSEEK_SIZE:
		LOG4CPLUS_DEBUG(logger, "AVSEEK_SIZE");
		return -1;
	case SEEK_SET:
		if (offset < 0) {
			LOG4CPLUS_DEBUG(logger,
					"SEEK_SET to negative offset of " << offset);
		} else {
			ios.seekg(offset, ios_base::beg);
			result = (ios.fail() || ios.eof() ? -1 : (int64_t) ((ios.tellg())));
			LOG4CPLUS_DEBUG( logger,
					"SEEK_SET to " << offset << " resulted in " << result);
		}
		break;
	case SEEK_CUR:
		ios.seekg(ios.tellg() + offset);
		result = (ios.fail() || ios.eof() ? -1 : (int64_t) ((ios.tellg())));
		LOG4CPLUS_DEBUG(logger, "SEEK_CUR to " << ios.tellg() + offset << " resulted in " << result);
		break;
	case SEEK_END:
		ios.seekg(offset, ios_base::end);
		result = ios.fail() || ios.eof() ? -1 : (int64_t) (ios.tellg());
		LOG4CPLUS_DEBUG(
				logger,
				"SEEK_END resulted in " << result);
		break;
	default:
		LOG4CPLUS_DEBUG(logger, "Unknown whence " << whence);
		break;
	}
	return result;
}

int StreamReader::readFunction(void* opaque, uint8_t *buf, int buf_size) {
	StreamReader *me = (StreamReader*) opaque;
	return me->read(buf, buf_size);
}

int64_t StreamReader::seekFunction(void* opaque, int64_t offset, int whence) {
	StreamReader *me = (StreamReader*) opaque;
	return me->seek(offset, whence);
}
