#include <gtest/gtest.h>
#include "../FFmpegEncodingContainer.hpp"

TEST(FFmpegEncodingContainer, ContainerFactory) {
	FFmpegEncodingContainer* container = FFmpegEncodingContainer::makeContainer(
			"webm");
	ASSERT_TRUE(container != NULL);
	ASSERT_STREQ("webm", container->getContainerId().c_str());
	ASSERT_STREQ("WebM file format", container->getContainerName().c_str());
}
