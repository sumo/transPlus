/*
 * StreamReader.cpp
 *
 *  Created on: 22 Sep 2011
 *      Author: mediabag
 */

#include "StreamReader.hpp"

StreamReader::StreamReader(istream& input) :
	ios(input) {
}

StreamReader::~StreamReader() {
	// TODO Auto-generated destructor stub
}

int StreamReader::read(uint8_t *buf, int buf_size) {
	if (ios.eof()) {
		cout << "Read complete" << endl;
		return -1;
	} else {
		ios.read(reinterpret_cast<char*> (buf), buf_size);
		int outcome = ios.fail() ? ios.gcount() : buf_size;
		cout << "Read asked " << buf_size << " read " << outcome << endl;
		if(ios.fail() || ios.eof()) {
			ios.clear(std::ios_base::failbit);
			ios.clear(std::ios_base::eofbit);
		}
		return outcome;
	}
}

int64_t StreamReader::seek(int64_t offset, int whence) {
	cout << "Seek: " << offset << " whence:";
	switch (whence) {
	case AVSEEK_SIZE:
		cout << "AVSEEK_SIZE" << endl;
		return -1;
	case SEEK_SET:
		cout << "SEEK_SET" << endl;
		if (offset < 0) {
			return -1;
		} else {
			ios.seekg(offset, ios_base::beg);
			cout << "Seek resulted in " << (ios.fail() || ios.eof() ? -1
					: (int64_t) (ios.tellg()));
			cout << endl;
			return ios.fail() || ios.eof() ? -1 : (int64_t) ios.tellg();
		}
	case SEEK_CUR:
		cout << "SEEK_CUR to " << ios.tellg() + offset << endl;
		ios.seekg(ios.tellg() + offset);
		return ios.fail() || ios.eof() ? -1 : (int64_t) ios.tellg();
	case SEEK_END:
		cout << "SEEK_END" << endl;
		ios.seekg(offset, ios_base::end);
		cout << "Seek resulted in " << (ios.fail() ? -39
				: (int64_t) (ios.tellg()));
		cout << endl;
		return ios.fail() || ios.eof() ? -1 : (int64_t) (ios.tellg());
	}
	cout << whence << endl;
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
