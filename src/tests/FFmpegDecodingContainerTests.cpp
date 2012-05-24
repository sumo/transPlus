#include <gtest/gtest.h>
#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <string>
#include "../FFmpegDecodingContainer.hpp"
#include "../streams/StreamReader.hpp"
#include <boost/shared_ptr.hpp>

using namespace boost;

TEST(FFmpegDecodingContainer, ReadContainer) {
	ifstream src;
	src.open("samplefiles/echo-hereweare.mp4", ios::binary);
	StreamReader reader(src);
	FFmpegDecodingContainer decoder(reader);
	map<string, string> m = decoder.getFormat();
	string format = (*m.begin()).first;
	ASSERT_STREQ("mov,mp4,m4a,3gp,3g2,mj2", format.c_str());

	ptr_vector<ObservableDecodingStream>& streams = decoder.getStreams();

	ptr_vector<ObservableDecodingStream>::iterator it;
	for (it = streams.begin(); it < streams.end(); it++) {
		if (it->getType() == AUDIO) {
			ASSERT_STREQ("aac", it->getCodec().c_str());
		} else {
			ASSERT_STREQ("h264", it->getCodec().c_str());
		}
	}
}
