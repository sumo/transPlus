/*
 * FFmpegEncoder.h
 *
 *  Created on: 21 May 2012
 *      Author: mediabag
 */

#ifndef FFMPEGENCODER_H_
#define FFMPEGENCODER_H_
#include "FFmpegContainer.hpp"
#include "Helper.hpp"

class StreamFactory;

class FFmpegEncodingContainer: public FFmpegContainer {
private:
	AVFormatContext* oc;
public:
	static FFmpegEncodingContainer* makeContainer(string ct);
	static ContainerList& getContainerList() {
		static ContainerList containerList; // constructor runs once, when someone first needs it
		return containerList;
	}
	FFmpegEncodingContainer(AVFormatContext* fc);
	virtual ~FFmpegEncodingContainer();
	string getContainerId();
	string getContainerName();
	friend class StreamFactory;
};

#endif /* FFMPEGENCODER_H_ */
