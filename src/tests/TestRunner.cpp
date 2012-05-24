#include "../fixstdint.hpp"
#include <gtest/gtest.h>
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

using namespace log4cplus;

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	BasicConfigurator::doConfigure();
	avcodec_register_all();
	av_register_all();
	return RUN_ALL_TESTS();
}
